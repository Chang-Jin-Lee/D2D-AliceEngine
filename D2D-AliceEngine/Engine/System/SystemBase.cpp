#include "pch.h"
#include "SystemBase.h"

SystemBase::SystemBase()
{

}

SystemBase::~SystemBase()
{

}

void SystemBase::Regist(const WeakObjectPtr<Component>& _component)
{
	if (!_component.expired())
	{
		m_Components.emplace_back(_component);
	}
}

void SystemBase::UnRegist(WeakObjectPtr<Component>&& _component)
{
	for (auto it = m_Components.begin(); it != m_Components.end(); ++it)
	{
		if ((*it).lock() && _component.lock())
		{
			if ((*it).lock() == _component.lock())
			{
				m_Components.erase(it);
				return;
			}
		}
	}
}

void SystemBase::UnRegistAll()
{
	m_Components.clear();
}
