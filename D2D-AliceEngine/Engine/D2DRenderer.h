#pragma once

#include "pch.h"

using namespace Microsoft::WRL;

class D2DRenderer
{
public:
	D2DRenderer();
	~D2DRenderer();
	// D2D �������� ���� �ʱ�ȭ �� ����
public:
	void Initialize(HWND hwnd, int width, int height);
	void Uninitialize();
	void Render();

	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap);

	// ���� ����
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;

	// For ImageDraw
	ComPtr<IWICImagingFactory> m_wicImagingFactory;
	ComPtr<ID2D1Bitmap1> m_d2dBitmapFromFile;
};

