#pragma once
#include "pch.h"
#include "Application.h"
#include "D2DRenderManager.h"

Application::Application()
{
	m_hwnd = nullptr;
	m_hInstance = nullptr;
	m_mainCamera = nullptr;
}

Application::~Application()
{

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Application* pThis = nullptr;

	// WM_NCCREATE: 윈도우 생성 아주 초기에, 프레임 생성 전에. WM_CREATE보다 이전에발생
	if (msg == WM_NCCREATE)
	{
		// lParam은 CREATESTRUCT* 이다
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		pThis = reinterpret_cast<Application*>(cs->lpCreateParams);

		// HWND에 this 포인터 저장
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else {
		// WM_NCCREATE가 아닐 때는 HWND에서 this 포인터를 가져온다
		pThis = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (pThis)
		pThis->MessageProc(hwnd, msg, wParam, lParam); // 멤버 함수 호출

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Application::Initialize()
{
	m_pD2DRenderManager = std::make_shared<D2DRenderManager>();

	char szPath[MAX_PATH] = { 0, };
	GetModuleFileNameA(NULL, szPath, MAX_PATH); // 현재 모듈의 경로
	m_ModulePath = szPath; // 모듈 경로
	GetCurrentDirectoryA(MAX_PATH, szPath); // 현재 작업 디렉토리
	m_WorkingPath = szPath; // 작업 디렉토리
	OutputDebugStringA(std::string(std::string(m_ModulePath) + std::string("\n")).c_str());
	OutputDebugStringA(std::string(std::string(m_WorkingPath) + std::string("\n")).c_str());

	m_hInstance = GetModuleHandle(NULL); // 현재 프로세스의 핸들

	// WndProc을 등록해야해서 여기서 초기화.
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = m_WindowName.c_str();

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 배경색 설정
	RegisterClassEx(&wc);

	// 크기를 조정하자.
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
		this // this 포인터를 lpCreateParams로 전달
	);
	ShowWindow(m_hwnd, SW_SHOW);

	CoInitialize(nullptr);

	m_mainCamera = std::make_shared<Camera>();
	m_mainCamera->Initialize();
}

void Application::Run()
{

}

void Application::Update()
{

}

void Application::Render()
{
	m_pD2DRenderManager->Render();
}

void Application::Uninitialize()
{
	m_mainCamera->Release();
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
			break; // 최소화는 무시

		UINT width = LOWORD(lParam); // 새 너비
		UINT height = HIWORD(lParam); // 새 높이			
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