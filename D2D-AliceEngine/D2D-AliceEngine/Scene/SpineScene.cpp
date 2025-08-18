#include "SpineScene.h"
#include <Manager/SceneManager.h>
#include <Scripts/Widget/TitleWidgetScript.h>
#include <Component/SpriteRenderer.h>
#include <Component/SpineRenderer.h>
#include <Scripts/Spine2D/SpineScript.h>

void SpineScene::Initialize()
{
	__super::Initialize();
}

void SpineScene::Release()
{
	__super::Release();
}

void SpineScene::Update()
{
	__super::Update();
}

void SpineScene::OnEnter()
{
	__super::OnEnter();
	//m_cameraController = NewObject<gameObject>(L"Camera");
	spineObject = NewObject<gameObject>(L"spineObject");
	SpineScript* spine = spineObject->AddComponent<SpineScript>();
	//spine->LoadData(L"Spine2D/Monster_1.atlas", L"Spine2D/Monster_1.json");
}

void SpineScene::OnExit()
{
	__super::OnExit();
}

void SpineScene::PlayerInput()
{
}
