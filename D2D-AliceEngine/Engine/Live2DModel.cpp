#include "pch.h"
#include "Live2DModel.h"
#include <Live2D/Framework/src/CubismModelSettingJson.hpp>

Live2DModel::Live2DModel(ID3D11Device* device)
    : m_device(device)
{

}

Live2DModel::~Live2DModel()
{
    DeleteRenderer();
}

bool Live2DModel::LoadAssets(const std::string& dir, const std::string& jsonFile)
{
    m_modelHomeDir = dir;

    std::string fullPath = dir + jsonFile;
    Csm::csmSizeInt size;
    Csm::csmByte* buffer = Live2DModel::LoadFileAsBytes(fullPath.c_str(), &size);
    if (!buffer) return false;

    Csm::ICubismModelSetting* setting = new Csm::CubismModelSettingJson(buffer, size);
    SetupModel(setting, this, fullPath);

    CreateRenderer(); // 마스크 버퍼 개수가 필요하면 CreateRenderer(1);
    auto* renderer = GetRenderer<Csm::Rendering::CubismRenderer_D3D11>();
    renderer->Initialize(_model); // _model은 CubismModel*

    delete setting;
    Live2DModel::ReleaseBytes(buffer);

    for (int i = 0; i < setting->GetTextureCount(); ++i)
    {
        std::string texPath = dir + setting->GetTextureFileName(i);
        LoadTexture(texPath, i);
    }

    return true;
}

Csm::csmByte* Live2DModel::LoadFileAsBytes(const char* path, Csm::csmSizeInt* outSize)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return nullptr;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    Csm::csmByte* buffer = static_cast<Csm::csmByte*>(malloc(size));
    if (file.read((char*)buffer, size)) 
    {
        *outSize = static_cast<Csm::csmSizeInt>(size);
        return buffer;
    }

    ::operator delete(buffer);
    return nullptr;
}

void Live2DModel::ReleaseBytes(Csm::csmByte* data)
{
    ::operator delete(data);
}

bool Live2DModel::LoadTexture(const std::string& path, int index)
{
    ID3D11ShaderResourceView* srv = nullptr;
    // TODO: WIC에서 SRV로 변환하는 함수 작성 필요
    std::wstring widePath = std::wstring(path.begin(), path.end());
    if (!Live2DModel::LoadTextureFromFile(m_device, widePath.c_str(), &srv)) return false;

    GetRenderer<Csm::Rendering::CubismRenderer_D3D11>()->BindTexture(index, srv);
    return true;
}

void Live2DModel::Update(float deltaTime)
{
    // 파라미터 애니메이션 등 여기에 추가
    _model->Update();
}

void Live2DModel::Draw(ID3D11DeviceContext* context, Csm::CubismMatrix44& projMatrix)
{
    auto* renderer = GetRenderer<Csm::Rendering::CubismRenderer_D3D11>();
    renderer->SetMvpMatrix(&projMatrix);
    renderer->DrawModel();
}


