#pragma once
#include "DemoGame.h"
#include "stdafx.h"

DemoGame::DemoGame()
{
	m_hwnd = nullptr;
	m_hInstance = nullptr;
	m_pInstance = this;
}

DemoGame::~DemoGame()
{

}

DemoGame* DemoGame::m_pInstance = nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	DemoGame* pThis = nullptr;

	// WM_NCCREATE: ������ ���� ���� �ʱ⿡, ������ ���� ����. WM_CREATE���� �������߻�
	if (msg == WM_NCCREATE)
	{
		// lParam�� CREATESTRUCT* �̴�
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		pThis = reinterpret_cast<DemoGame*>(cs->lpCreateParams);

		// HWND�� this ������ ����
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else {
		// WM_NCCREATE�� �ƴ� ���� HWND���� this �����͸� �����´�
		pThis = reinterpret_cast<DemoGame*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (pThis)
		pThis->MessageProc(hwnd, msg, wParam, lParam); // ��� �Լ� ȣ��

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void DemoGame::Initialize()
{
	__super::Initialize();

	// WndProc�� ����ؾ��ؼ� ���⼭ �ʱ�ȭ.
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = m_WindowName.c_str();

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // ���� ����
	RegisterClassEx(&wc);

	// ũ�⸦ ��������.
	SIZE clientSize = { (LONG)m_width,(LONG)m_height };
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

	m_hwnd = CreateWindowEx(
		0,
		m_WindowName.c_str(),
		m_TitleName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top,
		nullptr,
		nullptr,
		m_hInstance,
		this // this �����͸� lpCreateParams�� ����
	);
	ShowWindow(m_hwnd, SW_SHOW);

	CoInitialize(nullptr);
	m_pD2DRenderer->Initialize(m_hwnd);
}

void DemoGame::Run()
{
	__super::Run();
	// �޽��� ����
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

	CoUninitialize();
}

void DemoGame::MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_SPACE)
			m_pD2DRenderer->m_useScreenEffect = !m_pD2DRenderer->m_useScreenEffect;
		break;

	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
			break; // �ּ�ȭ�� ����

		UINT width = LOWORD(lParam); // �� �ʺ�
		UINT height = HIWORD(lParam); // �� ����			
		if (m_width != width || m_height != height)
		{
			m_width = width;
			m_height = height;
			m_resized = true;
		}
	}
	break;
	case WM_EXITSIZEMOVE:
		if (m_resized)
		{
			m_pD2DRenderer->Uninitialize();
			m_pD2DRenderer->Initialize(hwnd);
		}
		break;
	default:
		break;
	}
}