#include "DemoScene2_Tutorial.h"
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/SpineRenderer.h>
#include <Component/TransformComponent.h>
#include <Component/VideoComponent.h>
#include <Object/gameObject.h>
#include <Core/Input.h>
#include <Math/TColor.h>
#include <Helpers/CoordHelper.h>
#include <Scripts/Legacy/Aru.h>
#include <Scripts/Legacy/Aru2.h>
#include <Scripts/Legacy/Player.h>
#include <Scripts/Legacy/CameraController.h>
#include <Scripts/Spine2D/SpineScript.h>
#include <Component/ProgressBarComponent.h>

void DemoScene2_Tutorial::Initialize()
{
	__super::Initialize();
}

void DemoScene2_Tutorial::Release()
{
	__super::Release();
}

void DemoScene2_Tutorial::Update()
{
	__super::Update();
	static ProgressBarComponent* s_progress = nullptr;
	static bool s_initialized = false;
	static float s_p = 1.0f;
	static float s_dir = -1.0f;
	if (!s_initialized)
	{
		if (auto go = FindObjectByName<gameObject>(L"ProgressBarTest"))
		{
			s_progress = go->GetComponent<ProgressBarComponent>();
			s_initialized = (s_progress != nullptr);
		}
	}
	if (s_progress)
	{
		s_p += s_dir * 0.01f;
		if (s_p <= 0.0f) { s_p = 0.0f; s_dir = 1.0f; }
		if (s_p >= 1.0f) { s_p = 1.0f; s_dir = -1.0f; }
		s_progress->SetProgress(s_p);
	}

	if (Input::IsKeyPressed(VK_3))
	{
		SceneManager::ChangeScene(L"SolarSystemScene");
	}
}

void DemoScene2_Tutorial::OnEnter()
{
	__super::OnEnter();
	m_camera = NewObject<gameObject>(L"Camera");
	m_camera->AddComponent<CameraController>();
	spineObject = NewObject<gameObject>(L"spineObject");
	spine = spineObject->AddComponent<SpineScript>();
	spine->LoadData(L"Spine2D_g\\Monster_1.atlas", L"Spine2D_g\\Monster_1.json");
	if (spine->spineRenderer)
	{
		spine->spineRenderer->SetAnimation("Idle");
		spine->spineRenderer->SetRelativePosition(FVector2(0, 0));
		spine->spineRenderer->SetRendered(true);
		//spine->spineRenderer->SetLayer(94548823);
	}
	m_widget = NewObject<gameObject>(L"widget");
	m_widget2 = NewObject<gameObject>(L"widget2");
	m_widget3 = NewObject<gameObject>(L"widget3");
	m_widget4 = NewObject<gameObject>(L"widget4");
	m_widget5 = NewObject<gameObject>(L"widget5");
	m_widget6 = NewObject<gameObject>(L"widget6");
	m_widget7 = NewObject<gameObject>(L"widget6");
	m_widget->AddComponent<TextRenderComponent>()->SetText(
		L"\n"
		L" <카메라> \n"
		L" [W,A,S,D]  : 카메라 상,하,좌,우 이동 \n"
		L" [1/2] : D2D, Unity 좌표계 \n"
		L" [ [ / ] ] : 카메라 축소, 확대\n"
		L" [Q] : 카메라를 아루에게 붙이기 \n"
		L" [E] : 카메라를 떼기 \n"
		L" * 카메라를 붙이면 화살표로 카메라를 이동할 수 없습니다. \n"
		L"\n"
		L" <아루> \n"
		L" [화살표 상/하/좌/우] : 상,하,좌,우 이동 \n"
		L" [C] : 아루 복제 \n"
		L" [B/N] : 아루 스케일 변환 \n"
		L" [R] : 아루 회전 변환 \n"
		L" [5,6] : 아루 스폰, 무기 파괴 \n"
		L" [7,8] : 지갑 스폰, 지갑 파괴 \n"
		L" [4] : 아루 이름 한영 전환 \n"
		L"\n"
		L" ::Delegate \n"
		L" [T] : 아루가 아루2에게 데미지 주기 \n"
		L" [Y] : 아루 5 회복하기 \n"
		L" [U] : 아루 최대체력 10 늘리기 \n"
		L"\n"
		L" [G] : 아루2가 아루에게 데미지 주기 \n"
		L" [H] : 아루2 7 회복하기 \n"
		L" [J] : 아루 최대체력 15 늘리기"
	);
	m_widget->SetPosition(FVector2(0, 0));
	m_widget->GetComponent<TextRenderComponent>()->SetRelativePosition(CoordHelper::RatioCoordToScreen(FVector2(0.02, 0.1)));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(22.0f);
	m_widget->GetComponent<TextRenderComponent>()->SetColor(FColor(0, 0, 0, 255));
	m_widget2->transform()->SetPosition(0, 0);
	m_widget2->AddComponent<TextRenderComponent>()->SetText(L" <씬> \n [3] : 씬 전환");
	m_widget2->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopRight);
	m_widget2->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(Define::SCREEN_WIDTH * 0.9, 0));
	m_widget2->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget3->transform()->SetPosition(0, 0);
	m_widget3->AddComponent<TextRenderComponent>()->SetText(L" <현재 씬> " + GetName());
	m_widget3->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget3->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(20, 10));
	m_widget3->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget4->transform()->SetPosition(0, 0);
	m_widget4->AddComponent<TextRenderComponent>()->SetText(L" <gameObject> :" + GetName());
	m_widget4->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget4->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(480, 100));
	m_widget4->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget5->transform()->SetPosition(0, 0);
	m_widget5->AddComponent<TextRenderComponent>()->SetText(L" <gameObject> :" + GetName());
	m_widget5->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget5->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(480, 140));
	m_widget5->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget6->transform()->SetPosition(0, 0);
	m_widget6->AddComponent<TextRenderComponent>()->SetText(L" <gameObject> :" + GetName());
	m_widget6->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget6->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(480, 180));
	m_widget6->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
	m_widget7->transform()->SetPosition(0, 0);
	m_widget7->AddComponent<TextRenderComponent>()->SetText(L" <gameObject> :" + GetName());
	m_widget7->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget7->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(480, 220));
	m_widget7->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
}

