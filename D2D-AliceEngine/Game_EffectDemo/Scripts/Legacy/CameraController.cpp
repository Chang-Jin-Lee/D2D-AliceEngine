#include "CameraController.h"
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Math/Transform.h>
#include <Component/TextRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Core/Input.h>
#include <Component/VideoComponent.h>
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <System/ScriptSystem.h>

void CameraController::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(OnStart);
}

void CameraController::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	if (Input::IsKeyDown(VK_OEM_4))
	{
		//float fov = SceneManager::GetCamera()->fieldOfView;
		//fov -= 5.0f * deltaSeconds;
		//SceneManager::GetCamera()->SetFieldOfView(fov);

		FVector2 pos = SceneManager::GetCamera()->GetScale() + 1.1f * deltaSeconds;
		SceneManager::GetCamera()->SetScale(pos);
	}
	if (Input::IsKeyDown(VK_OEM_6))
	{
		//float fov = SceneManager::GetCamera()->fieldOfView;
		//fov += 5.0f * deltaSeconds;
		//SceneManager::GetCamera()->SetFieldOfView(fov);

		FVector2 pos = SceneManager::GetCamera()->GetScale() - 1.1f * deltaSeconds;
		SceneManager::GetCamera()->SetScale(pos);
	}
}

void CameraController::OnStart()
{
	m_camera = GetOwner();
	m_camera->AddComponent<InputComponent>()->SetAction(m_camera->GetHandle(), [this]() { Input(); });
}

void CameraController::OnEnd()
{

}

void CameraController::Input()
{
	if(Input::IsKeyDown(VK_D))
	{
		SceneManager::GetCamera()->AddPosition(5.0f, 0);
	}
	if (Input::IsKeyDown(VK_A))
	{
		SceneManager::GetCamera()->AddPosition(-5.0f, 0);
	}
	if (Input::IsKeyDown(VK_W))
	{
		SceneManager::GetCamera()->AddPosition(0, 5.0f);
	}
	if (Input::IsKeyDown(VK_S))
	{
		SceneManager::GetCamera()->AddPosition(0, -5.0f);
	}
	if (Input::IsKeyPressed(VK_1))
	{
		D2DRenderManager::GetInstance().SwitchTransformType(ETransformType::D2D);
	}
	if (Input::IsKeyPressed(VK_2))
	{
		D2DRenderManager::GetInstance().SwitchTransformType(ETransformType::Unity);
	}
}
