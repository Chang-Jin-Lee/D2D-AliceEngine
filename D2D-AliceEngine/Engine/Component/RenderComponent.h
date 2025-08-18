#pragma once

#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <wrl/client.h>
#include <memory>

#include "Component.h"
#include <Define/Define.h>
#include <Math/Transform.h>
#include <Component/TransformComponent.h>
#include <Core/Renderable.h>

class RenderComponent : public Component, public Renderable
{
	friend Camera;
public:
	RenderComponent();
	~RenderComponent();
public:

	void Initialize() override;
	void Update() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;

	virtual float GetBitmapSizeX() = 0;
	virtual float GetBitmapSizeY() = 0;
	virtual void Render() override;

	void SetFlip(const bool& _flip) { bFlip = _flip; }
	void SetRelativePosition(const FVector2& _pos);
	void SetRelativeRotation(const float& _rotation);
	void SetRelativeScale(const FVector2& _scale);
	
	void AddRelativePosition(const FVector2& _pos);
	void AddRelativeRotation(const float& _rotation);
	void AddRelativeScale(const FVector2& _scale);
	
	FVector2 GetRelativePosition() const;
	float GetRelativeRotation();
	FVector2 GetRelativeScale();

	virtual FVector2 GetRelativeSize();
	virtual void AddChildComponent(WeakObjectPtr<RenderComponent> rc);
	virtual void RemoveFromParent();

	TransformComponent* GetRelativeTransform() { return &relativeTransform; }

	bool bFlip = false;
protected:
	D2D1::Matrix3x2F view;
	TransformComponent relativeTransform;
};

