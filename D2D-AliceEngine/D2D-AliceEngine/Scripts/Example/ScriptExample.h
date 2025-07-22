#pragma once
#include <Component/ScriptComponent.h>

/*
*	@briefs :
*		ScriptComponent를 사용하는 예시입니다.
*		이 코드를 복사하여 사용하시면 됩니다.
*
*	@Caution : 
*		C++에는 리플랙션이 없기때문에 다음처럼 사용할 함수를 등록해주어야합니다.
*		Initialize() 부분에서 해줍니다.
*		REGISTER_SCRIPT_METHOD(Awake)
*		REGISTER_SCRIPT_METHOD(OnStart)
*		REGISTER_SCRIPT_METHOD(OnEnd)
*		REGISTER_SCRIPT_METHOD(OnDestroy)
*/

class gameObject;
class ScriptExample : public ScriptComponent
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

	void Input();

	gameObject* m_owner;
};

