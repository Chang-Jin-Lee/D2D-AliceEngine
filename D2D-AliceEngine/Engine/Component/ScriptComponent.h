#pragma once
#include "Component.h"
#include <Experimental/Collision/Collision2D.h>

class Scene;
class ScriptComponent : public Component
{
public:
	ScriptComponent();
	~ScriptComponent();
public:
	void Initialize() override;
	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);
	void Release() override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	virtual void OnCollisionEnter2D(Collision2D* collider) {};
	virtual void OnCollisionStay2D(Collision2D* collider) {};
	virtual void OnCollisionExit2D(Collision2D* collider) {};
	virtual void OnTriggerEnter2D(Collider* collider) {};
	virtual void OnTriggerStay2D(Collider* collider) {};
	virtual void OnTriggerExit2D(Collider* collider) {};

public:
	Scene* GetWorld();
};