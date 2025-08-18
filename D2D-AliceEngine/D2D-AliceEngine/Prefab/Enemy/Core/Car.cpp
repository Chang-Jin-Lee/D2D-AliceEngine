#include "Car.h"
#include <Component/SkewTransform.h>
#include <Scripts/Bike/LaneController.h>
#include "Component/SpriteRenderer.h"
#include "Scripts/Enemy/EnemyManager.h"
#include "System/ScriptSystem.h"
#include "Manager/SceneManager.h"
#include "Animation/AnimatorInstance.h"
#include <Object/Camera.h>
#include <Manager/TimerManager.h>
#include <memory>
#include <Helpers/StringHelper.h>
#include <Manager/UpdateTaskManager.h>
#include <GameManager/GamePlayManager.h>
#include <Scripts/Enemy/EnemyStatScript.h>
#include <Component/Collider.h>
#include <Scripts/Bike/BikeStatScript.h>
#include "Scripts/Player/PlayerManager.h"
#include "Scripts/Bike/BikeMovementScript.h"
#include <Helper/ParticleHelper.h>
#include "Scripts/Weapon/BulletColl.h"
#include "Scripts/Audio/StageAudioScript.h"
#include "Component/AudioComponent.h"

void Car::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(OnStart);
    // 보스 위치 보정을 위해 매 프레임 갱신 필요
    //REGISTER_UPDATE_TASK_IN_SCRIPT(Update, Define::ETickingGroup::TG_PrePhysics);
	//REGISTER_SCRIPT_TICK(Update);

	//?븷?땲硫붿씠?꽣 ?엳?쓣?떆
	//owner->AddComponent<Animator>();
	// 占쌍니몌옙占쏙옙占쏙옙占쏙옙 占쏙옙占쏙옙占쏙옙占쏙옙
	owner->AddComponent<AnimatorInstance>();
	// 占쏙옙占쏙옙占쏙옙
	
    if (bSkewed)
    {
        owner->AddComponent<SpriteRenderer>();
        owner->AddComponent<SkewTransform>();
        owner->AddComponent<LaneController>();
        // ?뒪?겕由쏀듃
        owner->AddComponent<EnemyManager>();

		// 기본 콜라이더 (총알 콜라이더는 레이어 0, 차량충돌은 5)
		Collider* col = owner->AddComponent<Collider>();
		col->SetBoxSize(FVector2(0.0f, 0.0f));
		col->SetLayer(5);
    }

	TimerManager::GetInstance().ClearTimer(timer);
	// 2珥덉뿉 ?븳踰덉뵫 移대찓?씪 而щ쭅 泥댄겕
	TimerManager::GetInstance().SetTimer(
		timer,
		[weak = WeakFromThis<Car>()]() {
			if (!weak) return;
			FVector2 camPos = weak->GetCamera()->GetPosition(); // Unity 醫뚰몴
			float halfW = Define::SCREEN_WIDTH * 0.5f;
			float halfH = Define::SCREEN_HEIGHT * 0.5f;
			FVector2 bulletPos = weak->GetOwner()->transform()->GetPosition();

			float marginX = 100.0f;
			float marginY = 150.0f;
			bool outLeft      = bulletPos.x < camPos.x - halfW - marginX;
			bool outVertical  = (bulletPos.y < camPos.y - halfH - marginY) || (bulletPos.y > camPos.y + halfH + marginY);

			if (outLeft || outVertical)
			{
				weak->GetWorld()->RemoveObject(weak->GetOwner());
			}
		},
		2.0f,
		true,
		0.5f);
}

void Car::OnStart()
{
	// SkewTransform?쑝濡? 蹂??솚?븯湲?
    if (bSkewed)
    {
        SkewTransform* st = owner->GetComponent<SkewTransform>();
        st->groundTile = GetWorld()->FindObjectByName<gameObject>(L"TileMap");
        st->ToSkewPos();
    }
}

void Car::OnDestroy()
{
    TimerManager::GetInstance().ClearTimer(timer);
    if (m_skipGhostOnDestroy) return; // DelayDestroy로 이미 처리한 경우 스킵
    // 기존: 즉시 잔상 생성 페이드. 현재는 DelayDestroy로 대체되어 일반 OnDestroy에서는 생략
}

