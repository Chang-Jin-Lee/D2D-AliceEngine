#include "BackGroundImage.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Component/VideoComponent.h>
#include <System/ScriptSystem.h>
#include <Object/Camera.h>

void BackGroundImage::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(OnStart);
}

void BackGroundImage::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void BackGroundImage::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성

}

void BackGroundImage::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void BackGroundImage::OnStart()
{
	// 여기에 OnStart에 대한 로직 작성
	m_owner = GetOwner();
	//m_owner->transform()->SetPosition(0, 0);
	//m_owner->transform()->SetRotation(0);
	//m_owner->transform()->SetScale(2.0f, 2.0f);
	//m_owner->transform()->SetPivot(0.5f);
	//m_owner->AddComponent<SpriteRenderer>()->LoadData(L"cloud.jpg");
	GetCamera()->AddChildObject(m_owner);

	m_owner->transform()->SetPosition(0, 0);
	m_owner->transform()->SetRotation(0);
	m_owner->transform()->SetScale(1.3f, 1.0f);
	m_owner->transform()->SetPivot(0.5f);
	m_owner->AddComponent<VideoComponent>()->LoadData(L"DemoGame1/BackGround/Sea.mp4", 30, L"jpg", 95, true);
	m_owner->GetComponent<VideoComponent>()->Play();
	
	m_owner->AddComponent<InputComponent>()->SetAction(m_owner->GetHandle(), [this]() { Input(); });
}

void BackGroundImage::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void BackGroundImage::Input()
{
	// 여기에 Input에 대한 로직 작성

	if (Input::IsKeyPressed(VK_P))
	{
		if (m_owner->GetComponent<VideoComponent>())
		{
			if (m_owner->GetComponent<VideoComponent>()->bPlay == true)
				m_owner->GetComponent<VideoComponent>()->Stop();
			else
				m_owner->GetComponent<VideoComponent>()->Play();
		}
	}
}
