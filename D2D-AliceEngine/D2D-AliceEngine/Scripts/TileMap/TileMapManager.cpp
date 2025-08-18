#include "TileMapManager.h"
#include "System/ScriptSystem.h"
#include "TileMap/TileMapComponent.h"
#include "Object/gameObject.h"
#include "Manager/D2DRenderManager.h"
#include "Manager/SceneManager.h"
#include <Scripts/Enemy/Spawn/SpawnCollider.h>
#include <GameManager/GamePlayManager.h>

TileMapManager* TileMapManager::instance = nullptr;

void TileMapManager::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(OnStart);

	// 싱글톤으로 따로 빼서 사용하기로 함.
	//if (instance == nullptr)
	//	instance = this;
	//else
	//	SceneManager::GetInstance().GetWorld()->RemoveObject(this->owner.lock());

	/*gameObject* coll = SceneManager::GetInstance().GetWorld()->NewObject<gameObject>(L"SpawnCollider");
	spawnerCollider = coll->AddComponent<Collider>();
	spawnerCollider->SetBoxSize(FVector2(00.0f, 500.0f));*/
}

void TileMapManager::OnStart()
{
	TileMapComponent* tc = owner->GetComponent<TileMapComponent>();

	if (tc) {
		// 경로가 설정되지 않았다면 기본값 사용 (Stage1 기준)
		const std::wstring defaultMapPath       = L"TileMap/stage01_real/stage01_real.tmj";
		const std::wstring defaultSetPath       = L"TileMap/stage01_real/Tile_Road.tsj";
		const std::wstring defaultCollisionPath = L"TileMap/TileMapColiderInfo.json";

		const std::wstring& mapPath       = m_tileMapPath.empty()       ? defaultMapPath       : m_tileMapPath;
		const std::wstring& setPath       = m_tileSetPath.empty()       ? defaultSetPath       : m_tileSetPath;
		const std::wstring& collisionPath = m_tileCollisionPath.empty() ? defaultCollisionPath : m_tileCollisionPath;

		tc->LoadTileMapData(mapPath.c_str());
		tc->LoadTileSetData(setPath.c_str());
		tc->LoadTileCollisionData(collisionPath.c_str());
		tc->SetSkew({ 45, 0 });
		tc->CreateTileRenderers();
		tc->CreateTileCollision();
		tc->SetTileLayer(Define::TileLayer);

		// 타일맵 위치 조절
		int t_height = tc->tileMap.tileHeight;
		int map_height = tc->tileMap.height;
		float yOffSet = t_height * ((float)map_height / 2.0f);

		FVector2 screenSize = D2DRenderManager::GetInstance().GetApplicationSize();
		FVector2 offSet = { 0.0f, -(screenSize.y / 2 - yOffSet) };

		gameObject* realTile = tc->m_tile;
		realTile->transform()->AddPosition(offSet.x, offSet.y);

		// 타일맵 콜라이더 위치 조절
		for (auto& obj : tc->go) {
			obj->transform()->AddPosition(offSet.x, offSet.y);
		}

		GamePlayManager::GetInstance().SetStopXAxis(tc->tileMap.tileWidth * tc->tileMap.layers[0].width);
	}
	else {
		std::wstring message = owner->GetName() + L" : TileMapManager에서 TileMapComponent를 가져오는데 실패했습니다!!!\n";
		OutputDebugStringW(message.c_str());
	}
}

TileMapComponent* TileMapManager::GetTileMapComponent()
{
	return owner->GetComponent<TileMapComponent>();
}
