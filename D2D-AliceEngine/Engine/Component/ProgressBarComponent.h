#pragma once
#include "SpriteRenderer.h"

enum class EProgressBarType
{
    Linear,  // 막대 바 (오른쪽부터 잘림)
    Radial   // 원형 진행바 (부채꼴로 채움)
};

class ProgressBarComponent : public SpriteRenderer
{
public:
    ProgressBarComponent() = default;
    ~ProgressBarComponent() override = default;

    void Render() override;

    // 0..1 진행도
    void SetProgress(float p) { m_progress = (p < 0.f ? 0.f : (p > 1.f ? 1.f : p)); }
    float GetProgress() const { return m_progress; }

    void SetType(EProgressBarType type) { m_type = type; }
    EProgressBarType GetType() const { return m_type; }

    // 원형 진행 시작각 (도 단위). 기본 -90도(윗쪽부터)
    void SetStartAngleDeg(float deg) { m_startAngleDeg = deg; }
    float GetStartAngleDeg() const { return m_startAngleDeg; }

    // 시계 방향 여부 (기본: 반시계 방향)
    void SetClockwise(bool cw) { m_clockwise = cw; }
    bool IsClockwise() const { return m_clockwise; }

    // UI 편의를 위한 헬퍼: 픽셀 크기/색상 설정
    void SetSize(const FVector2& sizePixels);
    void SetColor(const FColor& color);

private:
    float m_progress = 1.0f;            // 0..1
    EProgressBarType m_type = EProgressBarType::Linear;
    float m_startAngleDeg = -90.0f;     // 위쪽 기준 시작 각
    bool m_clockwise = false;           // 반시계 기본
};
