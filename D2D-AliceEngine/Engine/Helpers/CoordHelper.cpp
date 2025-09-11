#include "pch.h"
#include "CoordHelper.h"
#include <Manager/D2DRenderManager.h>
#include <Application.h>

// 정적 멤버 변수 초기화
FVector2 CoordHelper::m_cameraOffset = FVector2(0, 0);
FVector2 CoordHelper::m_screenSize = FVector2(1920, 1080);

FVector2 CoordHelper::RatioCoordToScreen(const FVector2& _pos)
{
	// UI (0~1) → 픽셀 좌표로 변환, Y축 뒤집기
	return {
		_pos.x * static_cast<float>(Application::GetInstance().GetSize().x),
		_pos.y * static_cast<float>(Application::GetInstance().GetSize().y)
	};
}

FVector2 CoordHelper::RatioCoordToScreen(const FVector2& _size, const FVector2& _pivot)
{
	return {
		_pivot.x * static_cast<float>(_size.x),
		_pivot.y * static_cast<float>(_size.y)
	};
}

D2D1_POINT_2F CoordHelper::TransformPoint(const D2D1_MATRIX_3X2_F& mat, const D2D1_POINT_2F& pt)
{
	return D2D1::Point2F(
		pt.x * mat._11 + pt.y * mat._21 + mat._31,
		pt.x * mat._12 + pt.y * mat._22 + mat._32
	);
}

D2D1_MATRIX_3X2_F CoordHelper::GetSkewMatrix(const FVector2& _angle, const float& _height)
{
	float angleDeg = _angle.x; // 예: 30도
	float angleRad = angleDeg * (Define::PI / 180.0f);

	float totalHeight = _height;
	float xOffset = totalHeight * std::tan(angleRad);

	// 기준점: 왼쪽 아래 (붙어 있게 하려면 여기 기준으로 움직임)
	D2D1_MATRIX_3X2_F skewMatrix = D2D1::Matrix3x2F::Skew(angleDeg, 0);

	// 왼쪽 아래 기준, 오른쪽 위로 이동
	D2D1_MATRIX_3X2_F originTranslate = D2D1::Matrix3x2F::Translation(0, 0);
	D2D1_MATRIX_3X2_F skewTransform =
		originTranslate *
		skewMatrix *
		D2D1::Matrix3x2F::Translation(xOffset, 0);
	return skewTransform;
}

FVector2 CoordHelper::ConvertD2DToUnity(const FVector2& d2dPos)
{
	return FVector2(
		d2dPos.x - m_screenSize.x * 0.5f,
		-(d2dPos.y - m_screenSize.y * 0.5f)
	);
}

FVector2 CoordHelper::WorldToScreen(const FVector2& worldPos)
{
	// 월드 좌표를 화면 좌표로 변환
	// 카메라 오프셋을 고려하여 변환
	FVector2 screenPos = worldPos - m_cameraOffset;
	
	// 화면 중앙을 기준으로 변환
	screenPos.x += m_screenSize.x * 0.5f;
	screenPos.y += m_screenSize.y * 0.5f;
	
	return screenPos;
}

FVector2 CoordHelper::ScreenToWorld(const FVector2& screenPos)
{
	// 화면 좌표를 월드 좌표로 변환
	FVector2 worldPos = screenPos;
	
	// 화면 중앙을 기준으로 변환
	worldPos.x -= m_screenSize.x * 0.5f;
	worldPos.y -= m_screenSize.y * 0.5f;
	
	// 카메라 오프셋을 고려하여 변환
	worldPos += m_cameraOffset;
	
	return worldPos;
}

void CoordHelper::SetCameraOffset(const FVector2& offset)
{
	m_cameraOffset = offset;
}

void CoordHelper::SetScreenSize(const FVector2& size)
{
	m_screenSize = size;
}