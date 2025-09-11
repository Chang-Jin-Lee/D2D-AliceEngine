#include "pch.h"
#include "TileMapComponent.h"
#include <Component/Component.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Helpers/FileHelper.h>
#include <Math/Transform.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>
#include <Object/gameObject.h>
#include <Scene/Scene.h>
#include <Component/TileMapRenderer.h>
#include <Component/Collider.h>
#include <Helpers/CoordHelper.h>
#include <Manager/UpdateTaskManager.h>
#include <Component/BoxComponent.h>
#include "../../D2D-AliceEngine/Scripts/Enemy/Spawn/SpawnData.h"

#include <Application.h>

TileMapComponent::TileMapComponent()
{
}

TileMapComponent::~TileMapComponent()
{
	for(auto& tileRenderer : m_TileRenderers)
	{
		tileRenderer.reset();
	}
	m_TileRenderers.clear();
}

void TileMapComponent::Initialize()
{
	__super::Initialize();
	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_NewlySpawned);
}

void TileMapComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void TileMapComponent::LoadTileMapData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	TileMapLoader::LoadTileMap(filePath, tileMap);
}

void TileMapComponent::LoadTileSetData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	TileMapLoader::LoadTileSet(filePath, tileSet);
}

void TileMapComponent::LoadTileCollisionData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path);
	tileCollision = TileMapLoader::LoadTileMapColliderInfo(filePath);
}

void TileMapComponent::CreateTileCollision()
{
	int mapHeight = tileMap.layers[0].height;
	int mapWidth = tileMap.layers[0].width;
	int imageWidth = tileSet.imagewidth;
	int tileWidth = tileSet.tilewidth;
	int tileHeight = tileSet.tileheight;
	std::vector<int> tileData = tileMap.layers[0].data;

	const int tilePerRow = imageWidth / tileWidth;
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			int index = y * mapWidth + x;
			int tileId = tileData[index];
			if (tileId == 0)
				continue;
			if (tileCollision.find(tileId) == tileCollision.end()) continue;

			tileId -= 1;
			int sx = (tileId % tilePerRow) * tileWidth;
			int sy = (tileId / tilePerRow) * tileHeight;
			D2D1_RECT_U srcRect = { (UINT32)sx, (UINT32)sy, (UINT32)(sx + tileWidth), (UINT32)(sy + tileHeight) };

			D2D1_RECT_F destRect = D2D1::RectF(
				(FLOAT)(x * tileWidth),
				(FLOAT)(y * tileHeight),
				(FLOAT)((x + 1) * tileWidth),
				(FLOAT)((y + 1) * tileHeight)
			);

			D2D1_MATRIX_3X2_F fullTransform =
				D2D1::Matrix3x2F::Translation(-Application::GetInstance().GetSize().x / 2.0f, -Application::GetInstance().GetSize().y / 2.0f) *
				CoordHelper::GetSkewMatrix(skewAngle, mapHeight * tileHeight) * 
				D2D1::Matrix3x2F::Scale(1,-1);
			D2D1_POINT_2F tl = CoordHelper::TransformPoint(fullTransform, D2D1::Point2F(destRect.left, destRect.top));
			D2D1_POINT_2F br = CoordHelper::TransformPoint(fullTransform, D2D1::Point2F(destRect.right, destRect.bottom));
			const float cx = (tl.x + br.x) * 0.5f;
			const float cy = (tl.y + br.y) * 0.5f;

			gameObject* collisionGo = GetWorld()->NewObject<gameObject>(L"tileCollision");
			collisionGo->transform()->SetPosition({ cx, cy });
			collisionGo->transform()->SetPivot(0.5f, 0.5f);
			collisionGo->AddComponent<Collider>();
			if (auto collider = collisionGo->GetComponent<Collider>())
			{
				collider->SetBoxSize(FVector2(tileWidth, tileHeight));
				// 레거시 경로와 m_box 모두 일관 반영
				collider->SetDebugDraw(true);
				if (collider->boxComponent) collider->boxComponent->SetSkewAngle(FVector2(30.0f, 0.0f));
				collider->SetLayer(tileCollision[tileId+1].collisionChannel); // 실제 위치를 반환하기 위해 -1 했던것을 다시 +1
			}

			// 태그 설정
			collisionGo->SetTag(L"EnemySpawn");

			// 스폰데이터 설정
			collisionGo->AddComponent<SpawnData>()->SetCollData(tileCollision[tileId + 1]);

			go.push_back(collisionGo);
		}
	}
}

void TileMapComponent::CreateTileRenderers()
{
	m_tile = GetWorld()->NewObject<gameObject>(L"tileSprite");
	TileMapRenderer* tileRenderer = m_tile->AddComponent<TileMapRenderer>();
	tileRenderer->LoadData(StringHelper::string_to_wstring(tileSet.image));
	tileRenderer->SetSkew(skewAngle);
	tileRenderer->SetMapInfo(tileMap, tileSet);
	m_tile->transform()->SetPosition(tileRenderer->GetBitmapSizeX() / 2, 0);
}

void TileMapComponent::SetTileLayer(const int& layer)
{
	TileMapRenderer* tileRenderer = m_tile->GetComponent<TileMapRenderer>();
	tileRenderer->SetLayer(layer);
}

void TileMapComponent::Release()
{
	__super::Release();
}

/*
* @briefs : 변환행렬을 적용하여 비트맵을 원하는 위치에 그립니다.
* @details
*	ETransformType : 좌표계 구분
*/

void TileMapComponent::Render()
{
	
}

float TileMapComponent::GetBitmapSizeX()
{
	return 0;
}

float TileMapComponent::GetBitmapSizeY()
{
	return 0;
}

void TileMapComponent::SetSkew(const FVector2& angle)
{
	skewAngle = angle;
}

FVector2 TileMapComponent::GetSize()
{
	return FVector2(0, 0); // TODO: 실제 크기 계산 로직 추가)
}

int TileMapComponent::GetTileLayer()
{
	if (TileMapRenderer* tileRenderer = m_tile->GetComponent<TileMapRenderer>())
	{
		return tileRenderer->GetLayer();
	}
	return -1;
}
