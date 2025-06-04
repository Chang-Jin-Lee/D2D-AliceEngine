#pragma once
#include "DemoGame.h"

DemoGame::DemoGame()
{
	
}

DemoGame::~DemoGame()
{

}

void DemoGame::Initialize()
{
	__super::Initialize();
	m_pD2DRenderer->Initialize(m_hwnd);
}

void DemoGame::Run()
{
	__super::Run();
	// 메시지 루프
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			m_pD2DRenderer->Render();
		}
	}
}

void DemoGame::Uninitialize()
{
	__super::Uninitialize();

	m_pD2DRenderer->Uninitialize();
	m_pD2DRenderer = nullptr;
	CoUninitialize();
}