#pragma once
#include "pch.h"
#include <Math/Transform.h>
#include "System/RenderSystem.h"
#include "Component/RenderComponent.h"

/*
*	@briefs : �ִϸ��̼� ������Ʈ�Դϴ�. 
*	@details : 
*	mp4 ������ ��ȯ�Ͽ� �̹����� ��ȯ�� �Ŀ� �����մϴ�.
*	���� ������ �� �� cacheSize ��ŭ�� �Ҵ�, �����ϸ� �׸��ϴ�.
*/

using namespace Microsoft::WRL;
class AnimationComponent : public RenderComponent
{
public:
	AnimationComponent() {}
	~AnimationComponent();

	void Initialize() override;
	void Update() override;
	void UpdateFrames();
	void ReleaseFrames();
	void LoadData(const std::wstring& path, const int& fps);
	void LoadFrame(size_t frameIndex);
	void Release() override;
	void Render() override;

	FVector2 GetSize()
	{
		if (m_bitmaps.empty() == false)
		{
			ComPtr<ID2D1Bitmap1> bitmapStrong;
			if (m_bitmaps[0])
			{
				D2D1_SIZE_U bmpSize = m_bitmaps[0]->GetPixelSize();
				return FVector2(bmpSize.width, bmpSize.height);
			}
			return FVector2(0,0);
		}
		return FVector2();
	}
	std::vector<std::wstring> files;
	std::wstring fileDirPath; // ���� ���� ���
	std::vector<std::shared_ptr<ID2D1Bitmap1>> m_bitmaps; // BitmapImage ������Ʈ
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

