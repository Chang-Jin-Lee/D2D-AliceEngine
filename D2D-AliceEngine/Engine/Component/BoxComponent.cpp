#include "pch.h"
#include "BoxComponent.h"
#include <Manager/D2DRenderManager.h>
#include <Manager/SceneManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Math/TMath.h>
#include <Math/TColor.h>
#include <Math/Transform.h>
#include <Component/TransformComponent.h>
#include <Helpers/CoordHelper.h>

BoxComponent::BoxComponent()
{
	SetColor(FColor::Black);
}

BoxComponent::BoxComponent(const FVector2& _size = FVector2(50,50), const FColor& color = FColor::Black)
{
	SetSize(_size);
	SetColor(color);
}

BoxComponent::~BoxComponent()
{
	m_pBrush = nullptr;
}

void BoxComponent::Initialize()
{
	__super::Initialize();

	REGISTER_TICK_TASK(Update, ETickingGroup::TG_PostPhysics);
}

void BoxComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void BoxComponent::Release()
{
	__super::Initialize();
}

void BoxComponent::Render()
{
	if (!D2DRenderManager::GetInstance().bRenderedBoxRect) return;
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (!GetOwner() || !context) return;
	__super::Render();

	// 로컬 사이즈(상대 트랜스폼 기준). 스케일은 뷰 행렬에서 적용됨
	float drawWidth = m_size.x;
	float drawHeight = m_size.y;

	D2D1_MATRIX_3X2_F prevTransform;
	context->GetTransform(&prevTransform);

	D2D1_MATRIX_3X2_F skewMatrix = CoordHelper::GetSkewMatrix(skewAngle, drawHeight);

	// 스케일 보정: 충돌용은 부모+자기 스케일 모두 무시, 디버그/일반은 옵션에 따라 부모만 무시
	D2D1_MATRIX_3X2_F localAdjust = D2D1::Matrix3x2F::Identity();
	if (m_usage == BoxComponent::EUsage::Collision)
	{
		FVector2 ownerScale(1.0f, 1.0f);
		if (auto transformComp = GetOwner() ? GetOwner()->transform() : nullptr)
			ownerScale = transformComp->GetScale();
		FVector2 selfScale = relativeTransform.GetScale();
		const float invOwnerX = (ownerScale.x != 0.0f) ? (1.0f / ownerScale.x) : 1.0f;
		const float invOwnerY = (ownerScale.y != 0.0f) ? (1.0f / ownerScale.y) : 1.0f;
		const float invSelfX  = (selfScale.x  != 0.0f) ? (1.0f / selfScale.x ) : 1.0f;
		const float invSelfY  = (selfScale.y  != 0.0f) ? (1.0f / selfScale.y ) : 1.0f;
		localAdjust = D2D1::Matrix3x2F::Scale(invOwnerX * invSelfX, invOwnerY * invSelfY);
	}
	else if (bIgnoreOwnerScale)
	{
		FVector2 ownerScale(1.0f, 1.0f);
		if (auto transformComp = GetOwner() ? GetOwner()->transform() : nullptr)
			ownerScale = transformComp->GetScale();
		const float sx = (ownerScale.x != 0.0f) ? (1.0f / ownerScale.x) : 1.0f;
		const float sy = (ownerScale.y != 0.0f) ? (1.0f / ownerScale.y) : 1.0f;
		localAdjust = D2D1::Matrix3x2F::Scale(sx, sy);
	}

	context->SetTransform(localAdjust * skewMatrix * prevTransform);

	D2D1_POINT_2F pivot = 
	{
		drawWidth * (m_usage == BoxComponent::EUsage::Collision ? 0.5f : GetOwnerPivot()->x),
		drawHeight * (m_usage == BoxComponent::EUsage::Collision ? 0.5f : GetOwnerPivot()->y)
	};

	if (D2DRenderManager::GetInstance().bRenderedBoxRect)
	{
		D2DRenderManager::GetD2DDevice()->DrawRectangle(
			D2D1::RectF(-pivot.x, -pivot.y, pivot.x, pivot.y),
			m_pBrush.Get(), thickness
		);
	}
}

float BoxComponent::GetBitmapSizeX()
{
	return m_size.x;
}

float BoxComponent::GetBitmapSizeY()
{
	return m_size.y;
}

void BoxComponent::SetColor(const FColor& color)
{
	m_color = color;
	m_pBrush = nullptr;
	D2DRenderManager::GetInstance().m_d2dDeviceContext.Get()->CreateSolidColorBrush(D2D1::ColorF(m_color.r, m_color.g, m_color.b, m_color.a), m_pBrush.GetAddressOf());
}
