#include "../Public/FxException.h"
#include <sstream>


FxException::FxException(int line, const char* file) noexcept
	: m_iLine(line)
{
	m_szFile = std::wstring(file, file + strlen(file));
}

const wchar_t* FxException::what_w() noexcept
{
	UpdateInfo();
	
	std::wostringstream oss;
	oss << L"[Line number] " << GetLine() << std::endl
		<< L"[File] " << GetFile() << std::endl
		<< GetOriginString();

	WhatBuffer.append(oss.str());

	return WhatBuffer.c_str();
}

void FxException::UpdateInfo() noexcept
{

}

const wchar_t* FxException::GetType() const noexcept
{
	return L"FxException";
}

int FxException::GetLine() const noexcept
{
	return m_iLine;
}

const std::wstring& FxException::GetFile() const noexcept
{
	return m_szFile;
}

std::wstring FxException::GetOriginString() const noexcept
{
	return std::wstring();
}
