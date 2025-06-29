#pragma once
#include <Component/RenderComponent.h>

using namespace Microsoft::WRL;
class ID2D1Bitmap1;
class SpriteRenderer : public RenderComponent
{
public:
	SpriteRenderer() {}
	~SpriteRenderer();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void LoadData(const std::wstring& path);
	void Release() override;
	void Render() override;

	FVector2 GetSize();

	std::wstring filePath; // ������ ���
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
};