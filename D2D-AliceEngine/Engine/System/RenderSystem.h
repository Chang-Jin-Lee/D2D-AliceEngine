#pragma once
#include "Component/Component.h"
#include <Helpers/StringHelper.h>
#include <Core/Singleton.h>
#include "../System/SystemBase.h"

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
	std::vector<std::vector<WeakObjectPtr<RenderComponent>>> m_renderers;

	void Render();
	ViewRect GetCameraView();
	bool CheckCameraCulling(const WeakObjectPtr<RenderComponent>& renderer, const ViewRect& view);
	static bool RenderSortCompare(const WeakObjectPtr<RenderComponent>& a, const WeakObjectPtr<RenderComponent>& b);
};