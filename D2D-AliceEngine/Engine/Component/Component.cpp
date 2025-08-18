#include "pch.h"
#include "Component.h"
#include <Component/TransformComponent.h>
#include <Object/gameObject.h>
#include <Helpers/StringHelper.h>
#include <Math/Transform.h>
#include <Scene/Scene.h>
#include <Object/Camera.h>

Component::Component()
{
	SetName(L"Default Component Name");
	SetUUID(StringHelper::MakeUniqueName());
}

Component::Component(const std::wstring& name)
{
	SetName(name);
	SetUUID(StringHelper::MakeUniqueName());
}

Component::~Component()
{
}

void Component::Update(const float& deltaSeconds)
{

}

const std::wstring& Component::GetOwnerName()
{
	// TODO: 여기에 return 문을 삽입합니다.
	if (!owner.expired())
		return owner.lock()->GetName();
	return L"No Owner";
}

Transform* Component::GetOwnerTransform() const
{
	if (!owner.expired()) 
		return &owner.lock()->transform()->m_worldTransform;
	return nullptr;
}

TransformComponent* Component::GetOwnerTransformComponent() const
{
	if (!owner.expired())
		return owner.lock()->GetComponent<TransformComponent>();

	return nullptr;
}

FVector2* Component::GetOwnerPivot() const
{
	if (!owner.expired())
		return owner.lock()->transform()->GetPivot();
	return nullptr;
}

Camera* Component::GetCamera()
{
	if (Scene* scene = GetWorld())
	{
		if (Camera* camera = scene->GetCamera())
		{
			return camera;
		}
	}
	return nullptr;
}