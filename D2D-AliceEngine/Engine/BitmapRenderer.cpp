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
	HRESULT hr = Application::GetInstance()->m_pD2DRenderManager.get()->CreateBitmapFromFile(
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
	m_bitmap = nullptr;
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

	ID2D1DeviceContext7* context = Application::GetInstance()->m_pD2DRenderManager->m_d2dDeviceContext.Get();
	Camera* camera = Application::GetInstance()->m_mainCamera.get();

	// ��Ʈ�� ũ�� �� �ǹ�
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	D2D1_POINT_2F pivotOffset = {
		bmpSize.width * m_pivot.x,
		bmpSize.height * m_pivot.y
	};

	D2D1::Matrix3x2F pivotTranslate = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);

	// ���� �ǹ� ���� ���� ��ȯ
	D2D1::Matrix3x2F world = m_worldTransform->ToMatrix();

	// ī�޶� ����� ����
	D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();
	cameraInv.Invert();
	D2D1::Matrix3x2F view = pivotTranslate * world * cameraInv;

	// Unity ��ǥ��� ��ȯ �߰�
	if (Application::GetInstance()->m_pD2DRenderManager->m_eTransformType == ETransformType::Unity)
	{
		float centerX = Define::SCREEN_WIDTH * 0.5f;
		float centerY = Define::SCREEN_HEIGHT * 0.5f;
		D2D1::Matrix3x2F unity = D2D1::Matrix3x2F(
			1, 0,
			0, -1,
			centerX, centerY
		);
		view = view * unity;
	}

	// ���� ��ȯ ����
	context->SetTransform(view);

	// ��Ʈ�� ������ ���� �׸��� (Src ��ü ���)
	D2D1_RECT_F src = { 0, 0, (FLOAT)bmpSize.width, (FLOAT)bmpSize.height };
	context->DrawBitmap(m_bitmap.Get(), nullptr, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &src);

	context->DrawRectangle(
		D2D1::RectF(0, 0,
			bmpSize.width, bmpSize.height), Application::GetInstance()->m_pD2DRenderManager->m_pRedBrush.Get(), 5.0f);
}
