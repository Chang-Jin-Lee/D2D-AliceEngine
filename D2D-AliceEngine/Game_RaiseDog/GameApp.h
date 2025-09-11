#pragma once
#include <Application.h>

class GameApp : public Application
{
public:
	GameApp();
	virtual ~GameApp();

public:
	void Initialize() override;
	void Run() override;
	void Render() override;
	void Update() override;
	void Uninitialize() override;
};

