#pragma once
#include "pch.h"
#include "Application.h"
#include "D2DRenderer.h"

void Application::Initialize()
{
	m_pD2DRenderer = new D2DRenderer();

	char szPath[MAX_PATH] = { 0, };
	GetModuleFileNameA(NULL, szPath, MAX_PATH); // ���� ����� ���
	m_ModulePath = szPath; // ��� ���
	GetCurrentDirectoryA(MAX_PATH, szPath); // ���� �۾� ���丮
	m_WorkingPath = szPath; // �۾� ���丮
	OutputDebugStringA(std::string(std::string(m_ModulePath) + std::string("\n")).c_str());
	OutputDebugStringA(std::string(std::string(m_WorkingPath) + std::string("\n")).c_str());

	m_hInstance = GetModuleHandle(NULL); // ���� ���μ����� �ڵ�
}

void Application::Run()
{

}

void Application::Uninitialize()
{
	m_pD2DRenderer->Uninitialize();
	m_pD2DRenderer = nullptr;
}