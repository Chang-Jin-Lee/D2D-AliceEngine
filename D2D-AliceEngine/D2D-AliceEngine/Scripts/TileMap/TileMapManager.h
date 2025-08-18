#pragma once
#include "Component/ScriptComponent.h"
#include "Component/Collider.h"
#include "TileMap/TileMapComponent.h"

class TileMapManager : public ScriptComponent
{
public:
	static TileMapManager* instance;

	void Initialize() override;
	void OnStart() override;

	// 타일맵, 타일셋, 충돌 데이터 경로를 설정
	void SetTilePaths(const std::wstring& mapPath,
					  const std::wstring& setPath,
					  const std::wstring& collisionPath)
	{
		m_tileMapPath       = mapPath;
		m_tileSetPath       = setPath;
		m_tileCollisionPath = collisionPath;
	}

	TileMapComponent* GetTileMapComponent();
private:
	Collider* spawnerCollider = nullptr;
	// 경로 저장
	std::wstring m_tileMapPath;
	std::wstring m_tileSetPath;
	std::wstring m_tileCollisionPath;
};