void Live2DModel::SetupModel(Csm::ICubismModelSetting* setting, Live2DModel* model, const std::string& modelHomeDir)
{
    using namespace Csm;

    model->m_modelSetting = setting;

    // 모델 파일 로드
    if (strcmp(setting->GetModelFileName(), "") != 0)
    {
        std::string path = modelHomeDir + setting->GetModelFileName();
        csmSizeInt size;
        csmByte* buffer = Live2DModel::LoadFileAsBytes(path.c_str(), &size);
        model->LoadModel(buffer, size);
        Live2DModel::ReleaseBytes(buffer);
    }

    // 물리 파일
    if (strcmp(setting->GetPhysicsFileName(), "") != 0)
    {
        std::string path = modelHomeDir + setting->GetPhysicsFileName();
        csmSizeInt size;
        csmByte* buffer = Live2DModel::LoadFileAsBytes(path.c_str(), &size);
        model->LoadPhysics(buffer, size);
        Live2DModel::ReleaseBytes(buffer);
    }

    // 포즈 파일
    if (strcmp(setting->GetPoseFileName(), "") != 0)
    {
        std::string path = modelHomeDir + setting->GetPoseFileName();
        csmSizeInt size;
        csmByte* buffer = Live2DModel::LoadFileAsBytes(path.c_str(), &size);
        model->LoadPose(buffer, size);
        Live2DModel::ReleaseBytes(buffer);
    }

    // 익스프레션
    for (int i = 0; i < setting->GetExpressionCount(); ++i)
    {
        std::string name = setting->GetExpressionName(i);
        std::string path = modelHomeDir + setting->GetExpressionFileName(i);

        csmSizeInt size;
        csmByte* buffer = Live2DModel::LoadFileAsBytes(path.c_str(), &size);
        auto* motion = model->LoadExpression(buffer, size, name.c_str());
        model->m_expressions[name] = motion;
        Live2DModel::ReleaseBytes(buffer);
    }

    // 뷰 행렬
    {
        csmMap<csmString, csmFloat32> layout;
        setting->GetLayoutMap(layout);
        model->GetModelMatrix()->SetupFromLayout(layout);
    }

    model->GetModel()->SaveParameters();
    model->_initialized = true;
}


HRESULT Live2DModel::LoadTextureFromFile(ID3D11Device* device, const wchar_t* path, ID3D11ShaderResourceView** outSRV)
{
    *outSRV = nullptr;

    IWICImagingFactory* wicFactory = nullptr;
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    IWICFormatConverter* converter = nullptr;

    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&wicFactory));
    if (FAILED(hr)) return hr;

    hr = wicFactory->CreateDecoderFromFilename(
        path, nullptr, GENERIC_READ,
        WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) {
        if (converter) converter->Release();
        if (frame) frame->Release();
        if (decoder) decoder->Release();
        if (wicFactory) wicFactory->Release();
        return hr;
    }

    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) {
        if (converter) converter->Release();
        if (frame) frame->Release();
        if (decoder) decoder->Release();
        if (wicFactory) wicFactory->Release();
        return hr;
    }

    hr = wicFactory->CreateFormatConverter(&converter);
    if (FAILED(hr)) {
        if (converter) converter->Release();
        if (frame) frame->Release();
        if (decoder) decoder->Release();
        if (wicFactory) wicFactory->Release();
        return hr;
    }

    hr = converter->Initialize(
        frame, GUID_WICPixelFormat32bppRGBA,
        WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) {
        if (converter) converter->Release();
        if (frame) frame->Release();
        if (decoder) decoder->Release();
        if (wicFactory) wicFactory->Release();
        return hr;
    }

    UINT width = 0, height = 0;
    converter->GetSize(&width, &height);

    std::vector<BYTE> imageData(width * height * 4);
    hr = converter->CopyPixels(nullptr, width * 4, static_cast<UINT>(imageData.size()), imageData.data());
    if (FAILED(hr)) {
        if (converter) converter->Release();
        if (frame) frame->Release();
        if (decoder) decoder->Release();
        if (wicFactory) wicFactory->Release();
        return hr;
    }

    // 텍스처 생성
    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = imageData.data();
    initData.SysMemPitch = width * 4;

    ID3D11Texture2D* tex = nullptr;
    hr = device->CreateTexture2D(&texDesc, &initData, &tex);
   if (FAILED(hr)) {
        if (converter) converter->Release();
        if (frame) frame->Release();
        if (decoder) decoder->Release();
        if (wicFactory) wicFactory->Release();
        return hr;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = texDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;

    hr = device->CreateShaderResourceView(tex, &srvDesc, outSRV);
    tex->Release();

Cleanup:
    if (converter) converter->Release();
    if (frame) frame->Release();
    if (decoder) decoder->Release();
    if (wicFactory) wicFactory->Release();
    return hr;
}