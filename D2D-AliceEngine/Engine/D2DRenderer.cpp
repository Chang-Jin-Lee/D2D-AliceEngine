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
	// D3D11 디바이스 생성
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

	// D2D 팩토리 및 디바이스
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

	// hwnd의 width, height 가져오기
	RECT rc = {};
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// SwapChain 생성
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

	// 백버퍼를 타겟으로 설정
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

	// DirectWrite 팩터리를 만듭니다.
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(m_dWriteFactory),
		reinterpret_cast<IUnknown**>(m_dWriteFactory.GetAddressOf()));

	// DirectWrite 텍스트 형식 개체를 만듭니다.
	m_dWriteFactory->CreateTextFormat(
		L"Cooper", // FontName    제어판-모든제어판-항목-글꼴-클릭 으로 글꼴이름 확인가능
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		50.0f,   // Font Size
		L"", //locale
		&m_dWriteTextFormat
	);

	// 텍스트를 수평 및 수직으로 중앙에 맞춥니다.
	m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);


	// Effect
	//1.AffineTransform2D 이펙트 생성	
	hr = m_d2dDeviceContext->CreateEffect(CLSID_D2D12DAffineTransform, m_skewEffect.GetAddressOf());

	// 반투명 검정색으로 바꾸기 위해 ColorMatrix 사용 (선택적)	
	hr = m_d2dDeviceContext->CreateEffect(CLSID_D2D1ColorMatrix, m_shadowEffect.GetAddressOf());

	D2D1_MATRIX_3X2_F skewMatrix = {
		1.0f, 0.0f,      // X scale, Y shear
		1.4f, 1.0f,      // X shear, Y scale
		-60.0f, 0.0f       // X, Y translation
	};

	// 3. 이펙트에 입력 이미지와 행렬 설정
	m_skewEffect->SetInput(0, m_d2dBitmapFromFile.Get());  // 캐릭터 비트맵
	m_skewEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, skewMatrix);
	m_skewEffect->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR);
	D2D1_MATRIX_5X4_F shadowMatrix = {
		0, 0, 0, 0,  // R
		0, 0, 0, 0,  // G
		0, 0, 0, 0,  // B
		0, 0, 0, 0.5f,  // A (투명도 조절)
		0, 0, 0, 0   // Offset
	};
	m_shadowEffect->SetInputEffect(0, m_skewEffect.Get());
	m_shadowEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, shadowMatrix);


	//////////////////////////////////////////////////////////////////////////
	// Screen 관련생성 Effect	
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
		nullptr,            // 초기 데이터 없음
		0,                  // pitch
		&bmpSceneProps,
		m_d2dBitmapScene.GetAddressOf()
	);
	assert(SUCCEEDED(hr));

	hr = CreateBitmapFromFile(L"../Resource/Blood.png", m_d2dBitmapBlood.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = m_d2dDeviceContext->CreateEffect(CLSID_D2D1Saturation, m_sceneEffect.GetAddressOf());
	m_sceneEffect->SetInput(0, m_d2dBitmapScene.Get());     // 원래 화면
	m_sceneEffect->SetValue(D2D1_SATURATION_PROP_SATURATION, 0.0f);


	// 4. D2D 타겟 비트맵 및 SetTarget
	CreateSwapChainAndD2DTarget();

	// 5. WIC Imaging Factory 생성
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&m_wicImagingFactory));
	assert(SUCCEEDED(hr));

	// 6. Resource/mushroom.png 로드
	hr = CreateBitmapFromFile(L"../Resource/mushroom.png", &m_d2dBitmapFromFile);
	if (FAILED(hr)) {
		OutputError(hr);
	}

	// 7. SpriteBatch 생성
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
	m_pBlackBrush = nullptr;		// 렌더타겟이 생성하는 리소스 역시 장치의존
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

	DrawTestBrushAndShape();
	DrawTestImage();
	DrawTestText();
	DrawTestShadowEffect();
	DrawTestScreenEffect();
	DrawTestSpriteBatch();

	m_d2dDeviceContext->EndDraw();
	m_dxgiSwapChain->Present(1, 0);
}

void D2DRenderer::DrawTestBrushAndShape()
{
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // 변환 초기화
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
	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // 변환 초기화
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

	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // 변환 초기화

	//1. 0,0 위치에 비트맵 전체영역 그린다. (변환은 초기화)
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get());

	//2. DestPos(화면 위치) 설정과 SrcPos(비트맵 위치)로 그리기
	D2D1_VECTOR_2F DestPos{ 0,0 }, SrcPos{ 0,0 }; // 화면 위치, 비트맵 위치
	size = { 0,0 };	//	그릴 크기
	D2D1_RECT_F DestRect{ 0,0,0,0 }, SrcRect{ 0,0,0,0 }; // 화면 영역, 비트맵 영역
	D2D1_MATRIX_3X2_F transform;	// 변환 행렬

	size = m_d2dBitmapFromFile->GetSize();
	DestPos = { 100,0 };
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get(), DestRect);


	//3. DestRect(그릴 영역) 설정과 SrcRect(비트맵 일부 영역)로 그리기
	size = { 200,200 };
	DestPos = { 100,100 };
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };

	SrcPos = { 0,0 };
	SrcRect = { SrcPos.x,SrcPos.y, SrcPos.x + size.width - 1 ,SrcPos.y + size.height - 1 };
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get(), DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);


	//4. 변환을 사용한 반전으로 DestRect(그릴 영역) 설정과 SrcRect(비트맵 일부 영역)로 그리기
	DestPos = { 700,100 };
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };

	transform = D2D1::Matrix3x2F::Scale(-1.0f, 1.0f,  // x축 반전
		D2D1::Point2F(DestPos.x, DestPos.y));        // 기준점
	m_d2dDeviceContext->SetTransform(transform);
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get(), DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);

	//5. 복합변환을 사용한 반전으로 DestRect(그릴 영역) 설정과 SrcRect(비트맵 일부 영역)로 그리기
	DestPos = { 0,0 };   // 그릴 위치는 0,0으로 하고 이동변환을 사용한다.
	DestRect = { DestPos.x , DestPos.y, DestPos.x + size.width - 1 ,DestPos.y + size.height - 1 };

	transform = D2D1::Matrix3x2F::Scale(1.0f, 1.0f, D2D1::Point2F(0.0f, 0.0f)) *
		D2D1::Matrix3x2F::Rotation(90.0f, D2D1::Point2F(0.0f, 0.0f)) * // 90도 회전
		D2D1::Matrix3x2F::Translation(900.0f, 900.0f);  // 이동

	// 기준점
	m_d2dDeviceContext->SetTransform(transform);
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get(), DestRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &SrcRect);
}

