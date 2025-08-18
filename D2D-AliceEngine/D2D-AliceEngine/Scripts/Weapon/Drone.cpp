#include "Drone.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
#include <System/ScriptSystem.h>
#include <Manager/UpdateTaskManager.h>
#include <GameManager/BulletManager.h>
#include <Scripts/Bike/BikeMovementScript.h>
#include <Component/SkewTransform.h>
#include <GameManager/GamePlayManager.h>
#include <Scripts/Player/PlayerManager.h>
#include <Core/Input.cpp>

Drone::Drone(FDroneSpritePath path)
{
	spritePath = path;
}

void Drone::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);

	//REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PostPhysics);
	//REGISTER_SCRIPT_TICK(Update);

	TimerManager::GetInstance().ClearTimer(timer);
	TimerManager::GetInstance().SetTimer(
		timer,
		[this]()
		{
			bWaitForSecond = false;
		},
		0.0f,
		false,
		0.2f
	);
}

void Drone::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void Drone::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
    if (GamePlayManager::GetInstance().IsPaused()) return;
    if (bDelayDestroying) {
        // 페이드 아웃 진행 (Update 기반, 타이머는 삭제만 담당)
        if (body) {
            fadeElapsed += deltaSeconds;
            float t = fadeElapsed / fadeDuration;
            if (t > 1.0f) t = 1.0f;
            body->SetOpacity(1.0f - t);
            if (arm) arm->SetOpacity(1.0f - t);
        }
        return; // 소멸 중에는 로직 중단
    }
    if (bWaitForSecond) return;

	// 여기에 Update에 대한 로직 작성
	// Skew랑은 상관없음.
	if (!arm) return;
	FVector2 bodyPos = arm->GetRelativePosition();
	FVector2 worldMousePos = Input::GetMouseWorldPosition(); // 마우스의 실제 월드 좌표
	FVector2 dir = worldMousePos - bodyPos;
	FVector2 dirNormal = dir.Normalize();
	
	// 보스 드론 독립 이동 처리
	if (droneType == EDroneType::BossSpawn)
	{
		UpdateBossDroneMovement(deltaSeconds);
	}
	
    // 반동 복귀 처리 (총을 안 쏘는 동안 서서히 원래 위치로)
    if (recoilOffset.x != 0.0f || recoilOffset.y != 0.0f)
    {
        FVector2 toOrigin = FVector2(0.0f, 0.0f) - recoilOffset;
        float len = toOrigin.Length();
        if (len > 0.0f)
        {
            FVector2 step = toOrigin.Normalize() * (recoilReturnSpeed * deltaSeconds);
            if (step.Length() >= len) recoilOffset = FVector2(0.0f, 0.0f);
            else recoilOffset = recoilOffset + step;
        }
    }

    Floating(deltaSeconds, dirNormal);
	AttackAction(bodyPos, worldMousePos, dirNormal);
}

void Drone::Floating(const float& deltaSeconds, const FVector2& dirNormal)
{
	//body->SetRelativePosition(FVector2(27, 6));
	//// 시간 누적
	elapsed += deltaSeconds;
	// 구간이 끝나면 방향 전환
	if (elapsed >= duration)
	{
		elapsed = 0.0f;
		goingUp = !goingUp;
	}

	float currY;
	if (goingUp)
	{
		// 위로 이동
		currY = Math::EaseInOut(startY, endY, elapsed, duration);
	}
	else
	{
		// 아래로 이동
		currY = Math::EaseInOut(endY, startY, elapsed, duration);
	}

	// 오브젝트 Y좌표 적용
	auto pos = body->GetRelativePosition();
	pos.y = currY;
    // 반동 오프셋을 최종 위치에 가산
    body->SetRelativePosition(initBodyPos + FVector2(0, pos.y) + recoilOffset);
}

