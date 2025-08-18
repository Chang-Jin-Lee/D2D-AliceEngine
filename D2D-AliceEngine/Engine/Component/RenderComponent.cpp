#include "pch.h"
#include "RenderComponent.h"
#include <Manager/D2DRenderManager.h>
#include <System/RenderSystem.h>
#include <Object/Camera.h>
#include <Manager/SceneManager.h>
#include <Component/Rigidbody2D.h>
#include <Component/TransformComponent.h>
#include <Math/Transform.h>

RenderComponent::RenderComponent()
{
	view = D2D1::Matrix3x2F::Identity();
}

RenderComponent::~RenderComponent()
{
	RenderSystem::GetInstance().UnRegist(WeakFromThis<RenderComponent>());
}

void RenderComponent::Initialize()
{
	RenderSystem::GetInstance().Regist(WeakFromThis<RenderComponent>());
	relativeTransform.Initialize();
	if (owner) owner->transform()->AddChildObject(&relativeTransform);
}

void RenderComponent::Update()
{
}

void RenderComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void RenderComponent::Release()
{
}

void RenderComponent::Render()
{
	__super::Render();
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	Camera* camera = SceneManager::GetCamera();
	if (!context || !camera) return;
	view = D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F flipY = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F screen = D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
	D2D1::Matrix3x2F world = relativeTransform.m_worldTransform.ToMatrix();
	D2D1::Matrix3x2F cameraInv = camera->relativeTransform.m_worldTransform.ToMatrix();
	cameraInv.Invert();

	if (GetDrawType() == Define::EDrawType::WorldSpace)
	{
		//view = view * unity * world * cameraInv;
		//view = view * unity * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
		// Unity 좌표계 * Owner의 월드 위치 * 카메라 변환 *  화면 중심 보정
		view = flipY * world * cameraInv * flipY * screen;
	}
	else if (GetDrawType() == Define::EDrawType::ScreenSpace)
	{
		view = world;
	}

	// 상대 트랜스폼의 스케일을 최종 뷰 행렬에 포함시키도록 보장 (이미 world에 포함되어 있으나, bIgnoreOwnerScale 같은 로컬 보정과 조합됨)

	if (bFlip)
	{
		view = D2D1::Matrix3x2F::Scale(-1.f, 1.f) * view;
	}

	context->SetTransform(view);
}

void RenderComponent::SetRelativePosition(const FVector2& _pos)
{
	relativeTransform.SetPosition(_pos.x, _pos.y);
}

void RenderComponent::SetRelativeRotation(const float& _rotation)
{
	relativeTransform.SetRotation(_rotation);
}

void RenderComponent::SetRelativeScale(const FVector2& _scale)
{
	relativeTransform.SetScale(_scale.x, _scale.y);
}

void RenderComponent::AddRelativePosition(const FVector2& _pos)
{
	FVector2 currentPos = relativeTransform.GetPosition();
	relativeTransform.SetPosition(currentPos.x + _pos.x, currentPos.y + _pos.y);
}

void RenderComponent::AddRelativeRotation(const float& _rotation)
{
	float currentRotation = relativeTransform.GetRotation();
	relativeTransform.SetRotation(currentRotation + _rotation);
}

void RenderComponent::AddRelativeScale(const FVector2& _scale)
{
	FVector2 currentScale = relativeTransform.GetScale();
	relativeTransform.SetScale(currentScale.x + _scale.x, currentScale.y + _scale.y);
}

FVector2 RenderComponent::GetRelativePosition() const
{
	//D2D1_VECTOR_2F pos = relativeTransform.GetPosition();
	FVector2 pos = relativeTransform.GetPosition();
	return FVector2(pos.x, pos.y);
}

float RenderComponent::GetRelativeRotation()
{
	return relativeTransform.GetRotation();
}

FVector2 RenderComponent::GetRelativeScale()
{
	//D2D1_VECTOR_2F scale = relativeTransform.GetScale();
	FVector2 scale = relativeTransform.GetScale();
	return FVector2(scale.x, scale.y);
}

FVector2 RenderComponent::GetRelativeSize()
{
	// 상대 Transform의 Scale
	FVector2 relativeScale = FVector2(
		relativeTransform.GetScale().x,
		relativeTransform.GetScale().y
	);

	// 최종 크기 = 기본 크기 * 부모 Scale * 상대 Scale
	return FVector2(
		relativeScale.x,
		relativeScale.y
	);
}

void RenderComponent::AddChildComponent(WeakObjectPtr<RenderComponent> rc)
{
	if (!rc.expired())
	{
		relativeTransform.AddChildObject(&rc->relativeTransform);
	}
}

void RenderComponent::RemoveFromParent()
{
	relativeTransform.RemoveFromParent();
}
