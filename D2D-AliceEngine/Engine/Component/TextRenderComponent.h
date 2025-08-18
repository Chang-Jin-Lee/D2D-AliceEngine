#pragma once
#include "Component/RenderComponent.h"
#include <Math/TMath.h>
#include <Math/TColor.h>
#include <Math/Transform.h>
#include <Define/Define.h>
#include <sstream>

using namespace Define;
class TextRenderComponent : public RenderComponent
{
public:
	TextRenderComponent();
	TextRenderComponent(const std::wstring& content, const FColor& color, const std::wstring& font, const float& fontSize);
	~TextRenderComponent();

	void Initialize() override;
	void Update(const float& deltaSeconds) override;
	void Release() override;
	void Render() override;

	virtual float GetBitmapSizeX() override;
	virtual float GetBitmapSizeY() override;

	virtual FVector2 GetRelativeSize() override;

	ComPtr<IDWriteTextLayout> m_layout;
	ComPtr<IDWriteTextFormat> m_dWriteTextFormat;
	ComPtr<ID2D1SolidColorBrush> m_pBrush;

	void InitializeFormat();
	void InitializeColor();
	void InitializeLayout();

	void SetTextAlignment(ETextFormat format);
	inline std::wstring& GetText() { return m_content; }
	void SetText(const std::wstring& content);
	void SetText(const float& val);
	template<typename... Args>
	void SetTextFormat(const Args&... args)
	{
		std::wostringstream oss;

		// C++14에서는 fold expression이 없으므로 초기화 리스트 트릭 사용
		using expander = int[];
		(void)expander {
			0, (static_cast<void>(oss << args), 0)...
		};

		m_content = oss.str();
		InitializeFormat();
		InitializeLayout();
		m_metricsDirty = true;
	}
	void SetColor(const FColor& color);
	void SetFontSize(const float& _size);
	void SetFont(const std::wstring& _fontName, const std::wstring& _fontLocale);
	void SetFontFromFile(const std::wstring& filePath);
	void SetIgnoreCameraTransform(bool bIgnore);
  // Text opacity 0..1
  void SetOpacity(float alpha);

public:
	ETransformType m_eTransformType = ETransformType::D2D;
	std::wstring m_content = L"";
	FColor m_color;
	std::wstring m_font = L"Consolas";
	std::wstring m_locale = L"";
	float m_fontSize = 24.0f;

	DWRITE_TEXT_METRICS m_metrics{};
	bool m_metricsDirty = true;

	ETextSource m_eTextSource = ETextSource::System;
	std::wstring m_filePath;
    // 커스텀 폰트를 프로세스에 Private로 등록했는지 추적
    std::wstring m_fontFileAbsolutePath;
    bool m_privateFontLoaded = false;
private:
	bool bIgnoreCameraTransform = false;
  float m_opacity = 1.0f;

};

