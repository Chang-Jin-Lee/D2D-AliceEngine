#pragma once
#include <Core/Singleton.h>
#include <string>

// 플레이어 스탯 구조체
struct FPlayerStats
{
    int   maxHP       = 0;
    int   hp          = 0;
    float battery     = 0.0f;
    float bulletSpeed = 0.0f;
    float attackDelay = 0.0f;
    float damage      = 0.0f;
};

/*
 * @brief 플레이어 스탯 데이터를 로드하고 제공하는 매니저
 */
class PlayerDataManager : public Singleton<PlayerDataManager>
{
public:
    PlayerDataManager() = default;
    ~PlayerDataManager() = default;

    void LoadData(const std::wstring& relativePath);
    bool LoadFromJson(const std::wstring& relativePath);

    // 데이터가 로드되었는지
    bool HasData() const { return m_loaded; }

    // 스탯 가져오기 (로드 실패 시 기본값 반환)
    const FPlayerStats& GetStats() const { return m_stats; }

private:
    bool m_loaded = false;
    FPlayerStats m_stats;
};
