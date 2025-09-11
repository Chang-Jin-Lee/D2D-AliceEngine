#include "pch.h"
#include "Scene.h"
#include <System/InputSystem.h>
#include <System/ScriptSystem.h>
#include <System/RenderSystem.h>
#include <System/CollisionSystem.h>
#include <System/PhysicsSystem.h>
#include <Component/TextRenderComponent.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Manager/ClassManager.h>
#include <Manager/TimerManager.h>
#include <Core/Input.h>
#include <Math/TColor.h>
#include <Math/TMath.h>
#include <Component/ButtonComponent.h>
#include <Component/Effect/ParticleComponent.h>
#include <Helpers/CoordHelper.h>
#include <Application.h>

Scene::Scene()
{
	RenderSystem::GetInstance().Initialize();
	for (auto& object : m_objects)
	{
		if (WeakObjectPtr<gameObject> weak = object.second.get())
		{
			weak->Initialize();
		}
	}
	m_mainCamera = nullptr;
	m_mainCamera = new Camera();
	m_mainCamera->Initialize();
}

Scene::~Scene()
{
	m_mainCamera->Release();
}

void Scene::Initialize()
{
	CollisionSystem::GetInstance().Initialize();
	PhysicsSystem::GetInstance().Initialize();
	UpdateTaskManager::GetInstance().SetWorld();
}

void Scene::Release()
{
	for (auto it = m_objects.begin(); it != m_objects.end(); ++it)
	{
		it->second.get()->Release();
		it->second.reset();
	}
	m_objects.clear();
	m_nameToUUIDs.clear();
	UpdateTaskManager::GetInstance().ClearWorld();
	CollisionSystem::GetInstance().Release();
	PhysicsSystem::GetInstance().Release();
}

// 첫 프레임에서 ScriptSystem의 Start를 call
void Scene::Update()
{
	UpdateTaskManager::GetInstance().StartFrame();
	UpdateTaskManager::GetInstance().TickAll();
	MouseTrailAndClick();
    UpdateDebugHUD(0.0f);
	VisibleMemoryInfo();
	FlushPendingRemovals(); // 프레임 끝에서 업데이트 처리
	UpdateTaskManager::GetInstance().EndFrame();
}

void Scene::OnEnter()
{
	// 씬 진입 시 마우스 상태 초기화
	Input::ResetMouseState();

    m_sysinfoWidget = NewObject<gameObject>(L"SystemInfoWidget");
    auto* sysText = m_sysinfoWidget->AddComponent<TextRenderComponent>();
    sysText->SetDrawType(Define::EDrawType::ScreenSpace);
	sysText->SetColor(FColor(200, 0, 0, 255));
	sysText->SetOpacity(0);
    // ScreenSpace 좌표 (좌상단 0,0)
    	FVector2 appSize = Application::GetInstance().GetSize();
	sysText->SetRelativePosition(FVector2(appSize.x * 0.8f, appSize.y * 0.1f));
	sysText->SetLayer(987654321);

    // FPS 위젯
    m_fpsWidget = NewObject<gameObject>(L"FPSWidget");
    auto* fpsText = m_fpsWidget->AddComponent<TextRenderComponent>();
    fpsText->SetDrawType(Define::EDrawType::ScreenSpace);
    fpsText->SetColor(FColor(0, 255, 0, 255));
	fpsText->SetOpacity(1);
    	fpsText->SetRelativePosition(FVector2(appSize.x * 0.8f, appSize.y * 0.05f));
	fpsText->SetLayer(987654321);

	m_mouseTrail = NewObject<gameObject>(L"Scene_Default_ParticleScreenTrail");
	m_mouseParticle = m_mouseTrail->AddComponent<ParticleComponent>();
	m_mouseParticle->LoadData(L"Effect/MouseTrail.png");
	m_mouseParticle->SetDrawType(Define::EDrawType::ScreenSpace);
	m_mouseParticle->ToggleMouseTrail(true);
}

void Scene::OnExit()
{
	// 씬 종료 시 마우스 상태 초기화
	Input::ResetMouseState();
	UpdateTaskManager::GetInstance().ExitScene();
	D2DRenderManager::GetInstance().m_dxgiDevice->Trim();
	PackageResourceManager::GetInstance().UnloadData();
}

void Scene::OnSceneTransition()
{
	// 씬 전환 시 마우스 상태 초기화
	Input::ResetMouseState();
	
	// 모든 ButtonComponent의 마우스 상태도 초기화
	for (auto& object : m_objects)
	{
		if (auto gameObj = object.second.get())
		{
			// 모든 컴포넌트에서 ButtonComponent를 찾아서 초기화
			for (auto& component : gameObj->m_components)
			{
				if (auto buttonComp = dynamic_cast<ButtonComponent*>(component))
				{
					buttonComp->ResetMouseState();
				}
			}
		}
	}
	
	// 추가 안전장치: 모든 오브젝트의 OnStart 호출 전에 마우스 상태 초기화
	// 이는 새로 생성되는 컴포넌트들도 초기화하기 위함
}

