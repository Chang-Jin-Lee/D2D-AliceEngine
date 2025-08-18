#include "pch.h"
#include "TextRenderComponent.h"
#include <Manager/D2DRenderManager.h>
#include <Manager/SceneManager.h>
#include <Manager/UpdateTaskManager.h>
#include <Object/Camera.h>
#include <System/RenderSystem.h>
#include <Math/TColor.h>
#include <Math/TMath.h>
#include <Math/Transform.h>
#include <Component/TransformComponent.h>
#include <Helpers/CoordHelper.h>
#include <Helpers/FileHelper.h>
#include <dwrite_3.h>

TextRenderComponent::TextRenderComponent()
{
	m_color = FColor::Black;
	SetDrawType(EDrawType::ScreenSpace);
	m_layer = 101;
}

TextRenderComponent::TextRenderComponent(const std::wstring& content = L"", const FColor& color = FColor::Black, const std::wstring& font = L"Consolas", const float& fontSize = 24.0f)
{
	SetDrawType(EDrawType::ScreenSpace);
	m_content = content;
	m_color = color;
	m_font = font;
	m_fontSize = fontSize;
	InitializeFormat();
	InitializeColor();
	InitializeLayout();
}

TextRenderComponent::~TextRenderComponent()
{
}

void TextRenderComponent::Initialize()
{
	__super::Initialize();

	InitializeFormat();
	InitializeColor();
	InitializeLayout();

	REGISTER_TICK_TASK(Update, Define::ETickingGroup::TG_PostUpdateWork);
}

void TextRenderComponent::Update(const float& deltaSeconds)
{
	__super::Update(deltaSeconds);
}

void TextRenderComponent::Release()
{
	__super::Release();
}

void TextRenderComponent::Render()
{
	ID2D1DeviceContext7* context = D2DRenderManager::GetD2DDevice();
	if (!context || m_content.empty()) return;
	__super::Render();
	
	InitializeLayout();

    // 브러시 투명도 적용
    if (m_pBrush)
        m_pBrush->SetOpacity(m_opacity);

    D2D1_POINT_2F textPosition = D2D1::Point2F(0, 0); // destRect의 중앙 (0,0)을 기준점으로 사용
    context->DrawTextLayout(textPosition, m_layout.Get(), m_pBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

	//FVector2 relativeSize = GetRelativeSize();
	//D2D1_POINT_2F textPosition = D2D1::Point2F(-relativeSize.x / 2, -relativeSize.y / 2);
	//context->DrawTextLayout(textPosition, m_layout.Get(), m_pBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);

}

float TextRenderComponent::GetBitmapSizeX()
{
	InitializeLayout();
	return m_metrics.width;
}

float TextRenderComponent::GetBitmapSizeY()
{
	InitializeLayout();
	return m_metrics.height;
}

FVector2 TextRenderComponent::GetRelativeSize()
{
	FVector2 relativeSize = __super::GetRelativeSize();
	relativeSize.x *= GetBitmapSizeX();
	relativeSize.y *= GetBitmapSizeY();;
	return relativeSize;
}

void TextRenderComponent::InitializeFormat()
{
	IDWriteFactory* m_dWriteFactory = D2DRenderManager::GetInstance().m_dWriteFactory.Get();

	if (m_eTextSource == ETextSource::System)
	{
		m_dWriteFactory->CreateTextFormat(
			m_font.c_str(), // FontName    제어판-모든제어판-항목-글꼴-클릭 으로 글꼴이름 확인가능
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			m_fontSize,
			m_locale.c_str(), //locale
			&m_dWriteTextFormat
		);
	}
    else
    {
        // 커스텀 폰트 파일을 Private Font Collection으로 등록 후 사용
        ComPtr<IDWriteFactory> dwriteFactory = D2DRenderManager::GetInstance().m_dWriteFactory;
        ComPtr<IDWriteFactory3> factory3;
        HRESULT hrQI = dwriteFactory.As(&factory3);
        if (SUCCEEDED(hrQI) && factory3)
        {
            // 경로 변경 시 재등록
			if (!m_privateFontLoaded || m_fontFileAbsolutePath != m_filePath)
			{
				m_fontFileAbsolutePath = m_filePath;
				m_privateFontLoaded = false;

				ComPtr<IDWriteFontSetBuilder> builder; // 기본 인터페이스로 받기
				HRESULT hr = factory3->CreateFontSetBuilder(builder.GetAddressOf());
				assert(SUCCEEDED(hr));

				ComPtr<IDWriteFontSetBuilder1> fontSetBuilder;
				hr = builder.As(&fontSetBuilder);
				assert(SUCCEEDED(hr));

				ComPtr<IDWriteFontFile> fontFile;
				if (SUCCEEDED(dwriteFactory->CreateFontFileReference(m_filePath.c_str(), nullptr, &fontFile)) && fontFile)
				{
					fontSetBuilder->AddFontFile(fontFile.Get());
					ComPtr<IDWriteFontSet> fontSet;
					if (SUCCEEDED(fontSetBuilder->CreateFontSet(&fontSet)))
					{
						ComPtr<IDWriteFontCollection1> collection1;
						if (SUCCEEDED(factory3->CreateFontCollectionFromFontSet(fontSet.Get(), &collection1)))
						{
							// 지정한 패밀리/로케일로 TextFormat 생성
							factory3->CreateTextFormat(
								m_font.c_str(),
								collection1.Get(),
								DWRITE_FONT_WEIGHT_NORMAL,
								DWRITE_FONT_STYLE_NORMAL,
								DWRITE_FONT_STRETCH_NORMAL,
								m_fontSize,
								m_locale.c_str(),
								&m_dWriteTextFormat
							);
							m_privateFontLoaded = (m_dWriteTextFormat != nullptr);
						}
					}
				}
			}

            // 실패 시 시스템 컬렉션으로 폴백
            if (!m_dWriteTextFormat)
            {
                dwriteFactory->CreateTextFormat(
                    m_font.c_str(),
                    nullptr,
                    DWRITE_FONT_WEIGHT_NORMAL,
                    DWRITE_FONT_STYLE_NORMAL,
                    DWRITE_FONT_STRETCH_NORMAL,
                    m_fontSize,
                    m_locale.c_str(),
                    &m_dWriteTextFormat
                );
            }
        }
        else
        {
            // IDWriteFactory3가 없는 구버전 환경: 시스템 컬렉션만 사용 가능
            m_dWriteFactory->CreateTextFormat(
                m_font.c_str(),
                nullptr,
                DWRITE_FONT_WEIGHT_NORMAL,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                m_fontSize,
                m_locale.c_str(),
                &m_dWriteTextFormat
            );
        }
    }
}

void TextRenderComponent::InitializeColor()
{
	ID2D1DeviceContext7* d2dDeviceContext = D2DRenderManager::GetInstance().m_d2dDeviceContext.Get();
	m_pBrush = nullptr;
	d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(
		m_color.r / 255.0f,
		m_color.g / 255.0f,
		m_color.b / 255.0f,
		m_color.a / 255.0f
	), m_pBrush.GetAddressOf());
}

