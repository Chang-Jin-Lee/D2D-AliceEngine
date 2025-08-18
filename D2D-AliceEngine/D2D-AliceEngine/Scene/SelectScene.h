#pragma once
#include <Scene/Scene.h>

class SelectScene : public Scene
{
public:
	SelectScene() {}
	~SelectScene() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void Input();

	gameObject* m_camera;
	gameObject* m_UI;

	gameObject* m_sound;
};

