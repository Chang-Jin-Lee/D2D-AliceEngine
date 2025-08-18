#include "PlayerManager.h"
#include "System/ScriptSystem.h"
#include "Component/SkewTransform.h"
#include "Manager/SceneManager.h"
#include "Animation/AnimationController.h"
#include "Component/Collider.h"
#include "Component/BoxComponent.h"
#include <Core/Input.h>
#include "Effect/Prism.h"
#include "Manager/TimerManager.h"
#include <Component/InputComponent.h>
#include "Manager/UpdateTaskManager.h"
#include "../Bike/BikeMovementScript.h"
#include <Helpers/StringHelper.h>
#include <Component/SpriteRenderer.h>
#include <GameManager/GamePlayManager.h>
#include <Object/gameObject.h>
#include <Prefab/Enemy/Core/Car.h>
#include "Scripts/Player/PlayerMovement.h"
#include "../Weapon/BulletColl.h"
#include "../Audio/StageAudioScript.h"
#include "Component/AudioComponent.h"

PlayerManager* PlayerManager::instance = nullptr;

void PlayerManager::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);

	//REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PrePhysics);
	//REGISTER_SCRIPT_TICK(Update);
}

void PlayerManager::OnStart()
{
	__super::OnStart();

	// instance 설정
   	if (instance == nullptr)
		instance = this;
	else
		SceneManager::GetInstance().GetWorld()->RemoveObject(this->owner.lock());

	// SkewTransform 설정
	if (auto st = owner->GetComponent<SkewTransform>())
		st->groundTile = SceneManager::GetInstance().GetWorld()->FindObjectByName<gameObject>(L"TileMap");
	
	// SRT 설정
	owner->transform()->SetPosition(0, 0);
	owner->transform()->SetRotation(0);
	//owner->transform()->SetScale(0.7f, 0.7f);
	//owner->transform()->SetPivot(0.5f);

	// 애니메이터 설정
	AnimatorController::LoadAnimatorController(L"Player/KillDong/killdong_idle_AnimController.json", animController);
	animInstance = owner->GetComponent<AnimatorInstance>();
	animInstance->SetAnimatorController(&animController);
	animInstance->LoadSpriteSheet(L"Player/KillDong/killdong_idle_sprites.json");
	animInstance->LoadAnimationClip(L"Player/KillDong/killdong_idle_anim.json");
	animInstance->ApplyClipDurationsToStates();
	animInstance->Play();
	animInstance->OnStart();

	animInstance->SetRelativeScale(FVector2(0.7f, 0.7f));

	// 충돌 콜라이더 설정
	/*owner->GetComponent<Collider>()->SetBoxSize(FVector2(110, 50));
	if (auto collider = owner->GetComponent<Collider>())
	{
		collider->SetLayer(0);
		collider->boxComponent->SetRelativePosition(FVector2(0, -20));
	}*/

	owner->GetComponent<Collider>()->SetBoxSize(FVector2(150, 10));
	if (auto collider = owner->GetComponent<Collider>())
	{
		collider->SetLayer(5);
		collider->boxComponent->SetRelativePosition(FVector2(0, -50));
	}

	// 인풋 컴포넌트
	owner->AddComponent<InputComponent>()->SetAction(owner->GetHandle(), [this]() { Input(); });

	// 총알받이 설정
	gameObject* bColObj = GetWorld()->NewObject<gameObject>(L"EnemyBikeColl");
	bColObj->SetTag(L"Player");
	BulletColl* bc = bColObj->AddComponent<BulletColl>();
	bc->SetTarget(owner);
	bc->SetCollSize(FVector2(70.0f, 30.0f));

	// 사운드 재생
	StageAudioScript::instance->m_MotorSound->PlayByName(L"MotorSound", 0.0f, 0.1f);
}

void PlayerManager::OnEnd()
{
	instance = nullptr;
}

