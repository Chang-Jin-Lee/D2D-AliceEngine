#pragma once
#include <Component/RenderComponent.h>
#include <Animation/TextureLoader.h>

using namespace Microsoft::WRL;

struct ID2D1Bitmap1;
class SpriteRenderer : public RenderComponent
{
public:
	SpriteRenderer();
	~SpriteRenderer();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void LoadData(const std::wstring& path);
	void Release() override;
	void Render() override;

	virtual float GetSizeX() override;
	virtual float GetSizeY() override;

	FVector2 GetSize();

	std::wstring filePath; // 파일의 경로
	std::shared_ptr<ID2D1Bitmap1> m_bitmap;
};