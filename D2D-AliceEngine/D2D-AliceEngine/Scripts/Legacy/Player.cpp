#include "Player.h"
#include <Core/Input.h>
#include <Math/Transform.h>
#include <Object/gameObject.h>
#include <Component/TransformComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/InputComponent.h>
#include <Component/RenderComponent.h>
#include <Component/Animator.h>
#include <System/ScriptSystem.h>

#include <Animation/SpriteAnimationTextureLoader.h>
#include <Animation/AnimationController.h>
#include <Animation/AnimatorInstance.h>
#include <Component/Collider.h>
#include <Component/Rigidbody2D.h>
#include "Effect/Prism.h"
#include <Manager/SceneManager.h>
#include <Core/Singleton.h>

#include <Component/BackGroundComponent.h>
#include <Component/SkewTransform.h>

#include <Manager/TimerManager.h>
#include <Scripts/Bike/LaneController.h>

#include <GameManager/BulletManager.h>

#include <Helpers/CoordHelper.h>
#include <Scripts/Bike/BikeMovementScript.h>
#include <Scripts/Weapon/Drone.h>

void Player::Initialize()
{
	__super::Initialize();
	//REGISTER_SCRIPT_METHOD(Awake);
	//REGISTER_SCRIPT_METHOD(OnStart);
	//REGISTER_SCRIPT_METHOD(OnEnd);
	//REGISTER_SCRIPT_METHOD(OnDestroy);
}

void Player::FixedUpdate(const float& deltaSeconds)
{
	__super::FixedUpdate(deltaSeconds);
	// 여기에 FixedUpdate에 대한 로직 작성

}

void Player::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	// 여기에 Update에 대한 로직 작성

	float playerDeltaSeconds = deltaSeconds * playerTimeScale;

	prismTimeCount += deltaSeconds;

	float speed = walkSpeed * playerDeltaSeconds;
	//float speed = 125.0f;
	if (!(Input::IsKeyDown(VK_RIGHT) || Input::IsKeyDown(VK_LEFT) || Input::IsKeyDown(VK_DOWN) || Input::IsKeyDown(VK_UP)))
	{
		animInstance->SetFloat("speed", 0);
	}
	if (Input::IsKeyDown(VK_RIGHT))
	{
		if (bMoveRigidBody)
			m_owner->GetComponent<Rigidbody2D>()->AddForce(speed, 0);
		else
			m_owner->transform()->AddPosition(speed, 0);
		animInstance->SetFlip(true);
		animInstance->SetFloat("speed", speed);
	}
	if (Input::IsKeyDown(VK_LEFT))
	{
		if (bMoveRigidBody)
			m_owner->GetComponent<Rigidbody2D>()->AddForce(-speed, 0);
		else
			m_owner->transform()->AddPosition(-speed, 0);
		animInstance->SetFlip(false);
		animInstance->SetFloat("speed", speed);
	}
	if (Input::IsKeyDown(VK_DOWN))
	{
		if (bMoveRigidBody)
			m_owner->GetComponent<Rigidbody2D>()->AddForce(0, -speed);
		else
			m_owner->GetComponent<SkewTransform>()->zPos -= 300.0f * deltaSeconds;
	}
	if (Input::IsKeyDown(VK_UP))
	{
		if (bMoveRigidBody)
			m_owner->GetComponent<Rigidbody2D>()->AddForce(0, speed);
		else
			m_owner->GetComponent<SkewTransform>()->zPos += 300.0f * deltaSeconds;
	}
	/*if (Input::IsKeyPressed(VK_DOWN)) {
		if(m_owner->GetComponent<LaneController>()) m_owner->GetComponent<LaneController>()->MoveDown();
	}
	if (Input::IsKeyPressed(VK_UP)) {
		if (m_owner->GetComponent<LaneController>()) m_owner->GetComponent<LaneController>()->MoveUp();
	}*/
	// 점프 카운트 리셋: 땅에 닿으면 jumpCount = 0
	if (auto rb = m_owner->GetComponent<Rigidbody2D>())
	{
		bool isGround = (rb->m_eRigidBodyState == Define::ERigidBodyState::Ground ||
			rb->m_eRigidBodyState == Define::ERigidBodyState::OnRigidBody);
		if (isGround && prevGroundState == 0)
		{
			jumpCount = 0; // 땅에 처음 닿았을 때만 리셋
		}
		prevGroundState = isGround ? 1 : 0;
	}


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

	// 마우스 클릭 감지
	if (Input::IsMouseLeftDown())
	{
		FVector2 ownerPos = owner->GetPosition();
		FVector2 worldMousePos = Input::GetMouseWorldPosition(); // 마우스의 실제 월드 좌표
		//FVector2 speed = ;
		if (BikeMovementScript* bikeMovement = owner->GetComponent<BikeMovementScript>())
		{
			speed = bikeMovement->GetCurrSpeed();
		}
		BulletManager::GetInstance().FireBullet(ownerPos, worldMousePos, speed, EDroneType::Player);
	}
}

