#include "EnemySpawnTriggerBox.h"
#include <Component/Collider.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <System/ScriptSystem.h>
#include <Manager/SceneManager.h>
#include <Scripts/Enemy/Spawn/EnemySpawner.h>
#include <GameManager/EnemyDataManager.h>
#include <Scripts/Weapon/Drone.h>
#include <Prefab/Enemy/EnemyBike.h>
#include <Scripts/Enemy/EnemyStatScript.h>
#include <Prefab/Enemy/NormalTruck.h>
#include <Prefab/Enemy/JumpTruck.h>
#include <Prefab/Enemy/NormalCar.h>
#include <Scripts/Enemy/Spawn/SpawnData.h>
#include <GameManager/BulletManager.h>
#include <GameManager/GamePlayManager.h>
#include <Component/SpriteRenderer.h>

void EnemySpawnTriggerBox::Initialize()
{
    __super::Initialize();
    //REGISTER_SCRIPT_METHOD(OnStart);
}

void EnemySpawnTriggerBox::OnStart()
{
    m_collider = owner->AddComponent<Collider>();
    if (m_collider)
    {
        m_collider->SetBoxSize(m_boxSize);
        m_collider->SetLayer(m_boxLayer);
    }
    m_bSpawnable = true;
}

void EnemySpawnTriggerBox::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

// 일단 4번으로 보스가 나온다고 가정
void EnemySpawnTriggerBox::SpawnBossAt(const FVector2& worldPos)
{
    // 이름 정하기
    std::wstring name = L"Boss";
    gameObject* enemy = SceneManager::GetInstance().GetWorld()->NewObject<gameObject>(name);

    enemy->AddComponent<Car>(false);
    enemy->AddComponent<SpriteRenderer>()->LoadData(L"Enemy/Durang/boss_idle_notfix.png");
    enemy->GetComponent<SpriteRenderer>()->SetLayer(19999);
    enemy->AddComponent<Collider>()->SetBoxSize(FVector2(180, 180));
    enemy->SetScale(FVector2(1.2f, 1.2f));
   

    FDroneSpritePath dronePath(
        L"Enemy/Drone/enermy_Drone_body.png",
        L"Enemy/Drone/enermy_Drone_arm.png"
    );

    enemy->SetTag(L"Enemy");

    if (auto* statScript = enemy->AddComponent<EnemyStatScript>())
        statScript->SetEnemyTypeId(4);

    if (auto* drone = enemy->AddComponent<Drone>(dronePath))
    {
        drone->initBodyPos = FVector2(-50.0f, 130.0f);
        drone->initBodySize = FVector2(0.85f, 0.85f);
        drone->SetDroneType(EDroneType::Boss);
        
        // EnemyDataManager에서 데미지 값 가져와서 설정
        FEnemyStats stats{};
        if (EnemyDataManager::GetInstance().GetStats(4, stats))
        {
            drone->SetDamage(stats.damage);
            drone->SetAttackDelay(stats.attackDelay);
            drone->SetBulletSpeed(stats.bulletSpeed);
        }
    }

    if (auto* drone = enemy->AddComponent<Drone>(dronePath))
    {
        drone->initBodyPos = FVector2(-50.0f, -80.0f);
        drone->initBodySize = FVector2(0.85f, 0.85f);
        drone->SetDroneType(EDroneType::Boss);
        
        // EnemyDataManager에서 데미지 값 가져와서 설정
        FEnemyStats stats{};
        if (EnemyDataManager::GetInstance().GetStats(4, stats))
        {
            drone->SetDamage(stats.damage);
			drone->SetAttackDelay(stats.attackDelay);
			drone->SetBulletSpeed(stats.bulletSpeed);
        }
    }

    //enemy->SetPosition(worldPos);
	FVector2 playerPos = 0;
	if (auto player = GamePlayManager::GetInstance().GetPlayer())
	{
		playerPos += player->GetPosition();
	}
	enemy->SetPosition(playerPos + worldPos);
}

// 보스가 소환해내는 드론. 
// 보스 옆에 달려있는거 아닙니다. 보스가 시간초마다 소환하는 드론입니다.
void EnemySpawnTriggerBox::SpawnBossDroneAt(const FVector2& worldPos)
{
	// 이름 정하기
	std::wstring name = L"BossDrone";
	gameObject* enemy = SceneManager::GetInstance().GetWorld()->NewObject<gameObject>(name);

	enemy->AddComponent<Collider>()->SetBoxSize(FVector2(140, 140));

	FDroneSpritePath dronePath(
		L"Enemy/Drone/enermy_Drone_body.png",
		L"Enemy/Drone/enermy_Drone_arm.png"
	);

	enemy->SetTag(L"Enemy");

	if (auto* statScript = enemy->AddComponent<EnemyStatScript>())
		statScript->SetEnemyTypeId(999);

	if (auto* drone = enemy->AddComponent<Drone>(dronePath))
	{
		drone->initBodyPos = FVector2(0.0f, 0.0f);
		drone->initBodySize = FVector2(0.85f, 0.85f);
		drone->SetDroneType(EDroneType::BossSpawn);
        
        // 보스 드론 독립 이동 초기 설정
        
        drone->m_baseOffset = FVector2(
            FRandom::GetRandomInRange(1300, 1600),        // 플레이어 기준 상대 거리
            FRandom::GetRandomInRange(-300, 300)
        );     
        drone->m_movementSpeed = 100.0f;                   // 이동 속도
        drone->m_movementRadius = 100.0f;                  // 이동 반경
        drone->m_movementAngleSpeed = 120.0f;              // 각도 변화 속도
        drone->m_patternChangeInterval = 2.5f;             // 패턴 변경 간격
        
        // EnemyDataManager에서 데미지 값 가져와서 설정 (Boss 타입)
        FEnemyStats stats{};
        if (EnemyDataManager::GetInstance().GetStats(999, stats))
        {
            drone->SetDamage(stats.damage);
            drone->SetAttackDelay(stats.attackDelay);
            drone->SetBulletSpeed(stats.bulletSpeed);
        }
	}

	enemy->SetPosition(worldPos);
}

