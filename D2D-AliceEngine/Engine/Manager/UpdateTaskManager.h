#pragma once
#include <Core/Singleton.h>
#include <memory>
#include <vector>
#include <functional>
#include <Scene/Scene.h>

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

class UpdateTaskManager : public Singleton<UpdateTaskManager>
{
public: 
	std::function<void()> AllTickFunctions;
	FTickContext Context;

	void StartFrame(Scene* InWorld, float InDeltaSeconds)
	{
		Context.TickGroup = Define::ETickingGroup::TG_PrePhysics;
		Context.DeltaSeconds = InDeltaSeconds;
		Context.World = InWorld;
	}
};