void TextRenderComponent::InitializeLayout()
{
	if (m_metricsDirty) {
		m_layout = nullptr;
		HRESULT hr = D2DRenderManager::GetInstance().m_dWriteFactory->CreateTextLayout(
			m_content.c_str(),
			static_cast<UINT32>(m_content.length()),
			m_dWriteTextFormat.Get(),
			FLT_MAX, FLT_MAX,
			&m_layout
		);
		m_layout->GetMetrics(&m_metrics);
		m_metricsDirty = false;
		if (FAILED(hr)) return;
	}
}

void TextRenderComponent::SetTextAlignment(ETextFormat format)
{
	if (!m_dWriteTextFormat)
		return;

	switch (format)
	{
	case ETextFormat::TopLeft:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;

	case ETextFormat::TopCenter:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;

	case ETextFormat::TopRight:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		break;

	case ETextFormat::MiddleLeft:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;

	case ETextFormat::MiddleCenter:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;

	case ETextFormat::MiddleRight:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		break;

	case ETextFormat::BottomLeft:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	case ETextFormat::BottomCenter:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	case ETextFormat::BottomRight:
		m_dWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
		m_dWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
		break;

	default:
		break;
	}

	InitializeFormat();
	m_metricsDirty = true;
}

void TextRenderComponent::SetText(const std::wstring& content)
{
	m_content = content;
	InitializeFormat();
	m_metricsDirty = true;
}

void TextRenderComponent::SetText(const float& val)
{
	m_content = std::to_wstring(val);
	InitializeFormat();
	m_metricsDirty = true;
}

void TextRenderComponent::SetColor(const FColor& color)
{
	m_color = color;
	InitializeColor();
	m_metricsDirty = true;
}

void TextRenderComponent::SetFontSize(const float& _size)
{
	m_fontSize = _size;
	m_metricsDirty = true;
	InitializeFormat();
}

void TextRenderComponent::SetFont(const std::wstring& _fontName, const std::wstring& _fontLocale)
{
	m_font = _fontName;
	m_locale = _fontLocale;
	InitializeFormat();
}

void TextRenderComponent::SetFontFromFile(const std::wstring& filePath)
{
    m_filePath = FileHelper::ToAbsolutePath(Define::BASE_RESOURCE_PATH + filePath);
    m_eTextSource = ETextSource::File;
    m_privateFontLoaded = false;
    m_fontFileAbsolutePath.clear();
}

void TextRenderComponent::SetIgnoreCameraTransform(bool bIgnore)
{
	bIgnoreCameraTransform = bIgnore;
}

void TextRenderComponent::SetOpacity(float alpha)
{
    // 0..1 clamp
    if (alpha < 0.f) alpha = 0.f;
    if (alpha > 1.f) alpha = 1.f;
    m_opacity = alpha;
}
