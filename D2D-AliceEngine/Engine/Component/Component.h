#pragma once

#include <string>
#include <memory>
#include <Math/TMath.h>
#include <Core/Tickable.h>
#include <Object/UObject.h>

class gameObject;
class Transform;
class Component : public UObject, public ITickable
{
public:
	Component();
	Component(const std::wstring& name);
	~Component();
public:
	virtual void Initialize() override {}
	virtual void Update() override {}
	virtual void Update(const float& deltaSeconds) override;
	virtual void Release() override {}

	// Render가 필요없는 Component도 있음
	virtual void Render() {};

	virtual void Awake() {};
	virtual void OnStart() {}
	virtual void OnEnd() {}
	virtual void OnCreate() {}
	virtual void OnDestroy() {}

public:
	inline void SetOwner(WeakObjectPtr<gameObject> object) { owner = object; }
	inline gameObject* GetOwner() { return owner.Get(); }
	const std::wstring& GetOwnerName();
	Transform* GetTransform() const;
	FVector2* GetPivot() const;

protected:
	WeakObjectPtr<gameObject> owner;
};