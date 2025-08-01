#pragma once
#include <Core/Singleton.h>
#include <memory>
#include <vector>
#include <functional>
#include <Scene/Scene.h>
#include <unordered_map>
#include <Core/Tickable.h>

/*
* @brief : Update()안에서의 순서를 정하기 위한 매니저입니다
* @details :
*	Update 함수의 단계를 나누어 등록된 순서대로(혹은 정렬하여) 각각의 함수 포인터를 실행합니다.
*	여기는 내부를 언리얼 스타일로 만들었습니다.
*/

// Unreal ↔ Unity 주요 Tick/Update 비교
// TG_PrePhysics       : EarlyUpdate         // (AI/입력, Unity의 EarlyUpdate)
// TG_StartPhysics     : FixedUpdate 시작 전 // (Unity에 직접 대응 없음, 보통 FixedUpdate 준비)
// TG_DuringPhysics    : FixedUpdate         // (물리 연산, Unity의 FixedUpdate)
// TG_EndPhysics       : FixedUpdate 종료 후 // (Unity에 직접 대응 없음, 보통 FixedUpdate 후처리)
// TG_PostPhysics      : Update/LateUpdate   // (게임로직, Unity의 Update/LateUpdate)
// TG_PostUpdateWork   : PreRender/Render    // (카메라, 렌더링, Unity의 PreRender/Render)
// TG_LastDemotable    : (특수 상황용, 언리얼만의 단계)
// TG_NewlySpawned     : (새 객체 처리용, 언리얼만의 단계)

/** Update() 함수가 실행해야만 하는 목록들입니다 */
struct FTickContext
{
	/** Update 되고 있는 동안 가지고 있을 씬 */
	Scene* World;
	// 현재 Update중인 Group이 무엇인지 알려주는 변수
	Define::ETickingGroup TickGroup;
	/** Update에 대한 deltatime */
	float DeltaSeconds;
	/** true면 Update를 로그 처리함 */
	bool bLogTick;

	FTickContext(float InDeltaSeconds = 0.0f, Define::ETickingGroup InTickGroup = Define::ETickingGroup::TG_PrePhysics)
		: World(nullptr)
		, DeltaSeconds(InDeltaSeconds)
		, TickGroup(InTickGroup)
		, bLogTick(false)
	{
	}
	FTickContext(const FTickContext& In) = default;
	FTickContext& operator=(const FTickContext& In) = default;
};

struct UpdateWrapper
{
	WeakObjectPtr<ITickable> Target;
	std::function<void(const float&)> TickFunc;

	UpdateWrapper(WeakObjectPtr<ITickable> _Target, std::function<void(const float&)> _TickFunc)
		: Target(_Target), TickFunc(_TickFunc)
	{
	}
};

class UpdateTaskManager : public Singleton<UpdateTaskManager>
{
public: 
	std::unordered_map<Define::ETickingGroup, std::vector<UpdateWrapper>> m_TickLists;
	FTickContext Context;

	void Enque(WeakObjectPtr<ITickable> InTarget, Define::ETickingGroup InGroup, std::function<void(const float&)> TickFunc)
	{
		m_TickLists[InGroup].emplace_back(InTarget, TickFunc);
	}

	void StartFrame();
	void EndFrame();
	
	void SetWorld();
	void ClearWorld();

	void TickAll()
	{
		for (int group = 0; group < static_cast<int>(Define::ETickingGroup::TG_MAX); ++group)
		{
			Context.TickGroup = static_cast<Define::ETickingGroup>(group);

			// 소멸된 객체면 TickList를 삭제하고 it는 그 자리에 그대로 있는다
			// 아니라면 it는 계속 탐색.
			// swap-and-pop을 고려하고 만든 코드
			for (auto it = m_TickLists[Context.TickGroup].begin(); it != m_TickLists[Context.TickGroup].end(); )
			{
				if (auto sp = it->Target.lock())
				{
					sp->Update(Context.DeltaSeconds);
					++it;
				}
				else
				{
					it = m_TickLists[Context.TickGroup].erase(it); // 소멸된 객체 제거
				}
			}
		}
	}
};

