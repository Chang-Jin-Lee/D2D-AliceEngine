#pragma once
#include <Component/ScriptComponent.h>

class gameObject;
class AudioComponent;
class TitleAudioScript : public ScriptComponent
{
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	gameObject* m_owner;
	AudioComponent* m_Audio;
	AudioComponent* m_SFX;
};

