#pragma once
#define ZPOSINIT -21359786.0f

#include "Component/ScriptComponent.h"
#include "Component/TransformComponent.h"
#include "Object/gameObject.h"

class SkewTransform : public ScriptComponent
{
public:
	WeakObjectPtr<gameObject> groundTile;
	//TransformComponent transform;

	float zPos = 0.0f;
	bool autoLayer = true;
	
	const FVector2 GetOffset();
	const FVector2 GetRealPos() { return realPos; }
	const void SetRealPos(FVector2 _pos);

	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

	// x,y 좌표를 skewTransform 좌표로 변환해주는 함수
	void ToSkewPos();
private:
	TransformComponent* renderTransform = nullptr;
	std::pair<float, float> clamp = { 0.0f, 0.0f };

	FVector2 offset = { 0.0f, 0.0f };
	FVector2 realPos = { 0.0f, 0.0f };
	FVector2 realOffset = { 0.0f, 0.0f };
	float skewDeg = 0.0f;
	float prevzPos = ZPOSINIT;

	float GetSkew();
	void SetRenderLayer();
};

