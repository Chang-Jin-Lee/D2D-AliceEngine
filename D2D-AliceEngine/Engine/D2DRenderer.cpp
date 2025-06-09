#pragma once
#include "pch.h"
#include "D2DRenderer.h"

D2DRenderer::D2DRenderer()
{
}

D2DRenderer::~D2DRenderer()
{
}

void D2DRenderer::Initialize(HWND hwnd)
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

	// hwnd�� width, height ��������
	RECT rc = {};
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

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


	m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), m_pBlackBrush.GetAddressOf());
	m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray), m_pGrayBrush.GetAddressOf());

	// Image
	// Create WIC factory
	hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(m_wicImagingFactory),
		(void**)m_wicImagingFactory.GetAddressOf());

	hr = CreateBitmapFromFile(L"../Resource/mushroom.png", m_d2dBitmapFromFile.GetAddressOf());
	assert(SUCCEEDED(hr));

	// Text
	// Brush
	m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), m_blackBrush.GetAddressOf());

	// DirectWrite ���͸��� ����ϴ�.
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_dWriteFactory),
		reinterpret_cast<IUnknown**>(m_dWriteFactory.GetAddressOf()));

	// DirectWrite �ؽ�Ʈ ���� ��ü�� ����ϴ�.
	m_dWriteFactory->CreateTextFormat(
		L"Cooper", // FontName    ������-���������-�׸�-�۲�-Ŭ�� ���� �۲��̸� Ȯ�ΰ���
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		50.0f,   // Font Size
		L"", //locale
		&m_dWriteTextFormat
	);

	// �ؽ�Ʈ�� ���� �� �������� �߾ӿ� ����ϴ�.
	m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	// Effect
	//1.AffineTransform2D ����Ʈ ����	
	hr = m_d2dDeviceContext->CreateEffect(CLSID_D2D12DAffineTransform, m_skewEffect.GetAddressOf());

	// ������ ���������� �ٲٱ� ���� ColorMatrix ��� (������)	
	hr = m_d2dDeviceContext->CreateEffect(CLSID_D2D1ColorMatrix, m_shadowEffect.GetAddressOf());

	D2D1_MATRIX_3X2_F skewMatrix = {
		1.0f, 0.0f,      // X scale, Y shear
		1.4f, 1.0f,      // X shear, Y scale
		-60.0f, 0.0f       // X, Y translation
	};

	// 3. ����Ʈ�� �Է� �̹����� ��� ����
	m_skewEffect->SetInput(0, m_d2dBitmapFromFile.Get());  // ĳ���� ��Ʈ��
	m_skewEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, skewMatrix);
	m_skewEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR);
	D2D1_MATRIX_5X4_F shadowMatrix = {
		0, 0, 0, 0,  // R
		0, 0, 0, 0,  // G
		0, 0, 0, 0,  // B
		0, 0, 0, 0.5f,  // A (���� ����)
		0, 0, 0, 0   // Offset
	};
	m_shadowEffect->SetInputEffect(0, m_skewEffect.Get());
	m_shadowEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, shadowMatrix);


	//////////////////////////////////////////////////////////////////////////
	// Screen ���û��� Effect	
	DXGI_SURFACE_DESC scDesc1;
	D2D1_SIZE_U size;
	hr = backBuffer->GetDesc(&scDesc1);
	assert(SUCCEEDED(hr));
	size.width = scDesc.Width;
	size.height = scDesc.Height;

	D2D1_BITMAP_PROPERTIES1 bmpSceneProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	hr = m_d2dDeviceContext->CreateBitmap(
		size,
		nullptr,            // �ʱ� ������ ����
		0,                  // pitch
		&bmpSceneProps,
		m_d2dBitmapScene.GetAddressOf()
	);
	assert(SUCCEEDED(hr));

	hr = CreateBitmapFromFile(L"../Resource/Blood.png", m_d2dBitmapBlood.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = m_d2dDeviceContext->CreateEffect(CLSID_D2D1Saturation, m_sceneEffect.GetAddressOf());
	m_sceneEffect->SetInput(0, m_d2dBitmapScene.Get());     // ���� ȭ��
	m_sceneEffect->SetValue(D2D1_SATURATION_PROP_SATURATION, 0.0f);


	// 4. D2D Ÿ�� ��Ʈ�� �� SetTarget
	CreateSwapChainAndD2DTarget();

	// 5. WIC Imaging Factory ����
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_wicImagingFactory));
	assert(SUCCEEDED(hr));

	// 6. Resource/mushroom.png �ε�
	hr = CreateBitmapFromFile(L"../Resource/mushroom.png", &m_d2dBitmapFromFile);
	if (FAILED(hr)) {
		OutputError(hr);
	}

	// 7. SpriteBatch ����
	hr = m_d2dDeviceContext->CreateSpriteBatch(g_spriteBatch.GetAddressOf());
	assert(SUCCEEDED(hr));
}


