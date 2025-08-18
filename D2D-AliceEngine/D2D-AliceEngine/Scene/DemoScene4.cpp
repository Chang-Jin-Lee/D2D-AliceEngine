#include "DemoScene4.h"
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/TransformComponent.h>
#include <Component/VideoComponent.h>
#include <Component/AudioComponent.h>
#include <Object/gameObject.h>
#include <Core/Input.h>
#include <Math/TColor.h>
#include "../Scripts/BackGroundImage.h"
#include "../Scripts/Legacy/Aru.h"
#include "../Scripts/Legacy/Aru2.h"
#include "../Scripts/Legacy/Player.h"
#include "../Scripts/Legacy/Enemy.h"
#include "../Scripts/Audio.h"
#include "../Scripts/BackGroundVideo.h"
#include "../Scripts/BackGroundRender.h"
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>
#include <TileMap/TileMapComponent.h>
#include <Component/BackGroundComponent.h>
#include <Component/SkewTransform.h>
#include <Effect/Prism.h>
#include <Scripts/Widget/TitleWidgetScript.h>
#include <Component/ButtonComponent.h>
#include <Scripts/Bike/BikeMovementScript.h>
#include <Scripts/Camera/CameraMover.h>
#include <Prefab/Player/PlayerBike.h>
#include <Prefab/Truck.h>
#include <Scripts/Widget/StageWidgetScript.h>

void DemoScene4::Initialize()
{
	__super::Initialize();
}

void DemoScene4::Release()
{
	__super::Release();
}

void DemoScene4::Update()
{
	__super::Update();
}

void DemoScene4::OnEnter()
{
	__super::OnEnter();

	m_cameraController = NewObject<gameObject>(L"Camera");
	m_cameraController->AddComponent<CameraMover>();

	m_player = NewObject<gameObject>(L"Player");
	m_player->AddComponent<PlayerBike>();
	//m_player->AddComponent<Player>();
	//m_player->AddComponent<BackGroundRender>();

	//m_bg = NewObject<gameObject>(L"BackGround");
	//m_bg->AddComponent<BackGroundVideo>()->SetPlayer(m_player);

	// 오디오 추가, 오디오 관련 스크립트 넣기
	m_sound = NewObject<gameObject>(L"Sound");
	m_sound->AddComponent<Audio>();

	m_tile = NewObject<gameObject>(L"TileMap");
	m_tile->AddComponent<TileMapComponent>()->LoadTileMapData(L"TileMap/stage01_real/stage01_real.tmj");
	m_tile->GetComponent<TileMapComponent>()->LoadTileSetData(L"TileMap/stage01_real/Tile_Road.tsj");
	/*m_tile->AddComponent<TileMapComponent>()->LoadTileMapData(L"TileMap/BigMap.tmj");
	m_tile->GetComponent<TileMapComponent>()->LoadTileSetData(L"TileMap/blocks.tsj");*/
	m_tile->GetComponent<TileMapComponent>()->LoadTileCollisionData(L"TileMap/TileMapColiderInfo.json");
	m_tile->GetComponent<TileMapComponent>()->SetSkew({ 45, 0});
	m_tile->GetComponent<TileMapComponent>()->CreateTileRenderers();
	m_tile->GetComponent<TileMapComponent>()->CreateTileCollision();
	m_tile->GetComponent<TileMapComponent>()->SetTileLayer(3);	// 타일 레이어를 3으로 설정
	
	// SkewTransform 테스트
	/*if(auto st = m_player->GetComponent<SkewTransform>())
		st->groundTile = m_tile;*/
	//m_player->AddComponent<SkewTransform>()->groundTile = m_tile;
	//m_player->AddComponent<BikeMovementScript>();
	
	// UI 추가
	//m_UI = NewObject<gameObject>(L"ad");
	//m_UI->AddComponent<TitleWidgetScript>();

	//m_button = NewObject<gameObject>(L"Button");
	//m_button->AddComponent<ButtonComponent>();
	//m_button->GetComponent<ButtonComponent>()->SetImages(L"Button_Idle.png", L"Button_Hover.png", L"Button_Pressed.png");
	//m_button->GetComponent<ButtonComponent>()->LoadData(Define::EButtonState::Idle, L"Button_Idle.png");
	//m_button->GetComponent<ButtonComponent>()->SetRelativePosition(FVector2(100, 100));
	//m_button->GetComponent<ButtonComponent>()->SetRelativeScale(FVector2(1,1));
	//m_button->GetComponent<ButtonComponent>()->SetStateAction(Define::EButtonState::Pressed,[]() {});
	//m_button->GetComponent<ButtonComponent>()->m_layer = 510;

	//m_UI->AddComponent<StageWidgetScript>();
	// Truck(점프대)
	m_truck = NewObject<gameObject>(L"Truck");
	m_truck->AddComponent<Truck>();

	//m_wall = NewObject<gameObject>(L"wall");
	//m_wall->transform()->SetPivot(0.5f);
	//m_wall->transform()->SetPosition(0, -300);
	//m_wall->AddComponent<Collider>()->SetBoxSize(FVector2(5500, 200));

	//RemoveObject(m_wall);
	m_player->GetComponent<InputComponent>()->SetAction(m_player->GetHandle(), [this]() { PlayerInput();  });
	//m_player->AddComponent<InputComponent>()->SetAction(m_player->GetHandle(), [this]() { PlayerInput();  });
}

void DemoScene4::OnExit()
{
	__super::OnExit();
}

void DemoScene4::PlayerInput()
{
	if (Input::IsKeyPressed(VK_3))
	{
		//SceneManager::ChangeScene(L"aruScene");
		SceneManager::ChangeScene(L"TitleScene");
	}
}

void DemoScene4::EnemyInput()
{
}
