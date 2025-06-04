#pragma once
#include <CubismFramework.hpp>
#include <Model/CubismUserModel.hpp>
#include <Rendering/D3D11/CubismRenderer_D3D11.hpp>
#include <Math/CubismMatrix44.hpp>

class Live2DModel : public Csm::CubismUserModel
{
public:
    Live2DModel(ID3D11Device* device);
    ~Live2DModel();

    bool LoadAssets(const std::string& dir, const std::string& jsonFile);
    void Update(float deltaTime);
    void Draw(ID3D11DeviceContext* context, Csm::CubismMatrix44& projMatrix);

    /*  
    *  ÇïÆÛÇÔ¼ö
    */
    static Csm::csmByte* LoadFileAsBytes(const char* path, Csm::csmSizeInt* outSize);
    static void ReleaseBytes(Csm::csmByte* data);
    static void SetupModel(Csm::ICubismModelSetting* setting, Live2DModel* model, const std::string& modelHomeDir);
    static HRESULT LoadTextureFromFile(ID3D11Device* device, const wchar_t* path, ID3D11ShaderResourceView** outSRV);

private:
    bool LoadTexture(const std::string& path, int index);
    ID3D11Device* m_device = nullptr;
    std::string m_modelHomeDir;
    Csm::ICubismModelSetting* m_modelSetting = nullptr;
    std::map<std::string, Csm::ACubismMotion*> m_expressions;
};
