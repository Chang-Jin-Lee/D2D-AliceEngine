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

	m_Sun = std::make_shared<Object>();
	m_Sun->LoadBitmapData(L"Sun.png");
	m_Sun->m_localTransform->SetPosition(180, 100);
	m_Sun->m_localTransform->SetScale(0.5f, 0.5f);
	m_Sun->SetPivot(0.5f);
	m_pD2DRenderer->m_renderList.push_back(m_Sun);

	// 지구
	m_Earth = std::make_shared<Object>();
	m_Earth->LoadBitmapData(L"Earth.png");
	m_Earth->m_localTransform->SetPosition(300, 0);
	m_Earth->m_localTransform->SetScale(0.5f, 0.5f);
	m_Earth->SetPivot(0.5f);
	m_Sun->AddChild(m_Earth->weak_from_this());
	m_pD2DRenderer->m_renderList.push_back(m_Earth);

	// 달
	m_Moon = std::make_shared<Object>();
	m_Moon->LoadBitmapData(L"Moon.png");
	m_Moon->m_localTransform->SetPosition(0, 0);
	m_Moon->m_localTransform->SetScale(0.5f, 0.5f);
	m_Moon->SetPivot(0.5f);
	m_Earth->AddChild(m_Moon->weak_from_this());
	m_pD2DRenderer->m_renderList.push_back(m_Moon);
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
			m_Earth->m_localTransform->Rotation += 0.5f; // 지구 자전
			m_Moon->m_localTransform->Rotation -= 2.0f; // 달 자전
			m_Sun->m_localTransform->Rotation += 0.2f;
			m_Sun->UpdateWorldTransform();
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

	m_Sun->Release();
	m_Earth->Release();
	m_Moon->Release();
}