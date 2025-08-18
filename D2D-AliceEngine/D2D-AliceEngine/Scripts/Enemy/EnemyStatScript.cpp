#include "EnemyStatScript.h"
#include <Component/StatComponent.h>
#include <Component/ProgressBarComponent.h>
#include <Helpers/CoordHelper.h>
#include <Component/SpriteRenderer.h>
#include <GameManager/EnemyDataManager.h>
#include <Component/BoxComponent.h>
#include <Component/Collider.h>
#include <Prefab/Enemy/Core/Car.h>
#include <Scripts/Weapon/Drone.h>
#include <System/ScriptSystem.h>
#include <Object/gameObject.h>
#include <GameManager/GamePlayManager.h>
#include <Manager/SceneManager.h>
#include <Scripts/Weapon/BulletColl.h>

void EnemyStatScript::Initialize()
{
    __super::Initialize();
    //REGISTER_SCRIPT_METHOD(OnStart);
    //REGISTER_SCRIPT_METHOD(OnEnd);
    //REGISTER_SCRIPT_METHOD(OnDestroy);
}

void EnemyStatScript::Update(const float& deltaSeconds)
{
    __super::Update(deltaSeconds);
}

void EnemyStatScript::OnStart()
{
    // 스탯 컴포넌트 생성 및 기본값
    m_enemyStat = owner->AddComponent<StatComponent<EnemyStat>>();

    // EnemyDataManager에서 타입별 데이터 로드 및 주입
    FEnemyStats stats{};
    if (EnemyDataManager::GetInstance().GetStats(m_enemyTypeId, stats))
    {
        m_enemyStat->SetStat("MAXHP", static_cast<float>(stats.maxHP));
        m_enemyStat->SetStat("HP",    static_cast<float>(stats.hp));
        m_enemyStat->SetStat("BATTERY",      stats.battery);
        m_enemyStat->SetStat("BULLET_SPEED",   stats.bulletSpeed);
        m_enemyStat->SetStat("ATTACK_DELAY", stats.attackDelay);
        m_enemyStat->SetStat("DAMAGE",       stats.damage);
    }

    // HP 바: BikeStatScript와 동일한 방식으로 구성
    m_hpBarBack = owner->AddComponent<ProgressBarComponent>();
    m_hpBarBack->LoadData(L"UI/HealthBar_White.png");
    m_hpBarBack->SetType(EProgressBarType::Linear);
    m_hpBarBack->SetProgress(1.0f);
    m_hpBarBack->SetOpacity(0.6f);
    m_hpBarBack->SetRelativeScale(FVector2(0.6f, 0.4f));
    {
        FVector2 size = m_hpBarBack->GetRelativeSize();
        m_hpBarBack->SetRelativePosition(CoordHelper::RatioCoordToScreen(size, FVector2(0.0f, 2.0f)));
    }
    m_hpBarBack->SetLayer(10004);

    m_hpBarFill = owner->AddComponent<ProgressBarComponent>();
    m_hpBarFill->LoadData(L"UI/HealthBar_Red.png");
    m_hpBarFill->SetType(EProgressBarType::Linear);
    m_hpBarFill->SetProgress(1.0f);
    m_hpBarFill->SetRelativeScale(FVector2(0.6f, 0.4f));
    {
        FVector2 size = m_hpBarFill->GetRelativeSize();
        m_hpBarFill->SetRelativePosition(CoordHelper::RatioCoordToScreen(size, FVector2(0.0f, 2.0f)));
    }
    m_hpBarFill->SetLayer(m_hpBarBack->GetLayer() + 1);

    // 델리게이트 연동: HP/MAXHP 변경 시 진행도 갱신
    m_enemyStat->OnChangeStatMap["HP"].Add(owner->GetHandle(), [this](float /*oldVal*/, float newVal)
    {
        if (m_hpBarFill && m_enemyStat)
        {
            float maxv = m_enemyStat->GetStat("MAXHP");
            if (maxv <= 0.0f) maxv = 0.0001f;
            m_hpBarFill->SetProgress(newVal / maxv);
        }
        // HP 0 이하 → 파괴/이펙트 처리 (플레이어와 동일 로직)
        if (newVal <= 0)
        {
            if (auto box = owner->GetComponent<BoxComponent>())
                owner->RemoveComponent<BoxComponent>(box);
            if (auto col = owner->GetComponent<Collider>())
                owner->RemoveComponent<Collider>(col);

            GamePlayManager::GetInstance().AddKillEnemyAmount(1);
            if (owner->GetName() == L"Boss")
            {
                GamePlayManager::GetInstance().BackNormalMode();
            }

            if (auto car = owner->GetComponent<Car>()) {
                car->DelayDestroy();
                // BulletColl 삭제 -성근수정-
                car->DestroybColl();
            }
                
            if (auto drone = owner->GetComponent<Drone>())
                drone->DelayDestroy();

			TimerManager::GetInstance().ClearTimer(m_fadeHandle);
			TimerManager::GetInstance().SetTimer(
				m_fadeHandle,
				[weak = WeakFromThis<EnemyStatScript>()]() mutable {
					if (weak.expired()) return;
					// 자신 파괴
					weak->GetWorld()->RemoveObject(weak->GetOwner());
				},
				1.0f,
				false,
				1.0f
			);
            return;
        }
    });
    m_enemyStat->OnChangeStatMap["MAXHP"].Add(owner->GetHandle(), [this](float /*oldVal*/, float /*newVal*/)
    {
        if (m_hpBarFill && m_enemyStat)
        {
            float cur = m_enemyStat->GetStat("HP");
            float maxv = m_enemyStat->GetStat("MAXHP");
            if (maxv <= 0.0f) maxv = 0.0001f;
            m_hpBarFill->SetProgress(cur / maxv);
        }
    });

    // 초기 진행도 반영
    if (m_hpBarFill && m_enemyStat)
    {
        float cur = m_enemyStat->GetStat("HP");
        float maxv = m_enemyStat->GetStat("MAXHP");
        if (maxv <= 0.0f) maxv = 0.0001f;
        m_hpBarFill->SetProgress(cur / maxv);
    }
}

void EnemyStatScript::OnEnd()
{
}

void EnemyStatScript::OnDestroy()
{
}


