#pragma once
#include <Scene/Scene.h>
#include <Manager/TimerManager.h>

class TextRenderComponent;
class TitleScene : public Scene
{
public:
	TitleScene() {}
	~TitleScene() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void PlayerInput();

	gameObject* m_cameraController;
	gameObject* m_player;
	
	gameObject* m_sound;

	gameObject* m_bg;

	gameObject* m_widget;
	gameObject* m_button;

	gameObject* iamge;

	FTimerHandle timer;
	gameObject* spineObject;
	gameObject* spineObject2;
};

