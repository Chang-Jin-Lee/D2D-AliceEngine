#pragma once
#include <Component/ScriptComponent.h>
#include <Core/StatTraits.h>
#include <Component/StatComponent.h>
#include <string>
#include <Math/TMath.h>
#include <Math/TColor.h>

class gameObject;
class TextRenderComponent;
class ProgressBarComponent;

struct DogStat {
	float MAXHP = 100.f;
	float HP = 100.f;
	float STAMINA_MAX = 100.f;
	float STAMINA = 60.f;
	float SPEED = 50.f;
	float POWER = 40.f;
	float GUTS = 45.f;
	float WISDOM = 35.f;
	float CHARM = 55.f;
	float FRIENDLINESS = 70.f;
};

DEFINE_STAT_TRAITS_10(DogStat, MAXHP, HP, STAMINA_MAX, STAMINA, SPEED, POWER, GUTS, WISDOM, CHARM, FRIENDLINESS)

class DogStatusWidgetScript : public ScriptComponent
{
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	void SetDogStat(StatComponent<DogStat>* stat) { m_dogStat = stat; }

	// Grade UI layout (relative to root, ScreenSpace)
	float gradeX = 540.f;
	float gradeY = 90.f;
	float gradeStep = 30.f;

	// Outline controls
	float outlineOffset = 1.0f;   // 픽셀 오프셋
	unsigned char outlineAlpha = 140; // 아웃라인 투명도

private:
	void BuildUI();
	void UpdateUI();
	void BuildGrades();
	void UpdateGrades();

	void CreateOrUpdateOutlinedText(TextRenderComponent*& main,
		TextRenderComponent* (&outlineArr)[8],
		const std::wstring& text,
		const FVector2& pos,
		const FColor& color,
		float fontSize);

private:
	StatComponent<DogStat>* m_dogStat{ nullptr };
	gameObject* m_root{ nullptr };
	TextRenderComponent* m_title{ nullptr };
	TextRenderComponent* m_summary{ nullptr };
	ProgressBarComponent* m_hpBack{ nullptr };
	ProgressBarComponent* m_hpFill{ nullptr };
	ProgressBarComponent* m_stBack{ nullptr };
	ProgressBarComponent* m_stFill{ nullptr };
	TextRenderComponent* m_speedText{ nullptr };
	TextRenderComponent* m_powerText{ nullptr };
	TextRenderComponent* m_gutsText{ nullptr };
	TextRenderComponent* m_wisdomText{ nullptr };
	TextRenderComponent* m_charmText{ nullptr };
	TextRenderComponent* m_friendlinessText{ nullptr };

	// Grade components (root's child components, not separate objects)
	TextRenderComponent* m_gradeSpeed{ nullptr };
	TextRenderComponent* m_gradePower{ nullptr };
	TextRenderComponent* m_gradeGuts{ nullptr };
	TextRenderComponent* m_gradeWisdom{ nullptr };
	TextRenderComponent* m_gradeCharm{ nullptr };
	TextRenderComponent* m_gradeFriend{ nullptr };

	// 8방향 아웃라인 그림자 컴포넌트 배열
	TextRenderComponent* m_outlineSpeed[8] { nullptr };
	TextRenderComponent* m_outlinePower[8] { nullptr };
	TextRenderComponent* m_outlineGuts[8] { nullptr };
	TextRenderComponent* m_outlineWisdom[8] { nullptr };
	TextRenderComponent* m_outlineCharm[8] { nullptr };
	TextRenderComponent* m_outlineFriend[8] { nullptr };
}; 
