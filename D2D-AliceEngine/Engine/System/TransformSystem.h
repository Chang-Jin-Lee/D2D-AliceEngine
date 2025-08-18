#pragma once
#include <Component/TransformComponent.h>
#include <Core/Singleton.h>
#include "SystemBase.h"

class TransformSystem : public Singleton<TransformSystem>
{
public:
	TransformSystem();
	virtual ~TransformSystem();

public:
	void Regist(const WeakObjectPtr<TransformComponent>& _component);
	void UnRegist(WeakObjectPtr<TransformComponent>&& _component);
	void UnRegistAll();
	void Flush();

protected:
	std::vector<WeakObjectPtr<TransformComponent>> m_transformComponents;
};

