#include "BulletManager.h"
#include <Scripts/Weapon/Bullet.h>
#include <Manager/SceneManager.h>
#include <Math/TMath.h>

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
}

void BulletManager::FireBullet(const FVector2& start, const FVector2& target, const FVector2& initVelocity, const EDroneType& dronType, float damage)
{
	Scene* curScene = SceneManager::GetInstance().GetWorld();
	auto bulletObj = curScene->NewObject<gameObject>(L"bullet");
	auto bullet = bulletObj->AddComponent<Bullet>();

	bulletObj->SetPosition(start);
	bulletObj->SetScale(FVector2(1.7, 1.7));

	switch (dronType)
	{
	case EDroneType::Player :
		bullet->SetSpritePath(L"Player/bullet_blue_final.png");
		break;
	case EDroneType::Enemy :
	case EDroneType::BossSpawn:
	case EDroneType::Boss:
		bullet->SetSpritePath(L"Enemy/bullet_red_final.png");
		break;
	default:
		break;
	}

	bullet->SetDroneType(dronType);
	bullet->SetDamage(damage); // 데미지 설정
	bullet->inheritedVelocity = initVelocity;
	bullet->bulletType = EBulletType::Linear; // 기본적으로 BezierCurve로 설정

	FVector2 dir = target - start;
	float distance = dir.Length();
	FVector2 dirNormal = dir.Normalize();
	FVector2 perp(-dirNormal.y, dirNormal.x); // 수직 방향

	float randMultiply = FRandom::GetRandomInRange(0.1, 0.9);
	//컨트롤 포인트 계산: 시작점과 끝점 중간에서 위로 올림
	FVector2 mid = (start + target) * randMultiply;

	// 거리 기반으로 휘는 정도 비율 계산 (예: 거리의 10%)
	float randCurveMultiply = FRandom::GetRandomInRange(0.05f, 0.07f);
	float curveRatio = randCurveMultiply; // 이걸 낮추면 덜 휨
	float curveAmount = distance * curveRatio;

	// 좌우 랜덤 휘기
	float sign = (rand() % 2 == 0) ? 1.0f : -1.0f;
	FVector2 offset = perp * curveAmount * sign;

	bullet->P0 = start;
	bullet->P1 = mid + offset;
	bullet->P2 = target;

	bullet->startPos = start;
	bullet->currentPos = start;
	bullet->moveDir = dirNormal;

	bullets.push_back(bullet);
}

void BulletManager::FireBulletCatmull(const std::vector<FVector2>& controlPoints, float totalDuration, const EDroneType& droneType, float damage)
{
	if (controlPoints.size() < 4) return;
	Scene* curScene = SceneManager::GetInstance().GetWorld();
	auto bulletObj = curScene->NewObject<gameObject>(L"bullet_catmull");
	auto bullet = bulletObj->AddComponent<Bullet>();

	bulletObj->SetPosition(controlPoints.front());
	bulletObj->SetScale(FVector2(1.7, 1.7));

	switch (droneType)
	{
	case EDroneType::Player:
		bullet->SetSpritePath(L"Player/bullet_blue_final.png");
		break;
	case EDroneType::Enemy:
	case EDroneType::BossSpawn:
	case EDroneType::Boss:
		bullet->SetSpritePath(L"Enemy/bullet_red_final.png");
		break;
	default:
		break;
	}

	bullet->SetDroneType(droneType);
	bullet->SetDamage(damage);
	bullet->bulletType = EBulletType::CatmullRom;
	bullet->catmullPoints = controlPoints;
	bullet->splineDuration = totalDuration;

	// 초기 진행 방향 설정(첫 두 점 기준)
	FVector2 dir = controlPoints[1] - controlPoints[0];
	bullet->moveDir = dir.Normalize();

	bullets.push_back(bullet);
}