void D2DRenderer::Uninitialize()
{
	m_wicImagingFactory = nullptr;
	m_d2dBitmapFromFile = nullptr;

	m_d3dDevice = nullptr;
	m_dxgiSwapChain = nullptr;
	m_d2dDeviceContext = nullptr;
	m_d2dBitmapTarget = nullptr;

	// For BrushAndShape
	m_pBlackBrush = nullptr;		// ����Ÿ���� �����ϴ� ���ҽ� ���� ��ġ����
	m_pGrayBrush = nullptr;

	// For DrawText
	m_blackBrush = nullptr;
	m_dWriteFactory = nullptr;
	m_dWriteTextFormat = nullptr;

	// For ImageDraw
	m_wicImagingFactory = nullptr;
	m_d2dBitmapFromFile = nullptr;

	// Effect
	m_skewEffect = nullptr;
	m_shadowEffect = nullptr;

	//SceneBuffer
	m_sceneEffect = nullptr;
	m_d2dBitmapScene = nullptr;
	m_d2dBitmapBlood = nullptr;

	g_spriteBatch = nullptr;
}

void D2DRenderer::Render()
{
	m_d2dDeviceContext->BeginDraw();
	m_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));

	//DrawTestBrushAndShape();
	//DrawTestImage();
	//DrawTestText();
	//DrawTestShadowEffect();
	//DrawTestScreenEffect();
	//DrawTestSpriteBatch();
	DrawInRenderList();

	m_d2dDeviceContext->EndDraw();
	m_dxgiSwapChain->Present(1, 0);
}

void D2DRenderer::DrawTestBrushAndShape()
{
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // ��ȯ �ʱ�ȭ
	D2D1_SIZE_F size = m_d2dDeviceContext->GetSize();
	for (float y = 0; y < size.height; y += 10)
	{
		m_d2dDeviceContext->DrawLine(
			D2D1::Point2F(0.0f, y),
			D2D1::Point2F(size.width, y),
			m_pBlackBrush.Get(), 0.5f
		);
	}

	m_d2dDeviceContext->FillRectangle(
		D2D1::RectF(size.width / 2 - 150.0f, size.height / 2 - 150.0f,
			size.width / 2 + 150.0f, size.height / 2 + 150.0f), m_pGrayBrush.Get());

	m_d2dDeviceContext->DrawRectangle(
		D2D1::RectF(size.width / 2 - 50.0f, size.height / 2 - 50.0f,
			size.width / 2 + 50.0f, size.height / 2 + 50.0f), m_pBlackBrush.Get());
}

void D2DRenderer::DrawTestText()
{
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // ��ȯ �ʱ�ȭ
	D2D1_SIZE_F size = m_d2dDeviceContext->GetSize();

	WCHAR sc_helloWorld[] = L"Hello, World! abc";
	m_d2dDeviceContext->DrawText(
		sc_helloWorld,
		ARRAYSIZE(sc_helloWorld) - 1,
		m_dWriteTextFormat.Get(),
		D2D1::RectF(0, 0, size.width, size.height / 2),
		m_blackBrush.Get()
	);
}

