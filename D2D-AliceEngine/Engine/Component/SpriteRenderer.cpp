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
	spriteInfo.height = -1;
	spriteInfo.width = -1;
	spriteInfo.x = 0;
	spriteInfo.y = 0;
	spriteInfo.pivotX = 0.5f;
	spriteInfo.pivotY = 0.5f;
	spriteInfo.name = "";
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

	spriteInfo.height = GetBitmapSizeY();
	spriteInfo.width = GetBitmapSizeX();
	spriteInfo.pivotX = 0.5f;
	spriteInfo.pivotY = 0.5f;
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
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (!m_bitmap || !context) return;
	__super::Render();

	// 잘라올 영역 결정 ― 값이 -1이면 원본 전부
	float cropW = (slice.srcW > 0) ? slice.srcW : spriteInfo.width;
	float cropH = (slice.srcH > 0) ? slice.srcH : spriteInfo.height;
	float srcL = slice.srcX;
	float srcT = slice.srcY;
	D2D1_RECT_F srcRect = { srcL,               srcT,
							 srcL + cropW,       srcT + cropH };
	// 화면에 그릴 위치(센터링 포함)
	float offsetX = -cropW * spriteInfo.pivotX;
	float offsetY = -cropH * spriteInfo.pivotY;

	// 이펙트 있을시 이펙트 그리기
	if (!m_effect)
	{
		FVector2 relativeSize = GetBitmapSize();
		D2D1_RECT_F destRect = D2D1::RectF(-relativeSize.x / 2, -relativeSize.y / 2, relativeSize.x / 2, relativeSize.y / 2);
		context->DrawBitmap(m_bitmap.get(), destRect);
	}
	else {
 
		D2D1_POINT_2F destPos = D2D1::Point2F(offsetX, offsetY);
		context->DrawImage(m_effect.Get(), &destPos, &srcRect);

	}
		
}

float SpriteRenderer::GetBitmapSizeX()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.width);
}

float SpriteRenderer::GetBitmapSizeY()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.height);
}

FVector2 SpriteRenderer::GetBitmapSize()
{
	if (!m_bitmap) return FVector2(0);
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return FVector2(static_cast<float>(bmpSize.width), static_cast<float>(bmpSize.height));
}

FVector2 SpriteRenderer::GetRelativeSize()
{
	FVector2 relativeSize = __super::GetRelativeSize();
	relativeSize.x *= GetBitmapSizeX();
	relativeSize.y *= GetBitmapSizeY();;
	return relativeSize;
}

void SpriteRenderer::SetSlice(float x, float y, float w, float h)
{
	slice.srcX = x;  slice.srcY = y;
	slice.srcW = w;  slice.srcH = h;
}

void SpriteRenderer::SetSkewing(bool _isSkewing, FVector2 _skewAngle /*= FVector2(0.0f, 0.0f)*/)
{
	
}

void SpriteRenderer::SetOpacity(float alpha)
{
    ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
    if (!context || !m_bitmap) {
        m_effect.Reset();
        return;
    }

    // 0..1 clamp
    const float a = (alpha < 0.f) ? 0.f : (alpha > 1.f ? 1.f : alpha);

    ComPtr<ID2D1Effect> opacityEffect;
    if (SUCCEEDED(context->CreateEffect(CLSID_D2D1Opacity, &opacityEffect)))
    {
        opacityEffect->SetValue(D2D1_OPACITY_PROP_OPACITY, a);
        opacityEffect->SetInput(0, m_bitmap.get());
        m_effect = opacityEffect; // Render 단계에서 DrawImage로 그려짐
    }
}

