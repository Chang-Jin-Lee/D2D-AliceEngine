#pragma once
#include <CubismFramework.hpp>
#include <Model/CubismUserModel.hpp>
#include <Rendering/D3D11/CubismRenderer_D3D11.hpp>
#include <Math/CubismMatrix44.hpp>
#include <CubismModelSettingJson.hpp>

class Live2DHelper
{

public:
	static void PrintLog(const char* format, ...);
	static bool ConvertMultiByteToWide(const Csm::csmChar* multiByte, wchar_t* wide, int wideSize);
	static void PrintMessage(const Csm::csmChar* message);
	static Csm::csmByte* LoadFileAsBytes(const std::string filePath, Csm::csmSizeInt* outSize);
	static void ReleaseBytes(Csm::csmByte* byteData);
};

