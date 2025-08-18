#include "SpineScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/SpineRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Manager/UpdateTaskManager.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/SceneManager.h>
#include <System/RenderSystem.h>

void SpineScript::Initialize()
{
	__super::Initialize();

	spineRenderer = std::make_unique<SpineRenderer>();
	spineRenderer->RegistSystem(owner.Get());
	spineRenderer->Initialize();
}

void SpineScript::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void SpineScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성

	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if(bSpineReady)
		spineRenderer->UpdateAnimation(deltaSeconds);
}

void SpineScript::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void SpineScript::Awake()
{
}

void SpineScript::OnStart()
{
	// 여기에 OnStart에 대한 로직 작성
	m_owner = GetOwner();
	spineRenderer->LoadSpine(L"Spine2D_g/Monster_1.atlas", L"Spine2D_g/Monster_1.json");
	spineRenderer->SetRelativePosition(FVector2(0, -250));
	spineRenderer->SetNextAnimation();

	bSpineReady = true;

	owner->AddComponent<InputComponent>()->SetAction(owner->GetHandle(), 
		[this]() 
		{
			Input();
		});
}

void SpineScript::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void SpineScript::OnDestroy()
{
	spineRenderer->Shutdown();
	spineRenderer.reset();
}

void SpineScript::LoadData(const std::wstring& atlasPath, const std::wstring& jsonPath)
{
	spineRenderer->LoadSpine(atlasPath, jsonPath);
}

void SpineScript::OnCollisionEnter2D(Collision2D* collider)
{
	std::cout << "OnCollisionEnter2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnCollisionEnter2D 호출됨\n");
}

void SpineScript::OnCollisionStay2D(Collision2D* collider)
{
	std::cout << "OnCollisionStay2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnCollisionStay2D 호출됨\n");
}

void SpineScript::OnCollisionExit2D(Collision2D* collider)
{
	std::cout << "OnCollisionExit2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnCollisionExit2D 호출됨\n");
}

void SpineScript::OnTriggerEnter2D(Collider* collider)
{
	std::cout << "OnTriggerEnter2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerEnter2D 호출됨\n");
}

void SpineScript::OnTriggerStay2D(Collider* collider)
{
	std::cout << "OnTriggerStay2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerStay2D 호출됨\n");
}

void SpineScript::OnTriggerExit2D(Collider* collider)
{
	std::cout << "OnTriggerExit2D 호출됨" << std::endl;
	OutputDebugStringW(L"OnTriggerExit2D 호출됨\n");
}

void SpineScript::Input()
{
	// 여기에 Input에 대한 로직 작성
	if (Input::IsKeyPressed(VK_SPACE)) {
		spineRenderer->SetNextAnimation();
	}
	else if (Input::IsKeyPressed(VK_BACK)) {
		spineRenderer->SetPreviousAnimation();
	}
	else if (Input::IsKeyPressed('1')) {
		spineRenderer->SetAnimation("Idle");
	}
	else if (Input::IsKeyPressed('2')) {
		spineRenderer->SetAnimation("Move");
	}
	else if (Input::IsKeyPressed('3')) {
		spineRenderer->SetAnimation("Attack_Melee");
	}
}
