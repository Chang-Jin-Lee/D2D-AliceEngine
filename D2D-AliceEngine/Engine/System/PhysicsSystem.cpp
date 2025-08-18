#include "pch.h"
#include "PhysicsSystem.h"
#include <Manager/UpdateTaskManager.h>
#include <Core/Tickable.h>
#include <Core/ObjectHandler.h>
#include <Experimental/Collision/CollisionDetector.h>

PhysicsSystem::PhysicsSystem()
{

}

PhysicsSystem::~PhysicsSystem()
{
}

void PhysicsSystem::Regist(const WeakObjectPtr<Rigidbody2D>& _component)
{
	if (!_component.expired())
	{
		m_rigidBodies.emplace_back(_component);
	}
}

void PhysicsSystem::UnRegist(WeakObjectPtr<Rigidbody2D>&& _component)
{
	for (auto it = m_rigidBodies.begin(); it != m_rigidBodies.end(); ++it)
	{
		if ((*it).lock() && _component.lock())
		{
			if ((*it).lock() == _component.lock())
			{
				m_rigidBodies.erase(it);
				return;
			}
		}
	}
}

void PhysicsSystem::UnRegistAll()
{
	m_rigidBodies.clear();
}

void PhysicsSystem::Update(const float& deltaSeconds)
{
	for (auto it = m_rigidBodies.begin(); it != m_rigidBodies.end(); )
	{
		if (it->expired())
		{
			it = m_rigidBodies.erase(it);
		}
		else
		{
			Rigidbody2D* rb = it->Get();
			if (collidedBodies.find(rb) == collidedBodies.end())
			{
				rb->m_eRigidBodyState = Define::ERigidBodyState::Space;
			}
			// 부딪치지 않았다면 물리 시뮬레이션 함
			rb->Update(deltaSeconds);
			it++;
		}
	}
}

void PhysicsSystem::Initialize()
{
	UpdateTaskManager::GetInstance().Enque(
		WeakFromThis<ITickable>(),
		Define::ETickingGroup::TG_DuringPhysics,
		[weak = WeakFromThis<ITickable>()](const float& dt)
	{
		if (auto sp = weak.lock())
		{
			sp->Update(dt);
		}
	}
	);
}

void PhysicsSystem::Update()
{
}

void PhysicsSystem::Release()
{
	m_rigidBodies.clear();
}
