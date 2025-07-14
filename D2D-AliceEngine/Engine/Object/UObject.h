#pragma once
#include <string>
#include <Core/ObjectHandler.h>
#include <Core/Singleton.h>
#include <Object/UObjectBase.h>

class Scene;
class UObject : public UObjectBase
{
	friend class ClassManager;
public:
	UObject();
	virtual ~UObject();
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Release() = 0;
	ObjectHandle GetHandle() const { return objectHandle; }

	template<typename T>
	WeakObjectPtr<T> WeakFromThis()
	{
		// T*�� �����ϰ� ĳ�����ؼ� WeakObjectPtr ����
		return WeakObjectPtr<T>(dynamic_cast<T*>(this));
	}

	Scene* GetWorld();

protected:
	ObjectHandle objectHandle;
};

