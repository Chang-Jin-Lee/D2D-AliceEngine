#pragma once
#include "Object/UObject.h"
#include <Component/TransformComponent.h>

/*
* @brief 카메라 클래스. 
* @details 현재는 Object를 상속받아 기본적인 기능만 구현되어 있습니다.
*/
class Transform;
class gameObject;
class RenderComponent;
class Camera : public UObject
{
public:
	Camera();
	~Camera() {};

	void Initialize();
	void Update();
	void Release();

	FVector2 GetScale();
	void SetScale(const FVector2& scale);

	void SetPosition(const float& _x, const float& _y);
	FVector2 GetPosition();
	float GetPositionX();
	float GetPositionY();

	void SetRotation(const float& _val);

	void AddRotation(const float& _val);
	void AddPosition(const float& _x, const float& _y);

	// 상대 좌표, 스케일, 회전 관련 함수들
	void SetRelativePosition(const FVector2& _pos);
	void SetRelativeRotation(const float& _rotation);
	void SetRelativeScale(const FVector2& _scale);
	
	void AddRelativePosition(const FVector2& _pos);
	void AddRelativeRotation(const float& _rotation);
	void AddRelativeScale(const FVector2& _scale);
	
	FVector2 GetRelativePosition() const;
	float GetRelativeRotation();
	FVector2 GetRelativeScale();

	void SetOwner(gameObject* obj);
	void ClearOwner();

	// 부모-자식 관계 관리 함수들
	void AddChildObject(gameObject* obj);
	void AddChildObject(RenderComponent* obj);
	void RemoveFromParent();

	D2D1_POINT_2F TransformPoint(const D2D1_MATRIX_3X2_F& mat, const D2D1_POINT_2F& pt);

	// 좌표 변환 함수들
	D2D1_POINT_2F ViewportToWorldPoint(const D2D1_POINT_2F& viewport);
	D2D1_POINT_2F WorldToViewportPoint(const D2D1_POINT_2F& world);

	D2D1_POINT_2F ScreenToWorldPoint(const D2D1_POINT_2F& screenPos);
	D2D1_POINT_2F WorldToScreenPoint(const D2D1_POINT_2F& world);

	// D2D 스크린 픽셀 → UI 좌표계 (0~1, 좌하단 기준)
	D2D1_POINT_2F ScreenToUICoord(const D2D1_POINT_2F& screen);
	// UI 좌표계 (0~1, 좌하단 기준) → D2D 스크린 픽셀
	D2D1_POINT_2F UICoordToScreen(const D2D1_POINT_2F& ui);

	void SetFieldOfView(float& InFieldOfView) { fieldOfView = InFieldOfView; }
	float fieldOfView = 1.0f;
	gameObject* owner = nullptr;
	TransformComponent relativeTransform;
	bool bDebug = true;
  
  	// 카메라 컬링 스케일 조절
  	void SetCullingScale(const FVector2& scale) { cullingScale = scale; }
  	void SetCullingScale(float scaleX, float scaleY) { cullingScale = FVector2(scaleX, scaleY); }
  	FVector2 GetCullingScale() const { return cullingScale; }
  	FVector2 cullingScale{ 1.0f, 1.0f };

	D2D1::Matrix3x2F GetWorldToScreenMatrix();
};

