#include "GameClearAudioScript.h"

#include <Object/gameObject.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Math/Transform.h>
#include <Helpers/CoordHelper.h>

#include <Core/Input.h>
#include <Scene/Scene.h>
#include <Component/SpriteRenderer.h>
#include <Component/TextRenderComponent.h>
#include <Component/ButtonComponent.h>
#include <Manager/SceneManager.h>
#include <Component/AudioComponent.h>

#include <GameManager/GamePlayManager.h>

void GameClearAudioScript::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void GameClearAudioScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void GameClearAudioScript::Awake()
{
}

void GameClearAudioScript::OnStart()
{
	m_owner = GetOwner();
	
	m_Audio = m_owner->AddComponent<AudioComponent>(L"GameClearBGM");
	m_Audio->LoadData(L"Bg_music_Ending.wav", AudioMode::StreamLoop, SoundType::BGM);
	m_Audio->PlayByName(L"GameClearBGM");
}

void GameClearAudioScript::OnEnd()
{
	m_Audio->Stop();
}

void GameClearAudioScript::OnDestroy()
{
}
