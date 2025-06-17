#pragma once
#include "Object/Object.h"
#include <Math/Transform.h>

/*
* @brief ī�޶� Ŭ����. 
* @details ����� Object�� ��ӹ޾� �⺻���� ��ɸ� �����Ǿ� �ֽ��ϴ�.
*/

class Camera : public Object
{
public:
	Camera() {};
	~Camera() {};

	void Initialize();
	void Update();
	void Release();

	std::shared_ptr<Transform> m_transform;
};

