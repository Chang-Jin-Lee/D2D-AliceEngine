#pragma once
#include <Scene/Scene.h>

class ParticleComponent;
class EffectDemoScene : public Scene
{
public:
	EffectDemoScene() {}
	~EffectDemoScene() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void HandleInput();
	void SpawnVignette(float durationSec, float maxAlpha);
	void SpawnBlackOut(int modeIndex, bool useCrossFade, float durationSec, float maxAlpha);
	void SpawnParticleExplosion();
	void SpawnParticleImpact();
	void SpawnParticleClickL();
	void SpawnParticleClickR();
	void ToggleMouseTrail();
	void SpawnParticleAura();
	void SpawnParticleElectric();
	void SpawnParticlePortal();

	class gameObject* gotest;
	ParticleComponent* colorTransformEffect;
	ParticleComponent* distortionEffect;
	ParticleComponent* blendingEffect;
	ParticleComponent* physicsEffect;
	ParticleComponent* environmentEffect;
	ParticleComponent* specialEffect;
};

