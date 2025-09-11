#include "pch.h"
#include "TileMapRenderer.h"
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Helpers/FileHelper.h>
#include <TileMap/TileMapLoader.h>
#include <Helpers/CoordHelper.h>
#include <Application.h>

TileMapRenderer::TileMapRenderer()
{
	SetDrawType(Define::EDrawType::WorldSpace);
}

TileMapRenderer::~TileMapRenderer()
{
	PackageResourceManager::GetInstance().UnloadData(filePath); // 비트맵 언로드
}

void TileMapRenderer::Initialize()
{
	__super::Initialize();
}

void TileMapRenderer::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void TileMapRenderer::LoadData(const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	m_bitmap = PackageResourceManager::GetInstance().CreateBitmapFromFile(filePath.c_str());
}

void TileMapRenderer::Release()
{
	__super::Release();
}

void TileMapRenderer::Render()
{
	if (m_bitmap == nullptr) return;
	__super::Render();
	ID2D1DeviceContext7* context = D2DRenderManager::GetInstance().m_d2dDeviceContext.Get();
	ID2D1SpriteBatch* batch = D2DRenderManager::GetInstance().m_spriteBatch.Get();
	if (!context || !batch) return;

	AddTileToSpriteBatch();

	int batchSize = batch->GetSpriteCount();
	assert(batchSize > 0);

	D2D1_MATRIX_3X2_F skewTransform = CoordHelper::GetSkewMatrix(skewAngle, mapHeight * tileHeight);
	FVector2 appSize = Application::GetInstance().GetSize();
	D2D1_MATRIX_3X2_F backD2DTransform = D2D1::Matrix3x2F::Translation(-appSize.x/2 - GetBitmapSizeX() / 2, -appSize.y / 2);
	context->SetTransform(backD2DTransform *skewTransform * view); // 최종 View와 결합
	context->DrawSpriteBatch(
		batch,
		0, batchSize,
		m_bitmap.get(),
		D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		D2D1_SPRITE_OPTIONS_NONE);
}

void TileMapRenderer::AddTileToSpriteBatch()
{
	ID2D1SpriteBatch* batch = D2DRenderManager::GetInstance().m_spriteBatch.Get();
	if (!batch) return;
	batch->Clear();
	const int tilePerRow = m_bitmap->GetPixelSize().width / tileWidth;
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			int index = y * mapWidth + x;
			int tileId = tileData[index];
			if (tileId == 0)
				continue;

			tileId -= 1;
			int sx = (tileId % tilePerRow) * tileWidth;
			int sy = (tileId / tilePerRow) * tileHeight;
			D2D1_RECT_F destRect = D2D1::RectF(
				(FLOAT)(x * tileWidth),     // X에 오프셋 추가
				(FLOAT)(y * tileHeight),                        // Y는 그대로
				(FLOAT)((x + 1) * tileWidth), // 오른쪽도 동일 오프셋
				(FLOAT)((y + 1) * tileHeight)
			);
			D2D1_RECT_U srcRect = { (UINT32)sx, (UINT32)sy, (UINT32)(sx + tileWidth), (UINT32)(sy + tileHeight) };
			HRESULT hr = D2DRenderManager::GetInstance().m_spriteBatch->AddSprites(1, &destRect, &srcRect, nullptr, nullptr);
			assert(SUCCEEDED(hr));
		}
	}
}

float TileMapRenderer::GetBitmapSizeX()
{
	return static_cast<float>(mapWidth * tileWidth);
}

float TileMapRenderer::GetBitmapSizeY()
{
	return static_cast<float>(mapHeight * tileHeight);
}

void TileMapRenderer::SetMapInfo(const TileMap& mapInfo, const TileSet& setInfo)
{
	mapHeight = mapInfo.layers[0].height;
	mapWidth = mapInfo.layers[0].width;
	tileWidth = setInfo.tilewidth;
	tileHeight = setInfo.tileheight;
	tileData = mapInfo.layers[0].data;
}

void TileMapRenderer::SetSkew(FVector2 _skewAngle)
{
	skewAngle = _skewAngle;
}
