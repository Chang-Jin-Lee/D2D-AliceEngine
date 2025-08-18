#include "EnemyDataManager.h"
#include <Define/Define.h>
#include <Extension/json.hpp>
#include <Helpers/FileHelper.h>
#include <fstream>

using json = nlohmann::json;

void EnemyDataManager::LoadData(const std::wstring& relativePath)
{
    if (m_loaded) return;
    // 기본 경로
    LoadFromJson(Define::BASE_RESOURCE_PATH + relativePath);
    m_loaded = true; // 시도 플래그 (성공/실패와 무관하게 중복 로드 방지)
}

bool EnemyDataManager::LoadFromJson(const std::wstring& relativePath)
{
    try
    {
        std::wstring filePath = FileHelper::ToAbsolutePath(relativePath);
        std::ifstream in(filePath);
        if (!in.is_open()) return false;
        json j; in >> j; in.close();
        if (!j.is_array()) return false;
        m_stats.clear();
        for (const auto& entry : j)
        {
            int typeId = entry.value("typeId", -1);
            if (typeId < 0) continue;
            FEnemyStats st;
            st.maxHP       = entry.value("maxHP", 0);
            st.hp          = entry.value("hp", st.maxHP);
            st.battery     = entry.value("battery", 0.0f);
            st.bulletSpeed = entry.value("bulletSpeed", 0.0f);
            st.attackDelay = entry.value("attackDelay", 0.0f);
            st.damage      = entry.value("damage", 0.0f);
            m_stats[typeId] = st;
        }
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool EnemyDataManager::GetStats(int enemyTypeId, FEnemyStats& outStats) const
{
    auto it = m_stats.find(enemyTypeId);
    if (it == m_stats.end()) return false;
    outStats = it->second;
    return true;
}

bool EnemyDataManager::HasStats(int enemyTypeId) const
{
    return m_stats.find(enemyTypeId) != m_stats.end();
}

void EnemyDataManager::LoadSpawnData(const std::wstring& relativePath)
{
    if (m_spawnLoaded) return;
    try
    {
        const std::wstring path = Define::BASE_RESOURCE_PATH + relativePath;
        m_spawnTable = TileMapLoader::LoadTileMapColliderInfo(FileHelper::ToAbsolutePath(path));
        m_spawnLoaded = true;
    }
    catch (...)
    {
        m_spawnLoaded = false;
    }
}

bool EnemyDataManager::HasSpawnData(int index) const
{
    return m_spawnTable.find(index) != m_spawnTable.end();
}

bool EnemyDataManager::GetSpawnData(int index, CollData& outData) const
{
    auto it = m_spawnTable.find(index);
    if (it == m_spawnTable.end()) return false;
    outData = it->second;
    return true;
}


