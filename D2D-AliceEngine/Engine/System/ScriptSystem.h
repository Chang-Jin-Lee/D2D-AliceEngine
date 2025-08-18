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

// 추가: deltaSeconds 인자를 받는 메서드용 (예: Update, FixedUpdate, LateUpdate 등)
#define REGISTER_SCRIPT_TICK(Method) \
    ScriptSystem::GetInstance().Enque(WeakFromThis<Component>(), Define::ESCriptUpdateGroup::SG_##Method, \
        [weak = WeakFromThis<Component>()]() { \
            if (auto sp = weak.lock()) { \
                auto& ctx = ScriptSystem::GetInstance().Context; \
                static_cast<std::remove_pointer<decltype(this)>::type*>(sp)->Method(ctx.DeltaSeconds); \
            } \
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
	struct FScriptContext
	{
		float DeltaSeconds{ 0.f };
	} Context;

	Define::EScriptGroup currnetGroup;
	Define::ESCriptUpdateGroup currnetTickingGroup;
	std::unordered_map<Define::EScriptGroup, std::queue<ScriptWrapper>> m_scriptQueues;
	std::unordered_map<Define::ESCriptUpdateGroup, std::vector<ScriptWrapper>> m_scriptTickingQueues;

	void Enque(WeakObjectPtr<Component> InTarget, Define::EScriptGroup InGroup, std::function<void()> TickFunc);
	void Enque(WeakObjectPtr<Component> InTarget, Define::ESCriptUpdateGroup InGroup, std::function<void()> TickFunc);
    void ProcessScriptGroup(Define::EScriptGroup group);
	void ProcessScriptUpdateGroup(Define::ESCriptUpdateGroup group);
};

