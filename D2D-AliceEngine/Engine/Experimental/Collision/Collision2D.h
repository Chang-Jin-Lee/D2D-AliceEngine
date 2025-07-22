#pragma once

class Collider;
class Rigidbody2D;
class Transform;
class Collision2D
{
public:
	Collider* collider = nullptr;
	Collider* otherCollider = nullptr;
	Rigidbody2D* rigidbody = nullptr;
	Rigidbody2D* otherRigidbody = nullptr;
	Transform* transform = nullptr;
};

