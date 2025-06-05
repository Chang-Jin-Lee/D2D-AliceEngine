#pragma once

namespace Live2DDefine 
{
    using namespace Csm;

    extern const csmFloat32 ViewScale;              ///< 확대/축소 비율
    extern const csmFloat32 ViewMaxScale;           ///< 확대/축소 비율의 최대값
    extern const csmFloat32 ViewMinScale;           ///< 확대/축소 비율의 최소값

    extern const csmFloat32 ViewLogicalLeft;        ///< 논리적 뷰 좌표계의 좌측 끝 값
    extern const csmFloat32 ViewLogicalRight;       ///< 논리적 뷰 좌표계의 우측 끝 값
    extern const csmFloat32 ViewLogicalBottom;      ///< 논리적 뷰 좌표계의 하단 끝 값
    extern const csmFloat32 ViewLogicalTop;         ///< 논리적 뷰 좌표계의 상단 끝 값

    extern const csmFloat32 ViewLogicalMaxLeft;     ///< 논리적 뷰 좌표계의 좌측 끝 최대값
    extern const csmFloat32 ViewLogicalMaxRight;    ///< 논리적 뷰 좌표계의 우측 끝 최대값
    extern const csmFloat32 ViewLogicalMaxBottom;   ///< 논리적 뷰 좌표계의 하단 끝 최대값
    extern const csmFloat32 ViewLogicalMaxTop;      ///< 논리적 뷰 좌표계의 상단 끝 최대값

    extern const csmChar* ResourcesPath;            ///< 리소스(자료) 경로
    extern const csmChar* BackImageName;            ///< 배경 이미지 파일
    extern const csmChar* GearImageName;            ///< 톱니바퀴(기어) 이미지 파일
    extern const csmChar* PowerImageName;           ///< 종료 버튼 이미지 파일

    extern const csmChar* ShaderPath;               ///< 셰이더 경로
    extern const csmChar* ShaderName;               ///< 셰이더 본체

    // 모델 정의 --------------------------------------------
                                                    // 외부 정의 파일(json)과 일치시킨다
    extern const csmChar* MotionGroupIdle;          ///< 아이들(대기) 상태에서 재생할 모션의 목록
    extern const csmChar* MotionGroupTapBody;       ///< 몸체를 터치했을 때 재생할 모션의 목록

    // 외부 정의 파일(json)과 일치시킨다
    extern const csmChar* HitAreaNameHead;          ///< 충돌 판정 [Head] 태그
    extern const csmChar* HitAreaNameBody;          ///< 충돌 판정 [Body] 태그

    // 모션의 우선순위 상수
    extern const csmInt32 PriorityNone;             ///< 모션 우선순위 상수: 0
    extern const csmInt32 PriorityIdle;             ///< 모션 우선순위 상수: 1
    extern const csmInt32 PriorityNormal;           ///< 모션 우선순위 상수: 2
    extern const csmInt32 PriorityForce;            ///< 모션 우선순위 상수: 3

    extern const csmBool MocConsistencyValidationEnable; ///< MOC3 일관성 검증 기능의 사용/미사용

    // 디버그용 로그 표시
    extern const csmBool DebugLogEnable;            ///< 디버그용 로그 표시의 사용/미사용
    extern const csmBool DebugTouchLogEnable;       ///< 터치 처리 디버그용 로그 표시의 사용/미사용

    // Framework에서 출력하는 로그의 레벨 설정
    extern const CubismFramework::Option::LogLevel CubismLoggingLevel;

    // 기본 렌더 타겟 크기
    extern const csmInt32 RenderTargetWidth;
    extern const csmInt32 RenderTargetHeight;

}
