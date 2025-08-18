#pragma once
#include <Component/ScriptComponent.h>

class gameObject;
class SpriteRenderer;
class SpineScript;
class TitleWidgetScript : public ScriptComponent
{
public:
	void Initialize() override;
	void Update(const float& deltaSeconds) override;

	virtual void Awake() override;
	virtual void OnStart() override;
	virtual void OnEnd() override;
	virtual void OnDestroy() override;

	gameObject* m_owner;

	gameObject* spineObject;
	gameObject* spineObject2;
	SpineScript* spine;
	SpineScript* spine2;

private:
	//void WidgetClose();

	bool m_spineRender = false;
};
