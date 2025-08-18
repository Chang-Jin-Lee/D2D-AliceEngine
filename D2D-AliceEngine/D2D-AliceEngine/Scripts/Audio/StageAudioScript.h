#pragma once
#include <unordered_map>
#include <Component/ScriptComponent.h>

class gameObject;
class AudioComponent;
class StageAudioScript : public ScriptComponent
{
public:
	static StageAudioScript* instance;

	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	gameObject* m_owner;
	AudioComponent* m_TutorialAudio;
	AudioComponent* m_Stage1Audio;
	AudioComponent* m_Stage2Audio;
	AudioComponent* m_Stage3Audio;
	AudioComponent* m_Ambience;
	AudioComponent* m_MotorSound;

	std::vector<std::wstring> sfxList;
	std::unordered_map<std::wstring, AudioComponent*> m_Sfx;
};

