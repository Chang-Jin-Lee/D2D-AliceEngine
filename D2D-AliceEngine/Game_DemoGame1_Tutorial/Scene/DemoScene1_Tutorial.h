#pragma once
#include <Scene/Scene.h>

class TextRenderComponent;
class gameObject;
class DemoScene1_Tutorial : public Scene
{
public:
	DemoScene1_Tutorial() {}
	~DemoScene1_Tutorial() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void PlayerInput();

	gameObject* m_camera;
	gameObject* m_yuuka;
	gameObject* m_sun;
	gameObject* m_earth;
	gameObject* m_moon;
	gameObject* m_widget;
	gameObject* m_widget2;
	gameObject* m_widget3;
};