void Drone::AttackAction(const FVector2& bodyPos, const FVector2& worldMousePos, const FVector2& dirNormal)
{
	if (GamePlayManager::GetInstance().IsPaused()) return;
	switch (droneType)
	{
	case EDroneType::Player:
	{
		// float 버전 사용 + 항상 초기화
		const float angleRad = std::atan2f(dirNormal.y, dirNormal.x);
		const float angleDeg = angleRad * 180.0f / Define::PI + armDegree;
		arm->SetRelativeRotation(angleDeg);

		if (Input::IsMouseLeftDown() && !PlayerManager::instance->GetSande() && bCanFire)
		{
			float currentSpeed = 0.0f;
			if (auto bike = owner->GetComponent<BikeMovementScript>())
				currentSpeed = bike->GetCurrSpeed();
			currentSpeed += m_bulletSpeed;
			const FVector2 speed{ currentSpeed, 0.0f };
			BulletManager::GetInstance().FireBullet(bodyPos, worldMousePos, speed, droneType, damage);

			// Player 반동 적용: 발사 방향의 반대쪽으로 recoilImpulse 누적 (최대거리 제한)
			{
				FVector2 fireDir = (worldMousePos - bodyPos).Normalize();
				if (fireDir.Length() > 0.0f)
				{
					recoilDir = fireDir * -1.0f;
					FVector2 candidate = recoilOffset + recoilDir * recoilImpulse;
					if (candidate.Length() > recoilMaxDistance)
						candidate = candidate.Normalize() * recoilMaxDistance;
					recoilOffset = candidate;
				}
			}
			bCanFire = false;
		}

		if (Input::IsMouseLeftDown() && PlayerManager::instance->GetSande() && bCanFire)
		{
			if (auto player = BulletManager::GetInstance().GetPlayer())
			{
				// TODO : 지금은 20도 -20도 해서 총 3발을 발사하는것이지만, 캣멀롬 스플라인을 써서 마우스 위치로 탄환이 날아가는 것을 만들고 싶음. 
				// 이때 캣멀롬 스플라인의 중간 포인트들은 플레이어와 마우스 포지션을 잇는 직선의 위로 위치, 아래로 위치를 적절히 섞어서 랜덤하게 작은 미사일들이 많이 날아가는 느낌을 원함. 
				
				//float currentSpeed = 0.0f;
                //if (auto bike = player->GetComponent<BikeMovementScript>())
                //  currentSpeed = bike->GetCurrSpeed();
                //currentSpeed += m_bulletSpeed;
                //const FVector2 speed{ currentSpeed, 0.0f };
                //
                //// 0도(원본)
                //BulletManager::GetInstance().FireBullet(bodyPos, worldMousePos, speed, droneType);
                //
                //// 회전 함수
                //auto RotateVector = [](const FVector2& v, float degree) -> FVector2 {
                //  float rad = degree * Define::PI / 180.0f;
                //  float cs = std::cos(rad);
                //  float sn = std::sin(rad);
                //  return { -v.y * sn + v.x * cs, -v.y * cs + v.x * sn };
                //  };
                //
                //// +30도
                //{
                //  FVector2 dir30 = RotateVector(dirNormal, 10.0f);
                //  FVector2 newTarget = bodyPos + dir30 * 1000.0f; // 먼 위치로 타겟
                //  BulletManager::GetInstance().FireBullet(bodyPos, newTarget, speed, droneType);
                //}
                //
                //// -30도
                //{
                //  FVector2 dirNeg30 = RotateVector(dirNormal, -10.0f);
                //  FVector2 newTarget = bodyPos + dirNeg30 * 1000.0f;
                //  BulletManager::GetInstance().FireBullet(bodyPos, newTarget, speed, droneType);
                //}

				// Catmull-Rom 스플라인 탄환 추가 발사 (기존 코드는 유지)
				{
					const FVector2 start = bodyPos;
					const FVector2 target = worldMousePos;
					// 중간 포인트들을 무작위로 위/아래로 배치하여 물결치는 경로 생성
					const int numMissiles = 3; // 작은 미사일 여러 발
					for (int i = 0; i < numMissiles; ++i)
					{
						std::vector<FVector2> cps;
						cps.reserve(6);
						// 입력 최소 4포인트 필요: 시작 전/후 패딩 포함
						FVector2 dir = (target - start).Normalize();
						FVector2 amount = dir * 430;
						FVector2 relativeTarget = target + amount;
						FVector2 perp(-dir.y, dir.x);
						float dist = (relativeTarget - start).Length();
						float arc = FRandom::GetRandomInRange(0.08f, 0.18f) * dist; // 굴곡 크기
						float arc2 = FRandom::GetRandomInRange(0.06f, 0.14f) * dist;
						float splay = FRandom::GetRandomInRange(-1.0f, 1.0f);
						// 앞뒤 패딩 포인트(스플라인 안정)
						cps.push_back(start - dir * 60.0f);
						cps.push_back(start);
						cps.push_back(start + dir * (dist * 0.35f) + perp * arc * splay);
						cps.push_back(start + dir * (dist * 0.68f) - perp * arc2 * splay);
						cps.push_back(relativeTarget);
						cps.push_back(relativeTarget + dir * 60.0f);
						float totalDuration = FRandom::GetRandomInRange(0.55f, 0.85f);
						BulletManager::GetInstance().FireBulletCatmull(cps, totalDuration, droneType, damage);
					}
				}
			}
			bCanFire = false;
		}
		break;
	}
	case EDroneType::Enemy:
	case EDroneType::BossSpawn:
	{
		if (auto player = BulletManager::GetInstance().GetPlayer())
		{
			// 1발짜리
			//FVector2 targetPos = player->transform() ? player->transform()->GetPosition() : FVector2(0,0);
			////	: FVector2{ 0.0f, 0.0f };
			//float currentSpeed = 0.0f;
			//FVector2 dir = bodyPos - targetPos;
			//FVector2 _dirNormal = dir.Normalize();
			//// float 버전 사용 + 항상 초기화
			//const float angleRad = std::atan2f(_dirNormal.y, _dirNormal.x);
			//const float angleDeg = angleRad * 180.0f / Define::PI;
			//arm->SetRelativeRotation(angleDeg);

			//if (bCanFire)
			//{
			//	if (auto bike = player->GetComponent<BikeMovementScript>())
			//	{
			//		// 안전하게 접근
			//		if (auto ownerGO = bike->GetOwner())
			//			targetPos = ownerGO->transform() ? ownerGO->transform()->GetPosition() : targetPos;

			//		currentSpeed = bike->GetCurrSpeed();
			//	}

			//	const FVector2 speed{ currentSpeed, 0.0f };
			//	BulletManager::GetInstance().FireBullet(bodyPos, targetPos, speed, droneType);
			//	bCanFire = false;
			//}

			//// 3발짜리
			//FVector2 targetPos = player->transform() ? player->transform()->GetPosition()
			//	: FVector2{ 0.0f, 0.0f };
			//float currentSpeed = 0.0f;

			//FVector2 dir = bodyPos - targetPos; // 적 → 플레이어 방향
			//FVector2 dirNormal = dir.Normalize();

			//const float angleRad = std::atan2f(dirNormal.y, dirNormal.x);
			//const float angleDeg = angleRad * 180.0f / Define::PI + armDegree;;
			//arm->SetRelativeRotation(angleDeg);

			//if (bCanFire)
			//{
			//	if (auto bike = player->GetComponent<BikeMovementScript>())
			//	{
			//		if (auto ownerGO = bike->GetOwner())
			//			targetPos = ownerGO->transform() ? ownerGO->transform()->GetPosition() : targetPos;

			//		currentSpeed = bike->GetCurrSpeed();
			//	}

			//	const FVector2 speed{ currentSpeed, 0.0f };

			//	// 0도(원본)
			//	BulletManager::GetInstance().FireBullet(bodyPos, targetPos, speed, droneType);

			//	// 회전 함수
			//	auto RotateVector = [](const FVector2& v, float degree) -> FVector2 {
			//		float rad = degree * Define::PI / 180.0f;
			//		float cs = std::cos(rad);
			//		float sn = std::sin(rad);
			//		return {   v.y * sn - v.x * cs, v.y * cs - v.x * sn };
			//		};

			//	// +30도
			//	{
			//		FVector2 dir30 = RotateVector(dirNormal, 30.0f);
			//		FVector2 newTarget = bodyPos + dir30 * 1000.0f; // 먼 위치로 타겟
			//		BulletManager::GetInstance().FireBullet(bodyPos, newTarget, speed, droneType);
			//	}

			//	// -30도
			//	{
			//		FVector2 dirNeg30 = RotateVector(dirNormal, -30.0f);
			//		FVector2 newTarget = bodyPos + dirNeg30 * 1000.0f;
			//		BulletManager::GetInstance().FireBullet(bodyPos, newTarget, speed, droneType);
			//	}

			//	bCanFire = false;
			//}
			// 
			// 평시 포신은 플레이어 대략 조준(시각용)

			// 동그랗게 3발 
			FVector2 targetPos = player->transform() ? player->transform()->GetPosition() : FVector2{ 0,0 };
			FVector2 dir = targetPos - bodyPos ;
			FVector2 _dirNormal = dir.Normalize();
			// arm 회전
			const float angleRad = std::atan2f(_dirNormal.y, _dirNormal.x);
			const float angleDeg = angleRad * 180.0f / Define::PI;
			if (arm) arm->SetRelativeRotation(angleDeg);

			 //bCanFire가 true일 때만 버스트 시작(1회에 3발)
            if (bCanFire)
            {
                bCanFire = false;       // 외부 쿨다운 타이머로 다시 켜짐(기존 OnStart의 timer)
                burstRemaining = 3;     // 연속 3발
                // 첫 발 즉시 발사
                FireOneBurstShot();
                // 이후 남은 발은 간격을 두고 순차 발사
                if (burstRemaining > 0)
                {
                    TimerManager::GetInstance().SetTimer(
                        burstTimer,
                        [weak = WeakFromThis<Drone>()]() 
						{ 
							if(!weak.expired())
								weak->FireOneBurstShot();
						},
                        burstInterval,
                        true,              // 루프: 남은 발이 0이 되면 내부에서 타이머 정리
                        burstInterval      // 첫 지연도 동일 간격으로 설정
                    );
                }
            }
		}
		break;
	}
	case EDroneType::Boss:
	{
		if (auto player = BulletManager::GetInstance().GetPlayer())
		{
			FVector2 targetPos = player->transform() ? player->transform()->GetPosition() : FVector2{ 0,0 };
			FVector2 dir = targetPos - bodyPos;
			FVector2 _dirNormal = dir.Normalize();
			// arm 회전(시각용)
			const float angleRad = std::atan2f(_dirNormal.y, _dirNormal.x);
			const float angleDeg = angleRad * 180.0f / Define::PI;
			if (arm) arm->SetRelativeRotation(angleDeg);

			// 이미 어떤 패턴이 진행 중이면 타이머/프레임에 맡기고 종료
			if (bPatternRunning) break;

			// 쿨타임 OK면 새 패턴 선택
			if (bCanFire)
			{
				// 확률 정규화
				float p1 = Math::Clamp(probSingle, 0.0f, 1.0f);
				float p2 = Math::Clamp(probBurst3, 0.0f, 1.0f);
				float p3 = Math::Clamp(probSweep, 0.0f, 1.0f);
				float sum = p1 + p2 + p3;
				if (sum <= 0.0f) { p1 = 1.0f; p2 = p3 = 0.0f; sum = 1.0f; }
				p1 /= sum; p2 /= sum; p3 /= sum;

				float r = FRandom::GetRandomInRange(0.0f, 1.0f);

				// 공통: 시작 시 쿨다운
				bCanFire = false;
				bPatternRunning = true;

				if (r < p1)
				{
					// ① 단발 패턴
					currentPattern = EBossPattern::Single;

					float currentSpeed = 0.0f;
					if (auto bike = player->GetComponent<BikeMovementScript>())
						currentSpeed = bike->GetCurrSpeed();
					const FVector2 speed{ currentSpeed, 0.0f };

					BulletManager::GetInstance().FireBullet(bodyPos, targetPos, speed, droneType, damage);

					// 반동
					FVector2 fireDir = (targetPos - bodyPos).Normalize();
					if (fireDir.Length() > 0.0f) {
						recoilDir = fireDir * -1.0f;
						FVector2 candidate = recoilOffset + recoilDir * recoilImpulse;
						if (candidate.Length() > recoilMaxDistance)
							candidate = candidate.Normalize() * recoilMaxDistance;
						recoilOffset = candidate;
					}

					// 단발은 즉시 종료
					bPatternRunning = false;
					currentPattern = EBossPattern::None;
				}
				else if (r < p1 + p2)
				{
					// ② 3연사(버스트) 패턴
					currentPattern = EBossPattern::Burst3;
					bBurstRunning = true;

					burstRemaining = 3; // 첫 발 즉시
					FireOneBurstShot();

					if (burstRemaining > 0)
					{
						TimerManager::GetInstance().SetTimer(
							burstTimer,
							[weak = WeakFromThis<Drone>()]()
							{
								if (weak.expired()) return;
								Drone* self = weak.operator->();
								self->FireOneBurstShot();
								// FireOneBurstShot에서 잔여 0이면 타이머를 끄고 플래그를 내립니다.
							},
							burstInterval,
							true,
							burstInterval
						);
					}
					else
					{
						// 한 프레임에 3발 모두 끝난 경우 대비
						bBurstRunning = false;
						bPatternRunning = false;
						currentPattern = EBossPattern::None;
					}
				}
				else
				{
					// ③ 스윕 패턴
					currentPattern = EBossPattern::Sweep;
					bSweepRunning = true;

					// 기준 스냅샷
					sweepBasePos = bodyPos;
					sweepBaseDir = (targetPos - bodyPos).Normalize();

					// 각도 범위 설정 (-A ~ +A, step 간격)
					const float start = -sweepMaxAngle;
					const float end = sweepMaxAngle;
					sweepIndex = 0;
					sweepSteps = static_cast<int>(std::floor((end - start) / sweepStepDeg)) + 1;

					// 타이머 시작
					TimerManager::GetInstance().ClearTimer(sweepTimer);
					TimerManager::GetInstance().SetTimer(
						sweepTimer,
						[weak = WeakFromThis<Drone>(), start]()
						{
							if (weak.expired()) return;
							Drone* self = weak.operator->();

							// 끝났으면 종료 정리
							if (self->sweepIndex >= self->sweepSteps) {
								TimerManager::GetInstance().ClearTimer(self->sweepTimer);
								self->bSweepRunning = false;
								self->bPatternRunning = false;
								self->currentPattern = EBossPattern::None;
								return;
							}

							// 현재 각도
							float angle = start + self->sweepStepDeg * self->sweepIndex;

							// 발사 위치/방향
							FVector2 firePos = self->arm ? self->arm->GetRelativePosition() : self->sweepBasePos;
							FVector2 dir = Math::Rotate(self->sweepBaseDir, angle);
							FVector2 target = firePos + dir * self->sweepRange;

							// 플레이어 이동속도 보정
							float currentSpeed = 0.0f;
							if (auto player = BulletManager::GetInstance().GetPlayer()) {
								if (auto bike = player->GetComponent<BikeMovementScript>()) {
									currentSpeed = bike->GetCurrSpeed();
								}
							}
							const FVector2 speed{ currentSpeed, 0.0f };

							// 발사
							BulletManager::GetInstance().FireBullet(firePos, target, speed, self->droneType, self->damage);

							// 반동
							FVector2 fireDir = (target - firePos).Normalize();
							if (fireDir.Length() > 0.0f) {
								self->recoilDir = fireDir * -1.0f;
								FVector2 candidate = self->recoilOffset + self->recoilDir * self->recoilImpulse;
								if (candidate.Length() > self->recoilMaxDistance)
									candidate = candidate.Normalize() * self->recoilMaxDistance;
								self->recoilOffset = candidate;
							}

							++self->sweepIndex;
						},
						sweepInterval,
						true,
						0.0f
					);
				}
			}
		}
		break;
	}
	default:
		break;
	}
}

