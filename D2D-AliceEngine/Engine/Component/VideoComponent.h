#pragma once
#include <Component/RenderComponent.h>

/*
*	@briefs : 애니메이션 컴포넌트입니다. 
*	@details : 
*	mp4 파일을 변환하여 이미지로 변환한 후에 실행합니다.
*	현재 프레임 앞 뒤 cacheSize 만큼을 할당, 해제하며 그립니다.
*/

using namespace Microsoft::WRL;
class VideoComponent : public RenderComponent
{
public:
	VideoComponent();
	~VideoComponent();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void UpdateFrames(const float& deltaSeconds);
	void ReleaseFrames();
	// quality :
	//  png 일때는 압축률
	//  jpg 일때는 품질
	void LoadData(const std::wstring& path, const int& fps, const std::wstring& extension = L"jpg", const int& quality = 95, const bool& bIsBackGround = false);
	void LoadFrame(size_t frameIndex);
	void Release() override;
	void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	FVector2 GetSize();
	std::vector<std::wstring> files;
	std::wstring fileDirPath; // 비디오 파일 경로
	std::vector<std::shared_ptr<ID2D1Bitmap1>> m_bitmaps; // BitmapImage 컴포넌트
	const size_t cacheSize = 2; // 캐시할 프레임 수

	void Play() { bPlay = true; }
	void Stop() { bPlay = false; }

	size_t m_curClip = 0;
	size_t m_maxClipSize = 0;
	bool bPlay = false;
	float m_fFPSTime = 1.0f / 18.0f;
	float m_fFPSLastTime = 0;
};