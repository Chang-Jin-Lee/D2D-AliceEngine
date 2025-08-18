#include "BikeStatScript.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/ProgressBarComponent.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Component/StatComponent.h>
#include <System/ScriptSystem.h>
#include <Manager/SceneManager.h>
#include <Helpers/CoordHelper.h>
#include <GameManager/BulletManager.h>
#include <Manager/TimerManager.h>
#include <Component/Collider.h>
#include <Manager/D2DRenderManager.h>
#include <Prefab/Enemy/Core/Car.h>
#include <Scripts/Weapon/Drone.h>
#include <GameManager/PlayerDataManager.h>
#include <Scripts/Player/PlayerManager.h>
#include <GameManager/GamePlayManager.h>

void BikeStatScript::Initialize()
{
	__super::Initialize();

	(void)StatTraits<BikeStatScript>::GetOffsetMap();

	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}


void BikeStatScript::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void BikeStatScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성
	// 
	// 마우스 클릭 감지

	//if (Input::IsMouseLeftDown() && bCanFire)
	//{
	//	FVector2 ownerPos = owner->GetPosition();
	//	FVector2 worldMousePos = Input::GetMouseWorldPosition(); // 마우스의 실제 월드 좌표
	//	BulletManager::GetInstance().FireBullet(ownerPos, worldMousePos, 300);
	//	bCanFire = false;
	//}
}

void BikeStatScript::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void BikeStatScript::Invoke()
{

}

void BikeStatScript::OnStart()
{
	//TimerManager::GetInstance().SetTimer(
	//	timer,
	//	[this]()
	//	{
	//		bCanFire = true;
	//		OutputDebugStringW(L"람다 타이머 호출됨!\n");
	//	},
	//	0.1f,
	//	true,
	//	0.0f
	//);

    // 여기에 OnStart에 대한 로직 작성
	owner->AddComponent<SpriteRenderer>()->LoadData(L"BikeStatScript.png");
	BoxComponent* box = owner->AddComponent<BoxComponent>(owner->GetComponent<SpriteRenderer>()->GetRelativeSize(), FColor::Blue);
	box->SetSize(owner->GetComponent<SpriteRenderer>()->GetRelativeSize());
	box->SetIgnoreOwnerScale(false);

    // 스탯 컴포넌트 생성
    FPlayerStats stat = PlayerDataManager::GetInstance().GetStats();
    m_bikeStat = owner->AddComponent<StatComponent<BikeStat>>();
    m_bikeStat->SetStat("HP", stat.hp);
    m_bikeStat->SetStat("MAXHP", stat.maxHP);
    m_bikeStat->SetStat("BATTERY", stat.battery);

    // 체력바 배경
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

    // 체력바 전경(채워지는 부분)
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

    // 델리게이트: HP, MAXHP 변경 시 진행도 갱신
    m_bikeStat->OnChangeStatMap["HP"].Add(m_bikeStat->GetHandle(), [this](float /*oldVal*/, float newVal)
    {
        if (m_hpBarFill && m_bikeStat)
        {
            float maxv = m_bikeStat->GetStat("MAXHP");
            if (maxv <= 0.0f) maxv = 0.0001f;
            m_hpBarFill->SetProgress(newVal / maxv);
        }
        if (newVal <= 0)
        {
            owner->RemoveComponent<BoxComponent>(owner->GetComponent<BoxComponent>());
            owner->RemoveComponent<Collider>(owner->GetComponent<Collider>());
            GamePlayManager::GetInstance().GameOver();
            //if (auto player = owner->GetComponent<PlayerManager>()) player->DelayDestroy();
            //if (auto drone = owner->GetComponent<Drone>()) drone->DelayDestroy();
            return;
        }
    });

    m_bikeStat->OnChangeStatMap["MAXHP"].Add(owner->GetHandle(), [this](float /*oldVal*/, float /*newVal*/)
    {
        if (m_hpBarFill && m_bikeStat)
        {
            float cur = m_bikeStat->GetStat("HP");
            float maxv = m_bikeStat->GetStat("MAXHP");
            if (maxv <= 0.0f) maxv = 0.0001f;
            m_hpBarFill->SetProgress(cur / maxv);
        }
    });

    // 초기 진행도 반영
    if (m_hpBarFill && m_bikeStat)
    {
        float cur = m_bikeStat->GetStat("HP");
        float maxv = m_bikeStat->GetStat("MAXHP");
        if (maxv <= 0.0f) maxv = 0.0001f;
        m_hpBarFill->SetProgress(cur / maxv);
    }

	owner->AddComponent<InputComponent>()->SetAction(owner->GetHandle(), [this]() { Input(); });
}

void BikeStatScript::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
	TimerManager::GetInstance().ClearTimer(timer);
}

void BikeStatScript::OnDestroy()
{
    // 리소스 해제는 엔진 컴포넌트 파이프라인에 위임
}

void BikeStatScript::Input()
{
	// 여기에 Input에 대한 로직 작성
}
