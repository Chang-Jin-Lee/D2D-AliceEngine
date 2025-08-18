#include "pch.h"
#include "RenderSystem.h"
#include <Component/SpriteRenderer.h>
#include <Component/VideoComponent.h>
#include <Component/BoxComponent.h>
#include <Component/Animator.h>
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
#include <tuple>
#include <Define/Define.h>

RenderSystem::RenderSystem()
{
	m_renderers.clear();
}

RenderSystem::~RenderSystem()
{
	m_renderers.clear();
}

void RenderSystem::Regist(WeakObjectPtr<RenderComponent>&& renderer)
{
	if (!renderer.expired())
	{
		m_renderers.push_back(renderer);
	}
}
void RenderSystem::UnRegist(WeakObjectPtr<RenderComponent>&& renderer)
{
	if (auto ptr = renderer.lock())
	{
		// 기존 렌더러 목록에서 제거
		m_renderers.erase(std::remove_if(m_renderers.begin(), m_renderers.end(),
			[&](const WeakObjectPtr<RenderComponent>& r)
			{
				return r.handle == renderer.handle;
			}), m_renderers.end());
	}
}

void RenderSystem::UnRegistAll()
{
	m_renderers.clear();
}

void RenderSystem::Initialize()
{

}

void RenderSystem::UnInitialize()
{
	UnRegistAll();
	m_renderers.clear();
}

void RenderSystem::Render()
{
	// 만료된 아이템 제거
	m_renderers.erase(std::remove_if(m_renderers.begin(), m_renderers.end(),
		[](const WeakObjectPtr<RenderComponent>& item) { return item.expired(); }), m_renderers.end());

	RenderReady();
	RenderWorldSpace();
	DebugCamera();
	RenderAfterEffect();
	RenderScreenSpace();

	D2DRenderManager::GetInstance().m_dxgiSwapChain->Present(1, 0);
}

void RenderSystem::RenderReady()
{
	ComPtr<ID2D1DeviceContext7> deviceContext = D2DRenderManager::GetInstance().m_d2dDeviceContext;
	if (!deviceContext.Get()) return;
	bool m_resizePending = D2DRenderManager::GetInstance().m_resizePending;
	deviceContext->SetTarget(D2DRenderManager::GetInstance().m_screenBitmap.Get());
	deviceContext->BeginDraw();
	deviceContext->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));
	if (m_resizePending)
	{
		//D2DRenderManager::GetInstance().CreateSwapChainAndD2DTarget();
		m_resizePending = false;
	}
	deviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
}

void RenderSystem::RenderWorldSpace()
{
	ViewRect view = GetCameraView();

	std::vector<WeakObjectPtr<RenderComponent>> worldQueue;
	worldQueue.reserve(m_renderers.size());

	std::copy_if(
		m_renderers.begin(), m_renderers.end(),
		std::back_inserter(worldQueue),
		[&](const WeakObjectPtr<RenderComponent>& item) {
			return !item.expired() && item->GetDrawType() == Define::EDrawType::WorldSpace;
		}
	);

	std::sort(
		worldQueue.begin(), worldQueue.end(),
		[](const WeakObjectPtr<RenderComponent>& a, const WeakObjectPtr<RenderComponent>& b) {
			return RenderSortCompare(a, b);
		}
	);

	std::for_each(
		worldQueue.begin(), worldQueue.end(),
		[view](const WeakObjectPtr<RenderComponent>& item) {
			if (item.expired()) return;

			if (!RenderSystem::CheckCameraCulling(item, view))
			{
				item->Render();
			}
		}
	);
}

