#include "pch.h"
#include "CollisionSystem.h"
#include <Manager/UpdateTaskManager.h>
#include <Core/Tickable.h>
#include <Core/ObjectHandler.h>
#include <Experimental/Collision/CollisionDetector.h>
#include <System/PhysicsSystem.h>
#include <Component/Rigidbody2D.h>
#include <Component/Collider.h>

CollisionSystem::CollisionSystem()
{
	currentCollisions.clear();
	previousCollisions.clear();
	m_colliders.clear();
}

CollisionSystem::~CollisionSystem()
{
}

void CollisionSystem::Regist(const WeakObjectPtr<Collider>& _component)
{
	if (!_component.expired())
	{
		m_colliders.emplace_back(_component);
	}
}

void CollisionSystem::UnRegist(WeakObjectPtr<Collider>&& _component)
{
	for (auto it = m_colliders.begin(); it != m_colliders.end(); ++it)
	{
		if ((*it).lock() && _component.lock())
		{
			if ((*it).lock() == _component.lock())
			{
				m_colliders.erase(it);
				return;
			}
		}
	}
}

void CollisionSystem::UnRegistAll()
{
	m_colliders.clear();
}

void CollisionSystem::Update(const float& /*deltaSeconds*/)
{
	// prune expired
	m_colliders.erase(
		std::remove_if(m_colliders.begin(), m_colliders.end(), [](const WeakObjectPtr<Collider>& c){ return c.expired(); }),
		m_colliders.end()
	);

	// Let detector compute current/previous collisions and fire callbacks
	Physics::FCollisionDetector::SweepAndPruneOverlapCheck(m_colliders);
}

void CollisionSystem::Initialize()
{
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_StartPhysics);
}

void CollisionSystem::Update()
{
}

void CollisionSystem::Release()
{
	m_colliders.clear();
	previousCollisions.clear();
	currentCollisions.clear();
}
