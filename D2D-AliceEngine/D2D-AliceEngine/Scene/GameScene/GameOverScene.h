#pragma once
#include <Scene/Scene.h>

class GameOverScene : public Scene
{
public:
    GameOverScene() = default;
    ~GameOverScene() = default;

    void Initialize() override;
    void Release() override;
    void Update() override;

    void OnEnter() override;
    void OnExit() override;

private:
    gameObject* m_textGO{ nullptr };
	gameObject* m_widget;
};
