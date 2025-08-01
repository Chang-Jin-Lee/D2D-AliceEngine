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