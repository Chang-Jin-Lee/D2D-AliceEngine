#pragma once
#include "pch.h"
#include "Application.h"
#include "Manager/D2DRenderManager.h"

Application::Application()
{
	m_hwnd = nullptr;
	m_hInstance = nullptr;
}

Application::~Application()
{
	delete m_pD2DRenderManager;
	delete m_pRenderSystem;
	delete m_pSceneManager;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Application* pThis = nullptr;

	// WM_NCCREATE: ������ ���� ���� �ʱ⿡, ������ ���� ����. WM_CREATE���� �������߻�
	if (msg == WM_NCCREATE)
	{
		// lParam�� CREATESTRUCT* �̴�
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		pThis = reinterpret_cast<Application*>(cs->lpCreateParams);

		// HWND�� this ������ ����
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else {
		// WM_NCCREATE�� �ƴ� ���� HWND���� this �����͸� �����´�
		pThis = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (pThis)
		pThis->MessageProc(hwnd, msg, wParam, lParam); // ��� �Լ� ȣ��

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Application::Initialize()
{
	m_pD2DRenderManager = new D2DRenderManager();
	m_pRenderSystem = new RenderSystem();
	m_pSceneManager = new SceneManager();

	char szPath[MAX_PATH] = { 0, };
	GetModuleFileNameA(NULL, szPath, MAX_PATH); // ���� ����� ���
	m_ModulePath = szPath; // ��� ���
	GetCurrentDirectoryA(MAX_PATH, szPath); // ���� �۾� ���丮
	m_WorkingPath = szPath; // �۾� ���丮
	OutputDebugStringA(std::string(std::string(m_ModulePath) + std::string("\n")).c_str());
	OutputDebugStringA(std::string(std::string(m_WorkingPath) + std::string("\n")).c_str());

	m_hInstance = GetModuleHandle(NULL); // ���� ���μ����� �ڵ�

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

	m_pD2DRenderManager->Initialize(m_hwnd);
}

void Application::Run()
{

}

void Application::Update()
{
	m_pSceneManager->Update();
}

void Application::Input()
{

}

void Application::Render()
{
	m_pD2DRenderManager->Render();
}

void Application::Uninitialize()
{
	m_pRenderSystem->UnInitialize();
	m_pD2DRenderManager->UnInitialize();
	m_pD2DRenderManager = nullptr;
	m_pSceneManager->UnInitialize();
	CoUninitialize();
}

void Application::MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
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
	default:
		break;
	}
}