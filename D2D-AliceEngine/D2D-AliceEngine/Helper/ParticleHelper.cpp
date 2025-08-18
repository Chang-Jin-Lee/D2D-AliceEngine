#include "ParticleHelper.h"
#include <Manager/SceneManager.h>
#include <Scene/Scene.h>
#include <Component/Effect/ParticleComponent.h>
#include <Core/Input.h>
#include <Object/Camera.h>

// =============== 파티클 테스트 스폰 구현 ===============
void ParticleHelper::SpawnParticleExplosion(const FVector2& pos, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleWorldExplosion");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleWorldExplosion");
		}
		go->SetPosition(pos);
        if (auto* pc = go->GetComponent<ParticleComponent>())
        {
            pc->EmitExplosion(FVector2(0, 0));
        }
        else
        {
            auto* pc2 = go->AddComponent<ParticleComponent>();
            pc2->SetDrawType(Define::EDrawType::WorldSpace);
            if (!texturePath.empty()) pc2->LoadData(texturePath);
            pc2->SetAdditiveBlend(true);
            pc2->EmitExplosion(FVector2(0, 0));
        }
	}
}

void ParticleHelper::SpawnParticleImpact(const FVector2& pos, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleWorldImpact");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleWorldImpact");
		}
		go->SetPosition(pos);
        if (auto* pc = go->GetComponent<ParticleComponent>())
        {
            pc->EmitImpact(FVector2(0, 0));
        }
        else
        {
            auto* pc2 = go->AddComponent<ParticleComponent>();
            pc2->SetDrawType(Define::EDrawType::WorldSpace);
            if (!texturePath.empty()) pc2->LoadData(texturePath);
            pc2->SetAdditiveBlend(true);
            pc2->EmitImpact(FVector2(0, 0));
        }
	}
}

void ParticleHelper::SpawnParticleExplosionByColor(const FVector2& pos, const std::wstring& texturePath /* = L"" */, FColor colorA, FColor colorB)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (sceneWeak.expired()) return;

	WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleWorldExplosion");
	if (!go)
	{
		go = sceneWeak->NewObject<gameObject>(L"ParticleWorldExplosion");
	}
	go->SetPosition(pos);

    if (auto * pc = go->GetComponent<ParticleComponent>())
	{
		pc->EmitExplosionByColor(
			FVector2(0, 0),
			38, // count 값 필요 시 조정
			D2D1::ColorF(FLOAT(colorA.r), FLOAT(colorA.g), FLOAT(colorA.b), FLOAT(colorA.a)),
			D2D1::ColorF(FLOAT(colorB.r), FLOAT(colorB.g), FLOAT(colorB.b), FLOAT(colorB.a))
		);
	}
	else
	{
		auto* pc2 = go->AddComponent<ParticleComponent>();
		pc2->SetDrawType(Define::EDrawType::WorldSpace);
        if (!texturePath.empty()) pc2->LoadData(texturePath);
		pc2->SetAdditiveBlend(true);
		pc2->EmitExplosionByColor(
			FVector2(0, 0),
			38, // count 값 필요 시 조정
			D2D1::ColorF(FLOAT(colorA.r), FLOAT(colorA.g), FLOAT(colorA.b), FLOAT(colorA.a)),
			D2D1::ColorF(FLOAT(colorB.r), FLOAT(colorB.g), FLOAT(colorB.b), FLOAT(colorB.a))
		);
	}
	
}

void ParticleHelper::SpawnParticleImpactByColor(const FVector2& pos, const std::wstring& texturePath /* = L"" */, FColor colorA, FColor colorB)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleWorldImpact");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleWorldImpact");
		}
		go->SetPosition(pos);
        if (auto* pc = go->GetComponent<ParticleComponent>())
        {
            pc->EmitImpactByColor(
                FVector2(0, 0),
                38,
                D2D1::ColorF(FLOAT(colorA.r), FLOAT(colorA.g), FLOAT(colorA.b), FLOAT(colorA.a)),
                D2D1::ColorF(FLOAT(colorB.r), FLOAT(colorB.g), FLOAT(colorB.b), FLOAT(colorB.a))
            );
        }
        else
        {
            auto* pc2 = go->AddComponent<ParticleComponent>();
            pc2->SetDrawType(Define::EDrawType::WorldSpace);
            if (!texturePath.empty()) pc2->LoadData(texturePath);
            pc2->SetAdditiveBlend(true);
            pc2->EmitImpactByColor(
                FVector2(0, 0),
                38,
                D2D1::ColorF(FLOAT(colorA.r), FLOAT(colorA.g), FLOAT(colorA.b), FLOAT(colorA.a)),
                D2D1::ColorF(FLOAT(colorB.r), FLOAT(colorB.g), FLOAT(colorB.b), FLOAT(colorB.a))
            );
        }
	}
}

