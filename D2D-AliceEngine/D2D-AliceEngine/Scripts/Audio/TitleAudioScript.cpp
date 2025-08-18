#include "TitleAudioScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Scene/Scene.h>

#include <Component/AudioComponent.h>

void TitleAudioScript::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void TitleAudioScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void TitleAudioScript::Awake()
{
}

void TitleAudioScript::OnStart()
{
	m_owner = GetOwner();

	m_Audio = m_owner->AddComponent<AudioComponent>(L"TitleBGM");
	m_Audio->LoadData(L"Bg_music_Maintitle.wav",AudioMode::StreamLoop, SoundType::BGM);
	m_Audio->PlayByName(L"TitleBGM", 0, 0.5);

	m_SFX = m_owner->AddComponent<AudioComponent>(L"UISound");
	m_SFX->LoadData(L"UI_interact_sound.wav", AudioMode::Memory, SoundType::SFX);
}

void TitleAudioScript::OnEnd()
{
	m_Audio->Stop();
}

void TitleAudioScript::OnDestroy()
{
}
