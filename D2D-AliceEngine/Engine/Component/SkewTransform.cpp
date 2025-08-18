#include "pch.h"
#include "SkewTransform.h"
#include "Manager/UpdateTaskManager.h"
#include <TileMap/TileMapComponent.h>
#include "Define/Define.h"
#include <System/ScriptSystem.h>
#include "Object/gameObject.h"
#include <Animation/AnimatorInstance.h>
#include "Component/SpriteRenderer.h"

const FVector2 SkewTransform::GetOffset()
{
	return offset;
}

const void SkewTransform::SetRealPos(FVector2 _pos)
{
	realOffset = _pos - realPos;
	renderTransform->AddPosition(realOffset);
}

void SkewTransform::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_TICK(Update);
	//REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_EndPhysics);
}

void SkewTransform::OnStart()
{
	__super::OnStart();

   	if (!owner) {
		OutputDebugStringW(L"SkewTransform 컴포넌트의 owner가 없습니다!!!!\n");
		return;
	}

	renderTransform = owner->transform();
}

//void SkewTransform::Update(const float& deltaSeconds)
//{
//	__super::Update(deltaSeconds);
//
//	// 오프셋 안한 트랜스폼 저장하기
//	realPos = owner->GetPosition() - offset + realOffset;
//
//	if (!Math::Approximately(prevzPos, zPos)) {
//
//		// prevPos에 저장하기
//		prevzPos = zPos;
//
//		skewDeg = GetSkew();
//
//		// 원래대로 돌려놓기
//		owner->AddPosition(FVector2( - offset.x, -offset.y));
//
//		float rad = Math::Deg2Rad(skewDeg);
//
//		offset.x = -std::sin(rad) * zPos;
//		offset.y = std::cos(rad) * zPos;
//
//		// 트랜스폼 적용하기
//		owner->AddPosition(FVector2(offset.x, offset.y));
//		//renderTransform->AddPosition(realOffset);
//
//		// 렌더 레이어 설정하기
//		SetRenderLayer();
//
//		// 디버깅용
//		/*std::wstring message = owner->GetName() + L" : Zpos = " + std::to_wstring(zPos) + L"\n";
//		OutputDebugStringW(message.c_str());*/
//	}
//
//	realOffset = { 0.0f , 0.0f };
//}

void SkewTransform::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	if (!renderTransform)
		return;

	// 오프셋 안한 트랜스폼 저장하기
	realPos = renderTransform->GetPosition() - offset + realOffset;

	if (!Math::Approximately(prevzPos, zPos)) {

		// prevPos에 저장하기
		prevzPos = zPos;

		skewDeg = GetSkew();

		// 원래대로 돌려놓기
		renderTransform->AddPosition(-offset.x, -offset.y);

		float rad = Math::Deg2Rad(skewDeg);

		offset.x = -std::sin(rad) * zPos;
		offset.y = std::cos(rad) * zPos;

		// 트랜스폼 적용하기
		renderTransform->AddPosition(offset.x, offset.y);
		//renderTransform->AddPosition(realOffset);

		// 렌더 레이어 설정하기
		SetRenderLayer();

		// 디버깅용
		/*std::wstring message = owner->GetName() + L" : Zpos = " + std::to_wstring(zPos) + L"\n";
		OutputDebugStringW(message.c_str());*/
	}

	realOffset = { 0.0f , 0.0f };
}

void SkewTransform::ToSkewPos()
{
	zPos = 0.0f;
	skewDeg = GetSkew();

	float rad = Math::Deg2Rad(skewDeg);
	float z = owner->transform()->GetPosition().y / std::sin(rad);
	float xOffset = std::cos(rad) * z;

	owner->AddPosition(FVector2(xOffset, -(owner->GetPosition().y)));
	zPos = z;
}

float SkewTransform::GetSkew()
{
	if (groundTile)
		return groundTile->GetComponent<TileMapComponent>()->skewAngle.x;
	
	std::wstring message = owner->GetName() + L" : SkewTransform 컴포넌트에 groundTile 오브젝트가 등록되지 않았습니다!!!\n";

	OutputDebugStringW(message.c_str());
	return 0.0f;
}

void SkewTransform::SetRenderLayer()
{
	if (autoLayer) {
		int layer = static_cast<int>(-zPos);

		if (auto ani = owner->GetComponent<Animator>())
			ani->SetLayer(layer);
		else if (auto spr = owner->GetComponent<SpriteRenderer>())
			spr->SetLayer(layer);
		else {
			std::wstring message = owner->GetName() + L" : SkewTransform에서 Animator나 SpriteRenderer를 찾을수 없습니다!!\n";
			OutputDebugStringW(message.c_str());
		}
	}
}
