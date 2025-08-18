#pragma once
#include <Application.h>

class Game_DemoGame1_Tutorial : public Application
{
public:
	Game_DemoGame1_Tutorial();
	virtual ~Game_DemoGame1_Tutorial();

public:
	void Initialize() override;
	void Run() override;
	void Render() override;
	void Update() override;
	void Uninitialize() override;
};
