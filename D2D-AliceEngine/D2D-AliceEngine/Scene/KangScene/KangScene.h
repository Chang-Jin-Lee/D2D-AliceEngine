#pragma once
#include <Scene/Scene.h>

class Canvas;
class TextRenderComponent;
class KangScene : public Scene
{
public:
	KangScene() {}
	~KangScene() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	gameObject* m_cameraController;

	gameObject* m_player;
	gameObject* m_sound;
	gameObject* m_tile;	// = 3
	Canvas* m_canvas;
	gameObject* m_wall;

	gameObject* m_UI;
	gameObject* m_button;

	gameObject* m_bg;	// 임시 배경

	gameObject* m_truck;

	// 컷씬 종료 후 전환할 씬명
	std::wstring m_nextSceneName = L"KangeTest";
};

