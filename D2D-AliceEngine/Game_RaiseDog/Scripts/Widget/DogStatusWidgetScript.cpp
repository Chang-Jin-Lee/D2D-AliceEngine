#include "DogStatusWidgetScript.h"
#include <Object/gameObject.h>
#include <Scene/Scene.h>
#include <Component/TextRenderComponent.h>
#include <Component/ProgressBarComponent.h>
#include <Component/TransformComponent.h>
#include <Math/TColor.h>
#include <Manager/D2DRenderManager.h>
#include <Helpers/CoordHelper.h>

static const wchar_t* GradeFromValue(float v)
{
	if (v >= 120.f) return L"U";
	if (v >= 110.f) return L"SS";
	if (v >= 100.f) return L"S";
	if (v >= 90.f) return L"A";
	if (v >= 80.f) return L"B";
	if (v >= 70.f) return L"C";
	if (v >= 60.f) return L"D";
	return L"E";
}

static FColor ColorFromGrade(const wchar_t* g)
{
	// 낮은 등급 회색 → 높은 등급 화려한 색 (우마무스메풍 대략)
	if (wcscmp(g, L"U") == 0)  return FColor(255, 80, 80, 255);      // U: 강한 레드
	if (wcscmp(g, L"SS") == 0) return FColor(255, 195, 0, 255);      // SS: 골드
	if (wcscmp(g, L"S") == 0)  return FColor(255, 165, 0, 255);      // S: 오렌지
	if (wcscmp(g, L"A") == 0)  return FColor(120, 200, 255, 255);    // A: 하늘색
	if (wcscmp(g, L"B") == 0)  return FColor(120, 180, 255, 255);    // B: 블루
	if (wcscmp(g, L"C") == 0)  return FColor(140, 200, 140, 255);    // C: 연두
	if (wcscmp(g, L"D") == 0)  return FColor(170, 170, 170, 255);    // D: 회색
	return FColor(150, 150, 150, 255);
}

void DogStatusWidgetScript::Initialize()
{
	__super::Initialize();
}

void DogStatusWidgetScript::OnStart()
{
	BuildUI();
	BuildGrades();
}

void DogStatusWidgetScript::OnEnd()
{
}

void DogStatusWidgetScript::OnDestroy()
{
}

void DogStatusWidgetScript::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	UpdateUI();
	UpdateGrades();
}

void DogStatusWidgetScript::BuildUI()
{
	m_root = GetOwner();
	if (!m_root) return;

	// 화면 오버레이 (반투명 어둡게)
	{
		FVector2 app = D2DRenderManager::GetInstance().GetApplicationSize();
		FVector2 center(app.x * 0.5f, app.y * 0.5f);

		auto* overlay = m_root->AddComponent<ProgressBarComponent>();
		overlay->SetSize(app);
		overlay->SetColor(FColor(0, 0, 0, 150));
		overlay->SetRelativePosition(center);

		// 패널(창)
		auto* panel = m_root->AddComponent<ProgressBarComponent>();
		FVector2 panelSize(760, 360);
		panel->SetSize(panelSize);
		panel->SetColor(FColor(30, 30, 40, 220));
		panel->SetRelativePosition(center);
	}

	// 타이틀/설명 및 스탯 텍스트: 패널 기준 오프셋
	FVector2 app = D2DRenderManager::GetInstance().GetApplicationSize();
	FVector2 center(app.x * 0.5f, app.y * 0.5f);
	FVector2 panelTopLeft = center + FVector2(-760 * 0.5f, -360 * 0.5f);

	m_title = m_root->AddComponent<TextRenderComponent>();
	m_title->SetText(L" 왕쨩 더비 - 강아지 상태 ");
	m_title->SetFontSize(26.0f);
	m_title->SetRelativePosition(panelTopLeft + FVector2(20, 18));

	m_summary = m_root->AddComponent<TextRenderComponent>();
	m_summary->SetText(L" 스탯 상세 ");
	m_summary->SetFontSize(18.0f);
	m_summary->SetRelativePosition(panelTopLeft + FVector2(20, 50));

	// 수치 표 (좌측)
	m_speedText = m_root->AddComponent<TextRenderComponent>();
	m_powerText = m_root->AddComponent<TextRenderComponent>();
	m_gutsText = m_root->AddComponent<TextRenderComponent>();
	m_wisdomText = m_root->AddComponent<TextRenderComponent>();
	m_charmText = m_root->AddComponent<TextRenderComponent>();
	m_friendlinessText = m_root->AddComponent<TextRenderComponent>();

	m_speedText->SetRelativePosition(panelTopLeft + FVector2(40, 90));
	m_powerText->SetRelativePosition(panelTopLeft + FVector2(40, 120));
	m_gutsText->SetRelativePosition(panelTopLeft + FVector2(40, 150));
	m_wisdomText->SetRelativePosition(panelTopLeft + FVector2(40, 180));
	m_charmText->SetRelativePosition(panelTopLeft + FVector2(40, 210));
	m_friendlinessText->SetRelativePosition(panelTopLeft + FVector2(40, 240));

	m_speedText->SetFontSize(20.0f);
	m_powerText->SetFontSize(20.0f);
	m_gutsText->SetFontSize(20.0f);
	m_wisdomText->SetFontSize(20.0f);
	m_charmText->SetFontSize(20.0f);
	m_friendlinessText->SetFontSize(20.0f);

	UpdateUI();
}

