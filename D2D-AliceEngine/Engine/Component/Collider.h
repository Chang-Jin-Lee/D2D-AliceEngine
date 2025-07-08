#pragma once
#include <Component/Component.h>
#include <Experimental/Physics/AABB.h>

/*
*	@brief : �浹�� �����ϴ� ������Ʈ�Դϴ�.
*	@details : SetAABBBoxSize() �Լ��� �������־�� ũ�⿡ �´� AABB �ڽ��� ����ϴ�.
*/

class Collider : public Component
{
public:
	Collider();
	~Collider();
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;

	void SetAABBBoxSize(const FVector2& minVector, const FVector2& maxVector);

	bool bShowColliderBox = false;
	FAABB aabb;
};