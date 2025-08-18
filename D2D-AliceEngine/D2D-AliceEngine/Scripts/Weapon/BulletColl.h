#pragma once
#include "Component/ScriptComponent.h"

class BulletColl : public ScriptComponent
{
public:
	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

	void SetCollSize(FVector2 _size);
	void SetTarget(WeakObjectPtr<gameObject>& _target);
	
	gameObject* collObject;
	FVector2 offset = { 0.0f, 0.0f };

	WeakObjectPtr<gameObject> target;
};

