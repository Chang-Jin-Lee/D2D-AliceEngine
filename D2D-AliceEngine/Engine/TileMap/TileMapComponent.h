#pragma once
#include <Component/RenderComponent.h>
#include <Component/TileMapRenderer.h>
#include <TileMap/TileMapLoader.h>

class TileMapComponent : public RenderComponent
{
public:
	TileMapComponent();
	~TileMapComponent();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void LoadTileMapData(const std::wstring& path);
	void LoadTileSetData(const std::wstring& path);
	void LoadTileCollisionData(const std::wstring& path);
	void CreateTileCollision();
	void CreateTileRenderers();
	int GetTileLayer();
	void SetTileLayer(const int& layer);
	void Release() override;
	void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;
	void SetSkew(const FVector2& angle);

	FVector2 GetSize();
	
public:
	std::vector<WeakObjectPtr<TileMapRenderer>> m_TileRenderers;
	TileMap tileMap;
	TileSet tileSet;
	std::unordered_map<int,CollData> tileCollision;
	std::wstring filePath; // 파일의 경로

	FVector2 skewAngle;

	gameObject* m_tile;

	std::vector<gameObject*> go;
};

