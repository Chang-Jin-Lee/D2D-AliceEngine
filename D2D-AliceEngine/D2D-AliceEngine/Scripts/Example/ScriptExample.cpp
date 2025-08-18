#include "ScriptExample.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Manager/UpdateTaskManager.h>

void ScriptExample::Initialize()
{
	__super::Initialize();
}

void ScriptExample::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void ScriptExample::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성
}

void ScriptExample::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void ScriptExample::Awake()
{
}

void ScriptExample::OnStart()
{
	// 여기에 OnStart에 대한 로직 작성
	m_owner = GetOwner();

}

void ScriptExample::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void ScriptExample::OnDestroy()
{
}

void ScriptExample::OnCollisionEnter2D(Collision2D* collider)
{
    std::cout << "OnCollisionEnter2D 호출됨" << std::endl;
    OutputDebugStringW(L"OnCollisionEnter2D 호출됨\n");
}

void ScriptExample::OnCollisionStay2D(Collision2D* collider)
{
    std::cout << "OnCollisionStay2D 호출됨" << std::endl;
    OutputDebugStringW(L"OnCollisionStay2D 호출됨\n");
}

void ScriptExample::OnCollisionExit2D(Collision2D* collider)
{
    std::cout << "OnCollisionExit2D 호출됨" << std::endl;
    OutputDebugStringW(L"OnCollisionExit2D 호출됨\n");
}

void ScriptExample::OnTriggerEnter2D(Collider* collider)
{
    std::cout << "OnTriggerEnter2D 호출됨" << std::endl;
    OutputDebugStringW(L"OnTriggerEnter2D 호출됨\n");
}

void ScriptExample::OnTriggerStay2D(Collider* collider)
{
    std::cout << "OnTriggerStay2D 호출됨" << std::endl;
    OutputDebugStringW(L"OnTriggerStay2D 호출됨\n");
}

void ScriptExample::OnTriggerExit2D(Collider* collider)
{
    std::cout << "OnTriggerExit2D 호출됨" << std::endl;
    OutputDebugStringW(L"OnTriggerExit2D 호출됨\n");
}

void ScriptExample::Input()
{
	// 여기에 Input에 대한 로직 작성
}
