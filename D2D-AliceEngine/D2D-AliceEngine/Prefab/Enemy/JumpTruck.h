#pragma once
#include "Core/Car.h"
class JumpTruck : public Car
{
public:
	void Initialize() override;
	void OnStart() override;
};
