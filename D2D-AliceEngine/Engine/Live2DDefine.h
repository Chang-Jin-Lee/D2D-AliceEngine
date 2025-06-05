#pragma once

namespace Live2DDefine 
{
    using namespace Csm;

    extern const csmFloat32 ViewScale;              ///< Ȯ��/��� ����
    extern const csmFloat32 ViewMaxScale;           ///< Ȯ��/��� ������ �ִ밪
    extern const csmFloat32 ViewMinScale;           ///< Ȯ��/��� ������ �ּҰ�

    extern const csmFloat32 ViewLogicalLeft;        ///< ���� �� ��ǥ���� ���� �� ��
    extern const csmFloat32 ViewLogicalRight;       ///< ���� �� ��ǥ���� ���� �� ��
    extern const csmFloat32 ViewLogicalBottom;      ///< ���� �� ��ǥ���� �ϴ� �� ��
    extern const csmFloat32 ViewLogicalTop;         ///< ���� �� ��ǥ���� ��� �� ��

    extern const csmFloat32 ViewLogicalMaxLeft;     ///< ���� �� ��ǥ���� ���� �� �ִ밪
    extern const csmFloat32 ViewLogicalMaxRight;    ///< ���� �� ��ǥ���� ���� �� �ִ밪
    extern const csmFloat32 ViewLogicalMaxBottom;   ///< ���� �� ��ǥ���� �ϴ� �� �ִ밪
    extern const csmFloat32 ViewLogicalMaxTop;      ///< ���� �� ��ǥ���� ��� �� �ִ밪

    extern const csmChar* ResourcesPath;            ///< ���ҽ�(�ڷ�) ���
    extern const csmChar* BackImageName;            ///< ��� �̹��� ����
    extern const csmChar* GearImageName;            ///< ��Ϲ���(���) �̹��� ����
    extern const csmChar* PowerImageName;           ///< ���� ��ư �̹��� ����

    extern const csmChar* ShaderPath;               ///< ���̴� ���
    extern const csmChar* ShaderName;               ///< ���̴� ��ü

    // �� ���� --------------------------------------------
                                                    // �ܺ� ���� ����(json)�� ��ġ��Ų��
    extern const csmChar* MotionGroupIdle;          ///< ���̵�(���) ���¿��� ����� ����� ���
    extern const csmChar* MotionGroupTapBody;       ///< ��ü�� ��ġ���� �� ����� ����� ���

    // �ܺ� ���� ����(json)�� ��ġ��Ų��
    extern const csmChar* HitAreaNameHead;          ///< �浹 ���� [Head] �±�
    extern const csmChar* HitAreaNameBody;          ///< �浹 ���� [Body] �±�

    // ����� �켱���� ���
    extern const csmInt32 PriorityNone;             ///< ��� �켱���� ���: 0
    extern const csmInt32 PriorityIdle;             ///< ��� �켱���� ���: 1
    extern const csmInt32 PriorityNormal;           ///< ��� �켱���� ���: 2
    extern const csmInt32 PriorityForce;            ///< ��� �켱���� ���: 3

    extern const csmBool MocConsistencyValidationEnable; ///< MOC3 �ϰ��� ���� ����� ���/�̻��

    // ����׿� �α� ǥ��
    extern const csmBool DebugLogEnable;            ///< ����׿� �α� ǥ���� ���/�̻��
    extern const csmBool DebugTouchLogEnable;       ///< ��ġ ó�� ����׿� �α� ǥ���� ���/�̻��

    // Framework���� ����ϴ� �α��� ���� ����
    extern const CubismFramework::Option::LogLevel CubismLoggingLevel;

    // �⺻ ���� Ÿ�� ũ��
    extern const csmInt32 RenderTargetWidth;
    extern const csmInt32 RenderTargetHeight;

}
