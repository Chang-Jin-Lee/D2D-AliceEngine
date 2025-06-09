#pragma once
#include "pch.h"
#include "Live2DAllocater.h"
#include "Singleton.h"
#include "LAppAllocator_Common.hpp"

class LAppView;
class LAppTextureManager;

class Live2DRenderer
{
public:
	Live2DRenderer();
	virtual ~Live2DRenderer();

public:
	void Initialize(HWND hwnd);
	void Render();
	void StartFrame();
	void EndFrame();
	Csm::CubismFramework::Option m_live2DOption;

	LAppTextureManager* m_textureManager;
	LAppView* m_view;

	LAppAllocator_Common m_cubismAllocator;

	const Csm::csmInt32 BackBufferNum = 1; // 백 퍼버 종류
};