void Car::Update(const float& deltaSeconds)
{
    __super::Update(deltaSeconds);
    if (m_isFading && m_fadeTargetSR)
    {
        m_fadeElapsed += deltaSeconds;
        float t = m_fadeElapsed / m_fadeDuration;
        if (t > 1.0f) t = 1.0f;
        m_fadeTargetSR->SetOpacity(1.0f - t);
    }

    if (owner && owner->GetName() == L"Boss")
    {
		FVector2 playerPos = 0;
		if (auto player = GamePlayManager::GetInstance().GetPlayer())
		{
			playerPos += player->GetPosition();
		}
		
		// 보스의 체력에 비례하여 플레이어로부터의 거리 조정
		float baseDistance = 1300.0f; // 기본 거리
		float currentDistance = baseDistance;
		
		// EnemyStatScript에서 현재 체력과 최대 체력 가져오기
		if (EnemyStatScript* enemyStat = owner->GetComponent<EnemyStatScript>())
		{
            if (!enemyStat->m_enemyStat) return;
			float currentHP = enemyStat->m_enemyStat->GetStat("HP");
			float maxHP = enemyStat->m_enemyStat->GetStat("MAXHP");
			
			if (maxHP > 0.0f)
			{
				// 체력이 줄어들수록 플레이어에게 가까워짐 (거리가 줄어듦)
				float healthRatio = max(0.3f, currentHP / maxHP);
				// 체력이 100%일 때는 baseDistance, 0%일 때는 baseDistance * 0.3 (최소 거리)
				currentDistance = baseDistance * (0.3f + 0.7f * healthRatio);
			}
		}
		
        //owner->SetPosition(FVector2(playerPos.x + currentDistance, 0));

		const FVector2 basePos(playerPos.x + currentDistance, 0.0f);

		// 부드러운 흔들림 오프셋 계산
		m_bobTime += deltaSeconds;
		const float twoPI = 2.0f * Define::PI;

		float ox = std::sinf(twoPI * m_bobFreqX * m_bobTime) * m_bobAmpX;
		float oy = std::sinf(twoPI * m_bobFreqY * m_bobTime + m_bobPhase) * m_bobAmpY;

		// 최종 위치 = 기준점 + 오프셋
		owner->SetPosition(basePos + FVector2(ox, oy));
    }
}

void Car::OnTriggerEnter2D(Collider* collider)
{
	if (!collider->GetOwner()) return;
	if (collider->GetOwner()->GetTag() == L"Player")
	{
		// 무적일시 무시
		if (PlayerManager::instance->GetInvincible())
			return;

		if (BikeStatScript* bs = PlayerManager::instance->GetOwner()->GetComponent<BikeStatScript>())
		{
			// 사운드 재생
			std::wstring audName = L"Enemy/character_Enemy_sfx_downed.wav";
			auto& m_Sfx = StageAudioScript::instance->m_Sfx;
			if (m_Sfx.find(audName) != m_Sfx.end())
				m_Sfx[audName]->PlayByName(audName, 0.0f, 1.0f);

			// 크래시 상태
			bs->m_bikeStat->DecreaseAbility("HP", 8);
			PlayerManager::instance->CrashSlow();
		}

		if (gameObject* player = GamePlayManager::GetInstance().GetPlayer())
		{
			FVector2 pos = player->GetPosition() + FVector2(40, 0);
			//FVector2 pos = player->GetPosition();
			//ParticleHelper::SpawnParticleExplosion(pos, Define::Effect_Texture_Collision);
			ParticleHelper::SpawnParticleImpactByColor(
				pos,
				Define::Effect_Texture_Bullet_Bomb,
				FColor(1.0f, 0.95f, 0.85f, 1.0f),
				FColor(1.0f, 0.5f, 0.2f, 0.0f)
			);
			//ParticleHelper::SpawnParticleExplosionByValue(
			//    pos, 70,
			//    200.0f, 520.0f,
			//    16.0f, 38.0f,
			//    0.6f, 0.9f,
			//	  FColor(1.0f, 0.55f, 0.15f, 1.0f), // 주황
			//    FColor(0.9f, 0.2f, 0.0f, 0.0f),   // 붉은 꼬리
			//    4.0f,
			//    200.0f,
			//    true,
			//    20,8.0f,0.5f,
			//    FColor(1.0f, 0.8f, 0.4f, 0.8f),
			//    true,
			//    Define::Effect_Texture_Collision
			//);
		}
		//GetWorld()->RemoveObject(GetOwner());
	}
}

