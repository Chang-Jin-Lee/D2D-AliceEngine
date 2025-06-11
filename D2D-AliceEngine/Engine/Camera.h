#pragma once
#include "Object/Object.h"
#include "Transform.h"

/*
* @brief ī�޶� Ŭ����. 
* @details ����� Object�� ��ӹ޾� �⺻���� ��ɸ� �����Ǿ� �ֽ��ϴ�.
*/

class Camera
{
public:
	Camera() {};
	~Camera() {};

	void Initialize();
	void Release();
	void Render();

	std::shared_ptr<Transform> m_transform;
};

