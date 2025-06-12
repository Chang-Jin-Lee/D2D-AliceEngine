#include "pch.h"
#include "BitmapRenderer.h"
#include "Application.h"

void BitmapRenderer::Initialize()
{
	__super::Initialize();

	if (m_localTransform == nullptr)
	{
		m_localTransform = std::make_shared<Transform>();
	}
	if (m_worldTransform == nullptr)
	{
		m_worldTransform = std::make_shared<Transform>();
	}
}

void BitmapRenderer::LoadBitmapData(const std::wstring& path)
{
	HRESULT hr = Application::Get().m_pD2DRenderManager.get()->CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str(), &m_bitmap);
	assert(SUCCEEDED(hr));
}

void BitmapRenderer::Update()
{
	D2D1::Matrix3x2F mat;

	if (parent.lock())
	{
		mat = m_localTransform->ToMatrix() * parent.lock()->m_worldTransform->ToMatrix();
	}
	else
	{
		mat = m_localTransform->ToMatrix();
	}

	m_worldTransform->SetFromMatrix(mat);

	for (auto child : children)
	{
		if (child.lock())
		{
			child.lock()->Update();
		}
	}
}

void BitmapRenderer::Release()
{
	__super::Release();
	//m_bitmap = nullptr;
	m_localTransform.reset();
	m_worldTransform.reset();
	m_localTransform = nullptr;
	m_worldTransform = nullptr;

	for (auto child : children)
	{
		if (child.lock())
		{
			child.lock()->parent.reset();
		}
	}
}

void BitmapRenderer::Render()
{
	__super::Render();

	if (!m_bitmap)
		return;

	ID2D1DeviceContext7* context = Application::Get().m_pD2DRenderManager->m_d2dDeviceContext.Get();
	Camera* camera = Application::Get().m_mainCamera.get();

	// 비트맵 크기 및 피벗
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	D2D1_POINT_2F pivotOffset = {
		bmpSize.width * m_pivot.x,
		bmpSize.height * m_pivot.y
	};

	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);

	if (Application::Get().m_pD2DRenderManager->m_eTransformType == ETransformType::Unity)
	{
		unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
		view = view * unity;
	}

	// 로컬 피벗 기준 월드 변환
	D2D1::Matrix3x2F world = m_worldTransform->ToMatrix();

	// 카메라 역행렬 적용
	D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();
	cameraInv.Invert();
	view = view * world * cameraInv;

	// Unity 좌표계면 변환 추가
	if (Application::Get().m_pD2DRenderManager->m_eTransformType == ETransformType::Unity)
	{
		unity = unity * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
		view = view * unity;
	}

	// 최종 변환 적용
	context->SetTransform(view);

	// 비트맵 원점에 맞춰 그리기 (Src 전체 사용)
	D2D1_RECT_F src = { 0, 0, (FLOAT)bmpSize.width, (FLOAT)bmpSize.height };
	context->DrawBitmap(m_bitmap.Get(), nullptr, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &src);

	context->DrawRectangle(
		D2D1::RectF(0, 0,
			bmpSize.width, bmpSize.height), Application::Get().m_pD2DRenderManager->m_pRedBrush.Get(), 5.0f);
}
