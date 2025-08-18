#include "pch.h"
#include "TimerManager.h"

void TimerManager::Initialize()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&initCounter);
	prevCounter = initCounter;
}

void TimerManager::UpdateTime()
{
	QueryPerformanceCounter(&currentCounter);
	unscaledDeltaTime = static_cast<float>(currentCounter.QuadPart - prevCounter.QuadPart) / frequency.QuadPart;
	deltaTime = unscaledDeltaTime * globalTimeScale;
	prevCounter = currentCounter;

	// 밑에는 일단은 안쓰임. 디버그용FPS로만 쓰임
	accumulator += deltaTime;
	//OutputDebugStringW((L"DeltaTime : " + std::to_wstring(deltaTime) + L"\n").c_str());

	frameCount++;
	timeSinceLastFps += deltaTime;
	if (timeSinceLastFps >= 1.0f)
	{
		currentFps = frameCount / timeSinceLastFps;
		frameCount = 0;
		timeSinceLastFps = 0.0f;
		ShowFPSDebug();
	}

    // 타이머 업데이트 - 콜백 실행은 잠금 해제 상태에서 수행하여 재진입(타이머 추가/삭제)을 허용
    {
        std::unique_lock<std::mutex> lock(TimersMutex);

        TimersToRemove.clear(); // 재사용을 위해 클리어

        // 타이머 복사본 생성하여 안전하게 순회
        std::unordered_map<size_t, TimerData> TimersCopy = Timers;

        for (auto& Pair : TimersCopy)
        {
            const size_t timerId = Pair.first;
            auto timerIt = Timers.find(timerId);

            // 타이머가 이미 삭제되었는지 확인
            if (timerIt == Timers.end())
                continue;

            auto& Data = timerIt->second;

            if (Data.bPaused)
                continue;

            Data.TimeRemaining -= unscaledDeltaTime;
            Data.Elapsed += unscaledDeltaTime;

            if (Data.TimeRemaining <= 0.0f)
            {
                // 콜백 실행 전에 타이머가 여전히 유효한지 재확인
                timerIt = Timers.find(timerId);
                if (timerIt != Timers.end())
                {
                    // 콜백 복사 후 잠금 해제 → 실행 → 다시 잠금
                    auto tickCb = timerIt->second.TickCallback;
                    auto cb     = timerIt->second.Callback;
                    float elapsed = timerIt->second.Elapsed;

                    lock.unlock();
                    try
                    {
                        if (tickCb)
                            tickCb(elapsed);
                        else if (cb)
                            cb();
                    }
                    catch (...)
                    {
                        lock.lock();
                        TimersToRemove.push_back(timerId);
                        // 다음 타이머로 진행
                        continue;
                    }
                    lock.lock();
                }

                // 다시 한번 타이머 존재 확인 (콜백에서 삭제/추가되었을 수 있음)
                timerIt = Timers.find(timerId);
                if (timerIt != Timers.end())
                {
                    if (timerIt->second.bLooping)
                    {
                        timerIt->second.TimeRemaining = timerIt->second.OriginalRate;
                        timerIt->second.Elapsed = 0.0f;
                    }
                    else
                    {
                        TimersToRemove.push_back(timerId);
                    }
                }
            }
        }

        // 삭제 예정 타이머들 제거
        for (size_t Id : TimersToRemove)
        {
            Timers.erase(Id);
        }
    }
}

void TimerManager::UpdateFixedTime(std::function<void(const float&)> f)
{
	while (accumulator >= fixedDeltaTime)
	{
		float dt = min(fixedDeltaTime, accumulator);

		fixedTime += fixedDeltaTime;
		accumulator -= fixedDeltaTime;

		// 실제 FixedUpdate 로직 호출
		f(dt);
	}
	//OutputDebugStringW((L"accumulator : " + std::to_wstring(accumulator) + L"\n").c_str());
	//OutputDebugStringW((L"fixedTime : " + std::to_wstring(fixedTime) + L"\n").c_str());
}

bool TimerManager::IfFixedUpdatable()
{
	return accumulator >= fixedDeltaTime;
}

float TimerManager::GetElapsedTime()
{
	return deltaTime;
}

float TimerManager::GetTotalTime()
{
	float totalTime = static_cast<float>(currentCounter.QuadPart - initCounter.QuadPart) / frequency.QuadPart;
	return totalTime;
}

