#include "pch.h"
#include "Input.h"
#include <Helpers/CoordHelper.h>
#include <Manager/SceneManager.h>
#include <Application.h>

namespace Input
{
	SHORT prevState[256] = { 0 };
	SHORT currState[256] = { 0 };
	HWND m_hwnd;
	POINT m_point;                    // MousePos (client pixels)
	short m_mouseWheelDelta;          // MouseWheel

	void Initialize(HWND hwnd)
	{
		m_hwnd = hwnd;

		// 키보드 상태 초기화
		for (int i = 0; i < 256; i++)
		{
			currState[i] = GetAsyncKeyState(i);
			prevState[i] = currState[i];
		}

		// 마우스 좌표 초기화
		::GetCursorPos(&m_point);
		::ScreenToClient(m_hwnd, &m_point);
	}

	void Update()
	{
		::GetCursorPos(&m_point);
		::ScreenToClient(m_hwnd, &m_point);

		m_mouseWheelDelta = 0;

		memcpy_s(prevState, sizeof(prevState), currState, sizeof(currState));
		for (int i = 0; i < 256; i++)
		{
			currState[i] = GetAsyncKeyState(i);
		}

		GetCursorPos(&m_point);
		ScreenToClient(m_hwnd, &m_point);

		// 해상도 변화에 대응하여 CoordHelper screen size 업데이트
		int w = 0, h = 0; Application::GetInstance().GetApplicationSize(w, h);
		CoordHelper::SetScreenSize(FVector2((float)w, (float)h));

		// 마우스 상태 검증 및 자동 초기화
		// 마우스가 클릭된 상태로 너무 오래 유지되는 경우 자동 초기화
		static int mouseHoldFrameCount = 0;
		if (IsMouseLeftDown())
		{
			mouseHoldFrameCount++;
			// 300프레임(약 5초) 이상 마우스가 클릭된 상태로 유지되면 초기화
			if (mouseHoldFrameCount > 300)
			{
				ResetMouseState();
				mouseHoldFrameCount = 0;
			}
		}
		else
		{
			mouseHoldFrameCount = 0;
		}

		// 추가 안전장치: 마우스가 클릭된 상태로 너무 오래 유지되면 강제 초기화
		static int totalFrameCount = 0;
		totalFrameCount++;

		// 매 1000프레임마다 마우스 상태 검증
		if (totalFrameCount % 1000 == 0)
		{
			// 마우스가 클릭된 상태로 1000프레임 이상 유지되면 강제 초기화
			if (IsMouseLeftDown())
			{
				ResetMouseState();
			}
		}
	}

	bool IsKeyDown(int vKey)
	{
		return (currState[vKey] & 0x8000) != 0;
	}

	bool IsKeyPressed(int vKey)
	{
		return (!(prevState[vKey] & 0x8000) && (currState[vKey] & 0x8000));
	}

	bool IsKeyReleased(int vKey)
	{
		return ((prevState[vKey] & 0x8000) && !(currState[vKey] & 0x8000));
	}

	void ResetKeyState(int vKey)
	{
		prevState[vKey] = 0;
		currState[vKey] = 0;
	}

	FVector2 GetMousePosition()
	{
		// 반환은 항상 현재 클라이언트 픽셀 좌표
		return FVector2(float(m_point.x), float(m_point.y));
	}

	FVector2 GetMouseWorldPosition()
	{
		FVector2 mousePos = CoordHelper::ConvertD2DToUnity(GetMousePosition());
		FVector2 cameraWorldPos = SceneManager::GetInstance().GetCamera()->GetPosition();
		FVector2 worldMousePos = cameraWorldPos + mousePos;
		return worldMousePos;
	}

	FVector2 GetMouseWorldPositionInCanvas()
	{
		FVector2 mousePos = GetMousePosition();
		FVector2 cameraWorldPos = SceneManager::GetInstance().GetCamera()->GetPosition();
		FVector2 worldMousePos = cameraWorldPos + mousePos;
		return worldMousePos;
	}

	bool AnyKeyPressed()
	{
		for (int i = 0; i < 256; ++i)
		{
			if (!(prevState[i] & 0x8000) && (currState[i] & 0x8000))
				return true;
		}
		return false;
	}

	void SetMousePosition(HWND hWnd, int mousePosX, int mousePosY)
	{
		m_hwnd = hWnd;

		POINT pt = { mousePosX, mousePosY };
		ClientToScreen(m_hwnd, &pt);
		SetCursorPos(pt.x, pt.y);
	}

	bool IsMouseLeftReleased()
	{
		return ((prevState[VK_LBUTTON] & 0x8000) && !(currState[VK_LBUTTON] & 0x8000));
	}
	bool IsMouseLeftPressed()
	{
		return (!(prevState[VK_LBUTTON] & 0x8000) && (currState[VK_LBUTTON] & 0x8000));
	}
	bool IsMouseLeftDown()
	{
		return (currState[VK_LBUTTON] & 0x8000) != 0;
	}
	bool IsMouseRightDown()
	{
		return (currState[VK_RBUTTON] & 0x8000) != 0;
	}

	void ResetMouseState()
	{
		// 마우스 클릭 상태 초기화
		prevState[VK_LBUTTON] = 0;
		currState[VK_LBUTTON] = 0;
		
		// 마우스 휠 상태도 초기화
		m_mouseWheelDelta = 0;
		
		// 마우스 위치는 현재 위치로 유지 (클릭 상태만 초기화)
		::GetCursorPos(&m_point);
		::ScreenToClient(m_hwnd, &m_point);
	}
}
