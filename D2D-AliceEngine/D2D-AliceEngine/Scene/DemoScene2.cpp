#include "DemoScene2.h"
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
#include "../Scripts/BackGroundImage.h"
#include "../Scripts/Legacy/Aru.h"
#include "../Scripts/Legacy/Aru2.h"
#include "../Scripts/Legacy/Player.h"
#include "../Scripts/Legacy/CameraController.h"
#include <Helpers/CoordHelper.h>
#include <Scripts/Spine2D/SpineScript.h>
#include <Component/ProgressBarComponent.h>
#include <Scripts/Widget/VignetteWidgetScript.h>
#include <Component/Collider.h>

/*
*	NewObject<T>(std::wstring&) : 해당 이름의 게임오브젝트를 생성하고 rawPointer를 반환합니다.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : 좌표, 회전, 스케일, 피봇을 지정합니다.
*/

void DemoScene2::Initialize()
{
	__super::Initialize();
}

void DemoScene2::Release()
{
	__super::Release();
}

void DemoScene2::Update()
{
	__super::Update();
    // ProgressBar 테스트 애니메이션 (1 -> 0 반복)
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
        s_p += s_dir * 0.01f; // 약 2초 주기로 변화
        if (s_p <= 0.0f) { s_p = 0.0f; s_dir = 1.0f; }
        if (s_p >= 1.0f) { s_p = 1.0f; s_dir = -1.0f; }
        s_progress->SetProgress(s_p);
    }

    // SpaceBar로 비네트 연출 테스트 생성
    if (Input::IsKeyPressed(VK_SPACE))
    {
		//GetWorld()->RemoveObjectByName(L"VignetteOverlay");
        if (auto go = NewObject<gameObject>(L"VignetteOverlay")) 
        {
            auto* vig = go->AddComponent<VignetteWidgetScript>();
            vig->SetDuration(2.4f);
            vig->SetMaxEdgeAlpha(1.0f);
        }
    }
}

void DemoScene2::OnEnter()
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
		spine->spineRenderer->SetLayer(94548823);
	}

	//spine->LoadData(L"Spine2D_g\\Monster_1.atlas", L"Spine2D_g\\Monster_1.json");
	//if (spine->spineRenderer)
	//{
	//	//spine->spineRenderer->SetAnimation("animation");
	//	spine->spineRenderer->SetPosition(FVector2(0, 0));
	//	spine->spineRenderer->SetRendered(true);
	//	spine->spineRenderer->SetLayer(94548823);
	//	spine->spineRenderer->SetUseSpineLayerOrder(false);
	//	//spine->spineRenderer->SetDrawType(Define::EDrawType::WorldSpace);
	//}

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
	//m_widget->GetComponent<TextRenderComponent>()->SetRelativePosition(FVector2(20, 50))
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

	//m_backgroundImage = NewObject<gameObject>(L"yuuka");
	//m_backgroundImage->AddComponent<BackGroundImage>();

	//m_alice = NewObject<gameObject>(L"alice");
	//m_alice->transform()->SetPosition(-300, 0);
	//m_alice->transform()->SetRotation(0);
	//m_alice->transform()->SetScale(0.3f, 0.3f);
	//m_alice->transform()->SetPivot(0.5f);
	//m_alice->AddComponent<VideoComponent>()->LoadData(L"BackGround\\Alice.webm", 30, L"jpg", 95, false);
	//m_alice->GetComponent<VideoComponent>()->Play();

	m_aru = NewObject<gameObject>(L"Aru");
	m_aru->AddComponent<Aru>();
	Collider* t = m_aru->AddComponent<Collider>();
	t->SetBoxSize(FVector2(200,200));
	t->boxComponent->SetRelativePosition(FVector2(350, 50));
	t->SetCollisionChannel(5);
	//t->boxComponent->SetRelativePosition(FVector2(0, 0));
	//t->SetLocalOffset(FVector2(50, 50));

	Collider* tt = m_aru->AddComponent<Collider>();
	tt->SetBoxSize(FVector2(200, 200));
	tt->boxComponent->SetRelativePosition(FVector2(-350, 50));
	tt->SetCollisionChannel(3);
	//t->boxComponent->SetRelativePosition(FVector2(0, 0));
	//t->SetLocalOffset(FVector2(50, 50));

	m_aru2 = NewObject<gameObject>(L"Aru2");
	m_aru2->AddComponent<Aru2>();
	Collider* t2 = m_aru2->AddComponent<Collider>();
	t2->SetBoxSize(FVector2(200, 200));
	//t2->boxComponent->SetRelativePosition(FVector2(-350, 50));
	t2->boxComponent->SetRelativePosition(FVector2(-0, 0));
	t2->SetCollisionChannel(3);

	// =======================================  Tag Example  ==========================================
	// 태그 변하는걸 보려면 여기서 m_aru2를 Aru2로 바꿔주세요.
	// 지금은 2개가 찾아지는 걸 볼 수 있습니다.
	m_aru->SetTag(L"Aru");
	m_aru2->SetTag(L"Aru");
	//m_aru2->SetTag(L"Aru2");

	m_aru->AddComponent<InputComponent>()->SetAction(m_aru->GetHandle(), [this]() { aruInput(); });
	m_aru2->AddComponent<InputComponent>()->SetAction(m_aru2->GetHandle(), [this]() { aru2Input(); });
	m_aru2->SetScale(FVector2(0.4f, 0.4f));
	m_aru2->SetPosition(FVector2(400, 100));

    // ProgressBar 테스트 생성
    if (auto go = NewObject<gameObject>(L"ProgressBarTest"))
    {
        auto* bar = go->AddComponent<ProgressBarComponent>();
        bar->SetDrawType(Define::EDrawType::ScreenSpace);
        bar->LoadData(L"UI\\UI_ToOption.png");
        bar->SetType(EProgressBarType::Linear);
        bar->SetRelativeScale(FVector2(0.8f, 0.8f));
        bar->SetRelativePosition(FVector2(500, 300));
        bar->SetProgress(1.0f);
		bar->SetLayer(30000);
    }
}

