#include "pch.h"
#include "InputComponent.h"
#include <Manager/UpdateTaskManager.h>

InputComponent::InputComponent()
{
	InputSystem::GetInstance().Regist(WeakFromThis<Component>());
}

InputComponent::~InputComponent()
{
	InputSystem::GetInstance().UnRegist(WeakFromThis<Component>());
	slots.clear();
}

void InputComponent::Initialize()
{
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
}

void InputComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	for(auto it = slots.begin(); it != slots.end();)
	{
		if (ObjectHandler::GetInstance().IsValid(it->handle))
		{
			it->func();
			it++;
		}
		else
		{
			it = slots.erase(it);
		}
	}
}

void InputComponent::Release()
{
	slots.clear();
}
