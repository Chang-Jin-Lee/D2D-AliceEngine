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
	InputComponent();
	~InputComponent();
public:
	virtual void Initialize();
	void Update(const float& deltaSeconds) override;
	virtual void Release();
	
	template<typename F>
	void SetAction(F&& action)
	{
		actions.emplace_back(std::forward<F>(action));
	}

private:
	std::vector<std::function<void()>> actions;
};