void DemoScene2::OnExit()
{
	__super::OnExit();
}

void DemoScene2::aru2Input()
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

void DemoScene2::aruInput()
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

	float moveSpeed = 35.0f; // 한번 누를 때마다 이동하는 픽셀 수

	if (Input::IsKeyDown(VK_W)) // 위
	{
		FVector2 pos = spine->spineRenderer->GetRelativePosition();
		pos.y += moveSpeed; // y 증가 → 위로 이동
		spine->spineRenderer->SetRelativePosition(pos);
	}

	if (Input::IsKeyDown(VK_S)) // 아래
	{
		FVector2 pos = spine->spineRenderer->GetRelativePosition();
		pos.y -= moveSpeed; // y 감소 → 아래로 이동
		spine->spineRenderer->SetRelativePosition(pos);
	}

	if (Input::IsKeyDown(VK_A)) // 왼쪽
	{
		FVector2 pos = spine->spineRenderer->GetRelativePosition();
		pos.x -= moveSpeed; // x 감소 → 왼쪽 이동
		spine->spineRenderer->SetRelativePosition(pos);
	}

	if (Input::IsKeyDown(VK_D)) // 오른쪽
	{
		FVector2 pos = spine->spineRenderer->GetRelativePosition();
		pos.x += moveSpeed; // x 증가 → 오른쪽 이동
		spine->spineRenderer->SetRelativePosition(pos);
	}

	if (Input::IsKeyPressed(VK_F))
	{
		spine->spineRenderer->SetNextAnimation();
	}
	
	// 스파인 스케일 조절: [ (축소), ] (확대)
	if (Input::IsKeyDown(VK_OEM_4)) // '['
	{
		FVector2 sc = spine->spineRenderer->GetRelativeScale();
		sc.x *= 0.9f; sc.y *= 0.9f;
		sc.x = Math::Clamp(sc.x, 0.1f, 5.0f);
		sc.y = Math::Clamp(sc.y, 0.1f, 5.0f);
		spine->spineRenderer->SetRelativeScale(sc);
	}
	if (Input::IsKeyDown(VK_OEM_6)) // ']'
	{
		FVector2 sc = spine->spineRenderer->GetRelativeScale();
		sc.x *= 1.1f; sc.y *= 1.1f;
		sc.x = Math::Clamp(sc.x, 0.1f, 5.0f);
		sc.y = Math::Clamp(sc.y, 0.1f, 5.0f);
		spine->spineRenderer->SetRelativeScale(sc);
	}
	
}

void DemoScene2::CameraInput()
{
}