void RenderSystem::DebugCamera()
{
	if (!D2DRenderManager::GetInstance().bRenderedBoxRect) return;
	ComPtr<ID2D1DeviceContext7> deviceContext = D2DRenderManager::GetInstance().m_d2dDeviceContext;
	if (!deviceContext.Get()) return;
	if (Camera* camera = SceneManager::GetCamera())
	{
		if (camera->bDebug)
		{
			D2D1::Matrix3x2F flipY = D2D1::Matrix3x2F::Scale(1.0f, -1.0f);
			D2D1::Matrix3x2F screen = D2D1::Matrix3x2F::Translation(Define::SCREEN_WIDTH * 0.5f, Define::SCREEN_HEIGHT * 0.5f);
			D2D1::Matrix3x2F cameraInv = camera->relativeTransform.m_worldTransform.ToMatrix();
			cameraInv.Invert();
			deviceContext->SetTransform(cameraInv * flipY * screen);

			FVector2 pos = SceneManager::GetCamera()->GetRelativePosition();
			D2DRenderManager::GetInstance().DrawDebugBox(pos.x - 10, pos.y - 10, pos.x + 10, pos.y + 10, 0, 0, 255, 255);

			deviceContext->SetTransform(cameraInv * screen);
			D2DRenderManager::GetInstance().DrawDebugText(L"(" + std::to_wstring(pos.x) + L" " + std::to_wstring(pos.y) + L")", pos.x, pos.y, 24, D2D1::ColorF(0, 0, 255, 1));
		}
	}
}

void RenderSystem::RenderD2D()
{
	ViewRect view = GetCameraView();
	for (auto it = m_renderers.begin(); it != m_renderers.end(); )
	{
		if (it->expired())
		{
			it = m_renderers.erase(it);
			continue;
		}
		if (it->lock()->GetDrawType() == Define::EDrawType::WorldSpace && CheckCameraCulling(*it, view))
		{
			++it;
			continue;
		}

		if (auto renderer = it->lock())
		{
			renderer->Render();
		}
		++it;
	}
}

ViewRect RenderSystem::GetCameraView()
{
	const float camX = SceneManager::GetCamera()->GetPositionX();
	const float camY = SceneManager::GetCamera()->GetPositionY();
	const float fov = SceneManager::GetCamera()->fieldOfView;
	//FVector2 scale = SceneManager::GetCamera()->GetScale();
	//if (scale.x == 0.0f) scale.x = 1.0f;
	//if (scale.y == 0.0f) scale.y = 1.0f;
	FVector2 screen = D2DRenderManager::GetInstance().GetApplicationSize();
	//const float halfWidth = (screen.x * 0.5f * fov) * scale.x;
	//const float halfHeight = (screen.y * 0.5f * fov) * scale.y;
  const FVector2 cullScale = SceneManager::GetCamera()->GetCullingScale();
  const float halfWidth = (screen.x * 0.5f * fov) * (cullScale.x > 0.0f ? cullScale.x : 1.0f);
  const float halfHeight = (screen.y * 0.5f * fov) * (cullScale.y > 0.0f ? cullScale.y : 1.0f);
  return ViewRect{ camX - halfWidth, camX + halfWidth, camY - halfHeight, camY + halfHeight };
}

// 컬링이 되는걸 확실하게 보려면 주석처리 된 부분을 사용
bool RenderSystem::CheckCameraCulling(const WeakObjectPtr<RenderComponent>& renderer, const ViewRect& view)
{
	TransformComponent* transform = renderer->GetRelativeTransform();
	const auto pos = transform ? transform->GetPosition() : FVector2{ 0, 0 };
	const auto scale = transform ? transform->GetScale() : FVector2{ 1, 1 };

	/*const float shrinkRatio = 0.5;
	const float halfW = renderer->GetSizeX() * 0.5f * scale.x * shrinkRatio;
	const float halfH = renderer->GetSizeY() * 0.5f * scale.y * shrinkRatio;*/
	const float halfW = renderer->GetBitmapSizeX() * 0.5f * scale.x;
	const float halfH = renderer->GetBitmapSizeY() * 0.5f * scale.y;

	const float left = pos.x - halfW;
	const float right = pos.x + halfW;
	const float bottom = pos.y - halfH;
	const float top = pos.y + halfH;

	// margin 비율 (예: 0.1f = 10% 여유)
	const float marginRatio = 0.05f;
	const float marginX = (view.maxX - view.minX) * marginRatio;
	const float marginY = (view.maxY - view.minY) * 1.2f;
	const float minX = view.minX - marginX;
	const float maxX = view.maxX + marginX;
	const float minY = view.minY - marginY;
	const float maxY = view.maxY + marginY;

	// 카메라 뷰 안에만 보이게 (여유 포함)
	return (right < minX || left > maxX ||
		top < minY || bottom > maxY);
}

