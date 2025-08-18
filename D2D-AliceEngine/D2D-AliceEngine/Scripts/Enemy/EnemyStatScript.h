#pragma once
#include <Component/ScriptComponent.h>
#include <Core/StatTraits.h>
#include <Component/StatComponent.h>
#include <Manager/TimerManager.h>

struct EnemyStat {
    float HP = 10.f;
    float MAXHP = 10.f;
    float BATTERY = 0.f;
    float MOVE_SPEED = 0.f;
    float ATTACK_DELAY = 0.f;
    float DAMAGE = 0.f;
};

DEFINE_STAT_TRAITS_6(EnemyStat, HP, MAXHP, BATTERY, MOVE_SPEED, ATTACK_DELAY, DAMAGE)

class ProgressBarComponent;

class EnemyStatScript : public ScriptComponent
{
public:
    void Initialize() override;
    void Update(const float& deltaSeconds) override;

    virtual void OnStart() override;
    virtual void OnEnd() override;
    virtual void OnDestroy() override;

    // 외부(스폰 시)에서 주입할 수 있도록 노출
    void SetEnemyTypeId(int typeId) { m_enemyTypeId = typeId; }

    StatComponent<EnemyStat>* m_enemyStat{ nullptr };
private:
    // BikeStatScript와 동일한 HP 바 구성
    ProgressBarComponent* m_hpBarBack{ nullptr };
    ProgressBarComponent* m_hpBarFill{ nullptr };
    int m_enemyTypeId{ 0 };
    FTimerHandle m_fadeHandle;
};


