#pragma once
#include "SystemBase.h"
#include <Component/ScriptComponent.h>
#include <unordered_map>
#include <queue>
#include <memory>
#include <functional>
#include <Core/ObjectHandler.h>

#define REGISTER_SCRIPT_METHOD(Method) \
	ScriptSystem::GetInstance().Enque(WeakFromThis<Component>(), Define::EScriptGroup::SG_##Method, \
		[weak = WeakFromThis<Component>()]() { \
			if (auto sp = weak.lock()) \
			{ static_cast<std::remove_pointer<decltype(this)>::type*>(sp)->Method(); } \
		})

struct ScriptWrapper
{
	WeakObjectPtr<Component> Target;
	std::function<void()> TickFunc;

	ScriptWrapper(WeakObjectPtr<Component> _Target, std::function<void()> _TickFunc)
		: Target(_Target), TickFunc(_TickFunc)
	{
	}
};

class ScriptSystem : public SystemBase, public Singleton<ScriptSystem>
{
public:
	Define::EScriptGroup currnetGroup;
	std::unordered_map<Define::EScriptGroup, std::queue<ScriptWrapper>> m_scriptQueues;

	void Enque(WeakObjectPtr<Component> InTarget, Define::EScriptGroup InGroup, std::function<void()> TickFunc);
    void ProcessScriptGroup(Define::EScriptGroup group);
};

