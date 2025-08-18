#pragma once
#include <Component/ScriptComponent.h>
#include <vector>

struct LoopingBackGround
{
    std::vector<gameObject*> tiles; // 연속 타일들
    float speed = 1.0f; // 플레이어 속도에 곱해질 패럴랙스 계수
    float width = 0.0f; // 타일 폭(픽셀)
    float x = 0.0f;     // 스크롤 오프셋
    float y = 0.0f;     // 고정된 Y 위치 (y=0을 중심으로 ±)
    int   baseLayer = 0; // 원본 레이어(상대 레이어 적용 전)
};

class gameObject;
class BackGroundRender : public ScriptComponent
{
public:
	void Initialize() override;

	void FixedUpdate(const float& deltaSeconds);
	void Update(const float& deltaSeconds) override;
	void LateUpdate(const float& deltaSeconds);

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	gameObject* m_owner;

	// 이 값을 조절하여 전체 배경 레이어를 상대적으로 이동시킬 수 있습니다.
	int m_backgroundRelativeLayer = -3000;
	int m_bottomRelativeLayer = 45910;
	int m_topRelativeLayer = -500;
	//int m_topRelativeLayer = 5910;

	// background
	gameObject* m_sky;	// 밤하늘 배경(남산타워)
	gameObject* m_building;
	//gameObject* m_bridge;
	//gameObject* m_market;
	// 
	//gameObject* m_backBarrier;
	//gameObject* m_frontBarrier;
	//gameObject* m_guardrail;

	std::vector<LoopingBackGround> m_loopingLayers;

private:
	void AddLooping(const std::wstring& name,
		const std::wstring& path,
		int layer,
		float y,
		float speed);
};
