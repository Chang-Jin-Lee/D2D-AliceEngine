#pragma once
#include <Math/Transform.h>
#include "System/RenderSystem.h"
#include "Component/RenderComponent.h"

/*
*	@briefs : Spine2DRender ������Ʈ�Դϴ�.
*	@details :
*	Spine2D�� �� ������ �ľ��Ͽ� �׸��ϴ�.
*/

using namespace Microsoft::WRL;

class Spine2DRenderComponent : public RenderComponent
{
public:
	Spine2DRenderComponent() {}
	~Spine2DRenderComponent()
	{
		for (auto m_bitmap : m_bitmaps)
		{
			m_bitmap = nullptr;
		}
		files.clear();
		m_bitmaps.clear();
	}

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void UpdateFrames();
	void ReleaseFrames();
	void LoadData(const std::wstring& path);
	void LoadFrame(size_t frameIndex);
	void Release() override;
	void Render() override;

	FVector2 GetSize()
	{
		if (m_bitmaps.empty() == false)
		{
			D2D1_SIZE_U bmpSize = m_bitmaps[0]->GetPixelSize();
			return FVector2(bmpSize.width, bmpSize.height);
		}
		return FVector2();
	}
	std::vector<std::wstring> files;
	std::vector<ComPtr<ID2D1Bitmap1>> m_bitmaps; // BitmapImage ������Ʈ
	const size_t cacheSize = 2; // ĳ���� ������ ��

	void Play() { bPlay = true; }
	void Stop() { bPlay = false; }

	size_t m_curClip = 0;
	size_t m_maxClipSize = 0;
	bool bPlay = false;
	float m_fFPSTime = 1.0f / 18.0f;
	float m_fFPSLastTime = 0;
	float m_fcountOneSecond = 0;
};

