#pragma once
#include <wincodec.h> // for IWICImagingFactory
#include <WICTextureLoader.h>

#include <Rendering/D3D11/CubismNativeInclude_D3D11.hpp>
#include <Type/CubismBasicType.hpp>
#include <Type/csmVector.hpp>

class TextureManager
{
public:
    struct TextureInfo
    {
        Csm::csmUint32 id;      ///< 텍스쳐 ID
        int width;              ///< 넓이
        int height;             ///< 높이
        std::string fileName;   ///< 파일명
    };

    TextureManager();
    ~TextureManager();

public:
    void ReleaseTexturesInfo();
    TextureManager::TextureInfo* CreateTextureFromPngFile(std::string fileName, bool isPreMult, UINT maxSize);
    void ReleaseTextures();

	// R,G,B, A를 프리멀티플라이된 ARGB로 변환
    virtual inline unsigned int Premultiply(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
    {
        return static_cast<unsigned>(\
            (red * (alpha + 1) >> 8) | \
            ((green * (alpha + 1) >> 8) << 8) | \
            ((blue * (alpha + 1) >> 8) << 16) | \
            (((alpha)) << 24)   \
            );
    }

    TextureManager::TextureInfo* GetTextureInfoByName(std::string& fileName) const;
    TextureManager::TextureInfo* GetTextureInfoById(Csm::csmUint32 textureId) const;


private:
    Csm::csmVector<TextureInfo*> _texturesInfo;

    Csm::csmVector<ID3D11Resource*>             _textures;
    Csm::csmVector<ID3D11ShaderResourceView*>   _textureView;

    Csm::csmUint32   _sequenceId;
};

