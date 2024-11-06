#include "../Public/FxException.h"
#include <sstream>


FxException::FxException(int line, const char* file) noexcept
	: m_iLine(line), m_szFile(file)
{

}

const char* FxException::what() const noexcept
{
	return WhatBuffer.c_str();
}

void FxException::UpdateInfo() noexcept
{
	std::wostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();

	SetInfo(oss.str());
}

void FxException::SetInfo(std::wstring info) noexcept
{
	std::wstring wideWhatBuffer = info;
	WhatBuffer = std::string(wideWhatBuffer.begin(), wideWhatBuffer.end());
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
	return L"";
}

std::wstring FxException::GetOriginString() const noexcept
{
	return std::wstring();
}

