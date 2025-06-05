#pragma once

#include "pch.h"

using namespace Microsoft::WRL;

class D2DRenderer
{
public:
	D2DRenderer();
	~D2DRenderer();
	// D2D 렌더링을 위한 초기화 및 해제

private:
	HWND m_hwnd = nullptr;
public:
	void Initialize(HWND hwnd);
	void Uninitialize();
	void Render();

	void DrawTestBrushAndShape();
	void DrawTestText();
	void DrawTestImage();
	void DrawTestShadowEffect();
	void DrawTestScreenEffect();
	void DrawTestSpriteBatch();

	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap);
	void CreateSwapChainAndD2DTarget();	// 비트맵을 여러 개 묶어주는 함수
	void OutputError(HRESULT hr);
	
	// 전역 변수
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
	ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;

	// For BrushAndShape
	ComPtr<ID2D1SolidColorBrush> m_pBlackBrush;		// 렌더타겟이 생성하는 리소스 역시 장치의존
	ComPtr<ID2D1SolidColorBrush> m_pGrayBrush;

	// For DrawText
	ComPtr<ID2D1SolidColorBrush> m_blackBrush;
	ComPtr<IDWriteFactory> m_dWriteFactory;
	ComPtr<IDWriteTextFormat> m_dWriteTextFormat;

	// For ImageDraw
	ComPtr<IWICImagingFactory> m_wicImagingFactory;
	ComPtr<ID2D1Bitmap1> m_d2dBitmapFromFile;

	// Effect
	ComPtr<ID2D1Effect> m_skewEffect;
	ComPtr<ID2D1Effect> m_shadowEffect;

	//SceneBuffer
	ComPtr<ID2D1Effect> m_sceneEffect;
	ComPtr<ID2D1Bitmap1> m_d2dBitmapScene;
	ComPtr<ID2D1Bitmap1> m_d2dBitmapBlood;
	bool m_useScreenEffect = false;

	// SpriteBatch
	ComPtr<ID2D1SpriteBatch>    g_spriteBatch;
	bool m_resizePending = false;
};

