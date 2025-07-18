#include "DemoScene.h"
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/VideoComponent.h>
#include <Component/Spine2DRenderComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/TransformComponent.h>
#include <Core/Input.h>
#include <Math/TColor.h>
#include "../Scripts/Sun.h"
#include "../Scripts/Moon.h"
#include "../Scripts/Earth.h"
#include "../Scripts/YuukaVideo.h"
#include "../Scripts/CameraController.h"

/*
*	NewObject<T>(std::wstring&) : 해당 이름의 게임오브젝트를 생성하고 rawPointer를 반환합니다.
*	Initilize(std::wstring&, FVector2&, float&, FVector2&, FVector2&) : 좌표, 회전, 스케일, 피봇을 지정합니다.
*/

void DemoScene::Initialize()
{
	__super::Initialize();
}

void DemoScene::Release()
{
	__super::Release();
}

void DemoScene::Update()
{
	__super::Update();
}

void DemoScene::OnEnter()
{
	__super::OnEnter();

	m_camera = NewObject<gameObject>(L"Camera");
	m_camera->AddComponent<CameraController>();

	m_yuuka = NewObject<gameObject>(L"yuuka");
	m_yuuka->AddComponent<YuukaVideo>();

	m_sun = NewObject<gameObject>(L"Sun");
	m_sun->AddComponent<Sun>();

	m_earth = NewObject<gameObject>(L"Earth");
	m_earth->AddComponent<Earth>();

	m_moon = NewObject<gameObject>(L"Moon");
	m_moon->AddComponent<Moon>();

	m_widget = NewObject<gameObject>(L"widget");
	m_widget->AddComponent<TextRenderComponent>()->SetText(
		L" <카메라> \n"
		L" [w,s] : 카메라 상하 이동 \n"
		L" [a,d] : 카메라 좌우 이동 \n"
		L" [ [ / ] ] : 카메라 축소, 확대\n"
		L" [1/2] : D2D, Unity 좌표계 \n"
		L"\n"
		L" <태양, 지구, 달> \n"
		L" [z,c] : 태양 회전 \n"
		L" [화살표 상,하] : 태양 상하 이동 \n"
		L" [화살표 좌/우] : 태양 좌우 이동 \n"
		L" [4] : 태양 이름 한영 전환 \n"
		L" [b,m] : 지구 회전 \n"
		L" [y,h] : 지구 상하 이동 \n"
		L" [g,j] : 지구 좌우 이동 \n"
		L" [9,0] : 달 회전 \n"
		L" [o,l] : 달 상하 이동 \n"
		L" [k,;] : 달 좌우 이동 \n"
		L"\n"
		L" <배경 실행> \n"
		L" [P] : 배경 실행, 중지 \n"
		L"\n"
		L" <애니메이션> \n"
		L" [5,6] : 아스나, 유우카 전환"
	);

	m_widget2 = NewObject<gameObject>(L"widget2");
	m_widget2->transform()->SetPosition(0, 0);

	m_widget3 = NewObject<gameObject>(L"widget3");
	m_widget3->transform()->SetPosition(0, 0);

	m_sun->AddChildObject(m_earth);
	m_earth->AddChildObject(m_moon);
	/*
	* UI는 gameObject와 별개로 분리해서 만들어야함. 이후에 예정 
	*/
	m_widget->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 80));
	m_widget->GetComponent<TextRenderComponent>()->SetFontSize(18.0f);

	m_widget2->AddComponent<TextRenderComponent>()->SetText(L" <씬> \n [3] : 씬 전환");
	m_widget2->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopRight);
	m_widget2->GetComponent<TextRenderComponent>()->SetPosition(FVector2(850, 0));
	m_widget2->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);

	m_widget3->AddComponent<TextRenderComponent>()->SetText(L" <현재 씬> " + GetName());
	m_widget3->GetComponent<TextRenderComponent>()->SetTextAlignment(ETextFormat::TopLeft);
	m_widget3->GetComponent<TextRenderComponent>()->SetPosition(FVector2(20, 10));
	m_widget3->GetComponent<TextRenderComponent>()->SetFontSize(20.0f);
}

void DemoScene::OnExit()
{
	__super::OnExit();
}
