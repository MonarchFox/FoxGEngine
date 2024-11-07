#pragma once

#include <exception>
#include <string>


class FxException : public std::exception
{
public:
	FxException(int line, const char* file) noexcept;
	const wchar_t* what_w() noexcept;

	virtual void UpdateInfo() noexcept;

	
	virtual const wchar_t* GetType() const noexcept;

	int GetLine() const noexcept;
	const std::wstring& GetFile() const noexcept;
	std::wstring GetOriginString() const noexcept;


private:
	int m_iLine;
	std::wstring m_szFile;

protected:
	mutable std::wstring WhatBuffer;
};
