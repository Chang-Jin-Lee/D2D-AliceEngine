#pragma once
#include "../pch.h"
#include "../Transform.h"
#include "../BitmapRenderer.h"
#include "../Manager/ComponentManager.h"
#include "../Interface/IObject.h"

/*
* @brief Object Ŭ����.
* @details �̹����� ��ǥ�� �����ϴ� BitmapRenderer ������Ʈ�� �����մϴ�.
*/

class Object : public IObject, public std::enable_shared_from_this<Object>
{
public:
	Object()
	{}
	Object(const std::wstring& path, const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f))
	{
		Initialize(path, position, rotation, scale, pivot);
	}
	virtual ~Object()
	{
	}

	template<class T> 
	std::weak_ptr<T> AddComponent(const std::wstring& componentName)
	{
		return CreateDefaultSubobject(T, componentName, GetUUID());
	}

	template<class T>
	std::weak_ptr<T> GetComponent(const std::wstring& componentName)
	{
		//return CreateDefaultSubobject(T, componentName);
	}
	
	void Initialize() override;
	void Update() override;
	void Release() override;

	virtual void Initialize(const std::wstring& path, const FVector2& position = FVector2(0.0f), const float& rotation = 0.0f, const FVector2& scale = FVector2(1.0f), const FVector2& pivot = FVector2(0.0f));

	void LoadData(const std::wstring& path);

public:
	std::weak_ptr<BitmapRenderer> m_bitmapRenderer;

	inline std::weak_ptr<BitmapRenderer> GetRenderer()
	{
		return m_bitmapRenderer;
	}

	// �������� ����
	std::weak_ptr<Object> parent;
	std::vector<std::weak_ptr<Object>> children;

	inline void AddChildObject(std::weak_ptr<Object> child)
	{
		if (child.lock() == nullptr) return; // nullptr üũ
		child.lock()->parent = this->weak_from_this();
		children.push_back(child.lock());
	}

	FVector2 m_pivot{ 0,0 }; // ��ǥ �߽���
	std::weak_ptr<Transform> m_localTransform; // Transform ������Ʈ
	std::weak_ptr<Transform> m_worldTransform; // Transform ������Ʈ

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