#include "pch.h"
#include "gameObject.h"
#include <Math/Transform.h>
#include <Component/SpriteRenderer.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>
#include <FSM/FiniteStateMachine.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/ClassManager.h>
#include <Scene/Scene.h>

gameObject::gameObject(const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f))
{
	Initialize(position, rotation, scale, pivot);
}

gameObject::~gameObject()
{
    // 모든 컴포넌트에 대해 OnDestroy를 먼저 호출
    for (auto& component : m_components)
    {
        if (WeakObjectPtr<Component> comp = component)
        {
            comp.lock()->OnDestroy();
        }
    }

    // 그 다음 전부 삭제
    for (auto& component : m_components)
    {
        delete component;
    }
    m_components.clear();
    m_transformComponent.reset();
}

void gameObject::OnStart()
{
	//for (auto& component : m_components)
	//{
	//	if (WeakObjectPtr<Component> comp = component)
	//		comp.lock()->OnStart();
	//}
}

void gameObject::OnEnd()
{
	
}

void gameObject::Initialize()
{
	m_transformComponent = AddComponentByWeak<TransformComponent>();
	m_transformComponent.lock()->SetTransform(FVector2(0.0f), 0, FVector2(1.0f), FVector2(0.5f));
}

void gameObject::Initialize(const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.5f))
{
	m_transformComponent = AddComponentByWeak<TransformComponent>();
	m_transformComponent.lock()->SetTransform(position, rotation, scale, pivot);
}

TransformComponent* gameObject::transform() const
{
	if (m_transformComponent.expired()) return nullptr;
	return m_transformComponent.Get();
}

void gameObject::AddChildObject(const gameObject* obj)
{
	if (auto weakThis = WeakFromThis<gameObject>())
	{
		weakThis->transform()->AddChildObject(obj->m_transformComponent);
	}
}

void gameObject::AddChildTransform(TransformComponent* transform)
{
	if (auto weakThis = WeakFromThis<gameObject>())
	{
		weakThis->transform()->AddChildObject(transform);
	}
}

void gameObject::SetParent(TransformComponent* transform)
{
	if (transform) {
		transform->AddChildObject(this->transform());
	}
}

void gameObject::RemoveFromParent()
{
	if (auto transformComp = m_transformComponent.lock())
	{
		// 遺?紐⑤줈遺??꽣 ?옄?떊?쓣 ?젣嫄?
		transformComp->RemoveFromParent();
	}
}

// 醫뚰몴, ?뒪耳??씪, ?쉶?쟾 愿??젴 ?븿?닔?뱾
void gameObject::SetPosition(const FVector2& _pos)
{
	if (auto transformComp = m_transformComponent.lock())
	{
		transformComp->SetPosition(_pos.x, _pos.y);
	}
}

void gameObject::SetRotation(const float& _rotation)
{
	if (auto transformComp = m_transformComponent.lock())
	{
		transformComp->SetRotation(_rotation);
	}
}

void gameObject::SetScale(const FVector2& _scale)
{
	if (auto transformComp = m_transformComponent.lock())
	{
		transformComp->SetScale(_scale.x, _scale.y);
	}
}

void gameObject::AddPosition(const FVector2& _pos)
{
	if (auto transformComp = m_transformComponent.lock())
	{
		FVector2 currentPos = transformComp->GetPosition();
		transformComp->SetPosition(currentPos.x + _pos.x, currentPos.y + _pos.y);
	}
}

void gameObject::AddRotation(const float& _rotation)
{
	if (auto transformComp = m_transformComponent.lock())
	{
		float currentRotation = transformComp->GetRotation();
		transformComp->SetRotation(currentRotation + _rotation);
	}
}

void gameObject::AddScale(const FVector2& _scale)
{
	if (auto transformComp = m_transformComponent.lock())
	{
		FVector2 currentScale = transformComp->GetScale();
		transformComp->SetScale(currentScale.x + _scale.x, currentScale.y + _scale.y);
	}
}

FVector2 gameObject::GetPosition() const
{
	if (auto transformComp = m_transformComponent.lock())
	{
		FVector2 pos = transformComp->GetPosition();
		return FVector2(pos.x, pos.y);
	}
	return FVector2(0.0f, 0.0f);
}

float gameObject::GetRotation()
{
	if (auto transformComp = m_transformComponent.lock())
	{
		return transformComp->GetRotation();
	}
	return 0.0f;
}

FVector2 gameObject::GetScale()
{
	if (auto transformComp = m_transformComponent.lock())
	{
		FVector2 scale = transformComp->GetScale();
		return FVector2(scale.x, scale.y);
	}
	return FVector2(1.0f, 1.0f);
}

FVector2 gameObject::GetScaleInv()
{
	if (auto transformComp = m_transformComponent.lock())
	{
		FVector2 scale = transformComp->GetScale();
		return FVector2(1/scale.x, 1/scale.y);
	}
	return FVector2(1.0f, 1.0f);
}

void gameObject::Update()
{

}

void gameObject::Release()
{
	for (auto& component : m_components)
	{
		delete component;
	}
	m_components.clear();
}
