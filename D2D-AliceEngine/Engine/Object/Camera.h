#pragma once
#include "Object/UObject.h"
#include <Math/Transform.h>

/*
* @brief ī�޶� Ŭ����. 
* @details ����� Object�� ��ӹ޾� �⺻���� ��ɸ� �����Ǿ� �ֽ��ϴ�.
*/
class Camera : public UObject
{
public:
	Camera() {};
	~Camera() {};

	void Initialize();
	void Update();
	void Release();

	inline void SetPosition(const float& _x, const float& _y)
	{
		m_transform->SetPosition(_x, _y);
	}

	inline FVector2 GetPosition()
	{
		return FVector2(m_transform->GetPosition().x, m_transform->GetPosition().y);
	}

	inline void SetRotation(const float& _val)
	{
		m_transform->SetRotation(_val);
	}

	inline void AddRotation(const float& _val)
	{
		m_transform->SetRotation(m_transform->GetRotation() + _val);
	}

	inline void AddPosition(const float& _x, const float& _y)
	{
		m_transform->SetPosition(m_transform->GetPosition().x + _x, m_transform->GetPosition().y + _y);
	}

	inline void SetOwner(gameObject* obj)
	{
		owner = obj;
	}
	inline void ClearOwner()
	{
		owner = nullptr;
	}
	gameObject* owner = nullptr;
	std::shared_ptr<Transform> m_transform;
};

