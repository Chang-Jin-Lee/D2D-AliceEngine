#pragma once
#include <Core/Singleton.h>
#include <Math/TMath.h>
#include <Core/ObjectHandler.h>
#include <vector>

class gameObject;
class Bullet;
enum class EDroneType;
class BulletManager : public Singleton<BulletManager>
{ 
public:
	BulletManager();
	virtual ~BulletManager();


	std::vector<WeakObjectPtr<Bullet>> bullets;
	void FireBullet(const FVector2& start, const FVector2& target, const FVector2& initVelocity, const EDroneType& dronType, float damage = 5.0f);
	void FireBulletCatmull(const std::vector<FVector2>& controlPoints, float totalDuration, const EDroneType& droneType, float damage = 5.0f);

	gameObject* GetPlayer() const { return m_player; }
	void SetPlayer(gameObject* player) { m_player = player; }

	gameObject* m_player = nullptr;
};

