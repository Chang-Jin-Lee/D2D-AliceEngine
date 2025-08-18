#pragma once
#include <Scene/Scene.h>

class TextRenderComponent;
class DemoScene3_Tutorial : public Scene
{
public:
	DemoScene3_Tutorial() {}
	~DemoScene3_Tutorial() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void PlayerInput();
	void EnemyInput();

	class gameObject* m_backgroundImage = nullptr;
	class gameObject* m_spriteAnimationTest = nullptr;
	class gameObject* m_player = nullptr;
	class gameObject* m_wall = nullptr;
	class gameObject* m_wall2 = nullptr;
	class gameObject* m_wall3 = nullptr;
	std::vector<class gameObject*> m_enemies;
	int enemyMax = 20;
	class gameObject* m_widget = nullptr;
	class gameObject* m_widget2 = nullptr;
	class gameObject* m_widget3 = nullptr;
	class gameObject* m_widget4 = nullptr;
};

