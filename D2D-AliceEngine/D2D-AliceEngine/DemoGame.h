#pragma once
#include "stdafx.h"

class DemoGame : public Application
{
public:
	DemoGame();
	virtual ~DemoGame();

public:
	void Initialize() override;
	void Run() override;
	void Uninitialize() override;
};
