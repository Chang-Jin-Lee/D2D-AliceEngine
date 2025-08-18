#pragma once
#include <string>

class UObjectBase
{
public:
	std::wstring& GetName();
	void SetName(const std::wstring& value);

	std::wstring& GetUUID();
	void SetUUID(const std::wstring& value);

	std::wstring& GetTag();
	void SetTag(const std::wstring& value);

	std::wstring& GetClassType();
	void SetClassType(const std::wstring& value);

protected:
	std::wstring m_name;
	std::wstring m_uuid;
	std::wstring m_tag;
	std::wstring m_classType;
};

