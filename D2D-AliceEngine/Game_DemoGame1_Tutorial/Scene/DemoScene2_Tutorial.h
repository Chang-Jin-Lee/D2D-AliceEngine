#pragma once
#include <Scene/Scene.h>

class SpineScript;
class DemoScene2_Tutorial : public Scene
{
public:
	DemoScene2_Tutorial() {}
	~DemoScene2_Tutorial() {}

	void Initialize() override;
	void Release() override;
	void Update() override;

	void OnEnter() override;
	void OnExit() override;

	void aru2Input();
	void aruInput();
	void CameraInput();

	class gameObject* m_camera;
	class gameObject* m_backgroundImage;
	class gameObject* m_aru;
	class gameObject* m_aruClone;
	class gameObject* m_aru2;
	class gameObject* m_alice;
	class gameObject* m_widget;
	class gameObject* m_widget2;
	class gameObject* m_widget3;
	class gameObject* m_widget4;
	class gameObject* m_widget5;
	class gameObject* m_widget6;
	class gameObject* m_widget7;
	class gameObject* spineObject;
	SpineScript* spine;
};

