#pragma once
#include <Component/ScriptComponent.h>


class gameObject;
class BackGroundImage : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);

	virtual void OnStart() override;
	virtual void OnEnd() override;

	void Input();

	gameObject* m_owner = nullptr;
};

