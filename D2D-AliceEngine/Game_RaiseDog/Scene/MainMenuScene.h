#pragma once
#include <Scene/Scene.h>

class gameObject;
class ButtonComponent;
class MainMenuScene : public Scene
{
public:
	MainMenuScene() {}
	~MainMenuScene() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

private:
	gameObject* m_widget{ nullptr };
	bool m_mouseOverUI{ false };
	ButtonComponent* m_startBtn{ nullptr };
	ButtonComponent* m_exitBtn{ nullptr };
}; 