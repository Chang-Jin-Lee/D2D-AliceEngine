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

void BackGroundImage::Initialize()
{
	__super::Initialize();
	REGISTER_SCRIPT_METHOD(OnStart);
}

void BackGroundImage::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// ���⿡ FixedUpdate�� ���� ���� �ۼ�

}

void BackGroundImage::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// ���⿡ Update�� ���� ���� �ۼ�

}

void BackGroundImage::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// ���⿡ LateUpdate�� ���� ���� �ۼ�

}

void BackGroundImage::OnStart()
{
	// ���⿡ OnStart�� ���� ���� �ۼ�
	m_owner = GetOwner();
	m_owner->transform()->SetPosition(0, 0);
	m_owner->transform()->SetRotation(0);
	m_owner->transform()->SetScale(1.5f, 1.5f);
	m_owner->transform()->SetPivot(0.5f);
	m_owner->AddComponent<SpriteRenderer>()->LoadData(L"cloud.jpg");
	m_owner->GetComponent<SpriteRenderer>()->m_layer = -1;
	
	m_owner->AddComponent<InputComponent>()->SetAction(m_owner->GetHandle(), [this]() { Input(); });
}

void BackGroundImage::OnEnd()
{
	// ���⿡ OnEnd�� ���� ���� �ۼ�
}

void BackGroundImage::Input()
{
	// ���⿡ Input�� ���� ���� �ۼ�

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
