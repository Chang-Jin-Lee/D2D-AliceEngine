#pragma once
#include "Object/UObject.h"

/*
* @brief ī�޶� Ŭ����. 
* @details ����� Object�� ��ӹ޾� �⺻���� ��ɸ� �����Ǿ� �ֽ��ϴ�.
*/
class Transform;
class gameObject;

class Camera : public UObject
{
public:
	Camera() {};
	~Camera() {};

	void Initialize();
	void Update();
	void Release();

	void SetPosition(const float& _x, const float& _y);
	FVector2 GetPosition();

	void SetRotation(const float& _val);

	void AddRotation(const float& _val);
	void AddPosition(const float& _x, const float& _y);

	void SetOwner(gameObject* obj);
	void ClearOwner();
	gameObject* owner = nullptr;
	std::shared_ptr<Transform> m_transform;
};

