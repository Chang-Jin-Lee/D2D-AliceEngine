#pragma once
#include "Core/Car.h"
class NormalTruck : public Car
{
public:
	void Initialize() override;
	void OnStart() override;
};

