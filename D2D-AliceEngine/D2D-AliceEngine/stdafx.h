#pragma once
#include <windows.h>
#include <assert.h>
#include <wrl.h>  // ComPtr ����� ���� ���

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d2d1_3.h> //ID2D1Factory8,ID2D1DeviceContext7
#pragma comment(lib, "d2d1.lib")

#include <dxgi1_6.h> // IDXGIFactory7
#pragma comment(lib, "dxgi.lib")

#include <wincodec.h>
#pragma comment(lib,"windowscodecs.lib")
#include <string>

#include "../Engine/Application.h"
#include "../Engine/Object/Object.h"
#include "../Engine/Transform.h"