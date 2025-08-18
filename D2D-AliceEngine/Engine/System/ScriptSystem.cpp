#include "pch.h"
#include "ScriptSystem.h"

void ScriptSystem::Enque(WeakObjectPtr<Component> InTarget, Define::EScriptGroup InGroup, std::function<void()> TickFunc)
{
	m_scriptQueues[InGroup].emplace(InTarget, TickFunc);
}

void ScriptSystem::Enque(WeakObjectPtr<Component> InTarget, Define::ESCriptUpdateGroup InGroup, std::function<void()> TickFunc)
{
	m_scriptTickingQueues[InGroup].emplace_back(InTarget, TickFunc);
}

// 현재 큐의 내용을 통째로 빼서 임시 큐로 보관합니다.
// 임시 큐만 처리하고 실행 중 Enque 된 건 queueRef에 남아 다음 프레임에서 실행
// 이걸 하는 이유는 OnStart, Awake, OnEnd 등에서 NewObject로 오브젝트 생성시에 문제가 생길 수 있어서 그렇습니다.
// 무한 루프가 생깁니다. 
void ScriptSystem::ProcessScriptGroup(Define::EScriptGroup group)
{
	currnetGroup = group;
	std::queue<ScriptWrapper>& queueRef = m_scriptQueues[group];

	std::queue<ScriptWrapper> currentQueue;
	std::swap(currentQueue, m_scriptQueues[group]);

	while (!currentQueue.empty())
	{
		auto& wrapper = currentQueue.front();
		if (!wrapper.Target.expired())
		{
			wrapper.TickFunc();
		}
		currentQueue.pop();
	}
}
// 이번 프레임에 실행할 큐 사이즈를 가져온 다음
// 그 사이즈만 TickFunc()를 실행합니다
// 실행 이후에 파괴된 항목만 정리합니다.
// 안전하게 사용하기 위해서입니다.
void ScriptSystem::ProcessScriptUpdateGroup(Define::ESCriptUpdateGroup group)
{
	currnetTickingGroup = group;

	auto& v = m_scriptTickingQueues[group];
	const size_t n = v.size();

	for (size_t i = 0; i < n; ++i)
	{
		if (!v[i].Target.expired())
			v[i].TickFunc();  // 매 프레임 계속 실행
	}

	v.erase(std::remove_if(v.begin(), v.end(),
		[](const ScriptWrapper& w) { return w.Target.expired(); }), v.end());
}