void DemoScene2_Tutorial::OnExit()
{
	__super::OnExit();
}

void DemoScene2_Tutorial::aru2Input()
{
	if (Input::IsKeyPressed(VK_G))
	{
		m_aru->GetComponent<Aru>()->m_aruStat->DecreaseAbility("HP", 5);
		m_aru2->GetComponent<Aru2>()->m_aru2Stat->DecreaseAbility("MP", 4);
	}
	if (Input::IsKeyPressed(VK_H))
	{
		m_aru2->GetComponent<Aru2>()->m_aru2Stat->IncreaseAbility("MAXHP", "HP", 7);
	}
	if (Input::IsKeyPressed(VK_J))
	{
		m_aru2->GetComponent<Aru2>()->m_aru2Stat->SetStat("MAXHP", m_aru2->GetComponent<Aru2>()->m_aru2Stat->GetStat("MAXHP") + 15);
	}
}

void DemoScene2_Tutorial::aruInput()
{
	if (Input::IsKeyPressed(VK_Q))
	{
		SceneManager::GetCamera()->SetOwner(m_aru);
	}
	if (Input::IsKeyPressed(VK_E))
	{
		SceneManager::GetCamera()->ClearOwner();
	}
	if (Input::IsKeyPressed(VK_T))
	{
		m_aru2->GetComponent<Aru2>()->m_aru2Stat->DecreaseAbility("HP", 10);
		m_aru->GetComponent<Aru>()->m_aruStat->DecreaseAbility("MP", 5);
	}
	if (Input::IsKeyPressed(VK_Y))
	{
		m_aru->GetComponent<Aru>()->m_aruStat->IncreaseAbility("MAXHP", "HP", 5);
	}
	if (Input::IsKeyPressed(VK_U))
	{
		m_aru->GetComponent<Aru>()->m_aruStat->SetStat("MAXHP", m_aru->GetComponent<Aru>()->m_aruStat->GetStat("MAXHP")+10);
	}
	if (Input::IsKeyPressed(VK_C))
	{
		m_aruClone = Instantiate(m_aru);
	}
	if (Input::IsKeyDown(VK_B))
	{
		FVector2 scale = m_aru->transform()->GetScale() * 0.8f;
		m_aru->transform()->SetScale(scale.x, scale.y);
	}
	if (Input::IsKeyDown(VK_N))
	{
		FVector2 scale = m_aru->transform()->GetScale() * 1.3f;
		m_aru->transform()->SetScale(scale.x, scale.y);
	}
	if (Input::IsKeyDown(VK_R))
	{
		m_aru->transform()->SetRotation(m_aru->transform()->GetRotation() + 5.0f);
	}
	if (Input::IsKeyDown(VK_I))
	{
		WeakObjectPtr<gameObject> weakAru = FindObjectByName<gameObject>(L"Aru");
		if (!weakAru.expired())
		{
			m_widget4->GetComponent<TextRenderComponent>()->SetText(L" <gameObject> :" + weakAru->GetName());
		}
	}
	if (Input::IsKeyDown(VK_O))
	{
		WeakObjectPtr<gameObject> weakAru = FindObjectByTag<gameObject>(L"Aru2");
		if (!weakAru.expired())
		{
			m_widget5->GetComponent<TextRenderComponent>()->SetText(L" <gameObject> :" + weakAru->GetName());
		}
	}
	if (Input::IsKeyDown(VK_P))
	{
		std::vector<WeakObjectPtr<gameObject>> weakAru = FindObjectsByTag<gameObject>(L"Aru");
		if (!weakAru.empty())
		{
			m_widget6->GetComponent<TextRenderComponent>()->SetText(L" <gameObject> : " + std::to_wstring(weakAru.size()));
		}
	}
	if (Input::IsKeyDown(VK_L))
	{
		std::vector<WeakObjectPtr<gameObject>> weakAru = FindObjectsByName<gameObject>(L"Aru");
		if (!weakAru.empty())
		{
			m_widget7->GetComponent<TextRenderComponent>()->SetText(L" <gameObject> : " + std::to_wstring(weakAru.size()));
		}
	}
	if (Input::IsKeyDown(VK_R))
	{
		m_aru->transform()->SetRotation(m_aru->transform()->GetRotation() + 5.0f);
	}
	if (Input::IsKeyDown(VK_4))
	{
		for (int i = 0; i < 10; i++)
		{
			spineObject = NewObject<gameObject>(L"spineObject");
			SpineScript* spine = spineObject->AddComponent<SpineScript>();
			spine->LoadData(L"Spine2D/Monster_1.atlas", L"Spine2D/Monster_1.json");
		}
	}
	if (Input::IsKeyDown(VK_G))
	{
		for (int i = 0; i < 10; i++)
		{
			GetWorld()->RemoveObjectByName(L"spineObject");
		}
	}
	float moveSpeed = 35.0f;
	if (Input::IsKeyDown(VK_W))
	{
		FVector2 pos = spine->spineRenderer->GetRelativePosition();
		pos.y += moveSpeed;
		spine->spineRenderer->SetRelativePosition(pos);
	}
	if (Input::IsKeyDown(VK_S))
	{
		FVector2 pos = spine->spineRenderer->GetRelativePosition();
		pos.y -= moveSpeed;
		spine->spineRenderer->SetRelativePosition(pos);
	}
	if (Input::IsKeyDown(VK_A))
	{
		FVector2 pos = spine->spineRenderer->GetRelativePosition();
		pos.x -= moveSpeed;
		spine->spineRenderer->SetRelativePosition(pos);
	}
	if (Input::IsKeyDown(VK_D))
	{
		FVector2 pos = spine->spineRenderer->GetRelativePosition();
		pos.x += moveSpeed;
		spine->spineRenderer->SetRelativePosition(pos);
	}
	if (Input::IsKeyPressed(VK_F))
	{
		spine->spineRenderer->SetNextAnimation();
	}
	if (Input::IsKeyDown(VK_OEM_4))
	{
		FVector2 sc = spine->spineRenderer->GetRelativeScale();
		sc.x *= 0.9f; sc.y *= 0.9f;
		sc.x = Math::Clamp(sc.x, 0.1f, 5.0f);
		sc.y = Math::Clamp(sc.y, 0.1f, 5.0f);
		spine->spineRenderer->SetRelativeScale(sc);
	}
	if (Input::IsKeyDown(VK_OEM_6))
	{
		FVector2 sc = spine->spineRenderer->GetRelativeScale();
		sc.x *= 1.1f; sc.y *= 1.1f;
		sc.x = Math::Clamp(sc.x, 0.1f, 5.0f);
		sc.y = Math::Clamp(sc.y, 0.1f, 5.0f);
		spine->spineRenderer->SetRelativeScale(sc);
	}

}

void DemoScene2_Tutorial::CameraInput()
{
}
