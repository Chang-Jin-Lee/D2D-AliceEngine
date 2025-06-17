#pragma once
#include "pch.h"
#include "D2DRenderManager.h"

D2DRenderManager::D2DRenderManager()
{
}

D2DRenderManager::~D2DRenderManager()
{
}

void D2DRenderManager::Initialize(HWND hwnd)
{
	m_hwnd = hwnd;
	// D3D11 ����̽� ����
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL obtainedLevel;
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		m_d3dDevice.GetAddressOf(),
		&obtainedLevel,
		nullptr
	);
	assert(SUCCEEDED(hr));

	// D2D ���丮 �� ����̽�
	ComPtr<ID2D1Factory8> d2dFactory;
	D2D1_FACTORY_OPTIONS options = {};
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, d2dFactory.GetAddressOf());

	ComPtr<IDXGIDevice> dxgiDevice;
	m_d3dDevice.As(&dxgiDevice);

	ComPtr<ID2D1Device7> d2dDevice;
	hr = d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_d2dDeviceContext.GetAddressOf());
	assert(SUCCEEDED(hr));

	ComPtr<IDXGIFactory7> dxgiFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	int width{ 0 }, height{ 0 };
	GetApplicationSize(width, height);

	// SwapChain ����
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = width;
	scDesc.Height = height;
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 2;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	hr = dxgiFactory->CreateSwapChainForHwnd(
		m_d3dDevice.Get(),
		hwnd,
		&scDesc,
		nullptr,
		nullptr,
		m_dxgiSwapChain.GetAddressOf()
	);
	assert(SUCCEEDED(hr));

	// ����۸� Ÿ������ ����
	ComPtr<IDXGISurface> backBuffer;
	m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(scDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	m_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, m_d2dBitmapTarget.GetAddressOf());
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());
	m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), m_pRedBrush.GetAddressOf());

	// Image
	// Create WIC factory
	hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(m_wicImagingFactory),
		(void**)m_wicImagingFactory.GetAddressOf());

	// Text Brush
	m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), m_blackBrush.GetAddressOf());

	// DirectWrite ���͸��� ����ϴ�.
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_dWriteFactory),
		reinterpret_cast<IUnknown**>(m_dWriteFactory.GetAddressOf()));

	// DirectWrite �ؽ�Ʈ ���� ��ü�� ����ϴ�.
	m_dWriteFactory->CreateTextFormat(
		L"Consolas", // FontName    ������-���������-�׸�-�۲�-Ŭ�� ���� �۲��̸� Ȯ�ΰ���
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f,   // Font Size
		L"", //locale
		&m_dWriteTextFormat
	);

	// �ؽ�Ʈ�� ���� �� �������� �߾ӿ� ����ϴ�.
	m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
	m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	// 4. D2D Ÿ�� ��Ʈ�� �� SetTarget
	CreateSwapChainAndD2DTarget();

	// 5. WIC Imaging Factory ����
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_wicImagingFactory));
	assert(SUCCEEDED(hr));

	// 7. SpriteBatch ����
	hr = m_d2dDeviceContext->CreateSpriteBatch(g_spriteBatch.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void D2DRenderManager::UnInitialize()
{
	m_wicImagingFactory = nullptr;

	m_d3dDevice = nullptr;
	m_dxgiSwapChain = nullptr;
	m_d2dDeviceContext = nullptr;
	m_d2dBitmapTarget = nullptr;

	// For BrushAndShape
	m_pRedBrush = nullptr;

	// For DrawText
	m_blackBrush = nullptr;
	m_dWriteFactory = nullptr;
	m_dWriteTextFormat = nullptr;

	// For ImageDraw
	m_wicImagingFactory = nullptr;

	g_spriteBatch = nullptr;
}

void D2DRenderManager::Render()
{
	DrawInRenderList();
	DrawTestText();

	m_dxgiSwapChain->Present(1, 0);
}

void D2DRenderManager::DrawTestText()
{
	m_d2dDeviceContext->BeginDraw();
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // ��ȯ �ʱ�ȭ
	D2D1_SIZE_F size = m_d2dDeviceContext->GetSize();

	D2D1_MATRIX_3X2_F _transform = D2D1::Matrix3x2F::Scale(1.0f, 1.0f, D2D1::Point2F(0.0f, 0.0f)) *
		D2D1::Matrix3x2F::Rotation(0.0f, D2D1::Point2F(0.0f, 0.0f)) *
		D2D1::Matrix3x2F::Translation(0, 150);  // �̵�

	// ������
	m_d2dDeviceContext->SetTransform(_transform);

	WCHAR sc_helloWorld[] = L" <ī�޶�> \n [ȭ��ǥ ��,��] : ī�޶� ��,�Ʒ� �̵� \n [ȭ��ǥ ��/��] : ī�޶� ��,�� �̵� \n [1/2] : D2D, Unity ��ǥ�� \n\n <�¾�, ����, ��> \n [z,c] : �¾� ȸ�� \n [w,s] : �¾� ���� �̵� \n [a,d] : �¾� �¿� �̵� \n [b,m] : ���� ȸ�� \n [y,h] : ���� ���� �̵� \n [g,j] : ���� �¿� �̵� \n [9,0] : �� ȸ�� \n [o,l] : �� ���� �̵� \n [k.;] : �� �¿� �̵�";	m_d2dDeviceContext->DrawText(
		sc_helloWorld,
		ARRAYSIZE(sc_helloWorld) - 1,
		m_dWriteTextFormat.Get(),
		D2D1::RectF(0, 0, size.width, size.height / 2),
		m_blackBrush.Get()
	);
	m_d2dDeviceContext->EndDraw();
}

void D2DRenderManager::DrawInRenderList()
{
	m_d2dDeviceContext->BeginDraw();
	m_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));

	if (m_resizePending)
	{
		CreateSwapChainAndD2DTarget();
		m_resizePending = false;
	}
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());
	m_d2dDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	
	for (auto it = m_containerRenders.begin(); it != m_containerRenders.end(); ++it)
	{
		std::weak_ptr<RenderComponent> obj = *it;
		if (obj.lock())
		{
			obj.lock()->Render();
		}
	}
	HRESULT hr = m_d2dDeviceContext->EndDraw();
	if (FAILED(hr)) {
		OutputError(hr);
	}
	m_d2dDeviceContext->EndDraw();
}

