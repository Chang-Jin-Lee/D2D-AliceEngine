#pragma once
#include <Application.h>

class Game_SpineDemo : public Application
{
public:
	Game_SpineDemo();
	virtual ~Game_SpineDemo();

public:
	void Initialize() override;
	void Run() override;
	void Render() override;
	void Update() override;
	void Uninitialize() override;
};
