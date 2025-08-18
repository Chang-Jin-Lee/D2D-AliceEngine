#pragma once
#include <Math/TMath.h>
class CoordHelper
{
public:
	static FVector2 RatioCoordToScreen(const FVector2& _pos);
	static FVector2 RatioCoordToScreen(const FVector2& _size, const FVector2& _pos);
	static D2D1_POINT_2F TransformPoint(const D2D1_MATRIX_3X2_F& mat, const D2D1_POINT_2F& pt);
	static D2D1_MATRIX_3X2_F GetSkewMatrix(const FVector2& _angle, const float& _height);
	static FVector2 ConvertD2DToUnity(const FVector2& d2dPos);

	// 월드 좌표를 화면 좌표로 변환
	static FVector2 WorldToScreen(const FVector2& worldPos);

	// 화면 좌표를 월드 좌표로 변환
	static FVector2 ScreenToWorld(const FVector2& screenPos);

	// 카메라 오프셋 설정
	static void SetCameraOffset(const FVector2& offset);

	// 화면 크기 설정
	static void SetScreenSize(const FVector2& size);

private:
	static FVector2 m_cameraOffset;
	static FVector2 m_screenSize;
};

