#include "pch.h"
#include "TextureManager.h"
#include "../Application.h"
#include "../Live2DRenderer.h"
#include "../Helper/Live2DHelper.h"

TextureManager::TextureManager()
{
    _sequenceId = 0;
}

TextureManager::~TextureManager()
{
    ReleaseTextures();
	ReleaseTexturesInfo();
}

void TextureManager::ReleaseTexturesInfo()
{
    for (Csm::csmUint32 i = 0; i < _texturesInfo.GetSize(); i++)
    {
        delete _texturesInfo[i];
    }

    _texturesInfo.Clear();
}

TextureManager::TextureInfo* TextureManager::CreateTextureFromPngFile(std::string fileName, bool isPreMult, UINT maxSize)
{
    ID3D11Device* device = Application::GetInstance()->m_pD2DRenderer->m_d3dDevice.Get();
    ID3D11DeviceContext* context = Application::GetInstance()->m_pD2DRenderer->m_d3dDeviceContext.Get();

    // wchar�� �ٲٱ�
    const int WCHAR_LENGTH = 512;
    wchar_t wchrStr[WCHAR_LENGTH] = L"";
    Live2DHelper::ConvertMultiByteToWide(fileName.c_str(), wchrStr, sizeof(wchrStr));

    ID3D11Resource* texture = NULL;
    ID3D11ShaderResourceView* textureView = NULL;
    TextureManager::TextureInfo* textureInfo = NULL;

    HRESULT hr = S_OK;

    if (isPreMult)
    {
        hr = DirectX::CreateWICTextureFromFileEx(device,
            NULL,   // NULL�� �Ǹ� MTP = 1 �� ��
            wchrStr, maxSize,
            D3D11_USAGE_DYNAMIC,
            D3D11_BIND_SHADER_RESOURCE,
            D3D11_CPU_ACCESS_WRITE,
            0,
            DirectX::WIC_LOADER_DEFAULT,
            &texture, &textureView);
    }
    else
    {
        hr = DirectX::CreateWICTextureFromFileEx(device,
            context,
            wchrStr, maxSize,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE,
            0,
            0,
            DirectX::WIC_LOADER_DEFAULT,
            &texture, &textureView);
    }

    if (SUCCEEDED(hr))
    {
        do
        {
            Microsoft::WRL::ComPtr<IWICImagingFactory> factoryWic;  ///<
            hr = CoCreateInstance(
                CLSID_WICImagingFactory,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&factoryWic)
            );
            if (FAILED(hr))
            {
                break;
            }

            // decoder���� ���� ���� ����
            Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
            hr = factoryWic->CreateDecoderFromFilename(wchrStr, NULL,
                GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder);
            if (FAILED(hr))
            {
                break;
            }

            // decoder�κ���frame�� ����
            Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
            hr = decoder->GetFrame(0, &frame);
            if (FAILED(hr))
            {
                break;
            }
            UINT texWidth, texHeight;
            hr = frame->GetSize(&texWidth, &texHeight);
            if (FAILED(hr))
            {
                break;
            }

            // �ؽ��� ����
            textureInfo = new TextureManager::TextureInfo();

            if (!textureInfo)
            {
                break;
            }

            // ���� ���� ��� �ڵ尡 �����ߴٸ�
            {
                // ���� ID
                const Csm::csmUint32 addId = _sequenceId + 1;

                // ������ �б� LoadData �̷������� �ٲٱ� �ؾ���
                textureInfo->fileName = fileName;
                textureInfo->width = static_cast<int>(texWidth);
                textureInfo->height = static_cast<int>(texHeight);
                textureInfo->id = addId;

                _sequenceId = addId;

                _texturesInfo.PushBack(textureInfo);
                _textures.PushBack(texture);
                _textureView.PushBack(textureView);


                if (isPreMult)
                {
                    D3D11_MAPPED_SUBRESOURCE subRes;
                    if (SUCCEEDED(context->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes)))
                    {
                        // �ӽ� ����
                        byte* pdd = static_cast<byte*>(malloc(subRes.RowPitch * texHeight));
                        // ���� ���θ� ����
                        frame->CopyPixels(NULL, subRes.RowPitch, subRes.RowPitch * texHeight, pdd);

                        ULONG* pixel32 = static_cast<ULONG*>(subRes.pData);
                        for (unsigned int htLoop = 0; htLoop < texHeight; htLoop++)
                        {
                            unsigned char* pixel4 = reinterpret_cast<unsigned char*>(pdd) + subRes.RowPitch * htLoop;
                            unsigned int* pixel32 =
                                reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(subRes.pData) + subRes.RowPitch * htLoop);

                            for (UINT i = 0; i < subRes.RowPitch; i += 4)
                            {
                                unsigned int val = Premultiply(pixel4[i + 0], pixel4[i + 1], pixel4[i + 2], pixel4[i + 3]);
                                pixel32[(i >> 2)] = val;
                            }
                        }

                        // �ӽð�ü����
                        free(pdd);
                        // ����
                        context->Unmap(texture, 0);
                    }
                }

                return textureInfo;
            }
        } while (0);
    }

    // �������� �� �α� 
   /* if (DebugLogEnable)
    {
        
    }*/
    Live2DHelper::PrintLog("Texture Load Error : %s", fileName.c_str());

    return NULL;
}

void TextureManager::ReleaseTextures()
{
    for (Csm::csmUint32 i = 0; i < _texturesInfo.GetSize(); i++)
    {
        delete _texturesInfo[i];

        if (_textureView[i])
        {
            _textureView[i]->Release();
            _textureView[i] = NULL;
        }
        if (_textures[i])
        {
            _textures[i]->Release();
            _textures[i] = NULL;
        }
    }

    _texturesInfo.Clear();
    _textures.Clear();
    _textureView.Clear();
}

TextureManager::TextureInfo* TextureManager::GetTextureInfoByName(std::string& fileName) const
{
    for (Csm::csmUint32 i = 0; i < _texturesInfo.GetSize(); i++)
    {
        if (_texturesInfo[i]->fileName == fileName)
        {
            return _texturesInfo[i];
        }
    }

    return NULL;
}

TextureManager::TextureInfo* TextureManager::GetTextureInfoById(Csm::csmUint32 textureId) const
{
    for (Csm::csmUint32 i = 0; i < _texturesInfo.GetSize(); i++)
    {
        if (_texturesInfo[i]->id == textureId)
        {
            return _texturesInfo[i];
        }
    }

    return NULL;
}
