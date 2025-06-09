#pragma once
#include "pch.h"

namespace Define
{
	static const int SCREEN_WIDTH = 1024;	// ȭ�� �ʺ�
	static const int SCREEN_HEIGHT = 768;	// ȭ�� ����
	static const int FRAME_RATE = 60;		// ������ ����Ʈ
	static const wchar_t* WINDOW_CLASS_NAME;	// ������ Ŭ���� �̸�
	static const wchar_t* WINDOW_TITLE;		// ������ Ÿ��Ʋ
	static const std::string MODULE_PATH;		// ��� ���
	static const std::string WORKING_PATH;	// �۾� ���丮 ���
	static const bool ENABLE_DEBUG_MODE = true; // ����� ��� Ȱ��ȭ ����

	static const std::wstring BASE_RESOURCE_PATH = L"../Resource/";
};

