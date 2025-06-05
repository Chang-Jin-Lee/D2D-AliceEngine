#include "pch.h"
#include "Live2DRenderer.h"
#include "Helper/Live2DHelper.h"
#include "Application.h"
#include "Manager/TextureManager.h"

Live2DRenderer::Live2DRenderer()
{
}

Live2DRenderer::~Live2DRenderer()
{
}

void Live2DRenderer::Initialize(HWND hwnd)
{
	static Live2DAllocater live2DAllocater;

	m_live2DOption.LogFunction = Live2DHelper::PrintMessage;
	m_live2DOption.LoggingLevel = Csm::CubismFramework::Option::LogLevel_Verbose;
	m_live2DOption.LoadFileFunction = Live2DHelper::LoadFileAsBytes;
	m_live2DOption.ReleaseBytesFunction = Live2DHelper::ReleaseBytes;
	Csm::CubismFramework::StartUp(&live2DAllocater, &m_live2DOption);

	Csm::CubismFramework::Initialize();

	Live2D::Cubism::Framework::Rendering::CubismRenderer_D3D11::InitializeConstantSettings(BackBufferNum, Application::GetInstance()->m_pD2DRenderer->m_d3dDevice.Get());

	//load model
	// LAppLive2DManager::GetInstance();

	m_live2DModel = std::make_unique<Live2DAllocater>();

}