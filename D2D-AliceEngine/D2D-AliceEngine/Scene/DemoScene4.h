#pragma once
#include <Scene/Scene.h>

// HiroScene ÀÓ½Ã »ý¼º
class Canvas;
class TextRenderComponent;
class DemoScene4 : public Scene
{
public:
	DemoScene4() {}
	~DemoScene4() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void PlayerInput();
	void EnemyInput();

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

	//gameObject* m_backgroundImage;
	//gameObject* m_spriteAnimationTest;
	//gameObject* m_wall2;
	//gameObject* m_wall3;
	//std::vector<gameObject*> m_enemies;
	//int enemyMax = 20;
	//gameObject* m_widget;
	//gameObject* m_widget2;
	//gameObject* m_widget3;
	//gameObject* m_widget4;
};

