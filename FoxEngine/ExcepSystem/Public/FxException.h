#pragma once

#include <exception>
#include <string>


class FxException : public std::exception
{
public:
	FxException(int line, const char* file) noexcept;
	const char* what() const noexcept override;

	virtual void UpdateInfo() noexcept;
	void SetInfo(std::wstring info) noexcept;

	
	virtual const wchar_t* GetType() const noexcept;

	int GetLine() const noexcept;
	const std::wstring& GetFile() const noexcept;
	std::wstring GetOriginString() const noexcept;


private:
	int m_iLine;
	std::string m_szFile;

protected:
	mutable std::string WhatBuffer;
};
