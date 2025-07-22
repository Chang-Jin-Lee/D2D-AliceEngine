#pragma once
#include <Object/UObject.h>
#include <Component/Component.h>
#include <memory>
#include <Math/TMath.h>

/*
* @brief Object 클래스.
* @details 이미지와 좌표를 관리하는 BitmapRenderer 컴포넌트를 포함합니다.
*/

class TransformComponent;
class D2DRenderManager;
class gameObject : public UObject
{
public:
	gameObject() { Initialize(); }
	gameObject(const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot);	
	virtual ~gameObject();

	virtual void OnStart();
	virtual void OnEnd();
	/*
	* @ Component를 관리하는 로직입니다.
	*/

	std::vector<Component*> m_components;

	template<class T, typename... Args>
	T* AddComponent(Args&&... args)
	{	
		T* createdComp = new T(std::forward<Args>(args)...);
		createdComp->Initialize();
		createdComp->SetOwner(WeakFromThis<gameObject>());
		m_components.emplace_back(createdComp);

		return createdComp;
	}

	template<class T>
	std::vector<T*> GetComponents()
	{
		std::vector<T*> container;
		for (auto component : m_components)
		{
			if (WeakObjectPtr<T> castedComponent = dynamic_cast<T*>(component))
			{
				container.push_back(castedComponent.Get());
			}
		}
		return container;
	}

	template<class T>
	T* GetComponent()
	{
		for (auto& comp : m_components)
		{
			// shared_ptr<Component> → shared_ptr<T>로 캐스팅
			if (auto casted = dynamic_cast<T*>(comp))
			{
				return casted; // raw pointer 반환
			}
		}
		return nullptr;
	}


	template<class T>
	bool RemoveComponent(T* targetComponent)
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			if ((*it) == targetComponent)
			{
				(*it)->OnDestroy();
				m_components.erase(it);
				delete targetComponent;
				return true;
			}
		}
		return false;
	}

	template<class T, typename... Args>
	WeakObjectPtr<T> AddComponentByWeak(Args&&... args)
	{
		T* createdComp = new T(std::forward<Args>(args)...);
		createdComp->Initialize();
		createdComp->SetOwner(WeakFromThis<gameObject>());
		m_components.emplace_back(createdComp);
		return WeakObjectPtr<T>(createdComp);
	}

	template<class T>
	WeakObjectPtr<T> GetComponentByWeak()
	{
		for (auto it = m_components.begin(); it != m_components.end(); ++it)
		{
			if (T* component = dynamic_cast<T*>((*it).get()))
			{
				return WeakObjectPtr<T>(component);
			}
		}
		return nullptr;
	}
	
	void Initialize() override;
	void Update() override;
	void Release() override;
	void Initialize(const FVector2& position, const float& rotation, const FVector2& scale, const FVector2& pivot);

	TransformComponent* transform() const { return m_transformComponent.lock(); }
	void AddChildObject(const gameObject* obj);

public:
	WeakObjectPtr<TransformComponent> m_transformComponent;
};