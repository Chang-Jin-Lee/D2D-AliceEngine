#pragma once
#include <Component/ScriptComponent.h>

class gameObject;
class AudioComponent;
class ProgressBarComponent;
class SpriteRenderer;
class TextRenderComponent;
class SelectWidgetScript : public ScriptComponent
{
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	gameObject* m_owner;

	float m_pressValue = 0.0f;

	float m_seconds = 0.0f;

	ProgressBarComponent* m_progress;
	SpriteRenderer* m_progressSprite;
	TextRenderComponent* m_guideText;

	bool m_isInStage1 = false;

	void SetPressValue(float value)
	{
		if (value <= 0.0f) m_pressValue = 0.0f;

		m_pressValue = value;
	}
	float GetPressValue() { return m_pressValue; }

private:
	AudioComponent* m_sound;
	void SkipInput(float deltaSeconds);
};