void ParticleHelper::SpawnParticleClickL(const FVector2& pos, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleScreenClickL");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleScreenClickL");
		}
		go->SetPosition(pos);
        if (auto* pc = go->GetComponent<ParticleComponent>())
        {
            pc->EmitClickBurst(FVector2(0, 0), false);
        }
        else
        {
            auto* pc2 = go->AddComponent<ParticleComponent>();
            pc2->SetDrawType(Define::EDrawType::ScreenSpace);
            if (!texturePath.empty()) pc2->LoadData(texturePath);
            pc2->SetAdditiveBlend(true);
            pc2->EmitClickBurst(FVector2(0, 0), false);
        }
	}
}

void ParticleHelper::SpawnParticleClickR(const FVector2& pos, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleScreenClickR");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleScreenClickR");
		}
		go->SetPosition(pos);
        if (auto* pc = go->GetComponent<ParticleComponent>())
        {
            pc->EmitClickBurst(FVector2(0, 0), true);
        }
        else
        {
            auto* pc2 = go->AddComponent<ParticleComponent>();
            pc2->SetDrawType(Define::EDrawType::ScreenSpace);
            if (!texturePath.empty()) pc2->LoadData(texturePath);
            pc2->SetAdditiveBlend(true);
            pc2->EmitClickBurst(FVector2(0, 0), true);
        }
	}
}

void ParticleHelper::ToggleMouseTrail()
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleScreenTrail");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleScreenTrail");
		}
        auto* pc = go->GetComponent<ParticleComponent>();
        if (!pc)
        {
            pc = go->AddComponent<ParticleComponent>();
            pc->SetDrawType(Define::EDrawType::ScreenSpace);
        }
		static bool on = false;
		on = !on;
		pc->ToggleMouseTrail(on);
	}
}

void ParticleHelper::SpawnParticleAura(const FVector2& pos, float radius, int count, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleWorldAura");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleWorldAura");
		}
		go->SetPosition(pos);
        if (auto* pc = go->GetComponent<ParticleComponent>())
        {
            pc->EmitAura(pos, radius > 0 ? radius : 100.0f, count);
        }
        else
        {
            auto* pc2 = go->AddComponent<ParticleComponent>();
            pc2->SetDrawType(Define::EDrawType::WorldSpace);
            if (!texturePath.empty()) pc2->LoadData(texturePath);
            pc2->SetAdditiveBlend(true);
            pc2->EmitAura(pos, radius > 0 ? radius : 100.0f, count);
        }
	}
}

void ParticleHelper::SpawnParticleElectric(const FVector2& pos, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleWorldElectric");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleWorldElectric");
		}
		go->SetPosition(pos);
        if (auto* pc = go->GetComponent<ParticleComponent>())
        {
            pc->EmitElectric(FVector2(0, 0));
        }
        else
        {
            auto* pc2 = go->AddComponent<ParticleComponent>();
            pc2->SetDrawType(Define::EDrawType::WorldSpace);
            if (!texturePath.empty()) pc2->LoadData(texturePath);
            pc2->SetAdditiveBlend(true);
            pc2->EmitElectric(FVector2(0, 0));
        }
	}
}

