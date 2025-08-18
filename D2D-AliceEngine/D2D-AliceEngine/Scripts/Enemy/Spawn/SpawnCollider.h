#pragma once
#include "Component/ScriptComponent.h"

class Collider;
class EnemySpawner;
class SpawnCollider : public ScriptComponent
{
public:
	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

	void OnTriggerEnter2D(Collider* collider) override;
private:
	Collider* co;
	WeakObjectPtr<gameObject> player;
};

