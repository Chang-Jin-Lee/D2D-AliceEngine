#include "pch.h"
#include "SpriteRenderer.h"
#include "Manager/D2DRenderManager.h"
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Helpers/FileHelper.h>

SpriteRenderer::~SpriteRenderer()
{
	PackageResourceManager::GetInstance().UnloadData(filePath); // ��Ʈ�� ��ε�
}

void SpriteRenderer::Initialize()
{
	__super::Initialize();
}

void SpriteRenderer::LoadData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // ���� �̸��� ����
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
}

void SpriteRenderer::Release()
{
	__super::Release();
}

/*
* @briefs : ��ȯ����� �����Ͽ� ��Ʈ���� ���ϴ� ��ġ�� �׸��ϴ�.
* @details
*	ETransformType : ��ǥ�� ����
*/

void SpriteRenderer::Render()
{
	if (m_bitmap == nullptr) return;
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	Camera* camera = SceneManager::GetCamera();
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize(); // ��Ʈ�� ũ�� �� �ǹ�
	D2D1_POINT_2F pivotOffset = {
		bmpSize.width * m_pivot->x,
		bmpSize.height * m_pivot->y
	};
	D2D1::Matrix3x2F unity = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
	D2D1::Matrix3x2F view = D2D1::Matrix3x2F::Translation(-pivotOffset.x, -pivotOffset.y);
	D2D1::Matrix3x2F world = m_pTransform->ToMatrix();
	D2D1::Matrix3x2F cameraInv = camera->m_transform->ToMatrix();

	if (D2DRenderManager::GetInstance().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity;
	}

	// ���� �ǹ� ���� ���� ��ȯ, ī�޶� ����� ����
	cameraInv.Invert();
	view = view * world * cameraInv;

	// Unity ��ǥ��� ��ȯ �߰�
	if (D2DRenderManager::GetInstance().m_eTransformType == ETransformType::Unity)
	{
		view = view * unity * D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
	}

	// ���� ��ȯ ��Ʈ�� ������ ���� �׸��� (Src ��ü ���)
	context->SetTransform(view);
	context->DrawBitmap(m_bitmap.get());
}
