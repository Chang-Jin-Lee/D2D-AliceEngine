#pragma once
#include <Object/gameObject.h>
#include <Object/Camera.h>
#include <Helpers/StringHelper.h>
#include <unordered_set>

class ParticleComponent;
class Scene : public UObject
{
public:
	Scene();
	virtual ~Scene();

	WeakObjectPtr<Camera> m_mainCamera; // Main Camera

	void Initialize() override;
	void Release() override;
	void Update() override;

	virtual void OnEnter();
	virtual void OnExit();
	virtual void OnSceneTransition(); // 씬 전환 시 호출되는 함수 추가

	void VisibleMemoryInfo();
    void UpdateDebugHUD(float deltaTime);

	Camera* GetCamera();

public:
	template<class TReturnType, typename... Args>
	TReturnType* NewObject(const std::wstring& NewobjectName, Args&&... args)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");
		std::unique_ptr<TReturnType> createdObj = std::make_unique<TReturnType>(std::forward<Args>(args)...);
		std::wstring uuid = NewobjectName + StringHelper::MakeUniqueName();

		createdObj->SetName(NewobjectName);
		createdObj->SetUUID(uuid);
		m_nameToUUIDs[NewobjectName].insert(uuid);
		TReturnType* returnObject = createdObj.get();
		m_objects[uuid] = std::move(createdObj);

		return returnObject;
	}

	bool RemoveObject(gameObject* targetObj);

	bool RemoveObjectByName(const std::wstring& objectName);

	bool RemoveAllObjectsByName(const std::wstring& name);

	void FlushPendingRemovals();

	template<class T>
	bool RemoveObjectByType()
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (std::dynamic_pointer_cast<T>(it->second))
			{
				const std::wstring& name = it->second->GetName();
				auto nameIt = m_nameToUUIDs.find(name);
				if (nameIt != m_nameToUUIDs.end()) {
					nameIt->second.erase(it->first);
					if (nameIt->second.empty())
						m_nameToUUIDs.erase(nameIt);
				}
				m_objects.erase(it);
				return true;
			}
		}
		return false;
	}

	template<class TReturnType, typename... Args>
	WeakObjectPtr<TReturnType> NewObjectByWeak(const std::wstring& NewobjectName, Args&&... args)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");
		TReturnType* createdObj = new TReturnType(std::forward<Args>(args)...);
		std::wstring uuid = NewobjectName + StringHelper::MakeUniqueName();

		createdObj->SetName(NewobjectName);
		createdObj->SetUUID(uuid);
		m_objects.emplace(uuid, createdObj);

		return WeakObjectPtr<TReturnType>(createdObj);
	}

	template<class T>
	WeakObjectPtr<T> FindObject()
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (auto found = dynamic_cast<T*>(it->second.get()))
			{
				return WeakObjectPtr<T>(found);
			}
		}
		return WeakObjectPtr<T>();
	}

	template<class T>
	std::vector<WeakObjectPtr<T>> FindObjects()
	{
		std::vector<WeakObjectPtr<T>> foundObjects;
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (auto found = dynamic_cast<T*>(it->second.get()))
			{
				foundObjects.push_back(WeakObjectPtr<T>(found));
			}
		}
		return foundObjects;
	}

	template<class T>
	WeakObjectPtr<T> FindObjectByTag(std::wstring _tag)
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (auto found = dynamic_cast<T*>(it->second.get()))
			{
				if (found->GetTag() == _tag)
					return WeakObjectPtr<T>(found);
			}
		}
		return WeakObjectPtr<T>();
	}

	template<class T>
	std::vector<WeakObjectPtr<T>> FindObjectsByTag(std::wstring _tag)
	{
		std::vector<WeakObjectPtr<T>> foundObjects;
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (auto found = dynamic_cast<T*>(it->second.get()))
			{
				if (found->GetTag() == _tag)
					foundObjects.push_back(WeakObjectPtr<T>(found));
			}
		}
		return foundObjects;
	}

	template<class T>
	WeakObjectPtr<T> FindObjectByName(std::wstring _name)
	{
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (auto found = dynamic_cast<T*>(it->second.get()))
			{
				if (found->GetName() == _name)
					return WeakObjectPtr<T>(found);
			}
		}
		return WeakObjectPtr<T>();
	}

	template<class T>
	std::vector<WeakObjectPtr<T>> FindObjectsByName(std::wstring _name)
	{
		std::vector<WeakObjectPtr<T>> foundObjects;
		for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
		{
			if (auto found = dynamic_cast<T*>(it->second.get()))
			{
				if (found->GetName() == _name)
					foundObjects.push_back(WeakObjectPtr<T>(found));
			}
		}
		return foundObjects;
	}

	gameObject* Instantiate(gameObject* obj);
	void MouseTrailAndClick();
protected:
	ParticleComponent* m_mouseParticle;
private:
	gameObject* m_sysinfoWidget;
	gameObject* m_fpsWidget{ nullptr };
	gameObject* m_mouseTrail{ nullptr };
    bool m_debugHudVisible{ false };
	std::unordered_map<std::wstring, std::unique_ptr<gameObject>> m_objects;
	std::unordered_map<std::wstring, std::unordered_set<std::wstring>> m_nameToUUIDs;

	std::unordered_set<std::wstring> m_pendingDeleteUUIDs;

	std::wstring FindUUIDByPointer(gameObject* ptr) const;

public:
	bool GetClickable() const { return m_bClickable; }
	void SetClickable(const bool value) { m_bClickable = value; }

private:
	bool m_bClickable = false;
};

