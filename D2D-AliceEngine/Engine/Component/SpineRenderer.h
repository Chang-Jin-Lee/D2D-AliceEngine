#pragma once

#include <wrl.h>  // ComPtr 사용을 위한 헤더

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d2d1_3.h> //ID2D1Factory8,ID2D1DeviceContext7
#pragma comment(lib, "d2d1.lib")

#include <dxgi1_6.h> // IDXGIFactory7
#pragma comment(lib, "dxgi.lib")

#include <dwrite.h>
#pragma comment(lib,"dwrite.lib")

using namespace Microsoft::WRL;
using Microsoft::WRL::ComPtr;

#include <spine/Atlas.h>
#include <spine/Skeleton.h>
#include <spine/AnimationState.h>
#include <spine/AnimationStateData.h>
#include <spine/SkeletonData.h>
#include <spine/AtlasAttachmentLoader.h>
#include <spine/SkeletonJson.h>
#include <spine/TextureLoader.h>
#include <wrl/client.h>
#include <d2d1_3.h>
#include <string>
#include <vector>
#include <map>
#include <Component/RenderComponent.h>
#include <Core/Renderable.h>

// Direct2D용 TextureLoader 구현
class Direct2DTextureLoader : public spine::TextureLoader {
public:
    Direct2DTextureLoader(ID2D1RenderTarget* renderTarget);
    virtual ~Direct2DTextureLoader();
    void load(spine::AtlasPage& page, const spine::String& path) override;
    void unload(void* texture) override;
private:
    ID2D1RenderTarget* m_renderTarget;
    std::map<std::string, std::shared_ptr<ID2D1Bitmap>> m_bitmapMap;
};

class SpineRenderer : public RenderComponent
{
public:
    SpineRenderer();
    ~SpineRenderer();

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

    // 초기화
    void RegistSystem(WeakObjectPtr<UObject> object);
    virtual void Initialize() override;
    void LoadTextureLoader();
    void Shutdown();

    // 애니메이션 제어
    void SetAnimation(const std::string& animationName);
    void UpdateAnimation(float deltaTime);
    void SetAnimationTime(float time);
    float GetAnimationTime() const { return m_animationTime; }
    void SetNextAnimation();
    void SetPreviousAnimation();

    void HandleKeyInput(int keyCode);
    void Clear(const D2D1_COLOR_F& color = { 0.1f, 0.1f, 0.1f, 1.0f });
    D2D1_SIZE_F GetRenderTargetSize() const;

    // 렌더링
    void Render();

    void LoadSpine(const std::wstring& atlasPath, const std::wstring& jsonPath);

    void ReleaseSpine();

	bool GetRendered() const { return m_bRendered; }
	void SetRendered(const bool value) { m_bRendered = value; }
 
    // Spine 레이어(툴의 슬롯 순서) 사용 여부 토글
    void SetUseSpineLayerOrder(bool enable) { m_useSetupSlotOrder = enable; }
    bool GetUseSpineLayerOrder() const { return m_useSetupSlotOrder; }

private:
    D2D1::Matrix3x2F m_UnityScreen;

    // Spine-cpp 객체
    std::unique_ptr<Direct2DTextureLoader> m_textureLoader;

    std::unique_ptr<spine::Atlas> m_atlas;
    std::unique_ptr<spine::SkeletonData> m_skeletonData;
    std::unique_ptr<spine::Skeleton> m_skeleton;
    std::unique_ptr<spine::AnimationStateData> m_stateData;
    std::unique_ptr<spine::AnimationState> m_state;

    // 애니메이션 목록 및 상태
    std::vector<std::string> m_animationList;
    int m_currentAnimationIndex = 0;
    float m_animationTime = 0.0f;
    std::string m_currentAnimation = "idle";
    float m_currentAnimationTime = 0.0f;

    // 윈도우 핸들 및 크기
    HWND m_hwnd;
    int m_clientWidth = 0;
    int m_clientHeight = 0;
    bool m_initialized = false;

    // 내부 함수들(Direct2D/3D 초기화 등)
    void ReleaseResources();
    void ReleaseDirect2D();

    bool m_bRendered = true;

    // 디버그 렌더링 리소스
    ComPtr<ID2D1SolidColorBrush> m_brush;
    ComPtr<IDWriteFactory>       m_dwriteFactory;
    ComPtr<IDWriteTextFormat>    m_textFormat;

    // 툴의 슬롯 순서(레이어) 사용
    bool m_useSetupSlotOrder = true;

private:
    // Helper: 아틀라스 리전(회전/트리밍 포함) 배치 계산
    void ComputeRegionPlacement(
        spine::RegionAttachment* regionAtt,
        spine::AtlasRegion* atlasRegion,
        float srcW, float srcH, bool rotated,
        float& outDestW, float& outDestH,
        float& outAttachX, float& outAttachY,
        float& outAttachRot);

    // Helper: 소스 사각형 계산
    D2D1_RECT_F MakeSrcRect(spine::AtlasRegion* atlasRegion, bool rotated, float srcW, float srcH);
};