void ParticleHelper::SpawnParticlePortal(const FVector2& pos, const std::wstring& texturePath /* = L"" */)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (!sceneWeak.expired())
	{
		WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(L"ParticleWorldPortal");
		if (!go)
		{
			go = sceneWeak->NewObject<gameObject>(L"ParticleWorldPortal");
		}
		go->SetPosition(pos);
        if (auto* pc = go->GetComponent<ParticleComponent>())
        {
            pc->EmitPortalSwirl(FVector2(0, 0));
        }
        else
        {
            auto* pc2 = go->AddComponent<ParticleComponent>();
            pc2->SetDrawType(Define::EDrawType::WorldSpace);
            if (!texturePath.empty()) pc2->LoadData(texturePath);
            pc2->SetAdditiveBlend(true);
            pc2->EmitPortalSwirl(FVector2(0, 0));
        }
	}
}

// ===================== ByValue =====================
static WeakObjectPtr<gameObject> EnsureOrCreateNamedGO(const WeakObjectPtr<Scene>& sceneWeak, const wchar_t* name)
{
	WeakObjectPtr<gameObject> go = sceneWeak->FindObjectByName<gameObject>(name);
	if (!go)
	{
		go = sceneWeak->NewObject<gameObject>(name);
	}
	return go;
}

#include <d2d1.h>

void ParticleHelper::SpawnParticleExplosionByValue(
	const FVector2& pos,
	int count,
	float speedMin, float speedMax,
	float sizeMin, float sizeMax,
	float lifeMin, float lifeMax,
	const FColor& colorA, const FColor& colorB,
	float drag, float gravity,
	bool shockwaveEnabled,
	float shockwaveRadius, float shockwaveThickness, float shockwaveLife,
	const FColor& shockwaveColor,
	bool additive,
	const std::wstring& texturePath)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (sceneWeak.expired()) return;

	WeakObjectPtr<gameObject> go = EnsureOrCreateNamedGO(sceneWeak, L"ParticleWorldExplosionBV");
	go->SetPosition(pos);
	auto* pc = go->AddComponent<ParticleComponent>();
	pc->SetDrawType(Define::EDrawType::WorldSpace);
	if (!texturePath.empty()) pc->LoadData(texturePath);
	pc->SetAdditiveBlend(additive);

	auto toCF = [](const FColor& c) -> D2D1::ColorF {
		return D2D1::ColorF(
			static_cast<float>(c.r),
			static_cast<float>(c.g),
			static_cast<float>(c.b),
			static_cast<float>(c.a));
		};
	const D2D1::ColorF cA = toCF(colorA);
	const D2D1::ColorF cB = toCF(colorB);
	const D2D1::ColorF cShock = toCF(shockwaveColor);

	pc->EmitExplosion(
		pos, count,
		speedMin, speedMax,
		sizeMin, sizeMax,
		lifeMin, lifeMax,
		cA, cB,
		drag, gravity,
		shockwaveEnabled,
		shockwaveRadius, shockwaveThickness, shockwaveLife,
		cShock);
}

void ParticleHelper::SpawnParticleImpactByValue(
	const FVector2& pos,
	int count,
	float speedMin, float speedMax,
	float sizeMin, float sizeMax,
	float lifeMin, float lifeMax,
	const FColor& colorA, const FColor& colorB,
	float drag, float gravity,
	bool shockwaveEnabled,
	float shockwaveRadius, float shockwaveThickness, float shockwaveLife,
	const FColor& shockwaveColor,
	bool additive,
	const std::wstring& texturePath)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (sceneWeak.expired()) return;

	WeakObjectPtr<gameObject> go = EnsureOrCreateNamedGO(sceneWeak, L"ParticleWorldImpactBV");
	go->SetPosition(pos);
	auto* pc = go->AddComponent<ParticleComponent>();
	pc->SetDrawType(Define::EDrawType::WorldSpace);
	if (!texturePath.empty()) pc->LoadData(texturePath);
	pc->SetAdditiveBlend(additive);

	auto toCF = [](const FColor& c) -> D2D1::ColorF {
		return D2D1::ColorF(
			static_cast<float>(c.r),
			static_cast<float>(c.g),
			static_cast<float>(c.b),
			static_cast<float>(c.a));
		};
	const D2D1::ColorF cA = toCF(colorA);
	const D2D1::ColorF cB = toCF(colorB);
	const D2D1::ColorF cShock = toCF(shockwaveColor);

	pc->EmitImpact(
		pos, count,
		speedMin, speedMax,
		sizeMin, sizeMax,
		lifeMin, lifeMax,
		cA, cB,
		drag, gravity,
		shockwaveEnabled,
		shockwaveRadius, shockwaveThickness, shockwaveLife,
		cShock);
}

