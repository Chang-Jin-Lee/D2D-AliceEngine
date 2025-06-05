#pragma once
#include "pch.h"
#include "Live2DAllocater.h"
#include "Singleton.h"

class Live2DRenderer
{
protected:
	Live2DRenderer();
	virtual ~Live2DRenderer();

public:
	void Initialize(HWND hwnd);
	std::unique_ptr<Live2DAllocater> m_live2DModel;
	Csm::CubismFramework::Option m_live2DOption;

	const Csm::csmInt32 BackBufferNum = 1; // 백 퍼버 종류
};

