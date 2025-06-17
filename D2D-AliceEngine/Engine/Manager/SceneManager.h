#pragma once
#include "Object/gameObject.h"
#include "Scene/Scene.h"
#include <Core/Singleton.h>

class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	virtual ~SceneManager();

	virtual void Initialize();
	virtual void UnInitialize();
	virtual void Update();

	std::weak_ptr<Scene> m_currentScene;	// ���� ��

	Scene* GetCurrentScene()
	{
		if (std::shared_ptr<Scene> sharedScene = m_currentScene.lock())
		{
			return sharedScene.get();
		}
		else
		{
			return nullptr;
		}
	}

	static Camera* GetCamera()
	{
		if (Get().m_currentScene.lock())
		{
			return Get().m_currentScene.lock()->GetCamera();
		}
	}
	template<class T>
	static T* AddScene(const std::wstring& NewobjectName)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");

		std::shared_ptr<T> createdObj = std::make_shared<T>();

		createdObj->SetName(NewobjectName);
		createdObj->SetUUID(NewobjectName + StringHelper::MakeUniqueName());

		auto result = Get().m_scenes.emplace(createdObj->GetUUID(), createdObj);

		return createdObj.get();
	}

	static std::weak_ptr<Scene> GetUUIDFromObjectName(const std::wstring& name)
	{
		// �̸� �������� ã��
		for (auto& scenePair : Get().m_scenes)
		{
			std::weak_ptr<Scene> scene = scenePair.second;
			if (scene.lock()->GetName() == name)
			{
				return scene;
			}
		}
		return std::weak_ptr<Scene>();
	}

	static void ChangeScene(const std::wstring& NewobjectName)
	{
		std::weak_ptr<Scene> searchedScene = GetUUIDFromObjectName(NewobjectName);

		if (searchedScene.lock())
		{
			if (Get().m_currentScene.lock())	// ���� ���� �ִٸ� �� ���� Exit ��ŵ�ϴ�.
			{
				Get().m_currentScene.lock()->OnExit();
				Get().m_currentScene.lock()->Release();
			}
			searchedScene.lock()->Initialize();
			searchedScene.lock()->OnEnter();	// �ٲٷ��� ���� OnEnter() �Լ��� �����ŵ�ϴ�.
			Get().m_currentScene = searchedScene;
		}
	}

private:
	std::unordered_map<std::wstring, std::shared_ptr<Scene>> m_scenes;
};