#pragma once
#include "pch.h"

class FFmpegHelper
{
public:

	static bool CreateDirectoryIfNotExists(const std::wstring& dir) {
		if (CreateDirectoryW(dir.c_str(), NULL) ||
			GetLastError() == ERROR_ALREADY_EXISTS) {
			return true;
		}
		return false;
	}

	static void ClearOutputDirectory(const std::wstring& dir) {
		WIN32_FIND_DATAW findFileData;
		std::wstring searchPath = dir + L"\\*";
		HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findFileData);

		if (hFind == INVALID_HANDLE_VALUE) return;

		do {
			std::wstring fileName = findFileData.cFileName;
			if (fileName == L"." || fileName == L"..") continue;

			std::wstring filePath = dir + L"\\" + fileName;
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// ��������� ���� ���� ���� (�ʿ��ϴٸ�)
				// RemoveDirectoryW(filePath.c_str());
			}
			else {
				DeleteFileW(filePath.c_str());
			}
		} while (FindNextFileW(hFind, &findFileData) != 0);

		FindClose(hFind);
	}

	static std::wstring GetProjectRootPath()
	{
		WCHAR buffer[MAX_PATH] = { 0 };
		GetModuleFileNameW(NULL, buffer, MAX_PATH);
		std::wstring exePath(buffer);
		size_t lastSlash = exePath.find_last_of(L"\\/");
		if (lastSlash != std::wstring::npos)
			return exePath.substr(0, lastSlash); // ���� ������ �ִ� ����
		return L"";
	}

	static std::wstring ToAbsolutePath(const std::wstring& baseDir)
	{
		if (baseDir.empty()) return L"";
		// �̹� �����ζ�� �״�� ��ȯ
		if (baseDir.size() > 1 && (baseDir[1] == L':' || baseDir[0] == L'\\' || baseDir[0] == L'/'))
			return baseDir;

		std::wstring root = GetProjectRootPath();
		if (root.empty()) return baseDir;
		return root + L"\\" + baseDir;
	}

	static std::vector<std::wstring> GetFilesWithPattern(const std::wstring& outputDir, const std::wstring& pattern) {
		std::vector<std::wstring> files;
		std::wstring searchPath = outputDir + L"\\" + pattern;
		WIN32_FIND_DATAW findFileData;
		HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findFileData);

		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					std::wstring filePath = outputDir + L"\\" + findFileData.cFileName;
					files.push_back(filePath);
				}
			} while (FindNextFileW(hFind, &findFileData) != 0);
			FindClose(hFind);
		}
		std::sort(files.begin(), files.end());
		return files;
	}

	static std::vector<std::wstring> GetFramesFromVideo(const std::wstring& baseDir, const int& fps) {
		// ��� ���� ��θ� �迭�� ����
		std::vector<std::wstring> frameFiles;
		WCHAR folderNameBuffer[100] = L"frames";
		WCHAR prefixBuffer[100] = L"frame";

		std::wstring inputPath = ToAbsolutePath(baseDir); // ��: L"BackGround/Yuuka.mp4"

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

		CreateDirectoryIfNotExists(parentDir);
		if (CreateDirectoryIfNotExists(parentDir))
		{
			if (CreateDirectoryIfNotExists(outputDir))
			{
				std::vector<std::wstring> temp = GetFilesWithPattern(outputDir, prefixBuffer + std::wstring(L"_*.png"));
				if (temp.empty())
					ClearOutputDirectory(outputDir);
				else
					return temp;
			}
		}

		std::wstring command = L"ffmpeg -y -i \"" + inputPath + L"\" -vf fps=" + std::to_wstring(fps) +
			L" \"" + outputDir + L"\\" + prefixBuffer + L"_%04d.png\"";

		STARTUPINFOW si = { sizeof(si) };
		PROCESS_INFORMATION pi;

		// ffmpeg ����
		if (!CreateProcessW(nullptr, &command[0], nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi)) {
			assert(L"ffmpeg ���� ����\n\n��ο� ffmpeg.exe�� �ִ��� Ȯ���ϼ���.");
			return std::vector<std::wstring>();
		}
		// ffmpeg ���μ����� ���� ������ ���
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		return GetFilesWithPattern(outputDir, prefixBuffer + std::wstring(L"_*.png"));
	}
};

