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
	// �ؽ�ó �Ŵ��� �ʱ�ȭ
	m_textureManager = new LAppTextureManager();

	//AppView �ʱ�ȭ
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
	// ȭ�� Ŭ���� ��
	StartFrame();

	// ������
	m_view->Render();

	// ������ ������ ó��
	EndFrame();

	// ���ø����̼� ���� �޽����� ���� �����츦 �ı�
	//if (GetIsEnd() && _windowHandle != NULL)
	//{ // ������ �ı�
	//	DestroyWindow(_windowHandle);
	//	_windowHandle = NULL;
	//}
}

void Live2DRenderer::StartFrame()
{
    /*
     * ���ø����̼� ������ ���� ó��
     * �ٸ� ������ ��ü�� ������ ��츦 ����� ���� �� ���� Ÿ�� Ŭ���� ����
     */

     // ����̽� �̼��� ����
    if (!Application::GetInstance()->m_pD2DRenderer->m_d3dDevice.Get() || !Application::GetInstance()->m_pD2DRenderer->m_d3dDeviceContext.Get())
    {
        return;
    }

    // ����� Ŭ����
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    //Application::GetInstance()->m_pD2DRenderer->m_d3dDeviceContext.Get()->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
    //Application::GetInstance()->m_pD2DRenderer->m_d3dDeviceContext.Get()->ClearRenderTargetView(_renderTargetView, clearColor);
    //Application::GetInstance()->m_pD2DRenderer->m_d3dDeviceContext.Get()->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // ���� �׽�Ʈ ��Ȱ��ȭ
    //Application::GetInstance()->m_pD2DRenderer->m_d3dDeviceContext.Get()->OMSetDepthStencilState(_depthState, 0);
}

void Live2DRenderer::EndFrame()
{
    // ȭ�鿡 ������ ǥ��
    //HRESULT hr = _swapChain->Present(1, 0);
    //if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    //{ // ����̽� ���� �ս� Ȯ��
    //    _deviceStep = DeviceStep_Lost;
    //}
    //
    //// ������ ũ�� ���� ����
    //if (_deviceStep == DeviceStep_Size)
    //{
    //    ResizeDevice();
    //}
    //
    //if (_deviceStep == DeviceStep_Lost)
    //{ // ����̽� �ս� �߻�
    //    LAppPal::PrintLogLn("Device Lost Abort");
    //    // ���ø����̼� ����
    //    AppEnd();
    //}

    // ���� ���� ����͸� (���ҽ� ����)
    LAppLive2DManager::GetInstance()->EndFrame();
}
