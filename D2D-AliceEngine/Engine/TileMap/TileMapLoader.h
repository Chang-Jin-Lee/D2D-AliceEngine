// ─────────────────────────────────────────────
// TileParser.h
// ─────────────────────────────────────────────
#pragma once
#include <Extension/json.hpp>
#include <unordered_map>
#include <vector>
#include <string>
using json = nlohmann::json;

// 적 스폰 데이터
struct CollData {
	int index = -1;
	int collisionChannel = -1;
	int enemyType = -1;
	bool isUsable = true;
};

// blocks.tsj 파일에 대응하는 구조체
struct TileSet {
	int columns;                 // 타일셋의 열 수
	std::string image;           // 이미지 파일 경로
	int imageheight;             // 이미지 높이
	int imagewidth;              // 이미지 너비
	int margin;                  // 이미지 가장자리 여백
	std::string name;            // 타일셋 이름
	int spacing;                 // 타일 간 간격
	int tilecount;               // 전체 타일 개수
	std::string tiledversion;    // Tiled 버전
	int tileheight;              // 개별 타일 높이
	int tilewidth;               // 개별 타일 너비
	std::string type;            // 타입 ("tileset")
	std::string version;         // 파일 포맷 버전
};

/* ───── 타일맵(Map) ───── */
struct TileMapLayer {
	std::string name;
	int width{}, height{};
	int id;
	std::vector<int> data;            // GID 리스트 (row-major)
	bool visible{ true };
	float opacity{ 1.f };
	std::string type;
	int x{ 0 }, y{ 0 };
};

struct TileMap {
	std::string mapName;              // 파일명 등
	int compressionlevel{ 0 };       // 압축 레벨 (0 = 없음)
	bool infinite{ false };           // 무한 맵 여부
	int nextlayerid{ 0 };            // 다음 레이어 ID
	int nextobjectid{ 0 }; 		 // 다음 오브젝트 ID
	std::string version;         // Tiled 버전
	int width{}, height{};
	int tileWidth{}, tileHeight{};
	std::string orientation;          // "orthogonal"…
	std::string renderorder;          // "right-down" 등
	std::string type;        // "map" 고정
	std::vector<TileMapLayer> layers;
	std::vector<int> usedGIDs;        // 중복 제거된 GID 집합
};

struct TileMapColiderInfo 
{
	int index;						// 1번부터 시작
	int collisionChannel;			// 콜리전 레이어
	int enemyType;					// 적 타입
};

/* ───── 로더(정적 유틸) ───── */
class TileMapLoader {
public:
	static void LoadTileSet(const std::wstring& filePath, TileSet& tileSet);
	static void LoadTileMap(const std::wstring& filePath, TileMap& tileMap); // TileMap도 Load함수와 Parse함수 추가
	static std::unordered_map<int, CollData> LoadTileMapColliderInfo(const std::wstring& filePath); // TileMap도 Load함수와 Parse함수 추가
};

void from_json(const nlohmann::json& j, TileSet& ts);
void to_json(nlohmann::json& j, const TileSet& ts);
void from_json(const json& j, TileMapLayer& l);
void to_json(json& j, const TileMapLayer& l);
void from_json(const json& j, TileMap& m); 
void to_json(json& j, const TileMap& m);
void from_json(const json& j, TileMapColiderInfo& m);
void to_json(json& j, const TileMapColiderInfo& m);
