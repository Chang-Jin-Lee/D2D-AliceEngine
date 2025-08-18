#pragma once
#include <Component/ScriptComponent.h>
#include <Component/BackGroundComponent.h>

class BackGroundVideo : public ScriptComponent
{
public:
	void Initialize() override;

	void Update(const float& deltaSeconds) override;
	virtual void OnStart() override;
	virtual void OnEnd() override;

	void SetPlayer(gameObject* player);

	gameObject* m_owner = nullptr;
	gameObject* m_player = nullptr;
};

