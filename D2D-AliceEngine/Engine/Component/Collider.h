#pragma once
#include <Component/Component.h>
#include <Experimental/Physics/AABB.h>
#include "Math/TColor.h"

/*
* @brief : 충돌을 담당하는 컴포넌트입니다.
* @details : SetAABBBoxSize() 함수가 아닌 SetBoxSize()를 통해 크기에 맞는 AABB 값을 계산합니다.
*/
class BoxComponent;
class Collider : public Component
{
public:
	Collider();
	~Collider();
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;

	// 크기/오프셋/회전 설정
	void SetBoxSize(const FVector2& _size); // 유지: 호환용
	void SetSize(const FVector2& _size) { m_size = _size; dirty = true; SyncDebugBox(); }
	void SetLocalOffset(const FVector2& offset) { m_localOffset = offset; dirty = true; SyncDebugBox(); }
	void SetLocalRotation(float degrees) { m_localRotationDeg = degrees; dirty = true; SyncDebugBox(); }

	// 콜라이더 상대 트랜스폼 설정. BoundsBox의 트랜스폼.
	void SetRelativePosition(const FVector2& pos);
	void AddRelativePosition(const FVector2& delta);
	FVector2 GetRelativePosition() const;
	void SetRelativeRotation(float degrees);
	void AddRelativeRotation(float delta);
	float GetRelativeRotation() const;
	void SetRelativeScale(const FVector2& scale);
	void AddRelativeScale(const FVector2& delta);
	FVector2 GetRelativeScale() const;

	FVector2 GetSize() const { return m_size; }
	FVector2 GetLocalOffset() const { return m_localOffset; }
	float GetLocalRotation() const { return m_localRotationDeg; }

	// 디버그 렌더 박스
	void SetDebugDraw(bool enable);
	bool IsDebugDrawEnabled() const { return m_debugDraw; }
	void SetDebugRenderLayer(int layer);
	int GetDebugRenderLayer() const { return m_debugLayer; }
	void SetBoxColor(const FColor _color); // legacy-compatible

	// AABB 계산/접근
	void UpdateAABB();
	const FAABB& GetAABB() const { return aabb; }

	// 충돌 채널 (렌더 레이어와 무관)
	void SetCollisionChannel(int channel) { m_channel = channel; }
	int GetCollisionChannel() const { return m_channel; }
	// 호환용: SetLayer/GetLayer는 충돌 채널 의미로 동작합니다.
	void SetLayer(const int& _layer);
	int GetLayer() { return m_channel; }

	FAABB aabb;
	BoxComponent* boxComponent; // legacy pointer (kept for compatibility)

	bool dirty = true;
	float collisionDelay = 0.1f;

private:
	void EnsureDebugBox();
	void SyncDebugBox();
	FVector2 GetWorldCenter() const; // owner world pos + rotated local offset

private:
	// Local transform relative to owner
	FVector2 m_localOffset = FVector2(0.0f, 0.0f);
	FVector2 m_size = FVector2(0.0f, 0.0f);
	float m_localRotationDeg = 0.0f; // for future OBB use; AABB encloses rotated rect
	bool m_debugDraw = false;
	BoxComponent* m_box = nullptr; // internal debug box
	int m_debugLayer = 999; // render layer for debug box
	int m_channel = 0; // collision channel
};