void DogStatusWidgetScript::UpdateUI()
{
	if (!m_dogStat) return;

	wchar_t buf[256];
	swprintf(buf, 256, L" 스피드: %.0f", m_dogStat->GetStat("SPEED"));
	m_speedText->SetText(buf);
	swprintf(buf, 256, L" 파워: %.0f", m_dogStat->GetStat("POWER"));
	m_powerText->SetText(buf);
	swprintf(buf, 256, L" 근성: %.0f", m_dogStat->GetStat("GUTS"));
	m_gutsText->SetText(buf);
	swprintf(buf, 256, L" 지능: %.0f", m_dogStat->GetStat("WISDOM"));
	m_wisdomText->SetText(buf);
	swprintf(buf, 256, L" 매력: %.0f", m_dogStat->GetStat("CHARM"));
	m_charmText->SetText(buf);
	swprintf(buf, 256, L" 친밀: %.0f", m_dogStat->GetStat("FRIENDLINESS"));
	m_friendlinessText->SetText(buf);
}

void DogStatusWidgetScript::BuildGrades()
{
	// 루트 자식 컴포넌트로 생성 (우측 컬럼)
	m_gradeSpeed = m_root->AddComponent<TextRenderComponent>();
	m_gradePower = m_root->AddComponent<TextRenderComponent>();
	m_gradeGuts = m_root->AddComponent<TextRenderComponent>();
	m_gradeWisdom = m_root->AddComponent<TextRenderComponent>();
	m_gradeCharm = m_root->AddComponent<TextRenderComponent>();
	m_gradeFriend = m_root->AddComponent<TextRenderComponent>();

	TextRenderComponent* arr[6] = { m_gradeSpeed, m_gradePower, m_gradeGuts, m_gradeWisdom, m_gradeCharm, m_gradeFriend };
	for (int i = 0; i < 6; ++i)
	{
		arr[i]->SetFontSize(20.0f);
	}

	UpdateGrades();
}