void D2DRenderManager::GetApplicationSize(int& width, int& height)
{
	RECT rc = {};
	GetClientRect(m_hwnd, &rc);
	width = rc.right - rc.left;
	height = rc.bottom - rc.top;
}

HRESULT D2DRenderManager::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap)
{
	ComPtr<IWICBitmapDecoder>     decoder;
	ComPtr<IWICBitmapFrameDecode> frame;
	ComPtr<IWICFormatConverter>   converter;

	// �� ���ڴ� ����
	HRESULT hr = m_wicImagingFactory->CreateDecoderFromFilename(
		path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr)) return hr;

	// �� ù ������ ���
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) return hr;

	// �� ���� ��ȯ�� ����
	hr = m_wicImagingFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) return hr;

	// �� GUID_WICPixelFormat32bppPBGRA�� ��ȯ
	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);
	if (FAILED(hr)) return hr;

	// �� Direct2D ��Ʈ�� �Ӽ� (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// �� DeviceContext���� WIC ��Ʈ�����κ��� D2D1Bitmap1 ����
	hr = m_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, outBitmap);
	return hr;
}

void D2DRenderManager::CreateSwapChainAndD2DTarget()
{
	// 1. ���� Ÿ�� ����
	if (m_d2dDeviceContext)
	{
		m_d2dDeviceContext->SetTarget(nullptr);
	}
	m_d2dBitmapTarget.Reset();

	int width = 0, height = 0;
	GetApplicationSize(width, height);

	// 2. SwapChain �缳��
	if (m_dxgiSwapChain) {
		// ResizeBuffers: ����/���̰� 0�̸� ���� ���� ����, ���⼱ ���� ũ��� �ѱ�ϴ�.
		m_dxgiSwapChain->ResizeBuffers(
			0,
			width,
			height,
			DXGI_FORMAT_UNKNOWN,
			0
		);
	}

	// 3. ����� IDXGISurface ���
	ComPtr<IDXGISurface> backBuffer;
	HRESULT hr = m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		OutputError(hr);
		return;
	}

	// 4. D2D1BitmapProperties1 ���� 
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// 5. DeviceContext�� ���ε��� ID2D1Bitmap1 ����
	hr = m_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, &m_d2dBitmapTarget);
	if (FAILED(hr)) {
		OutputError(hr);
		return;
	}

	// 6. ���ؽ�Ʈ�� SetTarget
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());
}

void D2DRenderManager::OutputError(HRESULT hr)
{
	_com_error err(hr);
	OutputDebugString(err.ErrorMessage());
}
