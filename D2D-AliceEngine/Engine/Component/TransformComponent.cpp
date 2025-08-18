#include "pch.h"
#include "TransformComponent.h"
#include <Manager/UpdateTaskManager.h>
#include <Math/Transform.h>
#include <System/TransformSystem.h>

TransformComponent::TransformComponent()
{
	SetTransform(FVector2(0), 0, FVector2(1.0f), FVector2(0.5f));
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::Initialize()
{
	//REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_NewlySpawned);
	TransformSystem::GetInstance().Regist(WeakFromThis<TransformComponent>());
}

void TransformComponent::Release()
{
	TransformSystem::GetInstance().UnRegist(WeakFromThis<TransformComponent>());
	for (auto child : children)
	{
		if (child.lock())
		{
			child.lock()->parent.reset();
		}
	}
}

void TransformComponent::RecalcWorldRecursive(const D2D1::Matrix3x2F& parentWorld)
{
	// local/world dirty 체크는 Transform::ToMatrix()가 내부 캐시로 해결
	const auto local = m_localTransform.ToMatrix();
	const auto world = local * parentWorld;
	m_worldTransform.SetFromMatrix(world);

	for (auto& ch : children)
	{
		if (auto c = ch.lock())
			c->RecalcWorldRecursive(world);
	}
}

void TransformComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	D2D1::Matrix3x2F mat;

	if (!parent.expired())
	{
		mat = m_localTransform.ToMatrix() * parent.lock()->m_worldTransform.ToMatrix();
	}
	else
	{
		mat = m_localTransform.ToMatrix();
	}

	m_worldTransform.SetFromMatrix(mat);

	for (auto child : children)
	{
		if (!child.expired())
		{
			child->Update(deltaSeconds);
		}
	}
}

void TransformComponent::SetTransform(const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot)
{
	m_localTransform.SetPosition(position.x, position.y);
	m_localTransform.SetRotation(rotation);
	m_localTransform.SetScale(scale.x, scale.y);
	SetPivot(pivot.x, pivot.y);
}

void TransformComponent::AddChildObject(WeakObjectPtr<TransformComponent> child)
{
	auto childPtr = child.lock();
	if (!childPtr) return; // nullptr
	
	childPtr->parent = WeakFromThis<TransformComponent>();
	children.push_back(childPtr);
}

void TransformComponent::RemoveFromParent()
{
	auto parentPtr = parent.lock();
	if (!parentPtr) return; // 부모가 없으면 아무것도 하지 않음
	
	// 부모의 children 리스트에서 자신을 제거
	for (auto it = parentPtr->children.begin(); it != parentPtr->children.end(); ++it)
	{
		if (it->lock() == this)
		{
			parentPtr->children.erase(it);
			break;
		}
	}
	
	// 자신의 parent 참조 제거
	parent.reset();
}

FVector2 TransformComponent::GetPosition() const
{
	const D2D1_VECTOR_2F& pos = m_worldTransform.GetPosition();
	return FVector2(pos.x, pos.y);
}

void TransformComponent::SetPosition(const float& _x, const float& _y)
{
	m_localTransform.SetPosition(_x, _y);
	SetDirty();
	bMoved = true;
}

void TransformComponent::SetPosition(const float& _x)
{
	m_localTransform.SetPosition(_x, _x);
	SetDirty();
	bMoved = true;
}

void TransformComponent::SetPosition(const FVector2& _v)
{
	m_localTransform.SetPosition(_v.x, _v.y);
	SetDirty();
	bMoved = true;
}

void TransformComponent::SetWorldPosition(const FVector2& _v)
{
	m_worldTransform.SetPosition(_v.x, _v.y);
	SetDirty();
	bMoved = true;
}

void TransformComponent::SetRotation(const float& _val)
{
	m_localTransform.SetRotation(_val);
	SetDirty();
}

float TransformComponent::GetRotation()
{
	return m_localTransform.GetRotation();
}

FVector2 TransformComponent::GetScale()
{
	return FVector2(m_localTransform.GetScale().x, m_localTransform.GetScale().y);
}

void TransformComponent::SetScale(const FVector2& _v)
{
	m_localTransform.SetScale(_v.x, _v.y);
	SetDirty();
}

void TransformComponent::SetScale(const float& _x, const float& _y)
{
	m_localTransform.SetScale(_x, _y);
	SetDirty();
}

void TransformComponent::SetScale(const float& _x)
{
	m_localTransform.SetScale(_x, _x);
	SetDirty();
}

void TransformComponent::SetWorldScale(const FVector2& _v)
{
	m_worldTransform.SetScale(_v.x, _v.y);
	SetDirty();
}

void TransformComponent::AddRotation(const float& _val)
{
	m_localTransform.SetRotation(m_localTransform.GetRotation() + _val);
	SetDirty();
}

void TransformComponent::AddPosition(const float& _x, const float& _y)
{
	m_localTransform.SetPosition(m_localTransform.GetPosition().x + _x, m_localTransform.GetPosition().y + _y);
	SetDirty();
}

void TransformComponent::AddPosition(const FVector2& _v)
{
	m_localTransform.SetPosition(m_localTransform.GetPosition().x +_v.x, m_localTransform.GetPosition().y +_v.y);
	SetDirty();
}

void TransformComponent::SetPivot(const float& _x, const float& _y)
{
	m_pivot.x = _x;
	m_pivot.y = _y;
}

void TransformComponent::SetPivot(const float& _x)
{
	m_pivot.x = _x;
	m_pivot.y = _x;
}

FVector2* TransformComponent::GetPivot()
{
	return &m_pivot;
}

void TransformComponent::SetDirty()
{
	m_localTransform.dirty = true;
	m_worldTransform.dirty = true;
	for (auto child : children)
	{
		if (!child.expired())
			child->SetDirty();
	}
}
