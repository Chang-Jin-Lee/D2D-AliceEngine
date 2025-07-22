#include "pch.h"
#include "SpriteRenderer.h"
#include <Component/Component.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Helpers/FileHelper.h>
#include <Math/Transform.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>

SpriteRenderer::SpriteRenderer()
{
}

SpriteRenderer::~SpriteRenderer()
{
	PackageResourceManager::GetInstance().UnloadData(filePath); // 비트맵 언로드
}

void SpriteRenderer::Initialize()
{
	__super::Initialize();
}

void SpriteRenderer::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void SpriteRenderer::LoadData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
}

void SpriteRenderer::Release()
{
	__super::Release();
}

/*
* @briefs : 변환행렬을 적용하여 비트맵을 원하는 위치에 그립니다.
* @details
*	ETransformType : 좌표계 구분
*/

void SpriteRenderer::Render()
{
	if (m_bitmap == nullptr) return;
	__super::Render();

	// 최종 변환 비트맵 원점에 맞춰 그리기 (Src 전체 사용)
	D2D1_POINT_2F m_pivot =
	{
		GetSizeX() * GetPivot()->x,
		GetSizeY() * GetPivot()->y
	};
	D2D1_RECT_F destRect = { -m_pivot.x, -m_pivot.y,  m_pivot.x,  m_pivot.y };
	D2DRenderManager::GetD2DDevice()->DrawBitmap(m_bitmap.get(), &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
}

float SpriteRenderer::GetSizeX()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.width);
}

float SpriteRenderer::GetSizeY()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.height);
}

FVector2 SpriteRenderer::GetSize()
{
	if (!m_bitmap) return FVector2(0);
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return FVector2(static_cast<float>(bmpSize.width), static_cast<float>(bmpSize.height));
}