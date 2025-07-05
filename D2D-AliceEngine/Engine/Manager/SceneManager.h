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

	Scene* m_currentScene;	// ���� ��

	Scene* GetWorld();
	static Camera* GetCamera();

	template<class T>
	static T* AddScene(const std::wstring& NewobjectName)
	{
		//static_assert(std::is_base_of_v<IComponent, TReturnType>, "TReturnType must be derived from IComponent");

		std::unique_ptr<T> createdObj = std::make_unique<T>();

		createdObj->SetName(NewobjectName);
		createdObj->SetUUID(NewobjectName + StringHelper::MakeUniqueName());
		T* rawPointer = createdObj.get();
		GetInstance().m_scenes.emplace(createdObj->GetUUID(), std::move(createdObj));

		return rawPointer;
	}

	static WeakObjectPtr<Scene> GetUUIDFromObjectName(const std::wstring& name)
	{
		// �̸� �������� ã��
		for (auto& scenePair : GetInstance().m_scenes)
		{
			if (WeakObjectPtr<Scene> sceneWeak = scenePair.second.get())
			{
				if (sceneWeak->GetName() == name)
				{
					return sceneWeak;
				}
			}
		}
		return nullptr;
	}

	void PerformSceneChange(const std::wstring& NewobjectName)
	{
		if (WeakObjectPtr<Scene> searchedScene = GetUUIDFromObjectName(NewobjectName))
		{
			if (WeakObjectPtr<Scene> weak = m_currentScene)	// ���� ���� �ִٸ� �� ���� Exit ��ŵ�ϴ�.
			{
				weak->OnExit();
				weak->Release();
				SceneManager::GetInstance().GetCamera()->Release();
			}
			m_currentScene = searchedScene.Get();
			m_currentScene->Initialize();
			m_currentScene->OnEnter();	// �ٲٷ��� ���� OnEnter() �Լ��� �����ŵ�ϴ�.
			SceneManager::GetInstance().GetCamera()->Initialize();
		}
	}

	static void ChangeScene(const std::wstring& NewobjectName)
	{
		GetInstance().m_nextSceneName = NewobjectName;
	}

private:
	std::unordered_map<std::wstring, std::unique_ptr<Scene>> m_scenes;
	std::wstring m_nextSceneName;
};