#include "pch.h"
#include "TransformSystem.h"

TransformSystem::TransformSystem()
{

}

TransformSystem::~TransformSystem()
{

}

void TransformSystem::Regist(const WeakObjectPtr<TransformComponent>& _TransformComponent)
{
	if (!_TransformComponent.expired())
	{
		m_transformComponents.emplace_back(_TransformComponent);
	}
}

void TransformSystem::UnRegist(WeakObjectPtr<TransformComponent>&& _TransformComponent)
{
	for (auto it = m_transformComponents.begin(); it != m_transformComponents.end(); ++it)
	{
		if ((*it).lock() && _TransformComponent.lock())
		{
			if ((*it).lock() == _TransformComponent.lock())
			{
				m_transformComponents.erase(it);
				return;
			}
		}
	}
}

void TransformSystem::UnRegistAll()
{
	m_transformComponents.clear();
}


// 렌더 직전에 호출 모든 게임 Update 후, Render 전
void TransformSystem::Flush()
{
	static const auto I = D2D1::Matrix3x2F::Identity();

	std::vector<TransformComponent*> roots;
	roots.reserve(m_transformComponents.size());

	for (auto it = m_transformComponents.begin(); it != m_transformComponents.end(); )
	{
		if (it->expired()) { it = m_transformComponents.erase(it); continue; }

		TransformComponent* tc = it->Get();  // WeakObjectPtr API에 맞게 조정
		if (!tc) { it = m_transformComponents.erase(it); continue; }

		// 부모가 없으면 루트
		if (tc->parent.expired())
			roots.push_back(tc);

		++it;
	}

	// 3) 루트에서 한 번만 내려갱신
	for (auto* r : roots)
		r->RecalcWorldRecursive(I);
}
