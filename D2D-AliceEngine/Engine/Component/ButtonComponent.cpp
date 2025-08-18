#include "pch.h"
#include "ButtonComponent.h"
#include <Component/Component.h>
#include <Manager/D2DRenderManager.h>
#include <Manager/PackageResourceManager.h>
#include <Manager/SceneManager.h>
#include <Math/Transform.h>
#include <System/RenderSystem.h>
#include <Component/TransformComponent.h>
#include <Manager/UpdateTaskManager.h>
#include <Core/Input.h>
#include <Helpers/FileHelper.h>
#include <Helpers/CoordHelper.h>
#include <Manager/TimerManager.h>

ButtonComponent::ButtonComponent()
{
	SetDrawType(Define::EDrawType::ScreenSpace);
}

ButtonComponent::~ButtonComponent()
{
	PackageResourceManager::GetInstance().UnloadData(filePath); // 비트맵 언로드
}

void ButtonComponent::Initialize()
{
	__super::Initialize();

	// 마우스 상태 완전 초기화
	m_prevMouseDown = false;
	m_prevInArea = false;
	m_state = Define::EButtonState::Idle;
	
	// Idle 상태의 이미지로 m_bitmap 설정
	auto it = m_bitmaps.find(Define::EButtonState::Idle);
	if (it != m_bitmaps.end() && it->second)
	{
		m_bitmap = it->second;
	}
	
	// 모든 애니메이션 중지
	StopAllAnimations();

	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PostUpdateWork);
}

void ButtonComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
	if (!bActive) return;

	const bool mouseDown = Input::IsMouseLeftDown();
	const bool mouseUp = Input::IsMouseLeftReleased();

	//const FVector2 mousePos = Input::GetMouseWorldPositionInCanvas();
	const FVector2 mousePos = Input::GetMousePosition();
	const FVector2 relativePos(
		relativeTransform.GetPosition().x,
		relativeTransform.GetPosition().y
	);

	const FVector2 uiSize = GetRelativeSize();
	const FVector2 finalUIPos = relativePos + FVector2(-uiSize.x * GetOwnerPivot()->x,
		-uiSize.y * GetOwnerPivot()->y);

	const bool inArea = IsMouseInUIArea(mousePos, finalUIPos, uiSize);

	// 핵심 수정: 씬 전환 후 마우스 상태 검증
	// 마우스가 UI 영역에 들어왔을 때 이전에 클릭된 상태가 아니라면 초기화
	if (inArea && !m_prevInArea)
	{
		// UI 영역에 처음 들어왔을 때 마우스가 클릭된 상태라면 초기화
		if (mouseDown)
		{
			// 마우스가 클릭된 상태로 UI 영역에 들어왔다면 이는 씬 전환 후의 잘못된 상태
			ResetMouseState();
			return;
		}
	}

	// 마우스 상태 검증 및 자동 초기화
	// 마우스가 클릭된 상태로 너무 오래 유지되는 경우 자동 초기화
	static int mouseHoldFrameCount = 0;
	if (mouseDown)
	{
		mouseHoldFrameCount++;
		// 300프레임(약 5초) 이상 마우스가 클릭된 상태로 유지되면 초기화
		if (mouseHoldFrameCount > 300)
		{
			ResetMouseState();
			mouseHoldFrameCount = 0;
			return;
		}
	}
	else
	{
		mouseHoldFrameCount = 0;
	}

	// 비정상적인 상태 감지: 마우스가 UI 영역 밖에 있는데 이전에 클릭된 상태라면 초기화
	if (!inArea && m_prevMouseDown && !mouseDown)
	{
		// 마우스가 UI 영역 밖에서 클릭이 해제되었는데, 이전에 클릭된 상태였다면 초기화
		ResetMouseState();
		return;
	}

	if (inArea)
	{
		// (선택) 진입 이벤트가 필요하면 여기서 HoverEnter 발행:
		// if (!m_prevInArea) SetCurrentState(Define::EButtonState::HoverEnter);

		SetCursor(LoadCursorW(nullptr, IDC_HAND));

		// 핵심 수정: Release 상태 조건을 더 엄격하게 검증
		if (mouseUp && m_prevMouseDown && m_prevInArea)
		{
			// 마우스가 UI 영역 안에서 클릭되었다가 해제된 경우에만 Release
			SetCurrentState(Define::EButtonState::Release);
		}
		else if (mouseDown)
		{
			SetCurrentState(Define::EButtonState::Pressed);
		}
		else
		{
			SetCurrentState(Define::EButtonState::Hover);
		}
	}
	else
	{
		// 직전 프레임엔 안에 있었고 이번 프레임에 나갔다면 HoverLeave 1회 발행
		if (m_prevInArea)
		{
			SetCurrentState(Define::EButtonState::HoverLeave);
		}
		else
		{
			SetCurrentState(Define::EButtonState::Idle);
		}

		SetCursor(LoadCursorW(nullptr, IDC_ARROW));
	}

	m_prevMouseDown = mouseDown;
	m_prevInArea = inArea;
}

