#include "BackGroundVideo.h"
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
#include <TileMap/TileMapComponent.h>

void BackGroundVideo::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(OnStart);
}

void BackGroundVideo::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);

	if (!m_player || !m_owner) return;

	FVector2 playerPos = m_player->transform()->GetPosition();
	FVector2 myPos = m_owner->transform()->GetPosition();

	FVector2 size = m_owner->GetComponent<BackGroundComponent>()->GetSize();

	m_owner->transform()->SetPosition(playerPos.x - size.x / 2, (size.y /*+ 타일맵 이미지 사이즈*/) / 2);
	//m_owner->transform()->SetPosition(playerPos.x, Define::SCREEN_HEIGHT);
}

void BackGroundVideo::OnStart()
{
	m_owner = GetOwner();
	m_owner->transform()->SetScale(1, 1);
	m_owner->transform()->SetPivot(0.5, 0.5);
	
	//m_owner->AddComponent<BackGroundComponent>()->LoadData(L"BackGround\\Alice.webm", 10, L"jpg", 95, true);
	m_owner->AddComponent<BackGroundComponent>()->LoadFromFolder(L"BackGround\\Ena", 5, L"png");
	m_owner->GetComponent<BackGroundComponent>()->Play();
	m_owner->GetComponent<BackGroundComponent>()->SetLayer(-500);

	if (m_player != nullptr)
		m_owner->GetComponent<BackGroundComponent>()->SetPlayer(m_player);
}

void BackGroundVideo::OnEnd()
{

}

void BackGroundVideo::SetPlayer(gameObject* player)
{
	m_player = player;
}
