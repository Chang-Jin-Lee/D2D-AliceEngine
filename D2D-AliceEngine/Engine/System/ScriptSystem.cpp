#include "pch.h"
#include "ScriptSystem.h"

void ScriptSystem::Enque(WeakObjectPtr<Component> InTarget, Define::EScriptGroup InGroup, std::function<void()> TickFunc)
{
	m_scriptQueues[InGroup].emplace(InTarget, TickFunc);
}

void ScriptSystem::ProcessScriptGroup(Define::EScriptGroup group)
{
	currnetGroup = group;
	while (m_scriptQueues[group].empty() == false)
	{
		if (!m_scriptQueues[group].front().Target.expired())
		{
			m_scriptQueues[group].front().TickFunc();
		}
		m_scriptQueues[group].pop();
	}
}
