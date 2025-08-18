#pragma once
#include "Core/Car.h"
class NormalCar : public Car
{
public:
	void Initialize() override;
	void OnStart() override;
};