void D2DRenderer::DrawTestImage()
{
	D2D1_SIZE_F size;

	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // ��ȯ �ʱ�ȭ

	//1. 0,0 ��ġ�� ��Ʈ�� ��ü���� �׸���. (��ȯ�� �ʱ�ȭ)
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get());

	//2. DestPos(ȭ�� ��ġ) ������ SrcPos(��Ʈ�� ��ġ)�� �׸���
	D2D1_VECTOR_2F DestPos{ 0,0 }, SrcPos{ 0,0 }; // ȭ�� ��ġ, ��Ʈ�� ��ġ
	size = { 0,0 };	//	�׸� ũ��
	D2D1_RECT_F DestRect{ 0,0,0,0 }, SrcRect{ 0,0,0,0 }; // ȭ�� ����, ��Ʈ�� ����
	D2D1_MATRIX_3X2_F transform;	// ��ȯ ���

	size = m_d2dBitmapFromFile->GetSize();
	DestPos = { 100,0 };
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get(), DestRect);


	//3. DestRect(�׸� ����) ������ SrcRect(��Ʈ�� �Ϻ� ����)�� �׸���
	size = { 200,200 };
	DestPos = { 100,100 };
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };

	SrcPos = { 0,0 };
	SrcRect = { SrcPos.x,SrcPos.y, SrcPos.x + size.width - 1 ,SrcPos.y + size.height - 1 };
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get(), DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);


	//4. ��ȯ�� ����� �������� DestRect(�׸� ����) ������ SrcRect(��Ʈ�� �Ϻ� ����)�� �׸���
	DestPos = { 700,100 };
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };

	transform = D2D1::Matrix3x2F::Scale(-1.0f, 1.0f,  // x�� ����
		D2D1::Point2F(DestPos.x, DestPos.y));        // ������
	m_d2dDeviceContext->SetTransform(transform);
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get(), DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);

	//5. ���պ�ȯ�� ����� �������� DestRect(�׸� ����) ������ SrcRect(��Ʈ�� �Ϻ� ����)�� �׸���
	DestPos = { 0,0 };   // �׸� ��ġ�� 0,0���� �ϰ� �̵���ȯ�� ����Ѵ�.
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };

	transform = D2D1::Matrix3x2F::Scale(1.0f, 1.0f, D2D1::Point2F(0.0f, 0.0f)) *
		D2D1::Matrix3x2F::Rotation(90.0f, D2D1::Point2F(0.0f, 0.0f)) * // 90�� ȸ��
		D2D1::Matrix3x2F::Translation(900.0f, 900.0f);  // �̵�

	// ������
	m_d2dDeviceContext->SetTransform(transform);
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get(), DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);
}

void D2DRenderer::DrawTestShadowEffect()
{
	D2D1_SIZE_F size;
	//  �׸��� ���
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(400.0f, 400.0f);
	m_d2dDeviceContext->SetTransform(translation);

	m_d2dDeviceContext->DrawImage(m_shadowEffect.Get());
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get());
}

void D2DRenderer::DrawTestScreenEffect()
{
	m_d2dDeviceContext->SetTarget(m_d2dBitmapScene.Get());
	//  �׸��� ���
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(600.0f, 600.0f);
	m_d2dDeviceContext->SetTransform(translation);

	m_d2dDeviceContext->DrawImage(m_shadowEffect.Get());

	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

	D2D1_SIZE_F size = m_d2dBitmapScene->GetSize();	//	�׸� ũ��
	D2D1_RECT_F DestRect{ 0,0,size.width,size.height };
	m_d2dDeviceContext->DrawBitmap(
		m_d2dBitmapBlood.Get(),
		DestRect,           // g_d2dBitmapScene ũ�⿡ �°� �ø�
		1.0f,              // Opacity (0.0 ~ 1.0)
		D2D1_INTERPOLATION_MODE_LINEAR,
		nullptr            // �̹��� ���� ���� ��ü ���
	);

	m_d2dDeviceContext->EndDraw();
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());
	m_d2dDeviceContext->BeginDraw();

	if (m_useScreenEffect)
		m_d2dDeviceContext->DrawImage(m_sceneEffect.Get());
	else
		m_d2dDeviceContext->DrawBitmap(m_d2dBitmapScene.Get());
}