// 한 발 쏘고, 남았으면 다음 발 타이머 예약
void Drone::FireOneBurstShot()
{
	if (GamePlayManager::GetInstance().IsPaused()) return;
	auto player = BulletManager::GetInstance().GetPlayer();
	if (!player) { burstRemaining = 0; return; }

	// 현재 위치들 갱신
	const FVector2 bodyPos = arm ? arm->GetRelativePosition() : FVector2{ 0,0 };
	FVector2 targetPos = player->transform() ? player->transform()->GetPosition() : FVector2{ 0,0 };

	// 랜덤 목표점(플레이어 주변 원)
	const auto rnd = FRandom::GetRandomPointInCircle2D(targetPos.x, targetPos.y, spreadRadius);
	const FVector2 aimedTarget{ rnd.x, rnd.y };

	float currentSpeed = 1.0f;
	if (auto bike = player->GetComponent<BikeMovementScript>())
		currentSpeed = bike->GetCurrSpeed();
	
	switch (droneType)
	{
	case EDroneType::Enemy:
	case EDroneType::BossSpawn:
		// 속도 반영(플레이어 바이크 이동속도)
		currentSpeed *= 0.4f;
		break;
	case EDroneType::Boss:
		currentSpeed *= 0.5f;
		break;
	}
	currentSpeed += m_bulletSpeed;
	const FVector2 speed{ currentSpeed, 0.0f };

    // 발사
	BulletManager::GetInstance().FireBullet(bodyPos, aimedTarget, speed, droneType, damage);

    // 반동 업데이트: 발사 방향의 반대편으로 recoilImpulse만큼 이동 누적 (최대 반경 제한)
    FVector2 fireDir = (aimedTarget - bodyPos).Normalize();
    if (fireDir.Length() > 0.0f)
    {
        recoilDir = fireDir * -1.0f;
        FVector2 candidate = recoilOffset + recoilDir * recoilImpulse;
        if (candidate.Length() > recoilMaxDistance)
            candidate = candidate.Normalize() * recoilMaxDistance;
        recoilOffset = candidate;
    }

	// 남은 발 관리 및 다음 타이머
    if (--burstRemaining <= 0)
    {
        // 더 이상 남은 발이 없으면 반복 타이머 중지
        TimerManager::GetInstance().ClearTimer(burstTimer);
		bBurstRunning = false;
		bPatternRunning = false;
		currentPattern = EBossPattern::None;
    }
}