void Scene::VisibleMemoryInfo()
{
    FMemoryInfo info = PackageResourceManager::GetInstance().GetMemoryInfo();
    if (m_sysinfoWidget)
    {
        auto* t = m_sysinfoWidget->GetComponent<TextRenderComponent>();
        t->SetText(L"[F2] : 디버그 창 닫기 \n[F3] : 박스 컴포넌트 끄기/켜기 \n[F4] : 마우스 트레일 끄기/켜기 \nVRAM : " + info.VRAMUssage + L"\n" + L"DRAM : " + info.DRAMUssage + L"\n" + L"PageFile : " + info.PageFile + L"\n");
    }
}

void Scene::UpdateDebugHUD(float /*deltaTime*/)
{
    if (m_fpsWidget)
    {
        const float fps = TimerManager::GetInstance().GetCurrentFPS();
        auto* t = m_fpsWidget->GetComponent<TextRenderComponent>();
        t->SetText(L"FPS: " + std::to_wstring(static_cast<int>(std::round(fps))));
    }
}

Camera* Scene::GetCamera()
{
	if (!m_mainCamera.expired())
		return m_mainCamera.Get();
	return nullptr;
}

bool Scene::RemoveObject(gameObject* targetObj)
{
	if (!targetObj) return false;
	std::wstring uuid = FindUUIDByPointer(targetObj);
	if (uuid.empty()) return false;

	m_pendingDeleteUUIDs.insert(uuid);
	return true;
}

bool Scene::RemoveObjectByName(const std::wstring& objectName)
{
	auto it = m_nameToUUIDs.find(objectName);
	if (it == m_nameToUUIDs.end() || it->second.empty())
		return false;

	m_pendingDeleteUUIDs.insert(*it->second.begin()); // 하나만 삭제
	return true;
}

bool Scene::RemoveAllObjectsByName(const std::wstring& name)
{
	auto it = m_nameToUUIDs.find(name);
	if (it == m_nameToUUIDs.end() || it->second.empty())
		return false;

	for (const auto& uuid : it->second)
		m_pendingDeleteUUIDs.insert(uuid);
	return true;
}

void Scene::FlushPendingRemovals()
{
	if (m_pendingDeleteUUIDs.empty()) return;

	for (const auto& uuid : m_pendingDeleteUUIDs)
	{
		auto it = m_objects.find(uuid);
		if (it == m_objects.end()) continue;

		const std::wstring& name = it->second->GetName();
		auto nameIt = m_nameToUUIDs.find(name);
		if (nameIt != m_nameToUUIDs.end())
		{
			nameIt->second.erase(uuid);
			if (nameIt->second.empty())
				m_nameToUUIDs.erase(nameIt);
		}

		it->second.reset();
		m_objects.erase(it);
	}
	m_pendingDeleteUUIDs.clear();
}

std::wstring Scene::FindUUIDByPointer(gameObject* ptr) const
{
	if (!ptr) return L"";
	for (const auto& kv : m_objects)
	{
		if (kv.second.get() == ptr)
			return kv.first;
	}
	return L"";
}


gameObject* Scene::Instantiate(gameObject* obj)
{
	gameObject* target = NewObject<gameObject>(obj->GetName());
	//gameObject* target = NewObject<gameObject>(L"replicated name");
	for (UObject* objComp : obj->m_components)
	{
		UObject* createdobj = ClassManager::GetInstance().CreateClass(objComp);
		if (Component* createdComp = dynamic_cast<Component*>(createdobj))
		{
			if (createdComp && objComp)
			{
				if (dynamic_cast<TransformComponent*>(createdComp))
				{
					ClassManager::GetInstance().ReplicateAllMembers(target->m_transformComponent.Get(), objComp);
				}
				else
				{
					ClassManager::GetInstance().ReplicateAllMembers(createdComp, objComp);
				}
				// AddComponent의 로직을 여기에 넣으면 됩니다.
				createdComp->Initialize();
				createdComp->SetOwner(target);
				target->m_components.push_back(createdComp);
			}
		}
	}
	target->transform()->SetDirty();
	return target;
}

void Scene::MouseTrailAndClick()
{
	if (Input::IsKeyPressed(VK_F2)) {
		m_debugHudVisible = !m_debugHudVisible;
		if (m_sysinfoWidget) m_sysinfoWidget->GetComponent<TextRenderComponent>()->SetOpacity(m_debugHudVisible ? 1.0f : 0.0f);
		if (m_fpsWidget) m_fpsWidget->GetComponent<TextRenderComponent>()->SetOpacity(m_debugHudVisible ? 1.0f : 0.0f);
	}
	if (Input::IsKeyPressed(VK_F3)) {
		D2DRenderManager::GetInstance().bRenderedBoxRect = !D2DRenderManager::GetInstance().bRenderedBoxRect;
	}
	if (Input::IsKeyPressed(VK_F4)) {
		m_mouseParticle->ToggleMouseTrail();
	}

	if (m_bClickable && TimerManager::GetInstance().GetGlobalTimeScale() > 0 && Input::IsMouseLeftPressed())
	{
		WeakObjectPtr<gameObject> go = GetWorld()->NewObject<gameObject>(L"Scene_Default_ParticleScreenClick");
		auto* pc = go->AddComponent<ParticleComponent>();
		pc->LoadData(L"Effect/MouseClick.png");
		pc->SetDrawType(Define::EDrawType::ScreenSpace);
		pc->SetAdditiveBlend(true);
		pc->EmitClickBurst(Input::GetMousePosition(), true);
	}
}
