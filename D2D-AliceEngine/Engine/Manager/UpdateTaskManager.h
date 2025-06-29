#pragma once
#include <Core/Singleton.h>
#include <memory>
#include <vector>
#include <functional>
#include <Scene/Scene.h>
#include <unordered_map>
#include <Core/Tickable.h>

/*
* @brief : Update()�ȿ����� ������ ���ϱ� ���� �Ŵ����Դϴ�
* @details :
*	Update �Լ��� �ܰ踦 ������ ��ϵ� �������(Ȥ�� �����Ͽ�) ������ �Լ� �����͸� �����մϴ�.
*	����� ���θ� �𸮾� ��Ÿ�Ϸ� ��������ϴ�.
*/

// Unreal �� Unity �ֿ� Tick/Update ��
// TG_PrePhysics       : EarlyUpdate         // (AI/�Է�, Unity�� EarlyUpdate)
// TG_StartPhysics     : FixedUpdate ���� �� // (Unity�� ���� ���� ����, ���� FixedUpdate �غ�)
// TG_DuringPhysics    : FixedUpdate         // (���� ����, Unity�� FixedUpdate)
// TG_EndPhysics       : FixedUpdate ���� �� // (Unity�� ���� ���� ����, ���� FixedUpdate ��ó��)
// TG_PostPhysics      : Update/LateUpdate   // (���ӷ���, Unity�� Update/LateUpdate)
// TG_PostUpdateWork   : PreRender/Render    // (ī�޶�, ������, Unity�� PreRender/Render)
// TG_LastDemotable    : (Ư�� ��Ȳ��, �𸮾��� �ܰ�)
// TG_NewlySpawned     : (�� ��ü ó����, �𸮾��� �ܰ�)

/** Update() �Լ��� �����ؾ߸� �ϴ� ��ϵ��Դϴ� */
struct FTickContext
{
	/** Update �ǰ� �ִ� ���� ������ ���� �� */
	Scene* World;
	Define::ETickingGroup TickGroup;
	/** Update�� ���� deltatime */
	float DeltaSeconds;
	/** true�� Update�� �α� ó���� */
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
	Define::ETickingGroup Group;
	std::weak_ptr<ITickable> Target;

	UpdateWrapper(Define::ETickingGroup InGroup, std::weak_ptr<ITickable> InTarget)
		: Group(InGroup), Target(InTarget)
	{
	}
};

class UpdateTaskManager : public Singleton<UpdateTaskManager>
{
public: 
	std::unordered_map<Define::ETickingGroup, std::vector<UpdateWrapper>> m_TickLists;
	FTickContext Context;

	void Enque(std::weak_ptr<ITickable> InTarget, Define::ETickingGroup InGroup)
	{
		m_TickLists[InGroup].emplace_back(InGroup, InTarget);
	}

	void StartFrame(Scene* InWorld, float InDeltaSeconds)
	{
		Context.World = InWorld;
		Context.DeltaSeconds = InDeltaSeconds;
	}

	void TickAll()
	{
		for (int group = 0; group < static_cast<int>(Define::ETickingGroup::TG_MAX); ++group)
		{
			Context.TickGroup = static_cast<Define::ETickingGroup>(group);
			auto& TickList = m_TickLists[Context.TickGroup];

			// �Ҹ�� ��ü�� TickList�� �����ϰ� it�� �� �ڸ��� �״�� �ִ´�
			// �ƴ϶�� it�� ��� Ž��.
			// swap-and-pop�� ����ϰ� ���� �ڵ�
			for (auto it = TickList.begin(); it != TickList.end(); )
			{
				if (auto sp = it->Target.lock())
				{
					sp->Update(Context.DeltaSeconds);
					++it;
				}
				else
				{
					it = TickList.erase(it); // �Ҹ�� ��ü ����
				}
			}
		}
	}
};

