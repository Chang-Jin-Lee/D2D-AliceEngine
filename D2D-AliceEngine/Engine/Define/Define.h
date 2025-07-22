#pragma once
/*
* @brief Define 네임스페이스.
* @details 이 네임스페이스는 게임 엔진의 전역 상수 및 설정을 정의합니다.
*/

namespace Define
{
	static const int SCREEN_WIDTH = 1024;	// 화면 너비
	static const int SCREEN_HEIGHT = 768;	// 화면 높이
	static const int FRAME_RATE = 60;		// 프레임 레이트
	static const std::wstring WINDOW_CLASS_NAME = L"Sun, Earth, Moon";	// 윈도우 클래스 이름
	static const std::wstring WINDOW_TITLE = L"Sun, Earth, Moon Title";		// 윈도우 타이틀
	static const std::wstring MODULE_PATH;		// 모듈 경로
	static const std::wstring WORKING_PATH;	// 작업 디렉토리 경로
	static const bool ENABLE_DEBUG_MODE = true; // 디버그 모드 활성화 여부

	static const std::wstring BASE_RESOURCE_PATH = L"Resource\\";
	static const std::wstring BASE_EXTENSION_PATH = L"Extension\\";
	static const float PI = 3.14159265358979323846264f;
	static const float GRAVITYCONSTANT = 9.8f;
	static const float MIN_OVERLAP_EPSILON = 0.001f; // float 오차 방지용 최소 겹침값

	enum class EDrawType
	{
		ScreenSpace,
		WorldSpace,
		Max
	};

	enum class ETextFormat
	{
		TopLeft,
		TopCenter,
		TopRight,
		MiddleLeft,
		MiddleCenter,
		MiddleRight,
		BottomLeft,
		BottomCenter,
		BottomRight,
		Max
	};
	enum class ETransformType
	{
		D2D,
		Unity,
		Max
	};
	enum class ERenderLayer
	{
		VideoComponent,
		SpriteComponent,
		Animator,
		BoxComponent,
		TextRenderComponent,
		Max
	};

	// Unreal ↔ Unity 주요 Tick/Update 비교
	// TG_PrePhysics       : EarlyUpdate         // (AI/입력, Unity의 EarlyUpdate)
	// TG_StartPhysics     : FixedUpdate 시작 전 // (Unity에 직접 대응 없음, 보통 FixedUpdate 준비)
	// TG_DuringPhysics    : FixedUpdate         // (물리 연산, Unity의 FixedUpdate)
	// TG_EndPhysics       : FixedUpdate 종료 후 // (Unity에 직접 대응 없음, 보통 FixedUpdate 후처리)
	// TG_PostPhysics      : Update/LateUpdate   // (게임로직, Unity의 Update/LateUpdate)
	// TG_PostUpdateWork   : PreRender/Render    // (카메라, 렌더링, Unity의 PreRender/Render)
	// TG_LastDemotable    : (특수 상황용, 언리얼만의 단계)
	// TG_NewlySpawned     : (새 객체 처리용, 언리얼만의 단계)
	enum class ETickingGroup
	{
		TG_PrePhysics,       // Unity: EarlyUpdate (AI/입력)
		TG_StartPhysics,     // Unity: FixedUpdate 직전
		TG_DuringPhysics,    // Unity: FixedUpdate (물리 연산)
		TG_EndPhysics,       // Unity: FixedUpdate 직후
		TG_PostPhysics,      // Unity: Update/LateUpdate (게임로직)
		TG_PostUpdateWork,   // Unity: PreRender/Render (카메라, 렌더링)
		TG_LastDemotable,
		TG_NewlySpawned,
		TG_MAX,
	};

	enum class EScriptGroup
	{
		SG_Awake,
		SG_OnStart,
		SG_OnEnd,
		SG_OnDestroy,
		SG_OnCollisionEnter2D,
		SG_OnCollisionStay2D,
		SG_OnCollisionExit2D,
		SG_OnTriggerEnter2D,
		SG_OnTriggerStay2D,
		SG_OnTriggerExit2D,
		SG_MAX,
	};

	enum class ParameterType { Int, Float, Bool, Trigger };

	enum class EBoxType { RenderDebugBox, ColliderDebugBox, Max };

	enum class ERigidBodyType { Dynamic, Kinematic, Static, Max };

	enum class ERigidBodyState { Space, Ground, OnRigidBody, Max};
};

#define VK_A 0x41  // 'A'
#define VK_B 0x42  // 'B'
#define VK_C 0x43  // 'C'
#define VK_D 0x44  // 'D'
#define VK_E 0x45  // 'E'
#define VK_F 0x46  // 'F'
#define VK_G 0x47  // 'G'
#define VK_H 0x48  // 'H'
#define VK_I 0x49  // 'I'
#define VK_J 0x4A  // 'J'
#define VK_K 0x4B  // 'K'
#define VK_L 0x4C  // 'L'
#define VK_M 0x4D  // 'M'
#define VK_N 0x4E  // 'N'
#define VK_O 0x4F  // 'O'
#define VK_P 0x50  // 'P'
#define VK_Q 0x51  // 'Q'
#define VK_R 0x52  // 'R'
#define VK_S 0x53  // 'S'
#define VK_T 0x54  // 'T'
#define VK_U 0x55  // 'U'
#define VK_V 0x56  // 'V'
#define VK_W 0x57  // 'W'
#define VK_X 0x58  // 'X'
#define VK_Y 0x59  // 'Y'
#define VK_Z 0x5A  // 'Z'
#define VK_0 0x30
#define VK_1 0x31
#define VK_2 0x32
#define VK_3 0x33
#define VK_4 0x34
#define VK_5 0x35
#define VK_6 0x36
#define VK_7 0x37
#define VK_8 0x38
#define VK_9 0x39 

#define VK_OEM_4 0xDB  // [
#define VK_OEM_6 0xDD  // ]