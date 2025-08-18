#pragma once
#include "Component/Component.h"
#include <Helpers/StringHelper.h>
#include <Core/Singleton.h>
#include "../System/SystemBase.h"
#include <Define/Define.h>
#include <tuple>

/* 
*  @briefs : 렌더링을 담당하는 시스템입니다
*/
class RenderComponent;
struct ViewRect { float minX, maxX, minY, maxY; };

class RenderSystem : public Singleton<RenderSystem>
{
public:
	RenderSystem();
	~RenderSystem();

public:
	void Regist(WeakObjectPtr<RenderComponent>&& renderer);
	void UnRegist(WeakObjectPtr<RenderComponent>&& renderer);
	void UnRegistAll();

	void Initialize();
	void UnInitialize();

	// 렌더링 대기열
	std::vector<WeakObjectPtr<RenderComponent>> m_renderers;

	void Render();
	void RenderReady();
	void RenderWorldSpace(); // 새로운 통합 렌더링 함수
	void DebugCamera();
	void RenderD2D();

	ViewRect GetCameraView();
	static bool CheckCameraCulling(const WeakObjectPtr<RenderComponent>& renderer, const ViewRect& view);
	static bool RenderSortCompare(const WeakObjectPtr<RenderComponent>& a, const WeakObjectPtr<RenderComponent>& b);

	void RenderAfterEffect();

	void RenderScreenSpace();

	bool bUseScreenEffect = true;
};
