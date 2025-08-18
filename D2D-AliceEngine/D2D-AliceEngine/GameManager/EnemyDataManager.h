#pragma once
#include <Core/Singleton.h>
#include <unordered_map>
#include <string>
#include <TileMap/TileMapLoader.h>

struct FEnemyStats
{
    int   maxHP      = 0;
    int   hp         = 0;
    float battery    = 0.0f;
    float bulletSpeed  = 0.0f;
    float attackDelay= 0.0f;
    float damage     = 0.0f;
};

class EnemyDataManager : public Singleton<EnemyDataManager>
{
public:
    EnemyDataManager() = default;
    ~EnemyDataManager() = default;

    // 스탯 로드/조회
    void LoadData(const std::wstring& relativePath);
    bool LoadFromJson(const std::wstring& relativePath);
    bool GetStats(int enemyTypeId, FEnemyStats& outStats) const;
    bool HasStats(int enemyTypeId) const;

    // 스폰 데이터 로드/조회 (TileMapLoader의 CollData 기반)
    void LoadSpawnData(const std::wstring& relativePath);
    bool HasSpawnData(int index) const;
    bool GetSpawnData(int index, CollData& outData) const;

private:
    bool m_loaded = false;
    std::unordered_map<int, FEnemyStats> m_stats;

    bool m_spawnLoaded = false;
    std::unordered_map<int, CollData> m_spawnTable;
};


