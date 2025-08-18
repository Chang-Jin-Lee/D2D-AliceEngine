#pragma once
#include "Component/ScriptComponent.h"
#include <unordered_map>

class TileMapComponent;
class EnemySpawner : public ScriptComponent
{
public:
	static EnemySpawner* instance;

	enum EnemyType {
		Bike = 0,	// 적 바이크
		Truck,		// 중립 트럭
		JTruck,		// 점프 트럭
		Car,		// 중립 소형차
		Boss		// 두령 보스
	};

	void Initialize() override;
	void OnStart() override;

	void SpawnEnemy(int _enemyTypeId = 0, FVector2 _position = {0.0f ,0.0f});
	void SetStat(EnemyType type);
};

