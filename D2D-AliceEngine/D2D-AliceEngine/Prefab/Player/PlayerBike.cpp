#include "PlayerBike.h"
#include "Object/gameObject.h"
#include <Scripts/Legacy/Player.h>
#include <Scripts/BackGroundRender.h>
#include <Component/SkewTransform.h>
#include <Scripts/Bike/BikeMovementScript.h>
#include "Component/InputComponent.h"
#include "Manager/SceneManager.h"
#include "Scripts/Player/PlayerManager.h"
#include "System/ScriptSystem.h"
#include "Animation/AnimationController.h"
#include "Animation/AnimatorInstance.h"
#include "Component/Collider.h"
#include "Effect/Prism.h"

#include <Manager/UpdateTaskManager.h>
#include <Scripts/Weapon/Drone.h>
#include <Scripts/Bike/BikeStatScript.h>
#include "Scripts/Player/PlayerMovement.h"
#include <GameManager/PlayerDataManager.h>

void PlayerBike::Initialize()
{
	__super::Initialize();

	owner->SetTag(L"Player");
	// 여기에 컴포넌트 추가	
  	owner->AddComponent<AnimatorInstance>();
	//owner->AddComponent<BackGroundRender>();
	owner->AddComponent<SkewTransform>();
	owner->AddComponent<Collider>();
	owner->AddComponent<InputComponent>();
	owner->AddComponent<Prism>(10, 0.1f);
	
	FDroneSpritePath dronePath(
		L"Player/drone/drone_killdong_body.png",
		L"Player/drone/drone_killdong_arm.png"
	);
	if (Drone* drone = owner->AddComponent<Drone>(dronePath))
	{
		drone->SetDroneType(EDroneType::Player);
		drone->initBodyPos = FVector2(-70, 120);

		FPlayerStats stat = PlayerDataManager::GetInstance().GetStats();
		drone->SetDamage(stat.damage);
		drone->SetAttackDelay(stat.attackDelay);
		drone->SetBulletSpeed(stat.bulletSpeed);
	}
	owner->AddComponent<BikeStatScript>();

	// 게임 스크립트 컴포넌트
	owner->AddComponent<PlayerManager>();
	owner->AddComponent<PlayerMovement>();
}
