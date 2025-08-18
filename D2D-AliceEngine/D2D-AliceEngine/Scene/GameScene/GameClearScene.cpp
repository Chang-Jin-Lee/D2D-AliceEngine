#include "GameClearScene.h"
#include <Manager/SceneManager.h>
#include <Component/TextRenderComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Component/SpriteRenderer.h>
#include <Helpers/CoordHelper.h>
#include <GameManager/GamePlayManager.h>
#include <Scripts/Widget/GameClearWidgetScript.h>
#include <Scripts/Audio/GameClearAudioScript.h>
#include <Scripts/Widget/CutSceneWidgetScript.h>

void GameClearScene::Initialize()
{
    __super::Initialize();

    m_sound = NewObject<gameObject>(L"Sound");
    m_sound->AddComponent<GameClearAudioScript>();
}

void GameClearScene::Release()
{
    __super::Release();
}

void GameClearScene::Update()
{
    __super::Update();
}

void GameClearScene::OnEnter()
{
    __super::OnEnter();

    m_textGO = NewObject<gameObject>(L"GameClearLabel");
    auto* text = m_textGO->AddComponent<TextRenderComponent>();

	m_passedTimeText = NewObject<gameObject>(L"PassedTime");
    {
        auto* text = m_passedTimeText->AddComponent<TextRenderComponent>();
    }

    m_killEnemyText = NewObject<gameObject>(L"KillCount");
	{
		auto* text = m_killEnemyText->AddComponent<TextRenderComponent>();
	}

    m_playerHP = NewObject<gameObject>(L"PlayerHP");

    // VK_3 눌러 TitleScene 복귀
    // TODO : 디버깅 끝나면 지우기
    auto* input = m_textGO->AddComponent<InputComponent>();
    input->SetAction(m_textGO->GetHandle(), []() {
        if (Input::IsKeyDown(VK_3)) {
            SceneManager::ChangeScene(Define::Scene_Title);
        }
    });

    m_widget = NewObject<gameObject>(L"Widget");
    m_widget->AddComponent<GameClearWidgetScript>();
}

void GameClearScene::OnExit()
{
    __super::OnExit();
    GamePlayManager::GetInstance().ReleaseTimers();
}