float TimerManager::GetFixedTime()
{
	return fixedTime;
}

float TimerManager::GetAccumulator() const
{
	return accumulator;
}

float TimerManager::GetCurrentFPS() const
{
	return currentFps;
}

void TimerManager::ShowFPSDebug()
{

	OutputDebugStringW((L"FPS : " + std::to_wstring(currentFps) + L"\n").c_str());
}

float TimerManager::GetGlobalTimeScale() const
{
	return globalTimeScale;
}

void TimerManager::SetGlobalTimeScale(const float& _value)
{
	globalTimeScale = _value;
}

// =================== TimeHandle ===================

void TimerManager::ClearTimer(FTimerHandle Handle)
{
	std::lock_guard<std::mutex> lock(TimersMutex);
	Timers.erase(Handle.InternalHandle);
}

bool TimerManager::IsTimerActive(FTimerHandle Handle) const
{
	std::lock_guard<std::mutex> lock(TimersMutex);
	auto it = Timers.find(Handle.InternalHandle);
	return it != Timers.end() && !it->second.bPaused;
}

void TimerManager::PauseTimer(FTimerHandle Handle)
{
	std::lock_guard<std::mutex> lock(TimersMutex);
	auto it = Timers.find(Handle.InternalHandle);
	if (it != Timers.end())
		it->second.bPaused = true;
}

void TimerManager::UnPauseTimer(FTimerHandle Handle)
{
	std::lock_guard<std::mutex> lock(TimersMutex);
	auto it = Timers.find(Handle.InternalHandle);
	if (it != Timers.end())
		it->second.bPaused = false;
}

float TimerManager::GetTimerElapsed(FTimerHandle Handle) const
{
	std::lock_guard<std::mutex> lock(TimersMutex);
	auto it = Timers.find(Handle.InternalHandle);
	if (it != Timers.end())
		return it->second.Elapsed;
	return -1.0f;
}

float TimerManager::GetTimerRemaining(FTimerHandle Handle) const
{
	std::lock_guard<std::mutex> lock(TimersMutex);
	auto it = Timers.find(Handle.InternalHandle);
	if (it != Timers.end())
		return it->second.TimeRemaining;
	return -1.0f;
}

template <typename UserClass>
void TimerManager::SetTimer(
	FTimerHandle& OutHandle,
	UserClass* InObj,
	void (UserClass::* InFunc)(),
	float Rate,
	bool bLoop,
	float FirstDelay
)
{
	static_assert(std::is_base_of<UObjectBase, UserClass>::value, "SetTimer requires UObjectBase-derived class");

	FTimerHandle Handle;
	Handle.InternalHandle = NextHandle++;

	TimerData Data;
	Data.Callback = [InObj, InFunc]() {
		(InObj->*InFunc)();
		};
	Data.TimeRemaining = FirstDelay > 0 ? FirstDelay : Rate;
	Data.OriginalRate = Rate;
	Data.bLooping = bLoop;

	Timers[Handle.InternalHandle] = Data;
	OutHandle = Handle;
}

void TimerManager::SetTimer(FTimerHandle& OutHandle, std::function<void()> InCallback, float Rate, bool bLoop, float FirstDelay)
{
	std::lock_guard<std::mutex> lock(TimersMutex);
	
	FTimerHandle Handle;
	Handle.InternalHandle = NextHandle++;

	TimerData Data;
	Data.Callback = InCallback;
	Data.TimeRemaining = (FirstDelay > 0.0f) ? FirstDelay : Rate;
	Data.OriginalRate = Rate;
	Data.bLooping = bLoop;

	Timers[Handle.InternalHandle] = Data;
	OutHandle = Handle;
}

void TimerManager::SetTimerDt(FTimerHandle& OutHandle,
	std::function<void(float)> InCallback)
{
	std::lock_guard<std::mutex> lock(TimersMutex);
	
	FTimerHandle Handle;
	Handle.InternalHandle = NextHandle++;

	TimerData Data;
	Data.TickCallback = std::move(InCallback);
	Data.TimeRemaining = 0.0f;    // 매 프레임 발화
	Data.OriginalRate = 0.0f;    // 사용 안 함
	Data.bLooping = true;    // 항상 반복

	Timers[Handle.InternalHandle] = std::move(Data);
	OutHandle = Handle;
}
