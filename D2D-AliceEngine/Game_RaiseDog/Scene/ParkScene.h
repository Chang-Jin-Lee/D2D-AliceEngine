#pragma once
#include <Scene/Scene.h>

class gameObject;
class ParkScene : public Scene
{
public:
	ParkScene() {}
	~ParkScene() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

private:
	gameObject* m_widget{ nullptr };
}; 