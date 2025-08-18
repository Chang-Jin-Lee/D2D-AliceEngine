#pragma once
#include <Application.h>

class DemoGameApp : public Application
{
public:
	DemoGameApp();
	virtual ~DemoGameApp();

public:
	void Initialize() override;
	void Run() override;
	void Render() override;
	void Update() override;
	void Uninitialize() override;
};
