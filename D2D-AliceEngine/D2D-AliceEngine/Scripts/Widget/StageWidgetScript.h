#pragma once
#include <Component/ScriptComponent.h>

class gameObject;
class TextRenderComponent;
class ProgressBarComponent;
class BikeMovementScript;
class ButtonComponent;

class StageWidgetScript : public ScriptComponent
{
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	bool m_isPaused = false;
	void SetProgress();

	gameObject* m_owner;

	TextRenderComponent* m_passedTimeText;
	TextRenderComponent* m_killEnemyText;

	TextRenderComponent* m_dashboardText;
	TextRenderComponent* m_speedText;

	ProgressBarComponent* m_batteryProgress;
	ProgressBarComponent* m_speedProgress;

	ButtonComponent* m_pauseButton;

	//BikeMovementScript* m_bike;

private:
	int maxBattery = 5;
	int prevBattery = 0;
	int prevKillAmount = 0;

	bool m_ambiencePlayed = false;
};

