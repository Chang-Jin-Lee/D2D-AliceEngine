#pragma once
#include "Object.h"

/*
* @brief ī�޶� Ŭ����. 
* @details ����� Object�� ��ӹ޾� �⺻���� ��ɸ� �����Ǿ� �ֽ��ϴ�.
*/

class Camera : public Object
{
public:
	Camera() {};
	~Camera() {};

	void Initialize() override;
	void LoadBitmapData(const std::wstring& path) override;
	void Release() override;
	void Render() override;
};

