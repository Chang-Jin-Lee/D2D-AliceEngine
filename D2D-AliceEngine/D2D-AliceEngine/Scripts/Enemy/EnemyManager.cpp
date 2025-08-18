#include "EnemyManager.h"
#include "Manager/UpdateTaskManager.h"
#include "System/ScriptSystem.h"
#include "../Bike/LaneController.h"
#include <Helpers/StringHelper.h>

void EnemyManager::Initialize()
{
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
	//REGISTER_SCRIPT_TICK(Update);
}

void EnemyManager::OnStart()
{

}

void EnemyManager::Update(const float& dt)
{
	if (isMoving)
		Move(dt);
}

void EnemyManager::Move(const float& dt)
{
	owner->transform()->AddPosition(moveSpeed * dt, 0.0f);
}

void EnemyManager::MoveUp()
{
	if (auto lc = owner->GetComponent<LaneController>())
		lc->MoveUp();
}

void EnemyManager::MoveDown()
{
	if (auto lc = owner->GetComponent<LaneController>())
		lc->MoveDown();
}

void EnemyManager::Die()
{
	dieActions.BroadCast();
}

void EnemyManager::AddDieAction(ObjectHandle _handle, std::function<void()> _func)
{
	dieActions.Add(_handle, _func);
}

void EnemyManager::RemoveDieAction(ObjectHandle _handle)
{
	dieActions.Remove(_handle);
}

void EnemyManager::ClearDieAction()
{
	dieActions.Clear();
}
