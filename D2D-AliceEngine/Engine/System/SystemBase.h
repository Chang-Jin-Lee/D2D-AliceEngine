#pragma once
#include <Component/Component.h>
#include <Core/Singleton.h>

class SystemBase
{
public:
	SystemBase();
	virtual ~SystemBase();

public:
	void Regist(const WeakObjectPtr<Component>& _component);
	void UnRegist(WeakObjectPtr<Component>&& _component);
	void UnRegistAll();

protected:
	std::vector<WeakObjectPtr<Component>> m_Components;
};

