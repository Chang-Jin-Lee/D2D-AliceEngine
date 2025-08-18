#include "GameOverScene.h"
#include <Manager/SceneManager.h>
#include <Component/TextRenderComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Component/SpriteRenderer.h>
#include <Helpers/CoordHelper.h>
#include <GameManager/GamePlayManager.h>
#include <Scripts/Widget/GameOverWidgetScript.h>

void GameOverScene::Initialize()
{
    __super::Initialize();
}

void GameOverScene::Release()
{
    __super::Release();
}

void GameOverScene::Update()
{
    __super::Update();
}

void GameOverScene::OnEnter()
{
    __super::OnEnter();

    m_textGO = NewObject<gameObject>(L"GameClearLabel");
    auto* text = m_textGO->AddComponent<TextRenderComponent>();

    m_widget = NewObject<gameObject>(L"Widget");
    m_widget->AddComponent<GameOverWidgetScript>();

    // VK_3 눌러 TitleScene 복귀
    // TODO : 디버깅 끝나면 지우기
    auto* input = m_textGO->AddComponent<InputComponent>();
    input->SetAction(m_textGO->GetHandle(), []() {
        if (Input::IsKeyDown(VK_3)) {
            SceneManager::ChangeScene(Define::Scene_Title);
        }
    });
}

void GameOverScene::OnExit()
{
    __super::OnExit();
    GamePlayManager::GetInstance().ReleaseTimers();
}