void D2DRenderer::DrawTestSpriteBatch()
{
	// Resize �÷��װ� ��Ʈ�Ǿ����� SwapChain�� Ÿ�� ��Ʈ�� �����
	if (m_resizePending)
	{
		CreateSwapChainAndD2DTarget();
		m_resizePending = false;
	}

	// �� �� �����Ӹ��� SetTarget ȣ��
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());

	m_d2dDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED); //DrawSpriteBatch requires

	// �� BeginDraw
	//m_d2dDeviceContext->BeginDraw();
	//m_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::DarkSlateBlue));

	// �� SpriteBatch�� �̹��� �� �� ������ �׸���
	D2D1_SIZE_U bmpSize = m_d2dBitmapFromFile->GetPixelSize();
	D2D1_RECT_F destRects[3] = {
		D2D1::RectF(50,  50,  50 + (FLOAT)bmpSize.width,  50 + (FLOAT)bmpSize.height),
		D2D1::RectF(200, 100, 200 + (FLOAT)bmpSize.width, 100 + (FLOAT)bmpSize.height),
		D2D1::RectF(400, 200, 400 + (FLOAT)bmpSize.width, 200 + (FLOAT)bmpSize.height)
	};
	//D2D1_RECT_U srcRect = { 0, 0, bmpSize.width, bmpSize.height };
	D2D1_RECT_U srcRects[3] = {
	{ 0, 0, bmpSize.width, bmpSize.height },
	{ 0, 0, bmpSize.width, bmpSize.height },
	{ 0, 0, bmpSize.width, bmpSize.height }
	};

	g_spriteBatch->Clear();
	HRESULT hr = g_spriteBatch->AddSprites(3, destRects, srcRects, nullptr, nullptr);
	assert(SUCCEEDED(hr));

	m_d2dDeviceContext->DrawSpriteBatch(
		g_spriteBatch.Get(),
		0,
		3,
		m_d2dBitmapFromFile.Get(),
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1_SPRITE_OPTIONS_NONE
	);

	// �� EndDraw & ���� üũ
	hr = m_d2dDeviceContext->EndDraw();
	if (FAILED(hr)) {
		OutputError(hr);
	}
}

void D2DRenderer::DrawInRenderList()
{
	// Resize �÷��װ� ��Ʈ�Ǿ����� SwapChain�� Ÿ�� ��Ʈ�� �����
	if (m_resizePending)
	{
		CreateSwapChainAndD2DTarget();
		m_resizePending = false;
	}

	// �� �� �����Ӹ��� SetTarget ȣ��
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());
	m_d2dDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED); //DrawSpriteBatch requires

	// �� BeginDraw
	//m_d2dDeviceContext->BeginDraw();
	//m_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::DarkSlateBlue));
	
	//m_d2dDeviceContext->SetTransform(transform);
	// ���� ����Ʈ�� �ִ� ������Ʈ ���� �׸���
	D2D1_RECT_F* destRects = (D2D1_RECT_F*)malloc(sizeof(D2D1_RECT_F) * m_renderList.size());
	D2D1_RECT_U* srcRects = (D2D1_RECT_U*)malloc(sizeof(D2D1_RECT_U) * m_renderList.size());
	int index = 0;
	for (auto it = m_renderList.begin(); it != m_renderList.end(); it++)
	{
		Transform* transform = (*it)->m_localTransform;
		m_d2dDeviceContext->SetTransform(transform->ToMatrix());

		D2D1_SIZE_U bmpSize = (*it)->m_bitmapImage.GetBitmap()->GetPixelSize();
		destRects[index] = D2D1::RectF(
			transform->GetPosition().x,
			transform->GetPosition().y,
			transform->GetPosition().x + (FLOAT)bmpSize.width,
			transform->GetPosition().y + (FLOAT)bmpSize.height
		);
		srcRects[index] = { 0, 0, bmpSize.width, bmpSize.height };
	}

	g_spriteBatch->Clear();
	HRESULT hr = g_spriteBatch->AddSprites(3, destRects, srcRects, nullptr, nullptr);
	assert(SUCCEEDED(hr));

	m_d2dDeviceContext->DrawSpriteBatch(
		g_spriteBatch.Get(),
		0,
		3,
		m_d2dBitmapFromFile.Get(),
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		D2D1_SPRITE_OPTIONS_NONE
	);

	free(destRects);
	free(srcRects);

	// �� EndDraw & ���� üũ
	hr = m_d2dDeviceContext->EndDraw();
	if (FAILED(hr)) {
		OutputError(hr);
	}
}

HRESULT D2DRenderer::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap)
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

void D2DRenderer::CreateSwapChainAndD2DTarget()
{
	// 1. ���� Ÿ�� ����
	if (m_d2dDeviceContext)
	{
		m_d2dDeviceContext->SetTarget(nullptr);
	}
	m_d2dBitmapTarget.Reset();

	RECT rc = {};
	GetClientRect(m_hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

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

void D2DRenderer::OutputError(HRESULT hr)
{
	_com_error err(hr);
	OutputDebugString(err.ErrorMessage());
}
