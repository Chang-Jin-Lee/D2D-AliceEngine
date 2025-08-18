#include "SpawnCollider.h"
#include "Component/Collider.h"
#include "Object/gameObject.h"
#include "Component/TransformComponent.h"
#include "Manager/SceneManager.h"
#include <System/ScriptSystem.h>
#include "Manager/UpdateTaskManager.h"
#include "EnemySpawner.h"
#include <TileMap/TileMapLoader.h>
#include "SpawnData.h"
#include <Helpers/Logger.h>
#include "GameManager/GamePlayManager.h"

void SpawnCollider::Initialize()
{
	__super::Initialize();

	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
	//REGISTER_SCRIPT_TICK(Update);
}

void SpawnCollider::OnStart()
{
	co = owner->AddComponent<Collider>();
	owner->transform()->SetPivot(0.5f, 0.5f);
	//owner->SetParent(SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"Player")->transform());

	co->SetBoxSize(FVector2(1500.0f, 1500.0f));
	co->SetLayer(1);

	player = GamePlayManager::GetInstance().GetPlayer(); //SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"Player");
}

void SpawnCollider::Update(const float& deltaSeconds)
{
	if(!player.expired())
		owner->transform()->SetPosition(player->transform()->GetPosition());
}

void SpawnCollider::OnTriggerEnter2D(Collider* collider)
{
	// 디버깅
	/*std::wstring message = collider->GetOwner()->GetName() + L" : 스포너 콜라이더 Enter!!\n";
	OutputDebugStringW(message.c_str());*/

	if (collider->GetOwner()->GetTag() == L"EnemySpawn") {
		CollData temp = collider->GetOwner()->GetComponent<SpawnData>()->GetCollData();

		if (temp.isUsable) {
			temp.isUsable = false;
			int etype = temp.enemyType;
			collider->GetOwner()->GetComponent<SpawnData>()->SetCollData(temp);

			FVector2 pos = collider->GetOwner()->GetPosition();
			EnemySpawner::instance->SpawnEnemy(etype, pos);

			//LOG_INFO(L"스폰했습니다.");
		}
	}
}
