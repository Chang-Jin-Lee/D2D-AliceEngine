// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#include <windows.h>
#include <assert.h>
#include <wrl.h>  // ComPtr 사용을 위한 헤더

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d2d1_3.h> //ID2D1Factory8,ID2D1DeviceContext7
#pragma comment(lib, "d2d1.lib")

#include <dxgi1_6.h> // IDXGIFactory7
#pragma comment(lib, "dxgi.lib")

#include <dwrite.h>
#pragma comment(lib,"dwrite.lib")

#include <wincodec.h>
#pragma comment(lib,"windowscodecs.lib")

#include <d2d1effects_2.h>
#pragma comment(lib, "dxguid.lib")
#include <string>
#include <comdef.h>
#include <memory>

#include <fstream>
#include <vector>
#include <map>

#pragma comment(lib, "Live2DCubismCore.lib")
#pragma comment(lib, "Live2DCubismCore_MD.lib")

// Live2D 관련 헤더파일.
#include <Live2D/Framework/src/CubismFramework.hpp>

#endif //PCH_H
