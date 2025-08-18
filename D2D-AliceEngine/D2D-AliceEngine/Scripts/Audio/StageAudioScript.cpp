#include "StageAudioScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>

#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/VideoComponent.h>

#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Scene/Scene.h>
#include <Component/ButtonComponent.h>
#include <Helpers/CoordHelper.h>
#include <Manager/SceneManager.h>
#include <Component/AudioComponent.h>
#include <GameManager/GamePlayManager.h>
#include <Helpers/Logger.h>
#include <Component/ProgressBarComponent.h>

StageAudioScript* StageAudioScript::instance = nullptr;

void StageAudioScript::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void StageAudioScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void StageAudioScript::Awake()
{
	if (instance == nullptr)
		instance = this;
}

void StageAudioScript::OnStart()
{
	m_owner = GetOwner();

	m_TutorialAudio = m_owner->AddComponent<AudioComponent>(L"Tutorial");
	m_TutorialAudio->LoadData(L"Bg_music_tutorial.wav", AudioMode::StreamLoop, SoundType::BGM);

	m_Stage1Audio = m_owner->AddComponent<AudioComponent>(L"Stage1");
	m_Stage1Audio->LoadData(L"Bg_music_Stage_1.wav", AudioMode::StreamLoop, SoundType::BGM);

	m_Stage2Audio = m_owner->AddComponent<AudioComponent>(L"Stage2");
	m_Stage2Audio->LoadData(L"Bg_music_Stage_2.wav", AudioMode::StreamLoop, SoundType::BGM);

	m_Stage3Audio = m_owner->AddComponent<AudioComponent>(L"Stage3");
	m_Stage3Audio->LoadData(L"Bg_music_Stage_Finale.wav", AudioMode::StreamLoop, SoundType::BGM);

	m_Ambience = m_owner->AddComponent<AudioComponent>(L"Ambience");
	m_Ambience->LoadData(L"BG_Ambience_Loop.wav", AudioMode::StreamLoop, SoundType::SFX);

	m_MotorSound = m_owner->AddComponent<AudioComponent>(L"MotorSound");
	m_MotorSound->LoadData(L"Player/character_Player_Sfx_Rev_Idle.mp3", AudioMode::StreamLoop, SoundType::SFX);
	

	std::wstring SceneName = SceneManager::GetInstance().m_currentScene->GetName();

	if (SceneName == Define::Scene_Stage1)
	{
		m_Stage1Audio->PlayByName(L"Stage1");
	}
	else if (SceneName == Define::Scene_Stage2)
	{
		m_Stage2Audio->PlayByName(L"Stage2");
	}
	else if (SceneName == Define::Scene_Stage3)
	{
		m_Stage3Audio->PlayByName(L"Stage3");
	}


	// SFX 로드
	sfxList = {
		L"Enemy/character_Enemy_sfx_shot.wav",
		L"Enemy/character_Enemy_sfx_hit.wav",
		L"Enemy/character_Enemy_sfx_downed.wav",
		L"Player/character_Player__sfx_move_down.wav",
		L"Player/character_Player_sfx_accelation.wav",
		L"Player/character_Player_sfx_accelation_short.wav",
		L"Player/character_Player_sfx_Gameover.wav",
		L"Player/character_Player_sfx_hit.wav",
		L"Player/character_Player_sfx_itempickup.wav",
		L"Player/character_Player_sfx_move_up.wav",
		L"Player/character_Player_sfx_Sandebistan.wav",
		L"Player/character_Player_Sfx_stage_clear.mp3",
		L"Player/character_Player_Sfx_Rev.mp3"
	};

	for (auto& wstr : sfxList) {
		m_Sfx[wstr] = m_owner->AddComponent<AudioComponent>(wstr);
		m_Sfx[wstr]->LoadData(wstr, AudioMode::Stream, SoundType::SFX);
	}
}

void StageAudioScript::OnEnd()
{
	if(m_TutorialAudio->IsPlaying()) m_TutorialAudio->Stop();
	if (m_Stage1Audio->IsPlaying()) m_Stage1Audio->Stop();
	if (m_Stage2Audio->IsPlaying()) m_Stage2Audio->Stop();
	if (m_Stage3Audio->IsPlaying()) m_Stage3Audio->Stop();
	if (m_Ambience->IsPlaying()) m_Ambience->Stop();
	if (m_MotorSound->IsPlaying()) m_Ambience->Stop();

	for (auto& pair : m_Sfx) {
		auto& audio = pair.second;
		if (audio != nullptr) {
			if (audio->IsPlaying())
				audio->Stop();
		}
	}

	instance = nullptr;
}

void StageAudioScript::OnDestroy()
{
}