void Player::LateUpdate(const float& deltaSeconds)
{
	__super::LateUpdate(deltaSeconds);
	// 여기에 LateUpdate에 대한 로직 작성

}

void Player::Awake()
{
}
 
void Player::OnStart()
{
	// 여기에 OnStart에 대한 로직 작성
	m_owner = GetOwner();

	m_owner->transform()->SetPosition(0, 0);
	m_owner->transform()->SetRotation(0);
	m_owner->transform()->SetScale(1.5f, 1.5f);
	m_owner->transform()->SetPivot(0.5f);

	AnimatorController::LoadAnimatorController(L"Zero/Zero_AnimController.json", animController);
	animInstance = m_owner->AddComponent<AnimatorInstance>();
	animInstance->SetAnimatorController(&animController);
	animInstance->LoadSpriteSheet(L"Zero\\Zero_sprites.json");
	animInstance->LoadAnimationClip(L"Zero\\Zero_idle_anim.json");
	animInstance->LoadAnimationClip(L"Zero\\Zero_attack_anim.json");
	animInstance->LoadAnimationClip(L"Zero\\Zero_walk_anim.json");
	animInstance->ApplyClipDurationsToStates();
	//animInstance->SetLooping(true);
	animInstance->Play();
	animInstance->SetLayer(5);
	animInstance->OnStart();

	m_owner->AddComponent<Collider>()->SetBoxSize(FVector2(35, 10));
	if (auto collider = m_owner->GetComponent<Collider>())
	{
		collider->SetLayer(2);
		collider->boxComponent->SetRelativePosition(FVector2(0,-20));
	}
	//m_owner->AddComponent<Rigidbody2D>();
	if (auto rb = m_owner->GetComponent<Rigidbody2D>())
	{
		rb->m_eRigidBodyType = Define::ERigidBodyType::Kinematic;
		rb->gravityScale = 60.0f;
		rb->mass = 20.0f;
		rb->drag = 0.6f;
		//rb->angularDrag = 0;
		bMoveRigidBody = false;
	}

	m_owner->AddComponent<InputComponent>()->SetAction(m_owner->GetHandle(), [this]() { Input(); });

	/*m_background->AddComponent<BackGroundComponent>();
	m_background->transform()->SetPosition(m_owner->transform()->GetPosition().x, Define::SCREEN_HEIGHT);*/

	// 산데비스탄 테스트
	m_owner->AddComponent<Prism>(10, 0.1f);

	// LandController �׽�Ʈ
	m_owner->AddComponent<LaneController>();

	//SceneManager::GetCamera()->SetOwner(m_owner);
	//m_owner->AddChildTransform(&GetCamera()->relativeTransform);
}

void Player::OnEnd()
{
	// 여기에 OnEnd에 대한 로직 작성
	//m_owner->GetComponent<Collider>()->SetBoxSize(FVector2(55, 90));
}

void Player::OnDestroy()
{
}

