#pragma once
#include <Component/ScriptComponent.h>
#include <Animation/AnimationController.h>

class gameObject;
class AnimatorInstance;
class Player : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	virtual void OnCollisionEnter2D(Collision2D* collider) override;
	virtual void OnCollisionStay2D(Collision2D* collider) override;
	virtual void OnCollisionExit2D(Collision2D* collider) override;
	virtual void OnTriggerEnter2D(Collider* collider) override;
	virtual void OnTriggerStay2D(Collider* collider) override;
	virtual void OnTriggerExit2D(Collider* collider) override;

	int EnterIndex = 0;
	int ExitIndex = 0;

	void Input();

	gameObject* m_owner;
	int jumpCount = 0;
	int maxJumpCount = 2;
	int prevGroundState = 0;

	float walkSpeed = 100;

	bool bMoveRigidBody = false;

	AnimatorInstance* animInstance;
	AnimatorController animController;
};

