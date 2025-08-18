#include "BulletColl.h"
#include "Object/gameObject.h"
#include "System/ScriptSystem.h"
#include "Manager/UpdateTaskManager.h"
#include "Component/Collider.h"
#include "Component/BoxComponent.h"

void BulletColl::Initialize()
{
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_TICK(Update);
	//REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);

	owner->AddComponent<Collider>()->SetBoxSize(FVector2(70.0f, 70.0f));
}

void BulletColl::OnStart()
{
	
}

void BulletColl::Update(const float& deltaSeconds)
{
	if (target)
		owner->transform()->SetPosition(target->transform()->GetPosition() + offset);
	else
		GetWorld()->RemoveObject(owner.lock());
}

void BulletColl::SetCollSize(FVector2 _size)
{
	if (auto c = owner->GetComponent<Collider>())
	{
		c->SetLayer(6); // 히트박스 채널
		if (c->boxComponent) c->boxComponent->SetSize(_size);
	}
}

void BulletColl::SetTarget(WeakObjectPtr<gameObject>& _target)
{
	if (_target)
		target = _target;
}