void PlayerManager::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성

	float playerDeltaSeconds = deltaSeconds * playerTimeScale;

	if (Input::IsKeyDown(VK_OEM_4))
	{
		//float fov = SceneManager::GetCamera()->fieldOfView;
		//fov -= 5.0f * deltaSeconds;
		//SceneManager::GetCamera()->SetFieldOfView(fov);

		FVector2 pos = SceneManager::GetCamera()->GetScale() + 1.1f * playerDeltaSeconds;
		SceneManager::GetCamera()->SetScale(pos);
	}
	if (Input::IsKeyDown(VK_OEM_6))
	{
		//float fov = SceneManager::GetCamera()->fieldOfView;
		//fov += 5.0f * deltaSeconds;
		//SceneManager::GetCamera()->SetFieldOfView(fov);

		FVector2 pos = SceneManager::GetCamera()->GetScale() - 1.1f * playerDeltaSeconds;
		SceneManager::GetCamera()->SetScale(pos);
	}

	// 페이드 아웃 진행 중이면 투명도 감소
	if (m_isFading && m_fadeTargetSR)
	{
		m_fadeElapsed += deltaSeconds;
		float t = m_fadeElapsed / m_fadeDuration;
		if (t > 1.0f) t = 1.0f;
		m_fadeTargetSR->SetOpacity(1.0f - t);
	}

	// 산데비스탄 종료타이머
	if (bSande) {
		if (sandeElipsed > sandeTimer)
			Sande();

		sandeElipsed += deltaSeconds * TimerManager::GetInstance().GetGlobalTimeScale();
	}

	// 부스터(가속) 종료타이머
	if (bBoost) {
		if (boostElipsed > boostTimer)
			Boost();

		boostElipsed += (deltaSeconds * 0.5f) * TimerManager::GetInstance().GetGlobalTimeScale();
		
		/*std::wstring message = L"BoostTime : " + std::to_wstring(boostElipsed) + L"\n";
		OutputDebugStringW(message.c_str());*/
	}

}
 
void PlayerManager::Input()
{
	// 여기에 Input에 대한 로직 작성

	// 산데비스탄 실행키
	if (Input::IsKeyPressed(VK_Q)) {
		Sande(5.0f);
	}

	// 부스터 실행키
	if (Input::IsKeyPressed(VK_LSHIFT)) {
		if(!bBoost)
			Boost(5.0f);
	}
}

// Car::DelayDestroy 를 참고한 플레이어 지연 파괴
void PlayerManager::DelayDestroy()
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
        auto* clip = animator->animationClips[animator->curAnimationClip].get();
        if (clip && animator->m_curFrame < clip->frames.size())
        {
            FrameInfo* frameInfo = &(clip->frames[animator->m_curFrame]);
            int spriteIndex = animator->sheet->spriteIndexMap[frameInfo->sprite];
            SpriteInfo& sp = animator->sheet->sprites[spriteIndex];

            if (WeakObjectPtr<gameObject> temp = SceneManager::GetInstance().m_currentScene->NewObject<gameObject>(L"Player_Ghost"))
            {
                ghostObj = temp;
                ghostSR = temp->AddComponent<SpriteRenderer>();
                std::wstring textureRelPath = StringHelper::string_to_wstring(animator->sheet->texture);
                ghostSR->LoadData(textureRelPath);
                ghostSR->SetLayer(srSelf ? srSelf->GetLayer() : animator->GetLayer());

                temp->transform()->SetPosition(owner->transform()->GetPosition());
                temp->transform()->SetRotation(owner->transform()->GetRotation());
                temp->transform()->SetScale(animator->GetRelativeScale());
                ghostSR->SetSlice(sp.x, sp.y - sp.height, sp.width, sp.height);
                ghostSR->SetOpacity(1.0f);
                m_ghostGO = temp.Get();
            }
        }
    }

    // 더 이상 원본 애니메이션이 렌더되지 않도록 정지
    if (animator)
    {
        animator->Stop();
        animator->SetVisible(false);
    }
    if (srSelf)
    {
        srSelf->SetOpacity(0.0f);
    }

    // 1초 페이드아웃 설정
    m_skipGhostOnDestroy = true;
    m_fadeElapsed = 0.0f;
    m_fadeDuration = 1.0f;
    m_isFading = true;
    m_fadeTargetSR = ghostSR ? ghostSR : srSelf;

    TimerManager::GetInstance().ClearTimer(m_fadeHandle);
    TimerManager::GetInstance().SetTimer(
        m_fadeHandle,
        [weak = WeakFromThis<PlayerManager>(), ghostObj]() mutable {
            if (!weak) return;

			GamePlayManager::GetInstance().GameOver();
            //if (ghostObj)
            //{
            //    SceneManager::GetInstance().GetWorld()->RemoveObject(ghostObj.Get());
            //}
            //if (weak)
            //{
            //    weak->GetWorld()->RemoveObject(weak->GetOwner());
            //}
        },
        1.0f,
        false,
        1.0f);
}