void EnemySpawnTriggerBox::SpawnEnemyInBossModeAt(int _enemyTypeId, const FVector2& worldPos)
{
	// 이름 정하기
	std::wstring name = L"EnemyBike";
	// 적 오브젝트 생성 및 공통 세팅
	gameObject* enemy = SceneManager::GetInstance().GetWorld()->NewObject<gameObject>(name);

	enemy->AddComponent<Collider>()->SetBoxSize(FVector2(80, 80));
	FEnemyStats stats{};
	bool t = EnemyDataManager::GetInstance().GetStats(_enemyTypeId, stats);

	FDroneSpritePath dronePath(
		L"Enemy/Drone/enermy_Drone_body.png",
		L"Enemy/Drone/enermy_Drone_arm.png"
	);

	enemy->SetTag(L"Enemy");
	auto* eb = enemy->AddComponent<EnemyBike>();
    eb->SetSkewed(false);
	if (auto* statScript = enemy->AddComponent<EnemyStatScript>())
		statScript->SetEnemyTypeId(_enemyTypeId);
	if (auto* drone = enemy->AddComponent<Drone>(dronePath))
	{
		drone->initBodyPos = FVector2(-120.0f, 85.0f);
		drone->initBodySize = FVector2(0.7f, 0.7f);
		drone->SetDroneType(EDroneType::Enemy);
		drone->SetDamage(stats.damage);
		drone->SetAttackDelay(stats.attackDelay);
        drone->SetBulletSpeed(stats.bulletSpeed);
	}

	enemy->transform()->SetPosition(worldPos);
}

void EnemySpawnTriggerBox::SpawnEnemyAt(int _enemyTypeId, const FVector2& worldPos)
{
    // 이름 정하기
    std::wstring name = L"";
    switch (_enemyTypeId)
    {
    case 0: name = L"EnemyBike"; break;
    case 4: name = L"EnemyBoss"; break;
    default: name = L"Neutral"; break;
    }

    // 적 오브젝트 생성 및 공통 세팅
    gameObject* enemy = SceneManager::GetInstance().GetWorld()->NewObject<gameObject>(name);
    EnemySpawner::EnemyType etype = static_cast<EnemySpawner::EnemyType>(_enemyTypeId);

    //enemy->AddComponent<Collider>()->SetBoxSize(FVector2(80, 80));
	FEnemyStats stats{};
    bool t = EnemyDataManager::GetInstance().GetStats(_enemyTypeId, stats);

    FDroneSpritePath dronePath(
		L"Enemy/Drone/enermy_Drone_body.png",
		L"Enemy/Drone/enermy_Drone_arm.png"
    );

    switch (etype)
    {
    case EnemySpawner::Bike:
        enemy->SetTag(L"Enemy");
        enemy->AddComponent<EnemyBike>();
        if (auto* statScript = enemy->AddComponent<EnemyStatScript>())
            statScript->SetEnemyTypeId(_enemyTypeId);
        if (auto* drone = enemy->AddComponent<Drone>(dronePath))
        {
            drone->initBodyPos = FVector2(-120.0f, 85.0f);
            drone->initBodySize = FVector2(0.7f, 0.7f);
            drone->SetDroneType(EDroneType::Enemy);
			drone->SetDamage(stats.damage);
			drone->SetAttackDelay(stats.attackDelay);
        }
        break;
    case EnemySpawner::Truck:
        enemy->SetTag(L"Obstacle");
        enemy->AddComponent<NormalTruck>();
        break;
    case EnemySpawner::JTruck:
        enemy->SetTag(L"JumpTrigger");
        enemy->AddComponent<JumpTruck>();
        break;
    case EnemySpawner::Car:
        enemy->SetTag(L"Obstacle");
        enemy->AddComponent<NormalCar>();
        break;
    case EnemySpawner::Boss:
        enemy->SetTag(L"Enemy");
        if (auto* statScript = enemy->AddComponent<EnemyStatScript>())
            statScript->SetEnemyTypeId(_enemyTypeId);
        if (auto* drone = enemy->AddComponent<Drone>(dronePath))
        {
            drone->initBodyPos = FVector2(50.0f, 40.0f);
            drone->SetDroneType(EDroneType::Enemy);
			drone->SetDamage(stats.damage);
			drone->SetAttackDelay(stats.attackDelay);
        }
        break;
    default:
        break;
    }

    enemy->transform()->SetPosition(worldPos);
}

void EnemySpawnTriggerBox::OnTriggerEnter2D(Collider* collider)
{
    if (!m_bSpawnable || !collider) return;

    if (collider->GetOwner()->GetTag() == L"EnemySpawn")
    {
        CollData temp = collider->GetOwner()->GetComponent<SpawnData>()->GetCollData();
        int etype = temp.enemyType;

        SpawnEnemyAt(etype, collider->GetOwner()->GetPosition());
    }
}
