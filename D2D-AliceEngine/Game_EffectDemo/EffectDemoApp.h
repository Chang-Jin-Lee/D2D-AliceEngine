#pragma once
#include <Application.h>

class EffectDemoApp : public Application
{
public:
	EffectDemoApp() = default;
	~EffectDemoApp() override = default;

	void Initialize() override;
	void Run() override;
	void Render() override;
	void Update() override;
	void Uninitialize() override;
}; 