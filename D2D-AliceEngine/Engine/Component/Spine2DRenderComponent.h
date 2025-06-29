#pragma once
#include <Math/Transform.h>
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
	Spine2DRenderComponent();
	~Spine2DRenderComponent();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void UpdateFrames();
	void ReleaseFrames();
	void LoadData(const std::wstring& path);
	void LoadFrame(size_t frameIndex);
	void Release() override;
	void Render() override;

	FVector2 GetSize();
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

