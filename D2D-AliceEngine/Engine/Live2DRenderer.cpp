#include "pch.h"
#include "Live2DRenderer.h"
#include "Helper/Live2DHelper.h"
#include "Application.h"
#include "Manager/TextureManager.h"
#include "LAppView.hpp"
#include "LAppTextureManager.hpp"
#include "LAppLive2DManager.hpp"
//#include "LAppLive2DManager.hpp"
#include "LAppPal.hpp"
#include "LAppAllocator_Common.hpp"

Live2DRenderer::Live2DRenderer()
{
	m_view = new LAppView();
}

Live2DRenderer::~Live2DRenderer()
{
}

void Live2DRenderer::Initialize(HWND hwnd)
{
	// 텍스처 매니저 초기화
	m_textureManager = new LAppTextureManager();

	//AppView 초기화
	int clientWidth, clientHeight;
	Application::GetInstance()->GetClientSize(clientWidth, clientHeight);
	m_view->Initialize(clientWidth, clientHeight);

	m_live2DOption.LogFunction = Live2DHelper::PrintMessage;
	m_live2DOption.LoggingLevel = Csm::CubismFramework::Option::LogLevel_Verbose;
	m_live2DOption.LoadFileFunction = Live2DHelper::LoadFileAsBytes;
	m_live2DOption.ReleaseBytesFunction = Live2DHelper::ReleaseBytes;
	Csm::CubismFramework::StartUp(&m_cubismAllocator, &m_live2DOption);

	Csm::CubismFramework::Initialize();

	Live2D::Cubism::Framework::Rendering::CubismRenderer_D3D11::InitializeConstantSettings(BackBufferNum, Application::GetInstance()->m_pD2DRenderer->m_d3dDevice.Get());

	//load model
	LAppLive2DManager::GetInstance();

	LAppPal::UpdateTime();

	m_view->InitializeSprite();
}

void Live2DRenderer::Render()
{
	// 화면 클리어 등
	StartFrame();

	// 렌더링
	m_view->Render();

	// 프레임 마지막 처리
	EndFrame();

	// 애플리케이션 종료 메시지가 오면 윈도우를 파괴
	//if (GetIsEnd() && _windowHandle != NULL)
	//{ // 윈도우 파괴
	//	DestroyWindow(_windowHandle);
	//	_windowHandle = NULL;
	//}
}

void Live2DRenderer::StartFrame()
{
    /*
     * 애플리케이션 프레임 시작 처리
     * 다른 렌더링 객체가 존재할 경우를 대비한 설정 및 렌더 타겟 클리어 수행
     */

     // 디바이스 미설정 상태
    if (!Application::GetInstance()->m_pD2DRenderer->m_d3dDevice.Get() || !Application::GetInstance()->m_pD2DRenderer->m_d3dDeviceContext.Get())
    {
        return;
    }

    // 백버퍼 클리어
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    //Application::GetInstance()->m_pD2DRenderer->m_d3dDeviceContext.Get()->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
    //Application::GetInstance()->m_pD2DRenderer->m_d3dDeviceContext.Get()->ClearRenderTargetView(_renderTargetView, clearColor);
    //Application::GetInstance()->m_pD2DRenderer->m_d3dDeviceContext.Get()->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // 깊이 테스트 비활성화
    //Application::GetInstance()->m_pD2DRenderer->m_d3dDeviceContext.Get()->OMSetDepthStencilState(_depthState, 0);
}

void Live2DRenderer::EndFrame()
{
    // 화면에 프레임 표시
    //HRESULT hr = _swapChain->Present(1, 0);
    //if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    //{ // 디바이스 상태 손실 확인
    //    _deviceStep = DeviceStep_Lost;
    //}
    //
    //// 윈도우 크기 변경 대응
    //if (_deviceStep == DeviceStep_Size)
    //{
    //    ResizeDevice();
    //}
    //
    //if (_deviceStep == DeviceStep_Lost)
    //{ // 디바이스 손실 발생
    //    LAppPal::PrintLogLn("Device Lost Abort");
    //    // 애플리케이션 종료
    //    AppEnd();
    //}

    // 지연 해제 모니터링 (리소스 관리)
    LAppLive2DManager::GetInstance()->EndFrame();
}
