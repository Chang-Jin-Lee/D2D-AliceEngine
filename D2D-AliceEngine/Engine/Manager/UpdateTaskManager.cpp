#include "pch.h"
#include "UpdateTaskManager.h"
#include <Manager/SceneManager.h>
#include <Manager/TimerManager.h>
#include <System/ScriptSystem.h>

void UpdateTaskManager::Enque(WeakObjectPtr<ITickable> InTarget, Define::ETickingGroup InGroup, std::function<void(const float&)> TickFunc)
{
	m_TickLists[InGroup].emplace_back(InTarget, TickFunc);
}

void UpdateTaskManager::Dequeue(WeakObjectPtr<ITickable> InTarget)
{
	for (int group = 0; group < static_cast<int>(Define::ETickingGroup::TG_MAX); ++group)
	{
		auto& tickDeque = m_TickLists[static_cast<Define::ETickingGroup>(group)];

		for (auto it = tickDeque.begin(); it != tickDeque.end(); )
		{
			if (it->Target.lock() == InTarget.lock())
			{
				it = tickDeque.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

void UpdateTaskManager::DequeueAll()
{
	m_TickLists.clear();
}

void UpdateTaskManager::StartFrame()
{
	Context.DeltaSeconds = TimerManager::GetInstance().GetElapsedTime();
	ScriptSystem::GetInstance().ProcessScriptGroup(Define::EScriptGroup::SG_Awake);
	ScriptSystem::GetInstance().ProcessScriptGroup(Define::EScriptGroup::SG_OnStart);
}

void UpdateTaskManager::EndFrame()
{
	//ScriptSystem::GetInstance().ProcessScriptGroup(Define::EScriptGroup::SG_OnEnd);
	ScriptSystem::GetInstance().Context.DeltaSeconds = Context.DeltaSeconds;
	ScriptSystem::GetInstance().ProcessScriptUpdateGroup(Define::ESCriptUpdateGroup::SG_Update);
	ScriptSystem::GetInstance().ProcessScriptUpdateGroup(Define::ESCriptUpdateGroup::SG_LateUpdate);
	ScriptSystem::GetInstance().ProcessScriptUpdateGroup(Define::ESCriptUpdateGroup::SG_FixedUpdate);
}

void UpdateTaskManager::SetWorld()
{
	Context.World = SceneManager::GetInstance().GetWorld();
}

void UpdateTaskManager::ClearWorld()
{
	Context.World = nullptr;
	m_TickLists.clear();
}

void UpdateTaskManager::ExitScene()
{
	ScriptSystem::GetInstance().ProcessScriptGroup(Define::EScriptGroup::SG_OnEnd);
}

void UpdateTaskManager::TickAll()
{
	for (int group = 0; group < static_cast<int>(Define::ETickingGroup::TG_MAX); ++group)
	{
		Context.TickGroup = static_cast<Define::ETickingGroup>(group);

		// 현재 TickList를 복사하고 원본은 비워둔다.
		auto& currentList = m_TickLists[Context.TickGroup];
		std::vector<UpdateWrapper> tempList;
		tempList.swap(currentList); // currentList는 비워지고 tempList에 기존 내용이 들어감

		for (auto it = tempList.begin(); it != tempList.end(); )
		{
			if (auto sp = it->Target.lock())
			{
				float delta = Context.DeltaSeconds;
				switch (Context.TickGroup)
				{
				case Define::ETickingGroup::TG_DuringPhysics:
					if (TimerManager::GetInstance().IfFixedUpdatable())
					{
						TimerManager::GetInstance().UpdateFixedTime(it->TickFunc);
					}
					break;
				default:
					it->TickFunc(delta);
					break;
				}
				++it;
			}
			else
			{
				it = tempList.erase(it); // 소멸된 객체 제거
			}
		}

		// 이번 프레임 중 추가된 TickWrapper들은 m_TickLists[Context.TickGroup]에 이미 쌓여 있음.
		// 살아남은 기존 TickWrapper도 다음 프레임을 위해 다시 추가.
		auto& nextTickList = m_TickLists[Context.TickGroup];
		nextTickList.insert(
			nextTickList.end(),
			std::make_move_iterator(tempList.begin()),
			std::make_move_iterator(tempList.end())
		);
	}
}