void PlayerManager::Jump()
{
	owner->GetComponent<BikeMovementScript>()->Jump();
}

void PlayerManager::Boost(float _time, bool _battDec)
{
	// pause 상태 아닐시 실행
	if (!GamePlayManager::GetInstance().IsPaused()) {
		if (auto pm = owner->GetComponent<PlayerMovement>()) {

			// 배터리 가져오기
			int& batteryCount = GamePlayManager::GetInstance().batteryCount;

			if (!bBoost && batteryCount >= 3 || !_battDec) {
				// 사운드 재생
				std::wstring audName;
				if(_time <= 3.5f)
					audName = L"Player/character_Player_sfx_accelation_short.wav";
				else
					audName = L"Player/character_Player_sfx_accelation.wav";
				 
				StageAudioScript::instance->m_Sfx[audName]->PlayByName(audName, 0.0f, 0.5f);

				if(_battDec)
					batteryCount -= 3;
				bBoost = true;
				isInvincible = true;
				boostTimer = _time;
				boostElipsed = 0.0f;

				pm->SetBoost(true);
			}
			else {
				bBoost = false;
				isInvincible = false;
				boostTimer = 0.0f;
				boostElipsed = 0.0f;

				pm->SetBoost(false);
			}

			// 프리즘 가져오기
			auto prism = owner->GetComponent<Prism>();

			prism->SetActive(bSande || bBoost);
		}
	}
}

void PlayerManager::Sande(float _time)
{
	// pause 상태 아닐시 실행
	if (!GamePlayManager::GetInstance().IsPaused()) {
		if (auto prism = owner->GetComponent<Prism>())
		{
			// 조건 저장
			bool bVal = !bSande;
			
			// 배터리 조건계산
			int& batteryCount = GamePlayManager::GetInstance().batteryCount;

			if (bVal) {
				if (batteryCount >= 3) {
					batteryCount -= 3;
					// 사운드 재생
					std::wstring audName = L"Player/character_Player_sfx_Sandebistan.wav";
					StageAudioScript::instance->m_Sfx[audName]->PlayByName(audName, 0.0f, 1.0f);

					// 조건 설정
					bSande = true;

					// 타이머 설정
					sandeTimer = _time;
					sandeElipsed = 0.0f;

					// 타임스케일 조절
					TimerManager::GetInstance().SetGlobalTimeScale(0.5f);
					playerTimeScale = 2.0f;
				}
				else {
					// 배터리 부족시 실행할 코드들
				}
			}
			else {
				// 조건 설정
				bSande = false;

				// 타이머 설정
				sandeTimer = 0.0f;
				sandeElipsed = 0.0f;

				// 타임스케일 조절
				TimerManager::GetInstance().SetGlobalTimeScale(1.0f);
				playerTimeScale = 1.0f;
			}

			prism->SetActive(bSande || bBoost);
		}
		else {
			OutputDebugStringW(L"플레이어에게 Prism 컴포넌트가 없음!!\n");
		}
	}
}

void PlayerManager::CrashSlow()
{
	owner->GetComponent<PlayerMovement>()->CrashSlow();
}

void PlayerManager::SetMove(bool _val)
{
	if (auto pm = owner->GetComponent<PlayerMovement>()) {
		if (_val)
			pm->Start();
		else
			pm->Stop();
	}
}
