#pragma once
#include <Component/ScriptComponent.h>

/*
* @briefs :
*	ScriptComponent�� ����ϴ� �����Դϴ�.
*	�� �ڵ带 �����Ͽ� ����Ͻø� �˴ϴ�.
*/

class gameObject;
class ScriptExample : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);

	virtual void OnStart() override;
	virtual void OnEnd() override;

	void Input();

	gameObject* m_owner;
};