void Car::DelayDestroy()
{
    if (m_isDelaying) return;
    m_isDelaying = true;

    // 애니메이터/스프라이트 렌더러 가져오기
    AnimatorInstance* animator = owner->GetComponent<AnimatorInstance>();
    SpriteRenderer* srSelf = owner->GetComponent<SpriteRenderer>();

    // 고스트를 만들어 페이드 아웃 시킬 준비
    WeakObjectPtr<gameObject> ghostObj;
    SpriteRenderer* ghostSR = nullptr;

    if (animator && animator->sheet)
    {
        // 현재 프레임의 스프라이트 정보 추출 (Prism 구현 참고)
        auto* clip = animator->animationClips[animator->curAnimationClip].get();
        if (clip && animator->m_curFrame < clip->frames.size())
        {
            FrameInfo* frameInfo = &(clip->frames[animator->m_curFrame]);
            int spriteIndex = animator->sheet->spriteIndexMap[frameInfo->sprite];
            SpriteInfo& sp = animator->sheet->sprites[spriteIndex];

            // 동일 월드에 고스트 오브젝트 생성
            if (WeakObjectPtr<gameObject> temp = SceneManager::GetInstance().m_currentScene->NewObject<gameObject>(L"Car_Ghost"))
            {
                ghostObj = temp;
                // SpriteRenderer 세팅
                ghostSR = temp->AddComponent<SpriteRenderer>();
                // 시트의 텍스처 경로를 통해 새 비트맵 로드 (m_bitmap 복사하지 않음)
                if (animator->sheet)
                {
                    std::wstring textureRelPath = StringHelper::string_to_wstring(animator->sheet->texture);
                    ghostSR->LoadData(textureRelPath);
                }
                ghostSR->SetLayer(srSelf ? srSelf->GetLayer() : animator->GetLayer());
                
                // 좌표 변환 동기화
                temp->transform()->SetPosition(owner->transform()->GetPosition());
                temp->transform()->SetRotation(owner->transform()->GetRotation());
                temp->transform()->SetScale(animator->GetRelativeScale());
                // 현재 프레임 영역만 표시
                ghostSR->SetSlice(sp.x, sp.y - sp.height, sp.width, sp.height);
                // 효과 경로(DrawImage)를 사용하도록 초기 불투명도 설정
                ghostSR->SetOpacity(1.0f);
                m_ghostGO = temp.Get();
            }
        }
    }

    // 더 이상 원본 애니메이션이 렌더되지 않도록 정지
    if (animator)
    {
        animator->Stop();               // 프레임 진행 정지
        animator->SetVisible(false);    // 렌더 안되도록 숨김
    }
    if (srSelf)
    {
        // 원본 스프라이트가 있었다면 숨김
        srSelf->SetOpacity(0.0f);
    }

    // 1초 페이드아웃: 타이머는 삭제 타이밍만, 페이드는 Update에서 처리
    m_skipGhostOnDestroy = true; // OnDestroy에서 잔상 생성 스킵
    m_fadeElapsed = 0.0f;
    m_fadeDuration = 1.0f;
    m_isFading = true;
    m_fadeTargetSR = ghostSR ? ghostSR : srSelf;
}

void Car::DestroybColl()
{
	// 사운드 재생
	std::wstring audName = L"Player/character_Player_sfx_itempickup.wav";
	auto& m_Sfx = StageAudioScript::instance->m_Sfx;
	if(m_Sfx.find(audName) != m_Sfx.end())
		m_Sfx[audName]->PlayByName(audName, 0.0f, 0.5f);

	for (auto& obj : colObjs) {
		GetWorld()->RemoveObject(obj);
	}
}
