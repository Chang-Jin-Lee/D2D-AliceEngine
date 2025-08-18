#pragma once
#include <Component/ScriptComponent.h>
#include <Core/StatTraits.h>
#include <Component/StatComponent.h>

struct AruStat2 {
	float HP = 100.f;
	float MAXHP = 100.0f;
	float MP = 50.f;
	float STR = 10.f;
	float DEX = 20.f;
	float INT = 30.f;
};

DEFINE_STAT_TRAITS_6(AruStat2, HP, MAXHP, MP, STR, DEX, INT)

class gameObject;
class TextRenderComponent;
class Aru2 : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);

	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	void Input();

	gameObject* m_aru2;
	std::vector<TextRenderComponent*> m_aru2NameTexts;
	StatComponent<AruStat2>* m_aru2Stat;
};

