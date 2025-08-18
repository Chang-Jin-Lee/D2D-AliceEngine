#pragma once
#include <windows.h>
#include <assert.h>
#include <wrl.h>  // ComPtr 사용을 위한 헤더

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d2d1_3.h> //ID2D1Factory8,ID2D1DeviceContext7
#pragma comment(lib, "d2d1.lib")

#include <dxgi1_6.h> // IDXGIFactory7
#pragma comment(lib, "dxgi.lib")

#include <wincodec.h>
#pragma comment(lib,"windowscodecs.lib")
#include <string>

//====================Object====================//
#include <Object/gameObject.h>
//====================Component====================//
#include <Component/InputComponent.h>
#include <Component/TextRenderComponent.h>
#include <Component/BoxComponent.h>
#include <Component/VideoComponent.h>
#include <Component/SpriteRenderer.h>
#include <Component/TransformComponent.h>
#include <Component/RenderComponent.h>
#include <Component/StatComponent.h>
#include <Component/Animator.h>
//====================Manager====================//
#include <Manager/SceneManager.h>
#include <Manager/D2DRenderManager.h>
//====================Math====================//
#include <Math/TColor.h>
#include <Math/Transform.h>
//====================Core====================//
#include <Core/Input.h>
#include <Core/Delegate.h>
#include <Core/StatTraits.h>
//====================System====================//
#include <System/ScriptSystem.h>
//====================Animation====================//
#include <Animation/SpriteAnimationTextureLoader.h>
#include <Animation/AnimationController.h>
#include <FSM/FiniteStateMachine.h>
#include <fsm/FSMState.h>
