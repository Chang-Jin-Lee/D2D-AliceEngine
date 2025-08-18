#pragma once
#include <Component/ScriptComponent.h>

class gameObject;
class Earth : public ScriptComponent
{
public:
	void Initialize() override;

	void Update(const float& deltaSeconds) override;
	virtual void OnStart() override;
	virtual void OnEnd() override;

	void Input();

	gameObject* m_earth;
};

