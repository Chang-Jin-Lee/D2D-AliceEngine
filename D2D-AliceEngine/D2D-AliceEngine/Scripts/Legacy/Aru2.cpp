#include "Aru2.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Component/StatComponent.h>
#include <System/ScriptSystem.h>
#include <Helpers/CoordHelper.h>
#include <Component/Collider.h>

void Aru2::Initialize()
{
	__super::Initialize();
	(void)StatTraits<AruStat2>::GetOffsetMap();
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
}


void Aru2::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void Aru2::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성

}

void Aru2::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void Aru2::OnStart()
{
	// 여기에 OnStart에 대한 로직 작성
	m_aru2 = GetOwner();
	//m_aru2->transform()->SetPosition(300, 0);
	//m_aru2->transform()->SetRotation(0);
	//m_aru2->transform()->SetScale(0.5f, 0.5f);
	//m_aru2->transform()->SetPivot(0.5f);
	m_aru2->AddComponent<SpriteRenderer>()->LoadData(L"aru.png");
	m_aru2->AddComponent<BoxComponent>(m_aru2->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Green);

	//m_aru2->SetTag(L"Enemy");
	//if (auto* t = m_aru2->AddComponent<Collider>())
	//{
	//	t->SetBoxSize(FVector2(50, 50));
	//}

	/*
	* 게임오브젝트에 TextRenderComponent를 붙이는 예시
	*/

	TextRenderComponent* m_Aru2TextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_Aru2TextCmp->SetText(m_aru2->GetName());
	m_Aru2TextCmp->SetDrawType(EDrawType::WorldSpace);
	m_Aru2TextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_Aru2TextCmp->SetRelativeScale(FVector2(3, 3));
	m_Aru2TextCmp->SetFontSize(24);
	FVector2 widgetSize = m_Aru2TextCmp->GetRelativeSize();
	m_Aru2TextCmp->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.25f, 4.5f)));
	m_aru2NameTexts.push_back(m_Aru2TextCmp);

	m_Aru2TextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_Aru2TextCmp->SetText(L"test");
	m_Aru2TextCmp->SetDrawType(EDrawType::WorldSpace);
	m_Aru2TextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_Aru2TextCmp->SetRelativeScale(FVector2(3, 3));
	m_Aru2TextCmp->SetFontSize(24);
	m_aru2NameTexts.push_back(m_Aru2TextCmp);

	m_Aru2TextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_Aru2TextCmp->SetText(L"test");
	m_Aru2TextCmp->SetDrawType(EDrawType::WorldSpace);
	m_Aru2TextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_Aru2TextCmp->SetRelativeScale(FVector2(3, 3));
	m_Aru2TextCmp->SetFontSize(24);
	m_aru2NameTexts.push_back(m_Aru2TextCmp);

	m_Aru2TextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_Aru2TextCmp->SetText(L"test");
	m_Aru2TextCmp->SetDrawType(EDrawType::WorldSpace);
	m_Aru2TextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_Aru2TextCmp->SetRelativeScale(FVector2(3, 3));
	m_Aru2TextCmp->SetFontSize(24);
	m_aru2NameTexts.push_back(m_Aru2TextCmp);

	m_Aru2TextCmp = m_aru2->AddComponent<TextRenderComponent>();
	m_Aru2TextCmp->SetText(L"test");
	m_Aru2TextCmp->SetDrawType(EDrawType::WorldSpace);
	m_Aru2TextCmp->SetTextAlignment(ETextFormat::MiddleCenter);
	m_Aru2TextCmp->SetRelativeScale(FVector2(3, 3));
	m_Aru2TextCmp->SetFontSize(24);
	m_aru2NameTexts.push_back(m_Aru2TextCmp);


	/*
	* 커스텀 구조체로 델리게이트를 바인딩 하는 예제
	*/
	m_aru2Stat = m_aru2->AddComponent<StatComponent<AruStat2>>();
	m_aru2Stat->SetStat("HP", 100);
	m_aru2Stat->SetStat("MAXHP", 100);
	m_aru2Stat->SetStat("MP", 150);
	m_aru2NameTexts[1]->SetTextFormat(L"직전 체력 : ", m_aru2Stat->GetStat("HP"));
	m_aru2NameTexts[2]->SetTextFormat(L"현재 체력 : ", m_aru2Stat->GetStat("HP"));
	m_aru2NameTexts[3]->SetTextFormat(L"최대 체력 : ", m_aru2Stat->GetStat("MAXHP"));
	m_aru2NameTexts[4]->SetTextFormat(L"마나 : ", m_aru2Stat->GetStat("MP"));

	widgetSize = m_aru2NameTexts[1]->GetRelativeSize();
	m_aru2NameTexts[1]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 0.0)));
	widgetSize = m_aru2NameTexts[2]->GetRelativeSize();
	m_aru2NameTexts[2]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 1.0f)));
	widgetSize = m_aru2NameTexts[3]->GetRelativeSize();
	m_aru2NameTexts[3]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, 2.f)));
	widgetSize = m_aru2NameTexts[4]->GetRelativeSize();
	m_aru2NameTexts[4]->SetRelativePosition(CoordHelper::RatioCoordToScreen(widgetSize, FVector2(-0.5f, -1.0f)));

	m_aru2Stat->OnChangeStatMap["HP"].Add(m_aru2->GetHandle(), [this](float oldVal, float newVal)
	{
		if (newVal <= 0)	// 죽는 시점
		{
			m_aru2->RemoveComponent<BoxComponent>(m_aru2->GetComponent<BoxComponent>());
			m_aru2->GetComponent<SpriteRenderer>()->LoadData(L"dead.png");
			m_aru2->AddComponent<BoxComponent>(m_aru2->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Red);
		}
		else if (oldVal <= 0)	// 부활하는 시점
		{
			m_aru2->RemoveComponent<BoxComponent>(m_aru2->GetComponent<BoxComponent>());
			m_aru2->GetComponent<SpriteRenderer>()->LoadData(L"aru.png");
			m_aru2->AddComponent<BoxComponent>(m_aru2->GetComponent<SpriteRenderer>()->GetBitmapSize(), FColor::Blue);
		}
		m_aru2NameTexts[1]->SetTextFormat(L"직전 체력 : ", oldVal);
		m_aru2NameTexts[2]->SetTextFormat(L"현재 체력 : ", newVal);
	});;

	m_aru2Stat->OnChangeStatMap["MAXHP"].Add(m_aru2->GetHandle(), [this](float oldVal, float newVal)
	{
		m_aru2NameTexts[3]->SetTextFormat(L"최대 체력 : ", newVal);
	});;

	m_aru2Stat->OnChangeStatMap["MP"].Add(m_aru2->GetHandle(), [this](float oldVal, float newVal)
	{
		m_aru2NameTexts[4]->SetTextFormat(L"마나 : ", newVal);
	});;
}

void Aru2::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
}

void Aru2::OnDestroy()
{
	m_aru2NameTexts.clear();
}

void Aru2::Input()
{
	// 여기에 Input에 대한 로직 작성
}
