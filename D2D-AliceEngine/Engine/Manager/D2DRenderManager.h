#pragma once
#include "pch.h"
#include <Core/Singleton.h>

class Component;
using namespace Define;
class D2DRenderManager : public Singleton<D2DRenderManager>
{
public:
	D2DRenderManager();
	~D2DRenderManager();

	// ������ ��⿭
	std::vector<std::vector<std::weak_ptr<Component>>> m_renderers;

	void AddRenderer(std::weak_ptr<Component> renderer);

	static ID2D1DeviceContext7* GetD2DDevice();

private:
	HWND m_hwnd = nullptr;
public:
	void Initialize(HWND hwnd);
	void UnInitialize();
	void Render();

	static bool RenderSortCompare(const std::weak_ptr<Component>& a, const std::weak_ptr<Component>& b);

	void GetApplicationSize(int& width, int& height);
	void CreateSwapChainAndD2DTarget();	// ��Ʈ���� ���� �� �����ִ� �Լ�
	void OutputError(HRESULT hr);

	void DrawDebugBox(const float& startPosX, const float& startPosY, const float& ensPosX, const float& ensPosY, const float& r, const float& g, const float& b, const float& a);

	// D2D variable
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;

	// For MemoryInfo
	ComPtr<IDXGIAdapter3> m_dxgiAdapter;
	ComPtr<IDXGIDevice3> m_dxgiDevice;

	// For DrawText
	ComPtr<IDWriteFactory> m_dWriteFactory;
	// SpriteBatch
	ComPtr<ID2D1SpriteBatch>    g_spriteBatch;
	bool m_resizePending = false;

	// Debug�� box
	ComPtr<ID2D1SolidColorBrush> m_pBrush;

	// Transform Type
	ETransformType m_eTransformType = ETransformType::Unity;
	void SwitchTransformType(ETransformType _type)
	{
		m_eTransformType = _type;
	}
};