#pragma once
#include <Scene/Scene.h>

class ParticleComponent;
class EffectTestScene : public Scene
{
public:
    EffectTestScene() {}
    ~EffectTestScene() {}

    void Initialize() override;
    void Release() override;
    void Update() override;

    void OnEnter() override;
    void OnExit() override;


    void HandleInput();
    void SpawnVignette(float durationSec, float maxAlpha);
    void SpawnBlackOut(int modeIndex, bool useCrossFade, float durationSec, float maxAlpha);
    
    // a~h 테스트용 스포너
    void SpawnParticleExplosion();      // a
    void SpawnParticleImpact();         // b
    void SpawnParticleClickL();         // c (좌클릭 버스트)
    void SpawnParticleClickR();         // d (우클릭 버스트)
    void ToggleMouseTrail();            // e (토글)
    void SpawnParticleAura();           // f (플레이어 오라 가정, 화면 중앙 근처)
    void SpawnParticleElectric();       // g
    void SpawnParticlePortal();         // h

    gameObject* gotest;

    // 6개의 파티클 효과 핸들
    ParticleComponent* colorTransformEffect;    // A키 - 색상 변환
    ParticleComponent* distortionEffect;        // S키 - 왜곡 효과
    ParticleComponent* blendingEffect;          // D키 - 블렌딩 효과
    ParticleComponent* physicsEffect;           // F키 - 물리(샘플) 효과
    ParticleComponent* environmentEffect;       // G키 - 환경 효과
    ParticleComponent* specialEffect;           // H키 - 특수 효과
};


