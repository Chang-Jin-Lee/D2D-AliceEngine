#pragma once
#include "Component/ScriptComponent.h"

class Collider;
class EnemyDespawner : public ScriptComponent
{
public:
	FVector2 offset = { -1200.0f, 0.0f };

	void Initialize() override;
	void OnStart() override;
	void Update(const float& deltaSeconds) override;

	void OnTriggerEnter2D(Collider* collider) override;
};

