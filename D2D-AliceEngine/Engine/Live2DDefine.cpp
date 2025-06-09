#include "pch.h"
#include "Live2DDefine.h"

#include <CubismFramework.hpp>

namespace LAppDefine {

    using namespace Csm;

    // 화면
    const csmFloat32 ViewScale = 1.0f;
    const csmFloat32 ViewMaxScale = 2.0f;
    const csmFloat32 ViewMinScale = 0.8f;

    const csmFloat32 ViewLogicalLeft = -1.0f;
    const csmFloat32 ViewLogicalRight = 1.0f;
    const csmFloat32 ViewLogicalBottom = -1.0f;
    const csmFloat32 ViewLogicalTop = -1.0f;

    const csmFloat32 ViewLogicalMaxLeft = -2.0f;
    const csmFloat32 ViewLogicalMaxRight = 2.0f;
    const csmFloat32 ViewLogicalMaxBottom = -2.0f;
    const csmFloat32 ViewLogicalMaxTop = 2.0f;

    // 상대 경로
    const csmChar* ResourcesPath = "../Resource/Live2D/";

    // 모델 뒤에 있는 배경 이미지 파일
    const csmChar* BackImageName = "back_class_normal.png";
    // 톱니바퀴(기어)
    const csmChar* GearImageName = "icon_gear.png";
    // 종료 버튼
    const csmChar* PowerImageName = "close.png";

    // 셰이더 상대 경로
    const csmChar* ShaderPath = "SampleShaders/";
    // 셰이더 본체
    const csmChar* ShaderName = "SpriteEffect.fx";

    // 모델 정의------------------------------------------
    // 외부 정의 파일(json)과 일치
    const csmChar* MotionGroupIdle = "Idle"; // 아이들(대기)
    const csmChar* MotionGroupTapBody = "TapBody"; // 몸체를 터치했을 때

    // 외부 정의 파일(json)과 일치
    const csmChar* HitAreaNameHead = "Head";
    const csmChar* HitAreaNameBody = "Body";

    // 모션의 우선순위 상수
    const csmInt32 PriorityNone = 0;
    const csmInt32 PriorityIdle = 1;
    const csmInt32 PriorityNormal = 2;
    const csmInt32 PriorityForce = 3;

    // MOC3 일관성 검증 옵션
    const csmBool MocConsistencyValidationEnable = true;

    // 디버그용 로그 표시 옵션
    const csmBool DebugLogEnable = true;
    const csmBool DebugTouchLogEnable = false;

    // Framework에서 출력하는 로그 레벨 설정
    const CubismFramework::Option::LogLevel CubismLoggingLevel = CubismFramework::Option::LogLevel_Verbose;

    // 기본 렌더 타겟 크기
    const csmInt32 RenderTargetWidth = 1900;
    const csmInt32 RenderTargetHeight = 1000;

}