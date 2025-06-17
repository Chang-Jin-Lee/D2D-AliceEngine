#pragma once
#include "Component.h"
#include "System/InputSystem.h"
#include <memory>
#include <vector>
#include <functional>
/*
* @briefs : ��ǲ ������Ʈ�Դϴ�.
* @details : actions�� �Լ� �����͸� ����ϸ� Update()�ÿ� �ش� �Լ��� ����˴ϴ�.
*/

class InputComponent : public Component
{
public:
	InputComponent() { }
	~InputComponent() 
	{ 
		InputSystem::Get().UnRegist(this->weak_from_this()); 
		actions.clear();
	}
public:
	virtual void Initialize() { InputSystem::Get().Regist(this->weak_from_this()); }
	virtual void Update() 
	{ 
		for (auto action : actions)
		{
			action();
		}
	}
	virtual void Release() 
	{ 
		actions.clear(); 
	}
	
	template<typename F>
	void SetAction(F&& action)
	{
		actions.emplace_back(std::forward<F>(action));
	}

private:
	std::vector<std::function<void()>> actions;
};