void SpriteRenderer::SetFilter(float rMul, float gMul, float bMul)
{
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (!context || !m_bitmap) {
		m_effect.Reset();
		return;
	}

	float r = rMul / 255.0f;
	float g = gMul / 255.0f;
	float b = bMul / 255.0f;

	ComPtr<ID2D1Effect> colorEffect;
	if (SUCCEEDED(context->CreateEffect(CLSID_D2D1ColorMatrix, &colorEffect)))
	{
		// 색상 곱셈 행렬 생성
		D2D1_MATRIX_5X4_F matrix = D2D1::Matrix5x4F(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 1,
			rMul, gMul, bMul, 0
		);

		colorEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);
		colorEffect->SetInput(0, m_bitmap.get());
		m_effect = colorEffect; // Render에서 자동 적용
	}
}

void SpriteRenderer::SetFilter(FColor color)
{
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (!context || !m_bitmap)
	{
		m_effect.Reset();
		return;
	}

	ComPtr<ID2D1Effect> colorEffect;
	if (SUCCEEDED(context->CreateEffect(CLSID_D2D1ColorMatrix, &colorEffect)))
	{
		// FColor 기반 RGB 곱셈 매트릭스 생성

		float r = color.r / 255.0f;
		float g = color.g / 255.0f;
		float b = color.b / 255.0f;
		float a = color.a / 255.0f;

		D2D1_MATRIX_5X4_F matrix = D2D1::Matrix5x4F(
			0, 0, 0, 0,     
			0, 0, 0, 0,     
			0, 0, 0, 0,     
			0, 0, 0, a,  
			r, g, b, 0      // 지정된 색상으로 오프셋(덧셈)
		);

		colorEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);
		colorEffect->SetInput(0, m_bitmap.get());
		m_effect = colorEffect; // Render에서 DrawImage로 자동 적용
	}
}
//void SpriteRenderer::SetFilter(Filter filter)
//{
//	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
//	if (!context || !m_bitmap)
//	{
//		m_effect.Reset();
//		return;
//	}
//
//	ComPtr<ID2D1Effect> filterEffect;
//	if (SUCCEEDED(context->CreateEffect(CLSID_D2D1ColorMatrix, &filterEffect)))
//	{
//		D2D1_MATRIX_5X4_F matrix;
//
//		switch (filter)
//		{
//		case Filter::None:
//			matrix = D2D1::Matrix5x4F(
//				1, 0, 0, 0,
//				0, 1, 0, 0,
//				0, 0, 1, 0,
//				0, 0, 0, 1,
//				0, 0, 0, 0
//			);
//			break;
//
//		case Filter::Grayscale:
//			matrix = D2D1::Matrix5x4F(
//				0.3f, 0.59f, 0.11f, 0,
//				0.3f, 0.59f, 0.11f, 0,
//				0.3f, 0.59f, 0.11f, 0,
//				0, 0, 0, 1,
//				0, 0, 0, 0
//			);
//			break;
//
//		case Filter::Sepia:
//			matrix = D2D1::Matrix5x4F(
//				0.393f, 0.769f, 0.189f, 0,
//				0.349f, 0.686f, 0.168f, 0,
//				0.272f, 0.534f, 0.131f, 0,
//				0, 0, 0, 1,
//				0, 0, 0, 0
//			);
//			break;
//
//		case Filter::Invert:
//			matrix = D2D1::Matrix5x4F(
//				-1, 0, 0, 0,
//				0, -1, 0, 0,
//				0, 0, -1, 0,
//				0, 0, 0, 1,
//				1, 1, 1, 0
//			);
//			break;
//
//		case Filter::Brighten:
//			matrix = D2D1::Matrix5x4F(
//				1.2f, 0, 0, 0,
//				0, 1.2f, 0, 0,
//				0, 0, 1.2f, 0,
//				0, 0, 0, 1,
//				0, 0, 0, 0
//			);
//			break;
//
//		case Filter::Darken:
//			matrix = D2D1::Matrix5x4F(
//				0.8f, 0, 0, 0,
//				0, 0.8f, 0, 0,
//				0, 0, 0.8f, 0,
//				0, 0, 0, 1,
//				0, 0, 0, 0
//			);
//			break;
//		}
//
//		filterEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);
//		filterEffect->SetInput(0, m_bitmap.get());
//		m_effect = filterEffect;
//	}
//}
