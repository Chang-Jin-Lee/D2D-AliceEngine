#pragma once
#include <Windows.h>
namespace Input
{
	void Initialize(HWND hwnd);
	void Update();
	bool IsKeyDown(int vKey);
	bool IsKeyPressed(int vKey);
	bool IsKeyReleased(int vKey);
	bool AnyKeyPressed();
	void ResetKeyState(int vKey);
	FVector2 GetMousePosition();
	FVector2 GetMouseWorldPosition();
	FVector2 GetMouseWorldPositionInCanvas();
	void SetMousePosition(HWND hWnd, int mousePosX, int mousePosY);
	void ResetMouseState(); // 마우스 상태 초기화

	//FVector2 GetMouseWorldPosition(std::weak_ptr<ACameraActor> camera);
	bool IsMouseLeftReleased();
	bool IsMouseLeftPressed();
	bool IsMouseLeftDown();
}
