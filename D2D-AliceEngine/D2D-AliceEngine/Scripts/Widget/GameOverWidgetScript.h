#pragma once
#include <Component/ScriptComponent.h>
#include <Manager/TimerManager.h>

class gameObject;
class SpriteRenderer;
class ButtonComponent;
class GameOverWidgetScript : public ScriptComponent
{
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	gameObject* m_owner;
	SpriteRenderer* m_errorPopup;
	ButtonComponent* m_toRestartButton;
	ButtonComponent* m_toMainButton;

	float m_scale = 0.0f;
	float m_sec = 0.0f;

	bool m_isPop = false;

};

