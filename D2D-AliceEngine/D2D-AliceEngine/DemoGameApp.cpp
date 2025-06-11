#pragma once
#include "DemoGameApp.h"

DemoGameApp::DemoGameApp()
{

}

DemoGameApp::~DemoGameApp()
{

}

void DemoGameApp::Initialize()
{
	__super::Initialize();
	m_pD2DRenderManager->Initialize(m_hwnd);

	//태양
	m_sun = std::make_shared<Object>(L"Sun.png", FVector2(0, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
	m_pD2DRenderManager->AddRenderer(m_sun->GetRenderer());

	// 지구
	m_earth = std::make_shared<Object>(L"Earth.png", FVector2(500, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
	m_sun->m_bitmapRenderer->AddChild(m_earth->m_bitmapRenderer->weak_from_this());
	m_pD2DRenderManager->AddRenderer(m_earth->GetRenderer());

	// 달
	m_moon = std::make_shared<Object>(L"Moon.png", FVector2(300, 0), 0.0f, FVector2(0.5f, 0.5f), FVector2(0.5f));
	m_earth->m_bitmapRenderer->AddChild(m_moon->m_bitmapRenderer->weak_from_this());
	m_pD2DRenderManager->AddRenderer(m_moon->GetRenderer());
}

void DemoGameApp::Run()
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
			Update();
			Render();
		}
	}
}

void DemoGameApp::Render()
{
	__super::Render();
}

void DemoGameApp::Update()
{
	__super::Update();

	m_earth->m_bitmapRenderer->m_localTransform->Rotation += 0.5f; // 지구 자전
	m_moon->m_bitmapRenderer->m_localTransform->Rotation -= 2.0f; // 달 자전
	m_sun->m_bitmapRenderer->m_localTransform->Rotation += 0.2f;
	m_sun->Update();

    const std::vector<int> keys = {
     VK_Z, VK_C, VK_B, VK_M,
     VK_J, VK_G, VK_Y, VK_H,
     VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN,
     VK_1, VK_2,
     VK_W, VK_A, VK_S, VK_D,
     '9', '0', 'O', 'L', 'K', VK_OEM_1  // '9','0','O','L','K',';'
    };

    for (int key : keys)
    {
        if (GetAsyncKeyState(key) & 0x8000)
        {
            switch (key)
            {
            case VK_Z:
            {
                float rot = m_sun->m_bitmapRenderer->m_localTransform->GetRotation();
                m_sun->m_bitmapRenderer->m_localTransform->SetRotation(rot + 5.0f);
                break;
            }
            case VK_C:
            {
                float rot = m_sun->m_bitmapRenderer->m_localTransform->GetRotation();
                m_sun->m_bitmapRenderer->m_localTransform->SetRotation(rot - 5.0f);
                break;
            }
            case VK_B:
            {
                float rot = m_earth->m_bitmapRenderer->m_localTransform->GetRotation();
                m_earth->m_bitmapRenderer->m_localTransform->SetRotation(rot + 5.0f);
                break;
            }
            case VK_M:
            {
                float rot = m_earth->m_bitmapRenderer->m_localTransform->GetRotation();
                m_earth->m_bitmapRenderer->m_localTransform->SetRotation(rot - 5.0f);
                break;
            }
            case VK_J:
            {
                auto pos = m_earth->m_bitmapRenderer->m_localTransform->GetPosition();
                m_earth->m_bitmapRenderer->m_localTransform->SetPosition(pos.x + 5.0f, pos.y);
                break;
            }
            case VK_G:
            {
                auto pos = m_earth->m_bitmapRenderer->m_localTransform->GetPosition();
                m_earth->m_bitmapRenderer->m_localTransform->SetPosition(pos.x - 5.0f, pos.y);
                break;
            }
            case VK_Y:
            {
                auto pos = m_earth->m_bitmapRenderer->m_localTransform->GetPosition();
                m_earth->m_bitmapRenderer->m_localTransform->SetPosition(pos.x, pos.y + 5.0f);
                break;
            }
            case VK_H:
            {
                auto pos = m_earth->m_bitmapRenderer->m_localTransform->GetPosition();
                m_earth->m_bitmapRenderer->m_localTransform->SetPosition(pos.x, pos.y - 5.0f);
                break;
            }
            // 방향키 → m_mainCamera
            case VK_RIGHT:
            {
                auto pos = m_mainCamera->m_transform->GetPosition();
                m_mainCamera->m_transform->SetPosition(pos.x + 5.0f, pos.y);
                break;
            }
            case VK_LEFT:
            {
                auto pos = m_mainCamera->m_transform->GetPosition();
                m_mainCamera->m_transform->SetPosition(pos.x - 5.0f, pos.y);
                break;
            }
            case VK_UP:
            {
                auto pos = m_mainCamera->m_transform->GetPosition();
                m_mainCamera->m_transform->SetPosition(pos.x, pos.y - 5.0f);
                break;
            }
            case VK_DOWN:
            {
                auto pos = m_mainCamera->m_transform->GetPosition();
                m_mainCamera->m_transform->SetPosition(pos.x, pos.y + 5.0f);
                break;
            }
            case VK_1:
            {
                m_pD2DRenderManager->m_eTransformType = ETransformType::D2D;
                break;
            }
            case VK_2:
            {
                m_pD2DRenderManager->m_eTransformType = ETransformType::Unity;
                break;
            }
            // WASD → m_sun
            case VK_D:
            {
                auto pos = m_sun->m_bitmapRenderer->m_localTransform->GetPosition();
                m_sun->m_bitmapRenderer->m_localTransform->SetPosition(pos.x + 5.0f, pos.y);
                break;
            }
            case VK_A:
            {
                auto pos = m_sun->m_bitmapRenderer->m_localTransform->GetPosition();
                m_sun->m_bitmapRenderer->m_localTransform->SetPosition(pos.x - 5.0f, pos.y);
                break;
            }
            case VK_W:
            {
                auto pos = m_sun->m_bitmapRenderer->m_localTransform->GetPosition();
                m_sun->m_bitmapRenderer->m_localTransform->SetPosition(pos.x, pos.y - 5.0f);
                break;
            }
            case VK_S:
            {
                auto pos = m_sun->m_bitmapRenderer->m_localTransform->GetPosition();
                m_sun->m_bitmapRenderer->m_localTransform->SetPosition(pos.x, pos.y + 5.0f);
                break;
            }

            case '9':
            {
                float rot = m_moon->m_bitmapRenderer->m_localTransform->GetRotation();
                m_moon->m_bitmapRenderer->m_localTransform->SetRotation(rot + 5.0f);
                break;
            }
            case '0':
            {
                float rot = m_moon->m_bitmapRenderer->m_localTransform->GetRotation();
                m_moon->m_bitmapRenderer->m_localTransform->SetRotation(rot - 5.0f);
                break;
            }
            case 'O':
            {
                auto pos = m_moon->m_bitmapRenderer->m_localTransform->GetPosition();
                m_moon->m_bitmapRenderer->m_localTransform->SetPosition(pos.x, pos.y - 5.0f);
                break;
            }
            case 'L':
            {
                auto pos = m_moon->m_bitmapRenderer->m_localTransform->GetPosition();
                m_moon->m_bitmapRenderer->m_localTransform->SetPosition(pos.x, pos.y + 5.0f);
                break;
            }
            case 'K':
            {
                auto pos = m_moon->m_bitmapRenderer->m_localTransform->GetPosition();
                m_moon->m_bitmapRenderer->m_localTransform->SetPosition(pos.x - 5.0f, pos.y);
                break;
            }
            case VK_OEM_1:  // ';' 키
            {
                auto pos = m_moon->m_bitmapRenderer->m_localTransform->GetPosition();
                m_moon->m_bitmapRenderer->m_localTransform->SetPosition(pos.x + 5.0f, pos.y);
                break;
            }
            }
        }
    }

}

void DemoGameApp::Uninitialize()
{
	__super::Uninitialize();

	m_sun->Release();
	m_earth->Release();
	m_moon->Release();

	m_pD2DRenderManager->Uninitialize();
	m_pD2DRenderManager = nullptr;
	CoUninitialize();
}