void Player::Input()
{
	// 여기에 Input에 대한 로직 작성

	if (Input::IsKeyPressed(VK_Q))
	{
		//SceneManager::GetCamera()->SetOwner(m_owner);
		m_owner->RemoveFromParent();
		//GetCamera()->AddChildObject(m_owner);
		m_owner->AddChildTransform(&GetCamera()->relativeTransform);
		GetCamera()->SetRelativeScale(m_owner->GetScaleInv());
		GetCamera()->SetRelativePosition(FVector2(0,0));
	}
	if (Input::IsKeyPressed(VK_E))
	{
		//SceneManager::GetCamera()->ClearOwner();
		//m_owner->RemoveFromParent();
		GetCamera()->RemoveFromParent();
		//m_owner->AddChildTransform(&SceneManager::GetCamera()->relativeTransform);
	}
	if (Input::IsKeyDown(VK_K))
	{
		walkSpeed += 5.0f;
	}
	if (Input::IsKeyDown(VK_L))
	{
		walkSpeed -= 5.0f;
	}
	if (Input::IsKeyPressed(VK_U))
	{
		bMoveRigidBody = !bMoveRigidBody;
	}
	if (Input::IsKeyDown(VK_K))
	{
		walkSpeed += 5.0f;
	}
	if (Input::IsKeyDown(VK_L))
	{
		walkSpeed -= 5.0f;
	}
	if (Input::IsKeyDown(VK_G))
	{
		//m_owner->GetComponent<Rigidbody2D>()->gravityScale += 0.1f;
	}
	if (Input::IsKeyDown(VK_H))
	{
		//m_owner->GetComponent<Rigidbody2D>()->gravityScale -= 0.1f;
	}
	if (Input::IsKeyPressed(VK_U))
	{
		bMoveRigidBody = !bMoveRigidBody;
	}
	if (Input::IsKeyPressed(VK_I))
	{
		//m_owner->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Kinematic;
	}
	if (Input::IsKeyPressed(VK_O))
	{
		//m_owner->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Dynamic;
	}
	if (Input::IsKeyPressed(VK_P))
	{
		//m_owner->GetComponent<Rigidbody2D>()->m_eRigidBodyType = Define::ERigidBodyType::Static;
	}

	if (Input::IsKeyDown(VK_T))
	{
		//m_owner->RemoveComponent<Rigidbody2D>(m_owner->GetComponent<Rigidbody2D>());
	}
	if (Input::IsKeyDown(VK_Y))
	{
		if (m_owner->GetComponent<Rigidbody2D>() == nullptr)
		{
			m_owner->AddComponent<Rigidbody2D>();
			if (auto rb = m_owner->GetComponent<Rigidbody2D>())
			{
				rb->m_eRigidBodyType = Define::ERigidBodyType::Dynamic;
				rb->gravityScale = 60.0f;
				rb->mass = 20.0f;
				rb->drag = 0.6f;
			}
		}
	}
	if (Input::IsKeyDown(VK_8))
	{
		m_owner->RemoveComponent<Collider>(m_owner->GetComponent<Collider>());
	}
	if (Input::IsKeyDown(VK_9))
	{
		if (m_owner->GetComponent<Collider>() == nullptr)
			m_owner->AddComponent<Collider>()->SetBoxSize(FVector2(35, 60));
	}


	if (Input::IsKeyDown(VK_C))
	{
		//m_owner->stateMachine->SetNextState(L"Idle");
	}
	if (Input::IsKeyDown(VK_Z))
	{
		animInstance->SetBool("attack", true);
	}
	else
	{
		animInstance->SetBool("attack", false);
	}
	if (Input::IsKeyPressed(VK_SPACE))
	{
		if (jumpCount < maxJumpCount)
		{
			if(m_owner->GetComponent<Rigidbody2D>())
				m_owner->GetComponent<Rigidbody2D>()->AddForce(0, 120);
			jumpCount++;
		}
		//m_owner->GetComponent<Rigidbody2D>()->velocity.y = 150;
	}

	// 산데비스탄 테스트
	if (Input::IsKeyPressed(VK_G)) {
		if (auto prism = m_owner->GetComponent<Prism>())
		{
			prism->SetActive(!prism->IsActive());
			TimerManager::GetInstance().SetGlobalTimeScale(0.5);
			playerTimeScale = 2.0f;
		}
	}
}

void Player::OnCollisionEnter2D(Collision2D* collider)
{
	std::cout << "[Player] OnCollisionEnter2D 호출됨" << std::endl;
	OutputDebugStringW((L"[Player] OnCollisionEnter2D 호출됨" + std::to_wstring(EnterIndex++) + L"\n").c_str());
}

void Player::OnCollisionStay2D(Collision2D* collider)
{
	std::cout << "[Player] OnCollisionStay2D 호출됨" << std::endl;
	//OutputDebugStringW((L"[Player] OnCollisionStay2D 호출됨" + std::to_wstring(EnterIndex++) + L"\n").c_str());
}

void Player::OnCollisionExit2D(Collision2D* collider)
{
	std::cout << "[Player] OnCollisionExit2D 호출됨" << std::endl;
	OutputDebugStringW((L"[Player] OnCollisionExit2D 호출됨" + std::to_wstring(ExitIndex++) + L"\n").c_str());
}

void Player::OnTriggerEnter2D(Collider* collider)
{
	std::cout << "[Player] OnTriggerEnter2D 호출됨" << std::endl;
	OutputDebugStringW(L"[Player] OnTriggerEnter2D 호출됨\n");
}

void Player::OnTriggerStay2D(Collider* collider)
{
	std::cout << "[Player] OnTriggerStay2D 호출됨" << std::endl;
	OutputDebugStringW(L"[Player] OnTriggerStay2D 호출됨\n");
}

void Player::OnTriggerExit2D(Collider* collider)
{
	std::cout << "[Player] OnTriggerExit2D 호출됨" << std::endl;
	OutputDebugStringW(L"[Player] OnTriggerExit2D 호출됨\n");
}
