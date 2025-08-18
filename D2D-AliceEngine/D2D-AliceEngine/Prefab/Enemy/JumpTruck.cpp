#include "JumpTruck.h"
#include <System/ScriptSystem.h>
#include "Component/SpriteRenderer.h"
#include "Object/gameObject.h"
#include "Component/SkewTransform.h"
#include <Animation/AnimatorInstance.h>
#include <Prefab/Truck.h>
#include "Scene/Scene.h"
#include "Component/Collider.h"
#include "Scripts/Enemy/Jump/JumpTrigger.h"
#include <Component/BoxComponent.h>

void JumpTruck::Initialize()
{
	__super::Initialize();

	//REGISTER_SCRIPT_METHOD(OnStart);
}

void JumpTruck::OnStart()
{
	//__super::OnStart();

	//FVector2 scale = { 0.7f, 0.7f };

	//if (auto anim = owner->GetComponent<AnimatorInstance>()) {
	//	AnimatorController::LoadAnimatorController(L"Enemy/Tico/tico_sprite_AnimController.json", animController);
	//	anim->SetAnimatorController(&animController);
	//	anim->LoadSpriteSheet(L"Enemy/Tico/tico_sprites.json");
	//	anim->LoadAnimationClip(L"Enemy/Tico/tico_idle_anim.json");
	//	anim->ApplyClipDurationsToStates();

	//	anim->Play();
	//	anim->OnStart();
	//	anim->m_layer = 19999;
	//	// 스케일 조정
	//	anim->SetRelativeScale(scale);

	//	float height = anim->GetSpriteSizeY() * scale.y;
	//	float yOffset = height * 0.25f;

	//	SkewTransform* st = owner->GetComponent<SkewTransform>();
	//	st->zPos += yOffset / std::sin(Math::Deg2Rad(45.0f));
	//	//owner->transform()->SetWorldPosition(pos);
	//}
	//else {
	//	owner->GetComponent<SpriteRenderer>()->LoadData(L"Enemy/Tico/tico.png");
	//}

	__super::OnStart();

	owner->RemoveComponent<AnimatorInstance>(owner->GetComponent<AnimatorInstance>());

	auto sr = owner->GetComponent<SpriteRenderer>();

	sr->LoadData(L"Enemy/Jumporter/jumporter.png");
	//owner->transform()->SetScale(0.35f, 0.35f);
	// 스케일 조정
	FVector2 scale = { 0.35f, 0.35f };
	sr->SetRelativeScale(scale);

	float height = sr->GetBitmapSizeY() * scale.y;
	float width = sr->GetBitmapSizeX() * scale.x;
	float yOffset = height * 0.25f;

	// 점프 콜라이더 설정 넣기
	gameObject* lc = GetWorld()->NewObject<gameObject>(L"JumpCollider");
	JumpTrigger* jt = lc->AddComponent<JumpTrigger>();
	jt->SetBoxSize(FVector2(5.0f, height * 0.3f));
	jt->SetOffSet(FVector2(-(width * 0.3f), -(height * 0.25f)));
	jt->target = owner;

	// zPos 오프셋 넣기
	SkewTransform* st = owner->GetComponent<SkewTransform>();
	st->zPos += yOffset / std::sin(Math::Deg2Rad(45.0f));

	// 콜라이더 설정
	// 본체 충돌
	if (Collider* body = owner->GetComponent<Collider>())
	{
		body->SetLayer(5);
		if (body) {
			body->SetBoxSize(FVector2(100, 70));
			body->SetRelativePosition(FVector2(0.0f, -40.0f));
		}
	}
	// 총알 히트박스 (채널 0)
	if (Collider* hitbox = owner->AddComponent<Collider>())
	{
		hitbox->SetLayer(0);
		if (hitbox) {
			hitbox->SetBoxSize(FVector2(90, 60));
			hitbox->SetRelativePosition(FVector2(0.0f, -40.0f));
		}
	}
}