void ParticleHelper::SpawnParticleClickLByValue(
	const FVector2& pos,
	int count,
	float speedMin, float speedMax,
	float sizeMin, float sizeMax,
	float lifeMin, float lifeMax,
	const FColor& colorA, const FColor& colorB,
	float drag, float gravity,
	bool enableSelfDestruct, float selfDestructAfterSeconds,
	bool additive,
	const std::wstring& texturePath)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (sceneWeak.expired()) return;

	WeakObjectPtr<gameObject> go = EnsureOrCreateNamedGO(sceneWeak, L"ParticleScreenClickLBV");
	go->SetPosition(pos);
	auto* pc = go->AddComponent<ParticleComponent>();
	pc->SetDrawType(Define::EDrawType::ScreenSpace);
	if (!texturePath.empty()) pc->LoadData(texturePath);
	pc->SetAdditiveBlend(additive);

	auto toCF = [](const FColor& c) -> D2D1::ColorF {
		return D2D1::ColorF(
			static_cast<float>(c.r),
			static_cast<float>(c.g),
			static_cast<float>(c.b),
			static_cast<float>(c.a));
		};
	const D2D1::ColorF cA = toCF(colorA);
	const D2D1::ColorF cB = toCF(colorB);

	pc->EmitClickBurst(
		pos, count,
		speedMin, speedMax,
		sizeMin, sizeMax,
		lifeMin, lifeMax,
		cA, cB,
		drag, gravity,
		enableSelfDestruct, selfDestructAfterSeconds);
}

void ParticleHelper::SpawnParticleClickRByValue(
	const FVector2& pos,
	int count,
	float speedMin, float speedMax,
	float sizeMin, float sizeMax,
	float lifeMin, float lifeMax,
	const FColor& colorA, const FColor& colorB,
	float drag, float gravity,
	bool enableSelfDestruct, float selfDestructAfterSeconds,
	bool additive,
	const std::wstring& texturePath)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (sceneWeak.expired()) return;

	WeakObjectPtr<gameObject> go = EnsureOrCreateNamedGO(sceneWeak, L"ParticleScreenClickRBV");
	go->SetPosition(pos);
	auto* pc = go->AddComponent<ParticleComponent>();
	pc->SetDrawType(Define::EDrawType::ScreenSpace);
	if (!texturePath.empty()) pc->LoadData(texturePath);
	pc->SetAdditiveBlend(additive);

	auto toCF = [](const FColor& c) -> D2D1::ColorF {
		return D2D1::ColorF(
			static_cast<float>(c.r),
			static_cast<float>(c.g),
			static_cast<float>(c.b),
			static_cast<float>(c.a));
		};
	const D2D1::ColorF cA = toCF(colorA);
	const D2D1::ColorF cB = toCF(colorB);

	pc->EmitClickBurst(
		pos, count,
		speedMin, speedMax,
		sizeMin, sizeMax,
		lifeMin, lifeMax,
		cA, cB,
		drag, gravity,
		enableSelfDestruct, selfDestructAfterSeconds);
}

void ParticleHelper::ToggleMouseTrailByValue(bool enable)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (sceneWeak.expired()) return;

	WeakObjectPtr<gameObject> go = EnsureOrCreateNamedGO(sceneWeak, L"ParticleScreenTrailBV");
	auto* pc = go->AddComponent<ParticleComponent>();
	pc->SetDrawType(Define::EDrawType::ScreenSpace);
	pc->ToggleMouseTrail(enable);
}

