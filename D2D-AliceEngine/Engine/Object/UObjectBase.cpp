#include "pch.h"
#include "UObjectBase.h"

std::wstring& UObjectBase::GetName()
{
	return m_name;
}

void UObjectBase::SetName(const std::wstring& value)
{
	m_name = value;
}

std::wstring& UObjectBase::GetUUID()
{
	return m_uuid;
}

void UObjectBase::SetUUID(const std::wstring& value)
{
	m_uuid = value;
}

std::wstring& UObjectBase::GetTag()
{
	return m_tag;
}

void UObjectBase::SetTag(const std::wstring& value)
{
	m_tag = value;
}

std::wstring& UObjectBase::GetClassType()
{
	return m_classType;
}

void UObjectBase::SetClassType(const std::wstring& value)
{
	m_classType = value;
}
