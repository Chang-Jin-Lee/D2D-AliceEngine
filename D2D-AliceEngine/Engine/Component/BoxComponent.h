#pragma once
#include "Component/RenderComponent.h"
#include "Math/TMath.h"
#include <Math/TColor.h>
class BoxComponent : public RenderComponent
{
public:
	BoxComponent() {}
	BoxComponent(const FVector2& _size, const FColor& color);
	~BoxComponent() {}

	void Initialize() override;
	void Release() override;
	void Render() override;

	void SetSize(const FVector2& _size)
	{
		m_size = _size;		
	}

	void SetColor(const FColor& color);
	FColor GetColor() { return m_color; }

	ComPtr<ID2D1SolidColorBrush> m_pBrush;
	ComPtr<ID2D1Bitmap1> m_bitmap; // BitmapImage ������Ʈ
	FColor m_color = FColor::Black;
	FVector2 m_size;
};