void ParticleHelper::SpawnParticleAuraByValue(
	const FVector2& pos,
	float radius, int count,
	float radiusJitter,
	float angularSpeedMin, float angularSpeedMax,
	float sizeMin, float sizeMax,
	float lifeMin, float lifeMax,
	float drag, float gravity,
	const FColor& colorA, const FColor& colorB,
	bool additive,
	const std::wstring& texturePath)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (sceneWeak.expired()) return;

	WeakObjectPtr<gameObject> go = EnsureOrCreateNamedGO(sceneWeak, L"ParticleWorldAuraBV");
	go->SetPosition(pos);
	auto* pc = go->AddComponent<ParticleComponent>();
	pc->SetDrawType(Define::EDrawType::WorldSpace);
	if (!texturePath.empty()) pc->LoadData(texturePath);
	pc->SetAdditiveBlend(additive);

	auto toCF = [](const FColor& c) -> D2D1::ColorF {
		return D2D1::ColorF(
			static_cast<float>(c.r),
			static_cast<float>(c.g),
			static_cast<float>(c.b),
			static_cast<float>(c.a));
		};
	const D2D1::ColorF cA = toCF(colorA);
	const D2D1::ColorF cB = toCF(colorB);

	pc->EmitAura(
		pos,
		radius, count,
		radiusJitter,
		angularSpeedMin, angularSpeedMax,
		sizeMin, sizeMax,
		lifeMin, lifeMax,
		drag, gravity,
		cA, cB);
}

void ParticleHelper::SpawnParticleElectricByValue(
	const FVector2& pos,
	int count,
	float speedMin, float speedMax,
	float sizeMin, float sizeMax,
	float lifeMin, float lifeMax,
	const FColor& colorA, const FColor& colorB,
	float drag, float gravity,
	float spreadRadians,
	bool additive,
	const std::wstring& texturePath)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (sceneWeak.expired()) return;

	WeakObjectPtr<gameObject> go = EnsureOrCreateNamedGO(sceneWeak, L"ParticleWorldElectricBV");
	go->SetPosition(pos);
	auto* pc = go->AddComponent<ParticleComponent>();
	pc->SetDrawType(Define::EDrawType::WorldSpace);
	if (!texturePath.empty()) pc->LoadData(texturePath);
	pc->SetAdditiveBlend(additive);

	auto toCF = [](const FColor& c) -> D2D1::ColorF {
		return D2D1::ColorF(
			static_cast<float>(c.r),
			static_cast<float>(c.g),
			static_cast<float>(c.b),
			static_cast<float>(c.a));
		};
	const D2D1::ColorF cA = toCF(colorA);
	const D2D1::ColorF cB = toCF(colorB);

	pc->EmitElectric(
		pos, count,
		speedMin, speedMax,
		sizeMin, sizeMax,
		lifeMin, lifeMax,
		cA, cB,
		drag, gravity,
		spreadRadians);
}

void ParticleHelper::SpawnParticlePortalByValue(
	const FVector2& pos,
	int count,
	float speedMin, float speedMax,
	float sizeMin, float sizeMax,
	float lifeMin, float lifeMax,
	const FColor& colorA, const FColor& colorB,
	float drag, float gravity,
	float spreadRadians,
	bool additive,
	const std::wstring& texturePath)
{
	WeakObjectPtr<Scene> sceneWeak = SceneManager::GetInstance().GetWorldByWeak();
	if (sceneWeak.expired()) return;

	WeakObjectPtr<gameObject> go = EnsureOrCreateNamedGO(sceneWeak, L"ParticleWorldPortalBV");
	go->SetPosition(pos);
	auto* pc = go->AddComponent<ParticleComponent>();
	pc->SetDrawType(Define::EDrawType::WorldSpace);
	if (!texturePath.empty()) pc->LoadData(texturePath);
	pc->SetAdditiveBlend(additive);

	auto toCF = [](const FColor& c) -> D2D1::ColorF {
		return D2D1::ColorF(
			static_cast<float>(c.r),
			static_cast<float>(c.g),
			static_cast<float>(c.b),
			static_cast<float>(c.a));
		};
	const D2D1::ColorF cA = toCF(colorA);
	const D2D1::ColorF cB = toCF(colorB);

	pc->EmitPortalSwirl(
		pos, count,
		speedMin, speedMax,
		sizeMin, sizeMax,
		lifeMin, lifeMax,
		cA, cB,
		drag, gravity,
		spreadRadians);
}
