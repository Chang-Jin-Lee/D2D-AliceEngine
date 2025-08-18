#pragma once
#include <Scene/Scene.h>

class TextRenderComponent;
class DemoScene3 : public Scene
{
public:
	DemoScene3() {}
	~DemoScene3() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void PlayerInput();
	void EnemyInput();

	gameObject* m_backgroundImage = nullptr;
	gameObject* m_spriteAnimationTest = nullptr;
	gameObject* m_player = nullptr;
	gameObject* m_wall = nullptr;
	gameObject* m_wall2 = nullptr;
	gameObject* m_wall3 = nullptr;
	std::vector<gameObject*> m_enemies;
	int enemyMax = 20;
	gameObject* m_widget = nullptr;
	gameObject* m_widget2 = nullptr;
	gameObject* m_widget3 = nullptr;
	gameObject* m_widget4 = nullptr;
};

