#pragma once
#include "pch.h"
#include "Transform.h"
#include "IRenderer.h"

using namespace Microsoft::WRL;

class BitmapRenderer : public IRenderer, public std::enable_shared_from_this<BitmapRenderer>
{
public:
	BitmapRenderer()
	{
		m_localTransform = std::make_shared<Transform>();
		m_worldTransform = std::make_shared<Transform>();
	}
	virtual ~BitmapRenderer()
	{
		m_localTransform.reset();
		m_worldTransform.reset();
		m_localTransform = nullptr;
		m_worldTransform = nullptr;
		//m_bitmap = nullptr;
	}

	void Initialize() override;
	void LoadBitmapData(const std::wstring& path);
	void Update();
	void Release() override;
	void Render() override;

	// �������� ����
	std::weak_ptr<BitmapRenderer> parent;
	std::vector<std::weak_ptr<BitmapRenderer>> children;

	inline void AddChild(std::weak_ptr<BitmapRenderer> child)
	{
		if (child.lock() == nullptr) return; // nullptr üũ
		child.lock()->parent = this->shared_from_this();
		children.push_back(child.lock());
	}

	std::shared_ptr<Transform> m_localTransform; // Transform ������Ʈ
	std::shared_ptr<Transform> m_worldTransform; // Transform ������Ʈ
	D2D1_VECTOR_2F m_pivot{ 0,0 }; // ��ǥ �߽���
	ComPtr<ID2D1Bitmap1> m_bitmap; // BitmapImage ������Ʈ


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
};

