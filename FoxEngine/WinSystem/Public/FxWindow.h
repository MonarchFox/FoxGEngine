#pragma once

#include "../FxWinCore.h"
#include "../../ExcepSystem/Public/FxException.h"

#include <string>
#include <optional>
#include <memory>

class FxWindow
{
public:

	FxWindow(RECT windowRect, const wchar_t* windowName);
	~FxWindow();

	FxWindow(const FxWindow&)			 = delete;
	FxWindow& operator=(const FxWindow&) = delete;

	void SetTitle(const std::wstring_view& title);

	static std::optional<int> ProcessMessages() noexcept;

private:
	
	static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;		// ykyk
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

private:
	RECT m_sRect;
	HWND m_hWnd;

public:
	//~ Exception Class
	class FxWinException : public FxException
	{
	public:
		FxWinException(int line, const char* file, HRESULT hr) noexcept;

		void UpdateInfo() noexcept override;
		const wchar_t* GetType() const noexcept override;

		static std::wstring TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT				GetErrorCode()   const noexcept;
		std::wstring		GetErrorString() const noexcept;

	private:
		HRESULT m_lHR;
	};

private:
	//~ Window Registration Class
	class _FxWindowClass
	{
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		_FxWindowClass() noexcept;
		~_FxWindowClass();

		_FxWindowClass(const _FxWindowClass&)			   = delete;
		_FxWindowClass& operator = (const _FxWindowClass&) = delete;

		static constexpr const wchar_t* m_pWinClassName{ L"Fox Engine" };
		static _FxWindowClass		 m_sWindowClass;
		HINSTANCE					 m_hInstance;
	};
};


#define FXWND_EXCEPT(hr) FxWindow::FxWinException(__LINE__, __FILE__, hr);
