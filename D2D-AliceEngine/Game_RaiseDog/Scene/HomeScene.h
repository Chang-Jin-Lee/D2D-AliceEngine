#pragma once
#include <Scene/Scene.h>

class gameObject;
class HomeScene : public Scene
{
public:
	HomeScene() {}
	~HomeScene() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

private:
	gameObject* m_widget{ nullptr };
	gameObject* m_dog{ nullptr };
	gameObject* m_dogWidget{ nullptr };
}; 