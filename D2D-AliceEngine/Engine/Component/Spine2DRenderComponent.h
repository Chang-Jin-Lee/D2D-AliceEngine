#pragma once
#include <Math/Transform.h>
#include "Component/RenderComponent.h"
/*
*	@briefs : Spine2DRender 컴포넌트입니다.
*	@details :
*	Spine2D를 본 구조를 파악하여 그립니다.
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
	std::vector<ComPtr<ID2D1Bitmap1>> m_bitmaps; // BitmapImage 컴포넌트
	const size_t cacheSize = 2; // 캐시할 프레임 수

	void Play() { bPlay = true; }
	void Stop() { bPlay = false; }

	size_t m_curClip = 0;
	size_t m_maxClipSize = 0;
	bool bPlay = false;
	float m_fFPSTime = 1.0f / 18.0f;
	float m_fFPSLastTime = 0;
	float m_fcountOneSecond = 0;
};

