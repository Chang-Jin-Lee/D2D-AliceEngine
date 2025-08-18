#pragma once
#include "Object/gameObject.h"
#include "Core/Car.h"

class EnemyBike : public Car
{
public:
	void Initialize() override;
	void OnStart() override;
};

