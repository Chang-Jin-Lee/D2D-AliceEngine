#pragma once
#include "Object/gameObject.h"
#include <Core/Singleton.h>
#include <Helpers/StringHelper.h>
#include <Scene/Scene.h>

class Camera;
class SceneManager : public Singleton<SceneManager>
{
public:
	SceneManager();
	virtual ~SceneManager();

	virtual void Initialize();
	virtual void UnInitialize();
	virtual void Update();

	std::weak_ptr<Scene> m_currentScene;	// ���� ��

	Scene* GetWorld();
	static Camera* GetCamera();

	template<class T>
	static T* AddScene(const std::wstring& NewobjectName)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");

		std::shared_ptr<T> createdObj = std::make_shared<T>();

		createdObj->SetName(NewobjectName);
		createdObj->SetUUID(NewobjectName + StringHelper::MakeUniqueName());

		auto result = GetInstance().m_scenes.emplace(createdObj->GetUUID(), createdObj);

		return createdObj.get();
	}

	static std::weak_ptr<Scene> GetUUIDFromObjectName(const std::wstring& name)
	{
		// �̸� �������� ã��
		for (auto& scenePair : GetInstance().m_scenes)
		{
			std::weak_ptr<Scene> scene = scenePair.second;
			if (scene.lock()->GetName() == name)
			{
				return scene;
			}
		}
		return std::weak_ptr<Scene>();
	}

	void PerformSceneChange(const std::wstring& NewobjectName)
	{
		std::weak_ptr<Scene> searchedScene = GetUUIDFromObjectName(NewobjectName);

		if (searchedScene.lock())
		{
			if (m_currentScene.lock())	// ���� ���� �ִٸ� �� ���� Exit ��ŵ�ϴ�.
			{
				m_currentScene.lock()->OnExit();
				m_currentScene.lock()->Release();
				SceneManager::GetInstance().GetCamera()->Release();
			}
			m_currentScene = searchedScene;
			m_currentScene.lock()->Initialize();
			m_currentScene.lock()->OnEnter();	// �ٲٷ��� ���� OnEnter() �Լ��� �����ŵ�ϴ�.
			SceneManager::GetInstance().GetCamera()->Initialize();
		}
	}

	static void ChangeScene(const std::wstring& NewobjectName)
	{
		GetInstance().m_nextSceneName = NewobjectName;
	}

private:
	std::unordered_map<std::wstring, std::shared_ptr<Scene>> m_scenes;
	std::wstring m_nextSceneName;
};