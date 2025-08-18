#pragma once
#include <Math/TMath.h>
#include <Math/TColor.h>

class ParticleHelper
{
public:
	static void SpawnParticleExplosion(const FVector2& pos, const std::wstring& texturePath = L"");
	static void SpawnParticleImpact(const FVector2& pos, const std::wstring& texturePath = L"");
	static void SpawnParticleExplosionByColor(const FVector2& pos, const std::wstring& texturePath, FColor colorA, FColor colorB);
	static void SpawnParticleImpactByColor(const FVector2& pos, const std::wstring& texturePath, FColor colorA, FColor colorB);
	static void SpawnParticleClickL(const FVector2& pos, const std::wstring& texturePath = L"");
	static void SpawnParticleClickR(const FVector2& pos, const std::wstring& texturePath = L"");
	static void ToggleMouseTrail();
	static void SpawnParticleAura(const FVector2& pos, float radius, int count, const std::wstring& texturePath = L"");
	static void SpawnParticleElectric(const FVector2& pos, const std::wstring& texturePath = L"");
	static void SpawnParticlePortal(const FVector2& pos, const std::wstring& texturePath = L"");

	// 값으로 제어 가능한 ByValue 헬퍼들 ---------------------------------
	static void SpawnParticleExplosionByValue(
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
		bool additive = true,
		const std::wstring& texturePath = L"");

	static void SpawnParticleImpactByValue(
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
		bool additive = true,
		const std::wstring& texturePath = L"");

	static void SpawnParticleClickLByValue(
		const FVector2& pos,
		int count,
		float speedMin, float speedMax,
		float sizeMin, float sizeMax,
		float lifeMin, float lifeMax,
		const FColor& colorA, const FColor& colorB,
		float drag, float gravity,
		bool enableSelfDestruct, float selfDestructAfterSeconds,
		bool additive = true,
		const std::wstring& texturePath = L"");

	static void SpawnParticleClickRByValue(
		const FVector2& pos,
		int count,
		float speedMin, float speedMax,
		float sizeMin, float sizeMax,
		float lifeMin, float lifeMax,
		const FColor& colorA, const FColor& colorB,
		float drag, float gravity,
		bool enableSelfDestruct, float selfDestructAfterSeconds,
		bool additive = true,
		const std::wstring& texturePath = L"");

	static void ToggleMouseTrailByValue(bool enable);

	static void SpawnParticleAuraByValue(
		const FVector2& pos,
		float radius, int count,
		float radiusJitter,
		float angularSpeedMin, float angularSpeedMax,
		float sizeMin, float sizeMax,
		float lifeMin, float lifeMax,
		float drag, float gravity,
		const FColor& colorA, const FColor& colorB,
		bool additive = true,
		const std::wstring& texturePath = L"");

	static void SpawnParticleElectricByValue(
		const FVector2& pos,
		int count,
		float speedMin, float speedMax,
		float sizeMin, float sizeMax,
		float lifeMin, float lifeMax,
		const FColor& colorA, const FColor& colorB,
		float drag, float gravity,
		float spreadRadians,
		bool additive = true,
		const std::wstring& texturePath = L"");

	static void SpawnParticlePortalByValue(
		const FVector2& pos,
		int count,
		float speedMin, float speedMax,
		float sizeMin, float sizeMax,
		float lifeMin, float lifeMax,
		const FColor& colorA, const FColor& colorB,
		float drag, float gravity,
		float spreadRadians,
		bool additive = true,
		const std::wstring& texturePath = L"");
};

