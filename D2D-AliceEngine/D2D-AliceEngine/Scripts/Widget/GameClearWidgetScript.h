#pragma once
#include <Component/ScriptComponent.h>
#include <Manager/TimerManager.h>

class gameObject;
class SpriteRenderer;
class TextRenderComponent;
class GameClearWidgetScript : public ScriptComponent
{
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	gameObject* m_owner;
	SpriteRenderer* m_grade;
	FTimerHandle m_killHandle;
	FTimerHandle m_timeHandle;
	FTimerHandle m_gradeHandle;

	TextRenderComponent* m_killCount;
	TextRenderComponent* m_passedTime;
	TextRenderComponent* m_playerHP;

	// 계산식용
	float m_fTime = 0.0f;
	int m_iKill = 0;
	int m_iHealth = 0.0f;

	static std::wstring s_prevScene;
};

