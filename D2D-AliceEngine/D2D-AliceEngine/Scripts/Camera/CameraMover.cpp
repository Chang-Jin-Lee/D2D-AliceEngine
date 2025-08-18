#include "CameraMover.h"
#include "Manager/UpdateTaskManager.h"
#include <d2dengine_pch.cpp>
#include "../Bike/BikeMovementScript.h"
#include "Manager/SceneManager.h"
#include "Component/SkewTransform.h"
#include <Core/ObjectHandler.h>

void CameraMover::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PostPhysics);
	//REGISTER_SCRIPT_TICK(Update);
}

void CameraMover::Awake()
{
}

void CameraMover::OnStart()
{
	//owner->AddComponent<CameraController>();
	// 플레이어 찾아서 넣기
	player = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"Player");

	if (auto* cameraPtr = GetCamera())
	{
		camera = cameraPtr;
	}

	if (player) {
 		playerST = player->GetComponent<SkewTransform>();

		player->RemoveFromParent();
		player->AddChildTransform(&GetCamera()->relativeTransform);
		camera->SetRelativeScale(player->GetScaleInv());
		camera->SetRelativePosition(FVector2(0, 0));
		camera->RemoveFromParent();
	}
}

void CameraMover::Update(const float& dt)
{
	////xPos += playerBM->GetCurrSpeed() * playerBM->GetSpeedModifier() * dt;
	//float t1 = playerBM->GetCurrSpeed();
	//float t2 = playerBM->GetSpeedModifier();
	////xPos += playerBM->GetCurrSpeed() * playerBM->GetSpeedModifier() * dt;
	//xPos = player->transform()->GetPosition().x;
	//
	////camera->SetPosition(xPos, yPos);a
	//camera->SetPosition(xPos, yPos);


 	if (!playerST) return;
	if (!player) return;

	FVector2 playerPos = playerST->GetRealPos();


	//FVector2 targetPos = playerST->GetRealPos() + camOffSet;
	//FVector2 targetPos = player->GetPosition() + camOffSet;

	FVector2 targetPos;
	targetPos.x = playerPos.x + camOffSet.x;
	targetPos.y = player->GetPosition().y + camOffSet.y;

	FVector2 cameraPos = camera->GetPosition();  // camera는 이 스크립트의 owner임

	FVector2 delta = targetPos - cameraPos;

	// deadZone 안에 있으면 움직이지 않음
	if (fabs(delta.x) < deadZoneX) delta.x = 0.0f;
	if (fabs(delta.y) < deadZoneY) delta.y = 0.0f;

	// 느리게 따라오기 (선형 보간 방식)
	FVector2 newPos = cameraPos + delta * dt * lerpSpeed;
 	camera->SetRelativePosition(newPos);
}