void ButtonComponent::Release()
{
	__super::Release();
	stateActionSlots.clear();
}

void ButtonComponent::Render()
{
	if (!GetVisible()) return;
	if (!m_bitmap) return;
	__super::Render();

	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (!context) return;

	FVector2 relativeSize = FVector2(GetBitmapSizeX(), GetBitmapSizeY());
	D2D1_RECT_F destRect = D2D1::RectF(-relativeSize.x / 2, -relativeSize.y / 2, relativeSize.x / 2, relativeSize.y / 2);
	
	// 먼저 글로우 이펙트를 뒤에 그리기 (있는 경우)
	if (m_effect)
	{
		D2D1_POINT_2F glowPos = D2D1::Point2F(-relativeSize.x / 2, -relativeSize.y / 2);
		context->DrawImage(m_effect.Get(), &glowPos);
	}
	
	// 그 다음 기존 버튼 이미지를 위에 그리기
	context->DrawBitmap(m_bitmap.get(), destRect);
}

float ButtonComponent::GetBitmapSizeX()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.width);
}

float ButtonComponent::GetBitmapSizeY()
{
	if (!m_bitmap.get()) return 0;
	D2D1_SIZE_U bmpSize = m_bitmap->GetPixelSize();
	return static_cast<float>(bmpSize.height);
}

FVector2 ButtonComponent::GetRelativeSize()
{
	FVector2 relativeSize = __super::GetRelativeSize();
	relativeSize.x *= GetBitmapSizeX();
	relativeSize.y *= GetBitmapSizeY();;
	return relativeSize;
}

void ButtonComponent::LoadData(Define::EButtonState state, const std::wstring& path)
{
	filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + path); // 파일 이름만 저장
	m_bitmaps[state] = PackageResourceManager::GetInstance().CreateBitmapFromFile(
		(Define::BASE_RESOURCE_PATH + path).c_str());
	
	// 현재 상태의 이미지라면 m_bitmap도 업데이트
	if (state == m_state)
	{
		m_bitmap = m_bitmaps[state];
	}
}

bool ButtonComponent::IsMouseInUIArea(const FVector2& mousePos, const FVector2& uiPos, const FVector2& uiSize)
{
	return (mousePos.x >= uiPos.x &&
			mousePos.x <= uiPos.x + uiSize.x &&
			mousePos.y >= uiPos.y &&
			mousePos.y <= uiPos.y + uiSize.y);
}

void ButtonComponent::ExecuteStateAction(Define::EButtonState state)
{
	auto it = stateActionSlots.find(state);
	if (it != stateActionSlots.end())
	{
		// 부모 객체가 유효한지 확인
		if (!it->second.weakPtr.expired())
		{
			it->second.func(); // 해당 상태의 함수 실행
		}
		else
		{
			// 부모 객체가 파괴되었으면 슬롯 제거
			stateActionSlots.erase(it);
		}
	}
}

void ButtonComponent::SetCurrentState(Define::EButtonState state)
{
	// 이전 상태와 다르면 상태 변경
	if (m_state != state)
	{
		// 이전 상태의 종료 함수 실행 (있는 경우)
		ExecuteStateAction(static_cast<Define::EButtonState>(static_cast<int>(m_state) + 100)); // 종료 상태는 +100으로 가정
		
		// 상태 변경 전 검증
		if (state == Define::EButtonState::Release && !m_prevMouseDown)
		{
			// Release 상태인데 이전에 마우스가 클릭되지 않았다면 무시
			return;
		}
		
		// 추가 검증: Hover 상태로 변경할 때 이전에 클릭된 상태라면 무시
		if (state == Define::EButtonState::Hover && m_prevMouseDown)
		{
			// Hover 상태인데 이전에 마우스가 클릭된 상태라면 무시
			return;
		}
		
		m_state = state;
		
		// 새 상태의 시작 함수 실행
		ExecuteStateAction(state);
		
		// 현재 상태에 맞는 이미지로 m_bitmap 업데이트
		auto it = m_bitmaps.find(state);
		if (it != m_bitmaps.end() && it->second)
		{
			m_bitmap = it->second;
		}
	}
}

void ButtonComponent::StartHoverPulse(float period, float amp)
{
	m_hoverPeriod = (period > 0.f) ? period : 0.8f;
	m_hoverAmp = amp;
	m_hoverT = 0.f;

	// 현재 스케일을 기준 스케일로 저장
	// (엔진 API에 맞춰서 GetScale/SetScale 또는 relativeTransform 사용)
	m_hoverBaseScale = relativeTransform.GetScale();

	auto weakThis = WeakFromThis<ButtonComponent>(); // 없다면 owner를 통해 안전검사
	TimerManager::GetInstance().SetTimerDt(m_hoverTimer, [weakThis](float dt) mutable
	{
		if (auto self = weakThis.lock())
		{
			self->m_hoverT += dt;

			// 0..1 구간 시간 좌표
			const float u = std::fmod(self->m_hoverT, self->m_hoverPeriod) / self->m_hoverPeriod;

			// [-1,1] 모양값
			const float wave = Math::eased_triangle_pulse(u, self->m_hoverSharpness, self->m_hoverEase);

			// 1 ± amp 로 스케일 변환
			const float s = 1.0f + self->m_hoverAmp * wave;

			const FVector2 newScale(self->m_hoverBaseScale.x * s,
				self->m_hoverBaseScale.y * s);

			self->relativeTransform.SetScale(newScale);
		}
	});
}

