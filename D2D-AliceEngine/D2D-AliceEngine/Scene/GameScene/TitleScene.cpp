#include "TitleScene.h"
#include <Manager/SceneManager.h>
#include <Scripts/Widget/TitleWidgetScript.h>
#include <Component/SpriteRenderer.h>
#include <Component/SpineRenderer.h>
#include <Component/AudioComponent.h>
#include <Scripts/Audio/TitleAudioScript.h>
#include <Core/Input.h>
#include <Component/InputComponent.h>
#include <GameManager/GamePlayManager.h>
#include <Helpers/Logger.h>
#include <Scripts/Widget/CutSceneWidgetScript.h>
#include <Scripts/Spine2D/SpineScript.h>
#include <Scripts/Legacy/CameraController.h>
#include <Scripts/Widget/GameClearWidgetScript.h>
#include <Scripts/Example/ScriptExample.h>

void TitleScene::Initialize()
{
	__super::Initialize();

	GameClearWidgetScript::s_prevScene = L"";

    GamePlayManager& GPM = GamePlayManager::GetInstance();
	GPM.StartGame();
}

void TitleScene::Release()
{
	__super::Release();
}

void TitleScene::Update()
{
	__super::Update();
}

void TitleScene::OnEnter()
{
	__super::OnEnter();
	m_sound = NewObject<gameObject>(L"Sound");
	m_sound->AddComponent<TitleAudioScript>();
	SetClickable(true);
	m_cameraController = NewObject<gameObject>(L"Camera");
	m_cameraController->AddComponent<CameraController>();

	m_widget = NewObject<gameObject>(L"Widget");
	//m_UI->AddComponent<TitleUIScript>();
	m_widget->AddComponent<TitleWidgetScript>();

	// 테스트용 컷씬 위젯. 이걸 켜서 확인할 것.
	//m_UI->AddComponent<CutSceneWidgetScript>();

	spineObject = NewObject<gameObject>(L"spineObject");
	SpineScript* spine = spineObject->AddComponent<SpineScript>();
	spine->LoadData(L"Spine2D/MAIN_SPINE.atlas", L"Spine2D/MAIN_SPINE.json");
	if (SpineRenderer* t = spine->GetSpineRenderer())
	{
		t->SetAnimation("animation");
		t->SetRelativePosition(FVector2(0, 300));
		t->SetRendered(true);
		t->SetRelativeScale(0.34f);
		t->SetLayer(94548823);
		t->SetDrawType(Define::EDrawType::WorldSpace);
	}

	//spineObject2 = scene->NewObject<gameObject>(L"spineObject");
	//SpineScript* spine2 = spineObject2->AddComponent<SpineScript>();
	//spine2->LoadData(L"Spine2D/Monster_1.atlas", L"Spine2D/Monster_1.json");
	//if (spine2->spineRenderer)
	//{
	//	spine2->spineRenderer->SetAnimation("Idle");
	//	spine2->spineRenderer->SetPosition(FVector2(100, 0));
	//	spine2->spineRenderer->SetRendered(false);
	//}

	gameObject* test = NewObject<gameObject>(L"test");
	test->AddComponent<ScriptExample>();

	// 디버그용 씬 전환
	gameObject* sceneChanger = NewObject<gameObject>(L"SceneChanger");
	sceneChanger->AddComponent<InputComponent>()->SetAction(sceneChanger->GetHandle(), [this]() {
		if (Input::IsKeyPressed(VK_3)) {
			SceneManager::ChangeScene(Define::Scene_Stage1);
		}
	});
}

void TitleScene::OnExit()
{
	__super::OnExit();
	
	// 타이틀 씬 종료 시 마우스 상태 초기화
	Input::ResetMouseState();

	GamePlayManager::GetInstance().ReleaseTimers();
}

void TitleScene::PlayerInput()
{
}
