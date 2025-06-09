#pragma once
#include "pch.h"
#include "Transform.h"
#include "BitmapImage.h"

class Object
{
public:
	Object() 
	{
		m_localTransform = new Transform();
	}
	virtual ~Object() 
	{
		m_localTransform = nullptr;
	}

	virtual void Initialize();
	virtual void LoadData();
	virtual void Release();

	DoubleLinkedList<Transform*> childList;
	Transform* m_localTransform; // Transform ������Ʈ
	Transform* m_worldTransform; // Transform ������Ʈ
	BitmapImage m_bitmapImage; // BitmapImage ������Ʈ
};

