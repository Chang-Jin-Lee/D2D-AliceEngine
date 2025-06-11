#pragma once
#include "pch.h"
#include "D2DRenderManager.h"
#include "Singleton.h"
#include "Camera.h"

using namespace Microsoft::WRL;

class Application : public Singleton<Application>
{
public:
	Application();
	virtual ~Application();

public:
	virtual void Initialize();
	virtual void Run();
	virtual void Render();
	virtual void Update();
	virtual void Uninitialize();

	std::shared_ptr<D2DRenderManager> m_pD2DRenderManager;
	std::shared_ptr<Camera> m_mainCamera; // Main Camera

protected:
	HWND m_hwnd = nullptr;
	HINSTANCE m_hInstance;
	std::wstring	m_WindowName = Define::WINDOW_CLASS_NAME;
	std::wstring	m_TitleName = Define::WINDOW_TITLE;
	std::string		m_ModulePath;
	std::string		m_WorkingPath;

	UINT m_width = Define::SCREEN_WIDTH;
	UINT m_height = Define::SCREEN_HEIGHT;
	bool m_resized = false;

public:
	void MessageProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	std::shared_ptr<Object> m_Sun;
	std::shared_ptr<Object> m_Earth;
	std::shared_ptr<Object> m_Moon;
};

