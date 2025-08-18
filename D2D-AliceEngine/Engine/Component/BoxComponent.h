#pragma once
#include "Component/RenderComponent.h"
#include <Math/TColor.h>
#include <Math/TMath.h>
#include <Define/Define.h>

class BoxComponent : public RenderComponent
{
public:
	enum class EUsage { Debug, Collision };

	BoxComponent();
	BoxComponent(const FVector2& _size, const FColor& color);
	~BoxComponent();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;
	void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	void SetSize(const FVector2& _size){ m_size = _size; }
	void SetColor(const FColor& color);
	FColor GetColor() { return m_color; }
	void SetThickness(const float& _thickness) { thickness = _thickness; }
	float GetThickness() { return thickness; }
	void SetSkewAngle(const FVector2& _angle) { skewAngle = _angle; }

	void SetIgnoreOwnerScale(bool ignore) { bIgnoreOwnerScale = ignore; }
	bool GetIgnoreOwnerScale() const { return bIgnoreOwnerScale; }

	// Usage (debug rendering vs collision)
	void SetUsage(EUsage usage) { m_usage = usage; }
	EUsage GetUsage() const { return m_usage; }

	ComPtr<ID2D1SolidColorBrush> m_pBrush;
	FColor m_color;
	float thickness = 5.0f;
	FVector2 m_size;
	FVector2 skewAngle;
private:
	bool bIgnoreOwnerScale = false;
	EUsage m_usage = EUsage::Debug;
};

