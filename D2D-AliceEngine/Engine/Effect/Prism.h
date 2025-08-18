#pragma once

#include "Component/ScriptComponent.h"
#include "Component/Animator.h"
#include <Manager/TimerManager.h>

class Prism : public ScriptComponent
{
private:
	bool bReady = false;
	bool isActive = false;
	bool isEnabled = false;
	bool isDirty = false;

	int prismCount = 0;
	float interval = 0.0f;
	float ellipsedTime = 0.0f;
	
	//std::deque<ID2D1Bitmap1> bitmaps;
	std::deque<WeakObjectPtr<gameObject>> objects;

	// 애니메이터 관련
	Animator* animator = nullptr;
	std::weak_ptr<ID2D1Bitmap1> bitmap;
	SpriteInfo* spriteInfo = nullptr;

public:
	Prism(int _prismCount = 0, float _interval = 0.0f) : prismCount(_prismCount), interval(_interval) {};
	~Prism();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	void GetSpriteInfo();
	void GetCurrBitmap();
	void MakeEffect();
	void ChangeColor();
	void DeleteLast();
	void ClearObjects();

	void SetActive(bool _val);
	bool IsActive();
	void SetPrismTransform(gameObject* go); // 스프라이트 렌더러를 넣을 게임오브젝트 좌표 설정
	void SetSpriteRenderer(gameObject* go); // 스프라이트 렌더러 설정

	FTimerHandle timer;
};