void Drone::DelayDestroy()
{
    if (bDelayDestroying) return;
    bDelayDestroying = true;
  
    bCanFire = false; // 더 이상 공격 불가
	TimerManager::GetInstance().ClearTimer(timer);
    TimerManager::GetInstance().ClearTimer(burstTimer); // 즉시 잔여 타이머 정리
	TimerManager::GetInstance().ClearTimer(sweepTimer);
	bBurstRunning = false;
	bSweepRunning = false;
	bPatternRunning = false;
	currentPattern = EBossPattern::None;
    fadeElapsed = 0.0f;  // 페이드 파라미터 초기화
}

void Drone::UpdateBossDroneMovement(const float& deltaSeconds)
{
	// 보스 드론의 독립적인 이동 로직
	if (!owner) return;
	
	// 이동 패턴 변경 타이머 업데이트
	m_movementTimer += deltaSeconds;
	if (m_movementTimer >= m_patternChangeInterval)
	{
		m_movementTimer = 0.0f;
		// 새로운 이동 패턴 시작 (각도와 반지름 랜덤화)
		m_movementAngle = FRandom::GetRandomInRange(0.0f, 360.0f);
		m_movementRadius = FRandom::GetRandomInRange(80.0f, 150.0f);
	}
	
	// 각도 업데이트 (시간에 따라 부드럽게 변화)
	m_movementAngle += m_movementAngleSpeed * deltaSeconds;
	if (m_movementAngle > 360.0f) m_movementAngle -= 360.0f;
	
	// 부드러운 원형 이동 + 약간의 사인파 추가
	float baseRadius = m_movementRadius;
	float sinWave = std::sin(m_movementAngle * 2.0f * Define::PI / 180.0f) * 20.0f;
	float currentRadius = baseRadius + sinWave;
	
	// 현재 각도에서의 위치 계산 (부드러운 원형)
	float rad = m_movementAngle * Define::PI / 180.0f;
	m_movementOffset.x = std::cos(rad) * currentRadius;
	m_movementOffset.y = std::sin(rad) * currentRadius;
	
	// 플레이어 위치에 기본 오프셋과 이동 오프셋을 적용하여 최종 위치 계산
	if (auto player = GamePlayManager::GetInstance().GetPlayer())
	{
		FVector2 playerPos = player->GetPosition();
		FVector2 targetPos = playerPos + m_baseOffset + m_movementOffset;
		
		// 카메라 화면 경계 체크 (화면 밖으로 나가지 않도록)
		FVector2 screenSize = FVector2(Define::SCREEN_WIDTH, Define::SCREEN_HEIGHT);
		FVector2 screenCenter = playerPos; // 카메라가 플레이어를 따라가므로 플레이어 위치가 화면 중심
		
		// 화면 경계 내로 제한
		float margin = 100.0f; // 화면 가장자리에서 여유 공간
		targetPos.x = Math::Clamp(targetPos.x, screenCenter.x - screenSize.x * 0.5f + margin, 
								 screenCenter.x + screenSize.x * 0.5f - margin);
		targetPos.y = Math::Clamp(targetPos.y, screenCenter.y - screenSize.y * 0.5f + margin, 
								 screenCenter.y + screenSize.y * 0.5f - margin);
		
		// 현재 위치에서 목표 위치로 부드럽게 이동
		FVector2 currentPos = owner->GetPosition();
		FVector2 direction = (targetPos - currentPos).Normalize();
		FVector2 movement = direction * m_movementSpeed * deltaSeconds;
		
		// 목표 위치에 너무 가까우면 이동 중단 (떨림 방지)
		float distance = (targetPos - currentPos).Length();
		if (distance > 5.0f)
		{
			owner->SetPosition(currentPos + movement);
		}
		else
		{
			// 목표 위치에 도달했으면 정확한 위치로 설정
			owner->SetPosition(targetPos);
		}
	}
}

