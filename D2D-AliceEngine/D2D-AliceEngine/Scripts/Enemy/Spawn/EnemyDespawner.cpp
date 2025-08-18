#include "EnemyDespawner.h"
#include "System/ScriptSystem.h"
#include "Component/Collider.h"
#include "Object/gameObject.h"
#include "Manager/SceneManager.h"
#include "Object/Camera.h"
#include "Manager/UpdateTaskManager.h"

void EnemyDespawner::Initialize()
{
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
	//REGISTER_SCRIPT_TICK(Update);
}

void EnemyDespawner::OnStart()
{
	auto col = owner->AddComponent<Collider>();
	col->SetBoxSize(FVector2(100.0f, 1000.0f));
	col->SetLocalOffset(FVector2(-800, 0));

	//SceneManager::GetInstance().GetCamera()->AddChildObject(owner.lock());
	//owner->transform()->AddPosition(offset);
} 

void EnemyDespawner::Update(const float& deltaSeconds)
{
	
}

void EnemyDespawner::OnTriggerEnter2D(Collider* collider)
{
	if (collider->GetOwner()->GetTag() == L"Enemy" || collider->GetOwner()->GetTag() == L"Obstacle") {
		SceneManager::GetInstance().GetWorld()->RemoveObject(collider->GetOwner());
	}
}
