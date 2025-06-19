#pragma once
class FileHelper
{
	/*
	* ���� �����Դϴ�. Resource ������ �����մϴ�.
	*/
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
		// �̹� �����ζ�� �״�� ��ȯ
		if (baseDir.size() > 1 && (baseDir[1] == L':' || baseDir[0] == L'\\' || baseDir[0] == L'/'))
			return baseDir;

		std::wstring root = GetProjectRootPath();
		if (root.empty()) return baseDir;
		return root + L"\\" + baseDir;
	}

	static bool CreateDirectoryRecursive(const std::wstring& path) {
		if (path.empty()) return false;

		DWORD attr = GetFileAttributesW(path.c_str());
		if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY)) {
			// �̹� ���� ����
			return true;
		}

		size_t pos = path.find_last_of(L"\\/");
		if (pos != std::wstring::npos) {
			std::wstring parent = path.substr(0, pos);
			if (GetFileAttributesW(parent.c_str()) == INVALID_FILE_ATTRIBUTES) {
				if (!CreateDirectoryRecursive(parent))
					return false;
			}
		}

		return CreateDirectoryW(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
	}

	// ���� ���� �Լ� (�����)
	static bool CopyFileOverwrite(const std::wstring& src, const std::wstring& dst) {
		return CopyFileW(src.c_str(), dst.c_str(), FALSE) != 0;
	}

	// ���丮 ��� ���� �Լ�
	static bool CopyDirectoryRecursive(const std::wstring& sourceDir, const std::wstring& targetDir) {
		WIN32_FIND_DATAW findData;
		std::wstring searchPath = sourceDir + L"\\*";
		HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);
		if (hFind == INVALID_HANDLE_VALUE) {
			// �ҽ� ������ ���ų� �������
			return false;
		}

		// Ÿ�� ���� ����
		if (!CreateDirectoryRecursive(targetDir)) {
			FindClose(hFind);
			return false;
		}

		do {
			const std::wstring fileName = findData.cFileName;

			// "." �� ".." ����
			if (fileName == L"." || fileName == L"..") continue;

			std::wstring sourcePath = sourceDir + L"\\" + fileName;
			std::wstring targetPath = targetDir + L"\\" + fileName;

			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				// ���� ���� ��� ����
				if (!CopyDirectoryRecursive(sourcePath, targetPath)) {
					FindClose(hFind);
					return false;
				}
			}
			else {
				// ���� ����
				if (!CopyFileOverwrite(sourcePath, targetPath)) {
					FindClose(hFind);
					return false;
				}
			}
		} while (FindNextFileW(hFind, &findData) != 0);

		FindClose(hFind);
		return true;
	}

	static void CopyResourceToBuildPath()
	{
		std::wstring sourcePath = ToAbsolutePath(L"") + L"..\\..\\Resource";
		std::wstring targetPath = ToAbsolutePath(L"") + L"\\Resource";

		CreateDirectoryIfNotExists(targetPath);

		bool result = CopyDirectoryRecursive(sourcePath, targetPath);
		assert(result && L"Resource ���� ����");
	}
};

