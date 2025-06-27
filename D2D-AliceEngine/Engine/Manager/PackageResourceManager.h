#pragma once
#include <Core/Singleton.h>
#include <Core/ObjectHandler.h>
#include <unordered_map>

/*
*  @brief : ��Ű�� ���ҽ� ���� Ŭ����
 * @details : Direct2D ��Ʈ�� ���� �� ����
 */

enum class EResourceSpecifier
{
	Bitmap,
	Max
};

struct FPacakageInfo
{
	EResourceSpecifier resourceType;
	std::wstring name; // ���ҽ� �̸�
	std::wstring fileExtension; // ���� Ȯ����
	std::wstring uuid; // ���� �ĺ���
	bool bIsLoaded = false; // �ε� ����
	FPacakageInfo()
		: resourceType(EResourceSpecifier::Max), name(L""), fileExtension(L""), uuid(L""), bIsLoaded(false)
	{
	}
	FPacakageInfo(const EResourceSpecifier& _resourceType, const std::wstring& name, const std::wstring& extension, const std::wstring& uuid)
		: resourceType(_resourceType), name(name), fileExtension(extension), uuid(uuid), bIsLoaded(false)
	{
	}
};

struct FMemoryInfo
{
	std::wstring VRAMUssage;
	std::wstring DRAMUssage;
	std::wstring PageFile;
	FMemoryInfo() : VRAMUssage(L""), DRAMUssage(L""), PageFile(L"") {}
};

class PackageResourceManager : public Singleton<PackageResourceManager>
{
public:
	PackageResourceManager();
	~PackageResourceManager();

	void Initialize();
	// For ImageDraw
	ComPtr<IWICImagingFactory> m_wicImagingFactory;

	std::weak_ptr<ID2D1Bitmap1> CreateBitmapFromFile(const wchar_t* path);

	std::wstring FormAtBytes(UINT64 bytes);
	FMemoryInfo GetMemoryInfo();

	/*
	* key �����δ� Resource�� ���� ���� ��θ� ����մϴ�.
	*/
	 std::unordered_map<std::wstring, FPacakageInfo> m_preloadedPaths; // �̸� �ε�� ��Ʈ�� ĳ��

	 // key ���� FPacakageInfo�� �ִ� uuid �Դϴ�.
	 std::unordered_map<std::wstring, std::shared_ptr<ID2D1Bitmap1>> m_loadedBitmaps; // �ε�� ��Ʈ�� ĳ��
};