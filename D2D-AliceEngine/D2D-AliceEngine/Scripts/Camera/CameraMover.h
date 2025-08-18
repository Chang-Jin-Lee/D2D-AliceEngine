#pragma once
#include <Component/ScriptComponent.h>
#include <Object/gameObject.h>
#include "Object/Camera.h"

class BikeMovementScript;
class SkewTransform;
class CameraMover : public ScriptComponent
{
public:
	void Initialize() override;
	void Awake() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

	FVector2 camOffSet = { 500.0f, 0.0f };

private:
	WeakObjectPtr<SkewTransform> playerST;
	WeakObjectPtr<gameObject> player;
	Camera* camera;

	// 카메라 이동 속도 제어
	float lerpSpeed = 5.0f;
	// Dead zone (카메라 내부에서 플레이어가 자유롭게 움직일 수 있는 영역)
	float deadZoneX = 50.0f;
	float deadZoneY = 30.0f;
};

