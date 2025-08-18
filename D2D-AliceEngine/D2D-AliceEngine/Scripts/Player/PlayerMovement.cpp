#include "PlayerMovement.h"
#include "System/ScriptSystem.h"
#include "Manager/UpdateTaskManager.h"
#include "Manager/SceneManager.h"
#include "../Bike/BikeMovementScript.h"
#include "PlayerManager.h"
#include <Core/Input.h>
#include "Component/InputComponent.h"
#include <GameManager/GamePlayManager.h>
#include <Component/SkewTransform.h>

void PlayerMovement::Initialize()
{
	__super::Initialize();

	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
	//REGISTER_SCRIPT_TICK(Update);
}

void PlayerMovement::OnStart()
{
	if (!bMovement)
		bMovement = owner->AddComponent<BikeMovementScript>();

	if (bMovement) {
		initSpeed = minSpeed +  minSpeed * 0.25;
		bMovement->SetAcceleration(100);
	}

	if (auto ic = owner->GetComponent<InputComponent>())
		ic->SetAction(owner->GetHandle(), [this]() { Input(); });
	else
		OutputDebugStringW(L"플레이어의 movement 스크립트에서 인풋컴포넌트를 가져오는데 실패함!!\n");
}

void PlayerMovement::Update(const float& deltaSeconds)
{
	if (isMoving) {
		playerDeltaSeconds = PlayerManager::instance->GetTimeScale() * deltaSeconds;

		if ((moveDir.x > 0.0f && bMovement->GetMaxSpeed() < maxSpeed) || (moveDir.x < 0.0f && bMovement->GetMaxSpeed() > minSpeed))
			bMovement->AddMaxSpeed(moveDir.x * playerDeltaSeconds);
		else
			bMovement->AddMaxSpeed((initSpeed - bMovement->GetMaxSpeed()) * decVal * playerDeltaSeconds);


		if (!Math::Approximately(moveDir.y, 0.0f)) {
			owner->GetComponent<SkewTransform>()->zPos += moveDir.y * playerDeltaSeconds;
		}
	}
}

void PlayerMovement::SetZClamp(float _min, float _max)
{
	_min = minZ;
	_max = maxZ;
}

void PlayerMovement::SetBoost(bool _val)
{
	if (isBoosting == _val)
		return;

	if (_val) {
		maxSpeed += bonusSpeed;
		minSpeed += bonusSpeed * 0.5f;
		isBoosting = true;
	}
	else {
		maxSpeed -= bonusSpeed;
		minSpeed -= bonusSpeed * 0.5f;
		isBoosting = false;
	}
		
}

float PlayerMovement::GetRealSpeed()
{
	return bMovement->GetMaxSpeed() * 0.3f;
}

void PlayerMovement::CrashSlow()
{
	bMovement->SetCurrentSpeed(bMovement->GetCurrentSpeed() * 0.5f);
}

void PlayerMovement::Stop()
{
	isMoving = false;
	bMovement->SetCurrentSpeed(0.0f);
}

void PlayerMovement::Start()
{
	isMoving = true;
}

void PlayerMovement::Input()
{
	moveDir = { 0.0f, 0.0f };

	if (Input::IsKeyDown(VK_S))
	{
		auto st = owner->GetComponent<SkewTransform>();
		if (st && st->zPos > minZ)
			moveDir.y -= 200.0f;
	}
	if (Input::IsKeyDown(VK_W))
	{
		auto st = owner->GetComponent<SkewTransform>();
		if (st && st->zPos < maxZ)
			moveDir.y += 200.0f;
	}
	if (Input::IsKeyDown(VK_A)) {
		// initialSpeed 줄이기
		moveDir.x -= 200.0f;
	}
	if (Input::IsKeyDown(VK_D)) {
		// initialSpeed 늘리기
		moveDir.x += isBoosting ? 400.0f : 100.0f;
	}

	if (Input::IsKeyPressed(VK_B))
	{
		PlayerManager::instance->Boost(999.0f, false);
	}
	if (Input::IsKeyPressed(VK_V))
	{
		PlayerManager::instance->Boost(1.0f, false);
	}
}
