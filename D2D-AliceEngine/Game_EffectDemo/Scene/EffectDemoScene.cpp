#include "EffectDemoScene.h"
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Object/gameObject.h>
#include <Core/Input.h>
#include <Helpers/CoordHelper.h>
#include <Scripts/Widget/VignetteWidgetScript.h>
#include <Scripts/Widget/BlackOutWidgetScript.h>
#include <Component/Effect/ParticleComponent.h>
#include <Component/Effect/ShaderParticleComponent.h>
#include <Scripts/Video/YuukaVideo.h>
#include <Component/SpriteRenderer.h>
#include <Scripts/Legacy/CameraController.h>
#include <Helper/ParticleHelper.h>

void EffectDemoScene::Initialize()
{
	__super::Initialize();
}

void EffectDemoScene::Release()
{
	__super::Release();
}

void EffectDemoScene::Update()
{
	__super::Update();
	HandleInput();
}

void EffectDemoScene::OnEnter()
{
	__super::OnEnter();

	//gameObject* m_camera = NewObject<gameObject>(L"Camera");
	//m_camera->AddComponent<CameraController>();

	gameObject* m_yuuka = NewObject<gameObject>(L"yuuka");
	m_yuuka->AddComponent<SpriteRenderer>()->LoadData(L"tree.jpg");

	if (auto ui = NewObject<gameObject>(L"EffectUI"))
	{
		auto* txt = ui->AddComponent<TextRenderComponent>();
		txt->SetText(
			L"Effect Test Scene\n"
			L"[1] Vignette (2.0s)\n"
			L"[2] BlackOut - VignetteOnly (2.0s)\n"
			L"[3] BlackOut - PointDiffuse (2.0s)\n"
			L"[4] BlackOut - SpotDiffuse (2.0s)\n"
			L"[5] BlackOut - SpotSpecular (2.0s)\n"
			L"[A] Particle Explosion (월드)\n"
			L"[B] Particle Impact (월드)\n"
			L"[C] ClickBurst L (화면)\n"
			L"[D] ClickBurst R (화면)\n"
			L"[E] Toggle Mouse Trail (화면)\n"
			L"[F] Aura (월드)\n"
			L"[G] Electric (월드)\n"
			L"[H] Portal Swirl (월드)\n"
			L"[R] Shader Blending\n"
			L"[T] Shader ColorChange\n"
			L"[Y] Shader Distortion\n"
			L"[U] Shader Special\n"
			L"[I] Shader Environment\n"
			L"[O] Shader Physics\n"
			L"[SPACE] Clear All\n"
		);
		txt->SetFontSize(18.0f);
		txt->SetRelativePosition(CoordHelper::RatioCoordToScreen(FVector2(0.02f, 0.05f)));
	}
}

void EffectDemoScene::OnExit()
{
	__super::OnExit();
}

void EffectDemoScene::HandleInput()
{
	if (Input::IsKeyPressed('1'))
	{
		SpawnVignette(2.0f, 1.0f);
	}
	if (Input::IsKeyPressed('2'))
	{
		SpawnBlackOut(static_cast<int>(EBlackOutLightingMode::VignetteOnly), true, 2.0f, 1.0f);
	}
	if (Input::IsKeyPressed('3'))
	{
		SpawnBlackOut(static_cast<int>(EBlackOutLightingMode::PointDiffuse), true, 2.0f, 1.0f);
	}
	if (Input::IsKeyPressed('4'))
	{
		SpawnBlackOut(static_cast<int>(EBlackOutLightingMode::SpotDiffuse), true, 2.0f, 1.0f);
	}
	if (Input::IsKeyPressed('5'))
	{
		SpawnBlackOut(static_cast<int>(EBlackOutLightingMode::SpotSpecular), true, 2.0f, 1.0f);
	}
	if (Input::IsKeyPressed(VK_SPACE))
	{
		GetWorld()->RemoveObjectByName(L"VignetteOverlay");
		GetWorld()->RemoveObjectByName(L"BlackOutOverlay");
		GetWorld()->RemoveObjectByName(L"ParticleWorld");
		GetWorld()->RemoveObjectByName(L"ParticleScreen");
	}

	if (Input::IsKeyPressed(VK_A)) SpawnParticleExplosion();
	if (Input::IsKeyPressed(VK_B)) SpawnParticleImpact();
	if (Input::IsKeyPressed(VK_C)) SpawnParticleClickL();
	if (Input::IsKeyPressed(VK_D)) SpawnParticleClickR();
	if (Input::IsKeyPressed(VK_E)) ToggleMouseTrail();
	if (Input::IsKeyPressed(VK_F)) SpawnParticleAura();
	if (Input::IsKeyPressed(VK_G)) SpawnParticleElectric();
	if (Input::IsKeyPressed(VK_H)) SpawnParticlePortal();
}

void EffectDemoScene::SpawnVignette(float durationSec, float maxAlpha)
{
	if (auto go = NewObject<gameObject>(L"VignetteOverlay"))
	{
		auto* vig = go->AddComponent<VignetteWidgetScript>();
		vig->SetDuration(durationSec);
		vig->SetMaxEdgeAlpha(maxAlpha);
	}
}

void EffectDemoScene::SpawnBlackOut(int modeIndex, bool useCrossFade, float durationSec, float maxAlpha)
{
	if (auto go = NewObject<gameObject>(L"BlackOutOverlay"))
	{
		auto* bo = go->AddComponent<BlackOutWidgetScript>();
		bo->SetDuration(durationSec);
		bo->SetMaxEdgeAlpha(maxAlpha);
		bo->SetMode(static_cast<EBlackOutLightingMode>(modeIndex));
		bo->UseCrossFade(useCrossFade);
	}
}

void EffectDemoScene::SpawnParticleExplosion()
{
	ParticleHelper::SpawnParticleExplosion(Input::GetMouseWorldPosition(), Define::Effect_Texture_Collision);
}

void EffectDemoScene::SpawnParticleImpact()
{
	ParticleHelper::SpawnParticleImpact(Input::GetMouseWorldPosition(), Define::Effect_Texture_Collision);
}

void EffectDemoScene::SpawnParticleClickL()
{
	ParticleHelper::SpawnParticleClickL(Input::GetMousePosition(), Define::Effect_Texture_Collision);
}

void EffectDemoScene::SpawnParticleClickR()
{
	ParticleHelper::SpawnParticleClickR(Input::GetMousePosition(), Define::Effect_Texture_Collision);
}

void EffectDemoScene::ToggleMouseTrail()
{
	WeakObjectPtr<gameObject> go = GetWorld()->FindObjectByName<gameObject>(L"Scene_Default_ParticleScreenTrail");
	if (!go) go = NewObject<gameObject>(L"Scene_Default_ParticleScreenTrail");
	auto* pc = go->GetComponent<ParticleComponent>();
	if (!pc) pc = go->AddComponent<ParticleComponent>();
	pc->SetDrawType(Define::EDrawType::ScreenSpace);
	static bool on = false; on = !on;
	pc->ToggleMouseTrail(on);
}

void EffectDemoScene::SpawnParticleAura()
{
	ParticleHelper::SpawnParticleAura(Input::GetMouseWorldPosition(), 100, 180, Define::Effect_Texture_Collision);
}

void EffectDemoScene::SpawnParticleElectric()
{
	ParticleHelper::SpawnParticleElectric(Input::GetMouseWorldPosition(), Define::Effect_Texture_Collision);
}

void EffectDemoScene::SpawnParticlePortal()
{
	ParticleHelper::SpawnParticlePortal(Input::GetMouseWorldPosition(), Define::Effect_Texture_Collision);
}