bool RenderSystem::RenderSortCompare(const WeakObjectPtr<RenderComponent>& a, const WeakObjectPtr<RenderComponent>& b)
{
	if (a.expired()) return false;
	if (b.expired()) return false;
	return a->GetLayer() < b->GetLayer();
}

void RenderSystem::RenderAfterEffect()
{
	ComPtr<ID2D1DeviceContext7> deviceContext = D2DRenderManager::GetInstance().m_d2dDeviceContext;
	if (!deviceContext.Get()) return;

	if (D2DRenderManager::GetInstance().m_sceneEffect.Get())
	{
		deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
		D2D1_SIZE_F size = D2DRenderManager::GetInstance().m_screenBitmap->GetSize();	//	그릴 크기
		D2D1_RECT_F DestRect{ 0,0,size.width,size.height };
		deviceContext->DrawBitmap(
			D2DRenderManager::GetInstance().m_overlayBitmap.Get(),
			DestRect,           // g_d2dBitmapScene 크기에 맞게 늘림
			1.0f,              // Opacity (0.0 ~ 1.0)
			D2D1_INTERPOLATION_MODE_LINEAR,
			nullptr            // 이미지 원본 영역 전체 사용
		);
	}

	HRESULT hr = deviceContext->EndDraw();
	if (FAILED(hr)) {
		D2DRenderManager::GetInstance().OutputError(hr);
	}

	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	deviceContext->SetTarget(D2DRenderManager::GetInstance().m_bitmapTarget.Get());
	deviceContext->BeginDraw();

	if (D2DRenderManager::GetInstance().m_sceneEffect.Get())
		deviceContext->DrawImage(
			D2DRenderManager::GetInstance().m_sceneEffect.Get(),
			nullptr,
			nullptr,
			D2D1_INTERPOLATION_MODE_LINEAR,
			D2D1_COMPOSITE_MODE_SOURCE_OVER
		);
	else
	{
		deviceContext->DrawBitmap(D2DRenderManager::GetInstance().m_screenBitmap.Get());
	}
	deviceContext->EndDraw();
}

void RenderSystem::RenderScreenSpace()
{
	ViewRect view = GetCameraView();

	std::vector<WeakObjectPtr<RenderComponent>> screenQueue;
	screenQueue.reserve(m_renderers.size());

	std::copy_if(
		m_renderers.begin(), m_renderers.end(),
		std::back_inserter(screenQueue),
		[](const WeakObjectPtr<RenderComponent>& item) {
			return !item.expired() && item->GetDrawType() == Define::EDrawType::ScreenSpace;
		}
	);

	std::sort(
		screenQueue.begin(), screenQueue.end(),
		[](const WeakObjectPtr<RenderComponent>& a, const WeakObjectPtr<RenderComponent>& b) {
			return RenderSortCompare(a, b);
		}
	);

	ComPtr<ID2D1DeviceContext7> deviceContext = D2DRenderManager::GetInstance().m_d2dDeviceContext;
	if (!deviceContext.Get()) return;

	deviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	deviceContext->BeginDraw();

	std::for_each(
		screenQueue.begin(), screenQueue.end(),
		[](const WeakObjectPtr<RenderComponent>& item) {
			if (item.expired()) return;
			item->Render();
		}
	);
	deviceContext->EndDraw();
}
