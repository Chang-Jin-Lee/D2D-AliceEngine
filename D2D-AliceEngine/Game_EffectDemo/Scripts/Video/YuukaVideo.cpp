#include "YuukaVideo.h"
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Math/Transform.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Component/VideoComponent.h>
#include <System/ScriptSystem.h>

void YuukaVideo::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(OnStart);
}

void YuukaVideo::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void YuukaVideo::OnStart()
{
	m_yuuka = GetOwner();
	m_yuuka->transform()->SetPosition(0, 0);
	m_yuuka->transform()->SetRotation(0);
	m_yuuka->transform()->SetScale(1.0f, 1.0f);
	m_yuuka->transform()->SetPivot(0.5f, 0.5f);
	m_yuuka->AddComponent<VideoComponent>()->LoadData(L"BackGround\\Yuuka3.mp4", 30, L"jpg", 95, true);
	m_yuuka->GetComponent<VideoComponent>()->Play();

	m_yuuka->AddComponent<InputComponent>()->SetAction(m_yuuka->GetHandle(),[this]() { Input(); });
}

void YuukaVideo::OnEnd()
{

}

void YuukaVideo::Input()
{
	if (Input::IsKeyPressed(VK_P))
	{
		if (m_yuuka->GetComponent<VideoComponent>()->bPlay == true)
			m_yuuka->GetComponent<VideoComponent>()->Stop();
		else
			m_yuuka->GetComponent<VideoComponent>()->Play();
	}

	if (Input::IsKeyPressed(VK_5))
	{
		m_yuuka->GetComponent<VideoComponent>()->Stop();
		m_yuuka->RemoveComponent<VideoComponent>(m_yuuka->GetComponent<VideoComponent>());
		m_yuuka->AddComponent<VideoComponent>()->LoadData(L"BackGround\\asna.mp4", 30, L"jpg", 95, true);
		m_yuuka->GetComponent<VideoComponent>()->Play();
	}
	if (Input::IsKeyPressed(VK_6))
	{
		m_yuuka->GetComponent<VideoComponent>()->Stop();
		m_yuuka->RemoveComponent<VideoComponent>(m_yuuka->GetComponent<VideoComponent>());
		m_yuuka->AddComponent<VideoComponent>()->LoadData(L"BackGround\\Yuuka3.mp4", 30, L"jpg", 95, true);
		m_yuuka->GetComponent<VideoComponent>()->Play();
	}
	if (Input::IsKeyPressed(VK_7))
	{
		m_yuuka->GetComponent<VideoComponent>()->Stop();
		m_yuuka->RemoveComponent<VideoComponent>(m_yuuka->GetComponent<VideoComponent>());
		m_yuuka->AddComponent<VideoComponent>()->LoadData(L"BackGround\\Aru_NewYear.webm", 30, L"jpg", 95, true);
		m_yuuka->GetComponent<VideoComponent>()->Play();
	}
	if (Input::IsKeyPressed(VK_8))
	{
		m_yuuka->GetComponent<VideoComponent>()->Stop();
		m_yuuka->RemoveComponent<VideoComponent>(m_yuuka->GetComponent<VideoComponent>());
		m_yuuka->AddComponent<VideoComponent>()->LoadData(L"BackGround\\AruDress.webm", 30, L"jpg", 95, true);
		m_yuuka->GetComponent<VideoComponent>()->Play();
	}
	if (Input::IsKeyPressed(VK_9))
	{
		m_yuuka->GetComponent<VideoComponent>()->Stop();
		m_yuuka->RemoveComponent<VideoComponent>(m_yuuka->GetComponent<VideoComponent>());
		m_yuuka->AddComponent<VideoComponent>()->LoadData(L"BackGround\\Alice.webm", 30, L"jpg", 95, true);
		m_yuuka->GetComponent<VideoComponent>()->Play();
	}
	if (Input::IsKeyPressed(VK_0))
	{
		m_yuuka->GetComponent<VideoComponent>()->Stop();
		m_yuuka->RemoveComponent<VideoComponent>(m_yuuka->GetComponent<VideoComponent>());
		m_yuuka->AddComponent<VideoComponent>()->LoadData(L"BackGround\\Mari_Sportswear.webm", 30, L"jpg", 95, true);
		m_yuuka->GetComponent<VideoComponent>()->Play();
	}
}
