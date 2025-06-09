#include "pch.h"
#include "Live2DDefine.h"

#include <CubismFramework.hpp>

namespace LAppDefine {

    using namespace Csm;

    // ȭ��
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

    // ��� ���
    const csmChar* ResourcesPath = "../Resource/Live2D/";

    // �� �ڿ� �ִ� ��� �̹��� ����
    const csmChar* BackImageName = "back_class_normal.png";
    // ��Ϲ���(���)
    const csmChar* GearImageName = "icon_gear.png";
    // ���� ��ư
    const csmChar* PowerImageName = "close.png";

    // ���̴� ��� ���
    const csmChar* ShaderPath = "SampleShaders/";
    // ���̴� ��ü
    const csmChar* ShaderName = "SpriteEffect.fx";

    // �� ����------------------------------------------
    // �ܺ� ���� ����(json)�� ��ġ
    const csmChar* MotionGroupIdle = "Idle"; // ���̵�(���)
    const csmChar* MotionGroupTapBody = "TapBody"; // ��ü�� ��ġ���� ��

    // �ܺ� ���� ����(json)�� ��ġ
    const csmChar* HitAreaNameHead = "Head";
    const csmChar* HitAreaNameBody = "Body";

    // ����� �켱���� ���
    const csmInt32 PriorityNone = 0;
    const csmInt32 PriorityIdle = 1;
    const csmInt32 PriorityNormal = 2;
    const csmInt32 PriorityForce = 3;

    // MOC3 �ϰ��� ���� �ɼ�
    const csmBool MocConsistencyValidationEnable = true;

    // ����׿� �α� ǥ�� �ɼ�
    const csmBool DebugLogEnable = true;
    const csmBool DebugTouchLogEnable = false;

    // Framework���� ����ϴ� �α� ���� ����
    const CubismFramework::Option::LogLevel CubismLoggingLevel = CubismFramework::Option::LogLevel_Verbose;

    // �⺻ ���� Ÿ�� ũ��
    const csmInt32 RenderTargetWidth = 1900;
    const csmInt32 RenderTargetHeight = 1000;

}