void D2DRenderer::DrawTestShadowEffect()
{
	D2D1_SIZE_F size;
	//  그림자 출력
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(400.0f, 400.0f);
	m_d2dDeviceContext->SetTransform(translation);

	m_d2dDeviceContext->DrawImage(m_shadowEffect.Get());
	m_d2dDeviceContext->DrawBitmap(m_d2dBitmapFromFile.Get());
}

void D2DRenderer::DrawTestScreenEffect()
{
	m_d2dDeviceContext->SetTarget(m_d2dBitmapScene.Get());
	//  그림자 출력
	D2D1::Matrix3x2F translation = D2D1::Matrix3x2F::Translation(600.0f, 600.0f);
	m_d2dDeviceContext->SetTransform(translation);

	m_d2dDeviceContext->DrawImage(m_shadowEffect.Get());

	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

	D2D1_SIZE_F size = m_d2dBitmapScene->GetSize();	//	그릴 크기
	D2D1_RECT_F DestRect{ 0,0,size.width,size.height };
	m_d2dDeviceContext->DrawBitmap(
		m_d2dBitmapBlood.Get(),
		DestRect,           // g_d2dBitmapScene 크기에 맞게 늘림
		1.0f,              // Opacity (0.0 ~ 1.0)
		D2D1_INTERPOLATION_MODE_LINEAR,
		nullptr            // 이미지 원본 영역 전체 사용
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
	// Resize 플래그가 세트되었으면 SwapChain과 타겟 비트맵 재생성
	if (m_resizePending) 
	{
		CreateSwapChainAndD2DTarget();
		m_resizePending = false;
	}

	// ① 매 프레임마다 SetTarget 호출
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());

	m_d2dDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED); //DrawSpriteBatch requires

	// ② BeginDraw
	//m_d2dDeviceContext->BeginDraw();
	//m_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::DarkSlateBlue));

	// ③ SpriteBatch로 이미지 세 번 나란히 그리기
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

	// ④ EndDraw & 에러 체크
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

	// ① 디코더 생성
	HRESULT hr = m_wicImagingFactory->CreateDecoderFromFilename(
		path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr)) return hr;

	// ② 첫 프레임 얻기
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) return hr;

	// ③ 포맷 변환기 생성
	hr = m_wicImagingFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) return hr;

	// ④ GUID_WICPixelFormat32bppPBGRA로 변환
	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);
	if (FAILED(hr)) return hr;

	// ⑤ Direct2D 비트맵 속성 (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// ⑥ DeviceContext에서 WIC 비트맵으로부터 D2D1Bitmap1 생성
	hr = m_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, outBitmap);
	return hr;
}

void D2DRenderer::CreateSwapChainAndD2DTarget()
{
	// 1. 현재 타겟 해제
	if (m_d2dDeviceContext) 
	{
		m_d2dDeviceContext->SetTarget(nullptr);
	}
	m_d2dBitmapTarget.Reset();

	RECT rc = {};
	GetClientRect(m_hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// 2. SwapChain 재설정
	if (m_dxgiSwapChain) {
		// ResizeBuffers: 넓이/높이가 0이면 현재 설정 유지, 여기선 실제 크기로 넘깁니다.
		m_dxgiSwapChain->ResizeBuffers(
			0,
			width,
			height,
			DXGI_FORMAT_UNKNOWN,
			0
		);
	}

	// 3. 백버퍼 IDXGISurface 얻기
	ComPtr<IDXGISurface> backBuffer;
	HRESULT hr = m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(hr)) {
		OutputError(hr);
		return;
	}

	// 4. D2D1BitmapProperties1 생성 
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// 5. DeviceContext에 바인딩할 ID2D1Bitmap1 생성
	hr = m_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, &m_d2dBitmapTarget);
	if (FAILED(hr)) {
		OutputError(hr);
		return;
	}

	// 6. 컨텍스트에 SetTarget
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());
}

void D2DRenderer::OutputError(HRESULT hr)
{
	_com_error err(hr);
	OutputDebugString(err.ErrorMessage());
}

void Live2DLogger(const char* message)
{
	OutputDebugStringA(message);  // Windows용 디버그 출력
	printf("[Live2D] %s\n", message);  // 콘솔 출력
}

void D2DRenderer::Live2DInitialize(HWND hwnd)
{
	static Live2DAllocater live2DAllocater;

	Csm::CubismFramework::Option option;
	option.LogFunction = Live2DLogger;
	option.LoggingLevel = Csm::CubismFramework::Option::LogLevel_Verbose;

	Csm::CubismFramework::StartUp(&live2DAllocater, &option);
	Csm::CubismFramework::Initialize();

	m_live2DModel = std::make_unique<Live2DAllocater>();

}