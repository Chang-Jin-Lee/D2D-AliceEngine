#pragma once
#include <Core/Singleton.h>
#include <Windows.h>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <vector>

class FTimerHandle
{
public:
	size_t InternalHandle = 0;

	bool operator==(const FTimerHandle& Other) const { return InternalHandle == Other.InternalHandle; }
	bool operator!=(const FTimerHandle& Other) const { return !(*this == Other); }
};

class TimerManager : public Singleton<TimerManager>
{
public:
	void Initialize();
	void UpdateTime();
	void UpdateFixedTime(std::function<void(const float&)> f);
	bool IfFixedUpdatable();
	float GetElapsedTime();
	float GetTotalTime();

	float GetFixedTime();
	float GetAccumulator() const;
	float GetCurrentFPS() const;

	void ShowFPSDebug();
	float GetGlobalTimeScale() const;
	void SetGlobalTimeScale(const float& _value);

	LARGE_INTEGER frequency;
	LARGE_INTEGER prevCounter;
	LARGE_INTEGER currentCounter;
	LARGE_INTEGER initCounter;
	float unscaledDeltaTime{0};
	float deltaTime{0};
	float fixedTime{0};

	float fixedDeltaTime{ 0.08 };
private:
	float accumulator{ 0.0f };

	int frameCount = 0;          // 1초 동안 누적된 프레임 수
	float timeSinceLastFps = 0.0f; // 최근 FPS 측정 후 경과시간
	float currentFps = 0.0f;     // 현재 FPS 값

	float timeSinceLastDebug = 0.0f;

	float globalTimeScale = 1.0f;


public:

	// 일반 SetTimer
	template <typename UserClass>
	void SetTimer(
		FTimerHandle& OutHandle,
		UserClass* InObj,
		void (UserClass::* InFunc)(),
		float Rate,
		bool bLoop = false,
		float FirstDelay = 0.0f
	);

	void ClearTimer(FTimerHandle Handle);
	bool IsTimerActive(FTimerHandle Handle) const;
	void PauseTimer(FTimerHandle Handle);
	void UnPauseTimer(FTimerHandle Handle);
	float GetTimerElapsed(FTimerHandle Handle) const;
	float GetTimerRemaining(FTimerHandle Handle) const;
	void SetTimer(FTimerHandle& OutHandle, std::function<void()> InCallback, float Rate, bool bLoop, float FirstDelay);

	void SetTimerDt(FTimerHandle& OutHandle, std::function<void(float)> InCallback);

private:
	struct TimerData
	{
		std::function<void()> Callback;
		std::function<void(float)> TickCallback;
		float TimeRemaining = 0.0f;
		float OriginalRate = 0.0f;
		bool bLooping = false;
		bool bPaused = false;
		float Elapsed = 0.0f;
	};

	std::unordered_map<size_t, TimerData> Timers;
	size_t NextHandle = 1;
	
	// 스레드 안전성을 위한 뮤텍스
	mutable std::mutex TimersMutex;
	
	// 삭제 예정 타이머들 (Update 중 삭제를 위해)
	std::vector<size_t> TimersToRemove;
};

