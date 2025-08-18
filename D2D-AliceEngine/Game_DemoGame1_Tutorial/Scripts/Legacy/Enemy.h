#pragma once
#include <Component/ScriptComponent.h>
#include <Animation/AnimationController.h>

class gameObject;
class Animator;
class IdleState;
class AttackState;
class AnimatorInstance;
struct SpriteSheet;
struct AnimationClip;
class Enemy : public ScriptComponent
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

	void Input();

	gameObject* m_owner = nullptr;

	AnimatorInstance* animInstance = nullptr;
	AnimatorController animController;

	std::shared_ptr<SpriteSheet> Texture;
	std::shared_ptr<AnimationClip> idle;
	std::shared_ptr<AnimationClip> kick;
};

