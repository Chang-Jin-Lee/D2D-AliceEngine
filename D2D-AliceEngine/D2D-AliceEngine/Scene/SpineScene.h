#pragma once
#include <Scene/Scene.h>

class TextRenderComponent;
class gameObject;
class SpineScene : public Scene
{
public:
	SpineScene() {}
	~SpineScene() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void PlayerInput();

	gameObject* spineObject;
};

