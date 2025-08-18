#include "Audio.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Component/AudioComponent.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>

void Audio::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void Audio::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성
}

void Audio::Awake()
{
}

void Audio::OnStart()
{
	m_owner = GetOwner();
	//m_owner->AddComponent<AudioComponent>()->Load(L"Umapyoi.mp3", AudioMode::StreamLoop);
	//m_owner->GetComponent<AudioComponent>()->Play(62.5f);

	m_bgm = m_owner->AddComponent<AudioComponent>();
	if (m_bgm)
	{
		m_bgm->LoadData(L"Bg_music_tutorial.wav", AudioMode::StreamLoop, SoundType::BGM);
		m_bgm->Play();
	}

	m_ambience = m_owner->AddComponent<AudioComponent>();
	if (m_ambience)
	{
		m_ambience->LoadData(L"BG_Ambience_Loop.wav", AudioMode::StreamLoop, SoundType::SFX);
		m_ambience->Play();
	}

	// 음량 조절
	//m_owner->GetComponent<AudioComponent>()->SetVolume(0.3f);
}

void Audio::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
	m_bgm->Stop();
	m_ambience->Stop();
	//m_owner->GetComponent<AudioComponent>()->Stop();
}

void Audio::OnDestroy()
{
}
