#pragma once
#include "pch.h"
#include "Transform.h"

/*
* @brief Object Ŭ����.
* @details ComPtr�� ID2D1Bitmap1�� �����ϰ�, Transform ������Ʈ�� �����մϴ�.
* @details ���� ��ȯ�� ��������� ����ϰ�, �ڽ� ��ü�� Transform�� �����մϴ�.
*/

using namespace Microsoft::WRL;

class Object : public std::enable_shared_from_this<Object>
{
public:
	Object()
	{
		m_localTransform = std::make_shared<Transform>();
		m_worldTransform = std::make_shared<Transform>();
	}
	virtual ~Object()
	{
		m_localTransform.reset();
		m_worldTransform.reset();
		m_localTransform = nullptr;
		m_worldTransform = nullptr;
		//m_bitmap = nullptr;
	}

	std::shared_ptr<Transform> m_localTransform; // Transform ������Ʈ
	std::shared_ptr<Transform> m_worldTransform; // Transform ������Ʈ
	D2D1_VECTOR_2F m_pivot{0,0}; // ��ǥ �߽���
	ComPtr<ID2D1Bitmap1> m_bitmap; // BitmapImage ������Ʈ

	virtual void Initialize();
	virtual void LoadBitmapData(const std::wstring& path);
	virtual void Release();
	virtual void Render();

	// �������� ����
	std::weak_ptr<Object> parent;
	std::vector<std::shared_ptr<Object>> children;

	inline void AddChild(std::weak_ptr<Object> child)
	{
		if (child.lock() == nullptr) return; // nullptr üũ
		child.lock()->parent = this->shared_from_this();
		children.push_back(child.lock());
	}

	inline void SetPivot(const float& _x, const float& _y)
	{
		m_pivot.x = _x;
		m_pivot.y = _y;
	}
	inline void SetPivot(const float& _x)
	{
		m_pivot.x = _x;
		m_pivot.y = _x;
	}

	// ��ͷ� ���� ��ȯ ���
	void UpdateWorldTransform();
};