void DogStatusWidgetScript::CreateOrUpdateOutlinedText(
	TextRenderComponent*& main,
	TextRenderComponent* (&outlineArr)[8],
	const std::wstring& text,
	const FVector2& pos,
	const FColor& color,
	float fontSize)
{
	// 메인 텍스트
	if (!main)
	{
		main = m_root->AddComponent<TextRenderComponent>();
	}
	main->SetText(text);
	main->SetColor(color);
	main->SetFontSize(fontSize);
	main->SetRelativePosition(pos);

	// 아웃라인용 8방향 오프셋 벡터들
	FVector2 offs[8] = {
		FVector2(-outlineOffset, 0), FVector2(outlineOffset, 0),
		FVector2(0, -outlineOffset), FVector2(0, outlineOffset),
		FVector2(-outlineOffset, -outlineOffset), FVector2(outlineOffset, -outlineOffset),
		FVector2(-outlineOffset, outlineOffset), FVector2(outlineOffset, outlineOffset)
	};

	for (int i = 0; i < 8; ++i)
	{
		if (!outlineArr[i])
		{
			outlineArr[i] = m_root->AddComponent<TextRenderComponent>();
		}
		outlineArr[i]->SetText(text);
		outlineArr[i]->SetFontSize(fontSize);
		// 아웃라인은 검은색 + 사용자 지정 알파
		outlineArr[i]->SetColor(FColor(0, 0, 0, outlineAlpha));
		outlineArr[i]->SetRelativePosition(pos + offs[i]);
	}
}

void DogStatusWidgetScript::UpdateGrades()
{
	if (!m_dogStat) return;

	FVector2 app = D2DRenderManager::GetInstance().GetApplicationSize();
	FVector2 center(app.x * 0.5f, app.y * 0.5f);
	FVector2 base = center + FVector2(760 * -0.5f + 400, 360 * -0.5f + 90); // 패널 우측 컬럼 베이스

	// SPEED
	{
		float v = m_dogStat->GetStat("SPEED");
		const wchar_t* g = GradeFromValue(v);
		FColor c = ColorFromGrade(g);
		std::wstring txt = std::wstring(L" 스피드 등급: ") + g;
		CreateOrUpdateOutlinedText(m_gradeSpeed, m_outlineSpeed, txt, base + FVector2(0, 30.f * 0), c, 20.0f);
	}
	// POWER
	{
		float v = m_dogStat->GetStat("POWER");
		const wchar_t* g = GradeFromValue(v);
		FColor c = ColorFromGrade(g);
		std::wstring txt = std::wstring(L" 파워 등급: ") + g;
		CreateOrUpdateOutlinedText(m_gradePower, m_outlinePower, txt, base + FVector2(0, 30.f * 1), c, 20.0f);
	}
	// GUTS
	{
		float v = m_dogStat->GetStat("GUTS");
		const wchar_t* g = GradeFromValue(v);
		FColor c = ColorFromGrade(g);
		std::wstring txt = std::wstring(L" 근성 등급: ") + g;
		CreateOrUpdateOutlinedText(m_gradeGuts, m_outlineGuts, txt, base + FVector2(0, 30.f * 2), c, 20.0f);
	}
	// WISDOM
	{
		float v = m_dogStat->GetStat("WISDOM");
		const wchar_t* g = GradeFromValue(v);
		FColor c = ColorFromGrade(g);
		std::wstring txt = std::wstring(L" 지능 등급: ") + g;
		CreateOrUpdateOutlinedText(m_gradeWisdom, m_outlineWisdom, txt, base + FVector2(0, 30.f * 3), c, 20.0f);
	}
	// CHARM
	{
		float v = m_dogStat->GetStat("CHARM");
		const wchar_t* g = GradeFromValue(v);
		FColor c = ColorFromGrade(g);
		std::wstring txt = std::wstring(L" 매력 등급: ") + g;
		CreateOrUpdateOutlinedText(m_gradeCharm, m_outlineCharm, txt, base + FVector2(0, 30.f * 4), c, 20.0f);
	}
	// FRIENDLINESS
	{
		float v = m_dogStat->GetStat("FRIENDLINESS");
		const wchar_t* g = GradeFromValue(v);
		FColor c = ColorFromGrade(g);
		std::wstring txt = std::wstring(L" 친밀 등급: ") + g;
		CreateOrUpdateOutlinedText(m_gradeFriend, m_outlineFriend, txt, base + FVector2(0, 30.f * 5), c, 20.0f);
	}
} 