void ButtonComponent::StopHoverPulse()
{
	TimerManager::GetInstance().ClearTimer(m_hoverTimer);
	// 기준 스케일로 복원
	relativeTransform.SetScale(m_hoverBaseScale);
}

void ButtonComponent::CreateGlowEffect(float intensity, FColor color)
{
	if (intensity <= 0.0f)
	{
		m_effect = nullptr;
		return;
	}

	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (!context || !m_bitmap) return;

	try
	{
		// 그림자 이펙트 생성 (글로우 효과)
		ComPtr<ID2D1Effect> shadowEffect;
		HRESULT hr = context->CreateEffect(CLSID_D2D1Shadow, &shadowEffect);
		
		if (SUCCEEDED(hr) && shadowEffect)
		{
			// 그림자 색상 설정 (FColor를 0~1 범위로 변환)
			D2D1_VECTOR_4F shadowColor = { 
				color.r / 255.0f, 
				color.g / 255.0f, 
				color.b / 255.0f, 
				intensity * 1.5f 
			};
			shadowEffect->SetValue(D2D1_SHADOW_PROP_COLOR, shadowColor);
			
			// 블러 반지름 설정 (더 큰 글로우 크기)
			shadowEffect->SetValue(D2D1_SHADOW_PROP_BLUR_STANDARD_DEVIATION, 20.0f);

			// 입력 이미지 설정
			shadowEffect->SetInput(0, m_bitmap.get());
			
			m_effect = shadowEffect;
		}
		else
		{
			// 이펙트 생성 실패 시 이펙트 제거
			m_effect = nullptr;
		}
	}
	catch (...)
	{
		// 예외 발생 시 이펙트 제거
		m_effect = nullptr;
	}
}

void ButtonComponent::StartEffectAnimation(float duration, float targetIntensity, FColor glowColor)
{
	m_effectDuration = duration;
	m_effectStartIntensity = m_currentEffectIntensity;
	m_effectTargetIntensity = targetIntensity;
	m_effectColor = glowColor;
	m_effectT = 0.f;

	// 글로우 이펙트 생성
	CreateGlowEffect(m_currentEffectIntensity, m_effectColor);

	auto weakThis = WeakFromThis<ButtonComponent>();
	TimerManager::GetInstance().SetTimerDt(m_effectTimer, [weakThis](float dt) mutable
	{
		if (auto self = weakThis.lock())
		{
			self->m_effectT += dt;
			
			// 0..1 구간의 진행률
			float progress = Math::Clamp(self->m_effectT / self->m_effectDuration, 0.f, 1.f);
			
			// 부드러운 보간 (easeInOut)
			float easedProgress = progress * progress * (3.0f - 2.0f * progress);
			
			// 현재 강도 계산
			self->m_currentEffectIntensity = Math::Lerp(
				self->m_effectStartIntensity, 
				self->m_effectTargetIntensity, 
				easedProgress
			);
			
			// 이펙트 업데이트 (색상 포함)
			self->CreateGlowEffect(self->m_currentEffectIntensity, self->m_effectColor);
			
			// 애니메이션 완료 시 타이머 정리
			if (progress >= 1.0f)
			{
				TimerManager::GetInstance().ClearTimer(self->m_effectTimer);
			}
		}
	});
}

void ButtonComponent::StopEffectAnimation()
{
	TimerManager::GetInstance().ClearTimer(m_effectTimer);
	m_effect = nullptr;
	m_currentEffectIntensity = 0.f;
}

void ButtonComponent::StopAllAnimations()
{
	// 모든 타이머 중지
	TimerManager::GetInstance().ClearTimer(m_hoverTimer);
	TimerManager::GetInstance().ClearTimer(m_effectTimer);
	
	// 이펙트 제거
	m_effect = nullptr;
	m_currentEffectIntensity = 0.f;
	
	// 스케일 원상복구
	if (owner && owner->transform())
	{
		relativeTransform.SetScale(m_hoverBaseScale);
	}
}

void ButtonComponent::ResetMouseState()
{
	// 마우스 상태 완전 초기화
	m_prevMouseDown = false;
	m_prevInArea = false;
	
	// 현재 상태를 Idle로 설정 (이미지도 Idle 상태로)
	m_state = Define::EButtonState::Idle;
	
	// Idle 상태의 이미지로 m_bitmap 업데이트
	auto it = m_bitmaps.find(Define::EButtonState::Idle);
	if (it != m_bitmaps.end() && it->second)
	{
		m_bitmap = it->second;
	}
	
	// 모든 애니메이션 중지
	StopAllAnimations();
	
	// 커서를 기본 상태로 복원
	SetCursor(LoadCursorW(nullptr, IDC_ARROW));
}
