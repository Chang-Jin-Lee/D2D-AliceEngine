#pragma once
#include "Component/ScriptComponent.h"

class JumpTrigger : public ScriptComponent
{
public:
	WeakObjectPtr<gameObject> target;

	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

	void OnTriggerEnter2D(Collider* collider) override;

	void SetOffSet(FVector2 _offset);
	void SetBoxSize(FVector2 _size);

private:
	FVector2 offset;
};

