#pragma once
#include "pch.h"
#include "Transform.h"

using namespace Microsoft::WRL;

class Object
{
public:
	Object()
	{
		m_localTransform = new Transform();
		m_worldTransform = new Transform();
	}
	virtual ~Object()
	{
		delete m_localTransform;
		delete m_worldTransform;
		m_bitmap = nullptr;
	}

	void Initialize();
	void LoadBitmapData(const std::wstring& path);
	void Release();

	// �������� ����
	Object* parent = nullptr;
	std::vector<Object*> children;

	void AddChild(Object* child)
	{
		child->parent = this;
		children.push_back(child);
	}

	// ��ͷ� ���� ��ȯ ���
	void UpdateWorldTransform()
	{
		D2D1::Matrix3x2F mat;

		if (parent)
		{
			mat = m_localTransform->ToMatrix() * parent->m_worldTransform->ToMatrix();
		}
		else
		{
			mat = m_localTransform->ToMatrix();
		}

		m_worldTransform->SetFromMatrix(mat);

		for (auto child : children)
			child->UpdateWorldTransform();
	}

	virtual void Render()
	{
		// m_worldTransform ���� ������
	}

	Transform* m_localTransform; // Transform ������Ʈ
	Transform* m_worldTransform; // Transform ������Ʈ
	ComPtr<ID2D1Bitmap1> m_bitmap; // BitmapImage ������Ʈ
};

