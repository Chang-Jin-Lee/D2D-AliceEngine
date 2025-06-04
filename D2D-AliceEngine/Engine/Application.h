#pragma once
#include "pch.h"
#include "D2DRenderer.h"
#include "Singleton.h"

using namespace Microsoft::WRL;

class Application : public Singleton<Application>
{
protected:
	Application();
	virtual ~Application();
	static Application* m_pInstance;
	static Application* GetInstance()
	{
		assert(m_pInstance != nullptr && "Application instance not created!");
		return m_pInstance;
	}

public:
	virtual void Initialize();
	virtual void Run();
	virtual void Uninitialize();

	D2DRenderer* m_pD2DRenderer;

protected:
	HWND m_hwnd = nullptr;
	HINSTANCE m_hInstance;
	std::wstring	m_WindowName = L"D2DWindowClass";
	std::wstring	m_TitleName = L"Default Title Name";
	std::string		m_ModulePath;
	std::string		m_WorkingPath;

	UINT m_width = 1024;
	UINT m_height = 768;
	bool m_resized = false;

public:
	void MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

