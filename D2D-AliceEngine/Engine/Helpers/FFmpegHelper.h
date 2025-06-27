#pragma once
#include "pch.h"
#include "FileHelper.h"
#include <Manager/PackageResourceManager.h>
#include <manager/D2DRenderManager.h>

class FFmpegHelper
{
public:

	static std::vector<std::wstring> GetFramesFromVideo(const std::wstring& baseDir, const int& fps) {
		// ��� ���� ��θ� �迭�� ����
		std::vector<std::wstring> frameFiles;
		WCHAR folderNameBuffer[100] = L"frames";
		WCHAR prefixBuffer[100] = L"frame";
		std::wstring extension = L"jpg";

		std::wstring inputPath = FileHelper::ToAbsolutePath(baseDir);
		//std::wstring inputPath =  L"../" + Define::BASE_RESOURCE_PATH + baseDir; // ��: L"BackGround/Yuuka.mp4"

		// ���ϸ�(Ȯ���� ����) ����
		size_t lastSlash = inputPath.find_last_of(L"/\\");
		size_t lastDot = inputPath.find_last_of(L'.');
		std::wstring fileName = inputPath.substr(
			lastSlash + 1,
			(lastDot != std::wstring::npos ? lastDot : inputPath.length()) - (lastSlash + 1)
		);

		// ���� ���� ��� ����
		std::wstring folderPath = inputPath.substr(0, lastSlash);

		// ���� ��� ���� ��� ����
		std::wstring parentDir = folderPath + L"\\" + fileName;
		std::wstring outputDir = parentDir + L"\\" + folderNameBuffer;
		// ��: "BackGround\\Yuuka\\frames"

		FileHelper::CreateDirectoryIfNotExists(parentDir);
		if (FileHelper::CreateDirectoryIfNotExists(parentDir))
		{
			if (FileHelper::CreateDirectoryIfNotExists(outputDir))
			{
				std::vector<std::wstring> temp = FileHelper::GetFilesWithPattern(outputDir, prefixBuffer + std::wstring(L"_*.png"));
				if (temp.empty())
					FileHelper::ClearOutputDirectory(outputDir);
				else
					return temp;
			}
		}

		int x = 0, y = 0;
		D2DRenderManager::GetInstance().GetApplicationSize(x,y);
		std::wstring ffmpegPath = Define::BASE_EXTENSION_PATH + L"FFmpeg\\ffmpeg.exe";
		std::wstring command = ffmpegPath + L" -y -i \"" + inputPath + L"\" -vf fps=" + std::to_wstring(fps) +
			L",scale=" + std::to_wstring(x) + L":" + std::to_wstring(y) +
			L" -q:v 20 \"" + outputDir + L"\\" + prefixBuffer + L"_%04d." + extension +L"\"";

		STARTUPINFOW si = { sizeof(si) };
		PROCESS_INFORMATION pi;


		// ffmpeg ����
		if (!CreateProcessW(nullptr, &command[0], nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) 
		{
			assert(L"ffmpeg ���� ����\n\n��ο� ffmpeg.exe�� �ִ��� Ȯ���ϼ���.");
			return std::vector<std::wstring>();
		}
		// ffmpeg ���μ����� ���� ������ ���
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		PackageResourceManager::GetInstance().Initialize();

		return FileHelper::GetFilesWithPattern(outputDir, prefixBuffer + std::wstring(L"_*."+ extension));
	}
};

