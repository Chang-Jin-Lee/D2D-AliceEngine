#pragma once

class FileHelper
{
	/*
	* 파일 관련입니다. Resource 파일을 복사합니다.
	*/
public:

	static bool CreateDirectoryIfNotExists(const std::wstring& dir);

	static void ClearOutputDirectory(const std::wstring& dir);

	static std::vector<std::wstring> GetFilesWithPattern(const std::wstring& outputDir, const std::wstring& pattern);

	static std::wstring GetProjectRootPath();
	static std::wstring ToAbsolutePath(const std::wstring& baseDir);

	static bool CreateDirectoryRecursive(const std::wstring& path);
	// 파일 복사 함수 (덮어쓰기)
	static bool CopyFileOverwrite(const std::wstring& src, const std::wstring& dst);
	// 디렉토리 재귀 복사 함수
	static bool CopyDirectoryRecursive(const std::wstring& sourceDir, const std::wstring& targetDir);

	static void CopyFilesToBuildPath(const std::wstring& _str);

	// 재귀적으로 파일 경로를 수집하는 함수
	static void CollectFilePathsRecursive(const std::wstring& directory, std::vector<std::wstring>& outFiles);
	static void ResourceFilesInBuildPath(const std::wstring& _str, std::vector<std::wstring>& filesPaths);
	// 파일 이름과 확장자를 추출하는 함수
	// 반환값: {파일이름, 확장자}
	static std::pair<std::wstring, std::wstring> ExtractFileNameAndExtension(const std::wstring& absPath);
	// 해당 경로의 폴더와 이름을 분리하는 함수
	static std::pair<std::wstring, std::wstring> ExtractFileDirectoryAndName(const std::wstring& absPath);
	// 파일 경로에서 해당 폴더만 가져오는 함수 (std::wstring 버전)
	// "\\Resource\\content\\video\\aru.mp4" -> "\\Resource\\content\\video"
	static std::wstring get_folder_path(const std::wstring& filepath);
};

