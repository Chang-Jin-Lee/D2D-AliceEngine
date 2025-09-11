#include "pch.h"
#include "ProgressBarComponent.h"
#include <Manager/D2DRenderManager.h>
#include <Component/RenderComponent.h>

void ProgressBarComponent::Render()
{
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (!context || !m_bitmap) return;
	// 부모(SpriteRenderer)의 Render는 비트맵 전체를 그리므로 호출하지 않음.
	// 대신 상위(RenderComponent)의 변환/상태 설정만 수행
	RenderComponent::Render();

	// 원본 스프라이트 소스 영역
	float cropW = (slice.srcW > 0) ? slice.srcW : spriteInfo.width;
	float cropH = (slice.srcH > 0) ? slice.srcH : spriteInfo.height;
	float srcL = slice.srcX;
	float srcT = slice.srcY;

	if (m_type == EProgressBarType::Linear)
	{
		// 진행 비율만큼 오른쪽에서 잘라내기
		float visibleW = cropW * m_progress;     // 소스 픽셀 기준 가시 폭
		if (visibleW <= 0.0f) return; // 전부 숨김

		// 소스 사각형: 왼쪽부터 visibleW 만큼만 보이기
		D2D1_RECT_F srcRect = {
			srcL,
			srcT,
			srcL + visibleW,
			srcT + cropH
		};

		// 화면 대상: 컴포넌트 스케일을 반영한 크기 계산
		// 전체 비트맵의 화면 폭/높이
		const float fullDestWForBitmap = SpriteRenderer::GetRelativeSize().x; // Sx * bmpW
		const float fullDestHForBitmap = SpriteRenderer::GetRelativeSize().y; // Sy * bmpH
		// 현재 crop 영역(원본의 일부분)에 해당하는 화면 폭/높이
		const float destFullWForCrop = fullDestWForBitmap * (cropW / (float)SpriteRenderer::GetBitmapSizeX());
		const float destFullHForCrop = fullDestHForBitmap * (cropH / (float)SpriteRenderer::GetBitmapSizeY());
		// 진행도에 따른 가시 폭(화면 단위)
		const float destVisibleW = destFullWForCrop * (visibleW / cropW); // == destFullWForCrop * m_progress

		// 피벗을 고려한 좌표 (좌측 고정 효과: 좌측은 변하지 않고 우측만 줄어듦)
		const float destLeft = -destFullWForCrop * spriteInfo.pivotX;
		const float destTop  = -destFullHForCrop * spriteInfo.pivotY;

		D2D1_RECT_F destRect = {
			destLeft,
			destTop,
			destLeft + destVisibleW,
			destTop + destFullHForCrop
		};

		context->DrawBitmap(m_bitmap.get(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
		return;
	}
	else // Radial
	{
		// 진행도 0 = 0, 1이면 360(2PI);
		float progressAngle = 2 * Define::PI * m_progress;
		if (!m_clockwise) progressAngle = -progressAngle;

		float startAngleRad = m_startAngleDeg * (Define::PI / 180.0f);
		float endAngleRad = startAngleRad + progressAngle;

		float centerX = cropW * 0.5f;
		float centerY = cropH * 0.5f;

		D2D1_RECT_F srcRect = {
			srcL,
			srcT,
			srcL + cropW,
			srcT + cropH
		};
		D2D1_RECT_F destRect = {
			0,
			0,
			cropW,
			cropH
		};

		ComPtr<ID2D1Geometry> pieGeometry = D2DRenderManager::GetInstance().CreatePieGeometry(
			centerX, centerY, centerX, centerY,
			startAngleRad, endAngleRad, m_clockwise);

		if (pieGeometry)
		{
			context->PushLayer(D2D1::LayerParameters(D2D1::InfiniteRect(), pieGeometry.Get()), nullptr);
		}

		// 클리핑 영역 설정
		//context->SetTransform(D2D1::Matrix3x2F::Translation(-cropW * spriteInfo.pivotX, -cropH * spriteInfo.pivotY));
		context->DrawBitmap(m_bitmap.get(), destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
		
		// 클리핑 해제 설정
		if (pieGeometry)
		{
			context->PopLayer();
		}

		return;
	}
}

void ProgressBarComponent::SetSize(const FVector2& sizePixels)
{
	// SpriteRenderer의 상대 스케일은 비트맵 픽셀 기준 비율이므로, 원하는 픽셀 사이즈 / 비트맵 사이즈로 설정
	const float bmpW = GetBitmapSizeX();
	const float bmpH = GetBitmapSizeY();
	if (bmpW <= 0 || bmpH <= 0) return;
	SetRelativeScale(FVector2(sizePixels.x / bmpW, sizePixels.y / bmpH));
}

void ProgressBarComponent::SetColor(const FColor& color)
{
	// 간단히 컬러 매트릭스 이펙트 사용
	SetFilter(color);
}
