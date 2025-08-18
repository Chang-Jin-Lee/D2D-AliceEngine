#pragma once
#include "Component/ScriptComponent.h"
#include "Core/Delegate.h"
#include <unordered_map>

// 적 스탯 구조체는 EnemyDataManager에서 정의/관리
struct FEnemyStats;

class EnemyManager : public ScriptComponent
{
public:
	void Initialize() override;
	void OnStart() override;

	void Update(const float& deltaSeconds) override;

public:
	float moveSpeed = 50.0f;
	float isMoving = true;

	void Move(const float& dt);
	void MoveUp();
	void MoveDown();
	void Die();

	void AddDieAction(ObjectHandle _handle, std::function<void()> _func);
	void RemoveDieAction(ObjectHandle _handle);
	void ClearDieAction();

    // 이동/행동 관련만 담당. 데이터는 EnemyDataManager를 통해 관리
private:
	MultiDelegate<> dieActions;

    // 전역 캐시
    static bool s_statsLoaded;
    static std::unordered_map<int, FEnemyStats> s_typeToStats;
};