void Drone::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void Drone::Awake()
{
}

void Drone::OnStart()
{
	// 여기에 OnStart에 대한 로직 작성
	m_owner = GetOwner();

	TimerManager::GetInstance().SetTimer(
		timer,
		[this]()
		{
			bCanFire = true;
		},
		attackDelay,
		true,
		0.0f
	);

	body = owner->AddComponent<SpriteRenderer>();
	body->SetLayer(20000);
	body->LoadData(spritePath.body);
	body->SetRelativeScale(initBodySize);
   	body->SetRelativePosition(initBodyPos);

	arm = owner->AddComponent<SpriteRenderer>();
	arm->SetLayer(20001);
	arm->LoadData(spritePath.arm);
	arm->SetRelativeScale(FVector2(1.0f, 1.0f));
	arm->RemoveFromParent();
	body->AddChildComponent(arm);
	
	// 보스가 소환한 드론인 경우 초기 위치 설정
	if (droneType == EDroneType::BossSpawn)
	{
		if (auto player = GamePlayManager::GetInstance().GetPlayer())
		{
			FVector2 playerPos = player->GetPosition();
			FVector2 initialPos = playerPos + m_baseOffset;
			owner->SetPosition(initialPos);
		}
	}
}

void Drone::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
	TimerManager::GetInstance().ClearTimer(timer);
	TimerManager::GetInstance().ClearTimer(burstTimer);
	TimerManager::GetInstance().ClearTimer(sweepTimer);
	bBurstRunning = false;
	bSweepRunning = false;
	bPatternRunning = false;
	currentPattern = EBossPattern::None;
}

void Drone::OnDestroy()
{
	TimerManager::GetInstance().ClearTimer(timer);
	TimerManager::GetInstance().ClearTimer(burstTimer);
	TimerManager::GetInstance().ClearTimer(sweepTimer);
	bBurstRunning = false;
	bSweepRunning = false;
	bPatternRunning = false;
	currentPattern = EBossPattern::None;
}

void Drone::OnTriggerEnter2D(Collider* collider)
{
	//std::cout << "OnTriggerEnter2D 호출됨" << std::endl;
	//OutputDebugStringW(L"OnTriggerEnter2D 호출됨\n");
}

void Drone::OnTriggerStay2D(Collider* collider)
{
	//std::cout << "OnTriggerStay2D 호출됨" << std::endl;
	//OutputDebugStringW(L"OnTriggerStay2D 호출됨\n");
}

void Drone::OnTriggerExit2D(Collider* collider)
{
	//std::cout << "OnTriggerExit2D 호출됨" << std::endl;
	//OutputDebugStringW(L"OnTriggerExit2D 호출됨\n");
}

void Drone::Input()
{
	// 여기에 Input에 대한 로직 작성
}
