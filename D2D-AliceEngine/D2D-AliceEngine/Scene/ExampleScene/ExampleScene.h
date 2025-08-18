#pragma once
#include <Scene/Scene.h>

/*
*  @briefs : 씬 예시입니다.
*/

class TextRenderComponent;
class ExampleScene : public Scene
{
public:
	ExampleScene() {}
	~ExampleScene() {}

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
	std::vector<gameObject*> m_enemies;
	int enemyMax = 20;
	gameObject* m_widget = nullptr;
};

