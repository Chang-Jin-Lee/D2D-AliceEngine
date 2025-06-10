#include "pch.h"
#include "Object.h"
#include "Application.h"

void Object::Initialize()
{
	if (m_localTransform == nullptr)
	{
		m_localTransform = std::make_shared<Transform>();
	}
	if (m_worldTransform == nullptr)
	{
		m_worldTransform = std::make_shared<Transform>();
	}
}

void Object::LoadBitmapData(const std::wstring& path)
{
	HRESULT hr = Application::GetInstance()->m_pD2DRenderer.get()->CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str(), &m_bitmap);
	assert(SUCCEEDED(hr));
}

void Object::Release()
{
	m_localTransform.reset();
	m_worldTransform.reset();
	m_localTransform = nullptr;
	m_worldTransform = nullptr;
	//m_bitmap = nullptr;
	for (auto child : children)
	{
		child->parent.reset();
	}
}

void Object::UpdateWorldTransform()
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
		child->UpdateWorldTransform();
}

void Object::Render()
{
	if (!m_bitmap)
		return;

	ID2D1DeviceContext7* context = Application::GetInstance()->m_pD2DRenderer->m_d2dDeviceContext.Get();
	Camera* camera = Application::GetInstance()->m_mainCamera.get();

	// ��Ʈ�� ũ�� �� �ǹ�
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	D2D1_POINT_2F localPivot = {
		bmpSize.width * m_pivot.x,
		bmpSize.height * m_pivot.y
	};

	// ���� �ǹ� ���� ���� ��ȯ
	D2D1::Matrix3x2F world = m_worldTransform->ToMatrix(localPivot);

	// ī�޶� ����� ����
	D2D1::Matrix3x2F cameraInv = camera->m_worldTransform->ToMatrix();
	cameraInv.Invert();
	D2D1::Matrix3x2F view = world * cameraInv;

	// Unity ��ǥ��� ��ȯ �߰�
	if (Application::GetInstance()->m_pD2DRenderer->m_eTransformType == ETransformType::Unity)
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
}
