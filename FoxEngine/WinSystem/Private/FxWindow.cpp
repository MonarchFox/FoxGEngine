#include "../Public/FxWindow.h"
#include <sstream>

#include "../../../resource.h"


#pragma region FOX_WINDOW

FxWindow::_FxWindowClass FxWindow::_FxWindowClass::m_sWindowClass;

FxWindow::FxWindow(RECT windowRect, const wchar_t* windowName)
	: msRect(windowRect)
{
	// calculate window size based on desired client region size
	if (!AdjustWindowRect(&msRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))
	{
		throw FXWND_LAST_EXCEPT();
	}

	// create window & get hWnd
	mHwnd = CreateWindow(
		_FxWindowClass::GetName(),
		windowName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		msRect.right - msRect.left, msRect.bottom - msRect.top,
		nullptr, nullptr, _FxWindowClass::GetInstance(), this
	);

	if (mHwnd == nullptr) throw FXWND_LAST_EXCEPT();

	mpRenderAPI = std::make_unique<FxRenderAPI>();
	mpRenderAPI->Init(mHwnd);

	ShowWindow(mHwnd, SW_SHOWDEFAULT);
	
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02;		// mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	RegisterRawInputDevices(&rid, 1, sizeof(rid));
}

FxWindow::~FxWindow()
{
	DestroyWindow(mHwnd);
}

void FxWindow::SetTitle(const std::wstring_view& title)
{
	SetWindowText(mHwnd, title.data());
}

std::optional<int> FxWindow::ProcessMessages() noexcept
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

FxRenderAPI& FxWindow::RenderAPI()
{
	return *mpRenderAPI;
}

LRESULT CALLBACK FxWindow::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		FxWindow* const pWnd = static_cast<FxWindow*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&FxWindow::HandleMessageThunk));
		return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK FxWindow::HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	FxWindow* const pWnd = reinterpret_cast<FxWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
}

LRESULT FxWindow::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_DESTROY: 
	{
		PostQuitMessage(0);
		return S_OK;
	}
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		if (!(lParam & 0x4'00'00'000) || mKeyboard.IsAutoRepeatEnabled())
		{
			mKeyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		return S_OK;
	}
	case WM_SYSKEYUP:
	case WM_KEYUP:
	{
		mKeyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		return S_OK;
	}
	case WM_CHAR:
	{
		mKeyboard.OnChar(static_cast<char>(wParam));
		return S_OK;
	}
	case WM_KILLFOCUS:
	{
		mKeyboard.ClearState();
		return S_OK;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS point = MAKEPOINTS(lParam);
		mMouse.OnLeftMousePressed(point.x, point.y);
		return S_OK;
	}
	case WM_LBUTTONUP:
	{
		const POINTS point = MAKEPOINTS(lParam);
		mMouse.OnLeftMouseRelased(point.x, point.y);
		return S_OK;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS point = MAKEPOINTS(lParam);
		mMouse.OnRightMousePressed(point.x, point.y);
		return S_OK;
	}
	case WM_RBUTTONUP:
	{
		const POINTS point = MAKEPOINTS(lParam);
		mMouse.OnRightMouseRelased(point.x, point.y);
		return S_OK;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS point = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);

		mMouse.OnWheelDelta(point.x, point.y, delta);

		return S_OK;
	}
	case WM_MBUTTONDOWN:
	{
		const POINTS point = MAKEPOINTS(lParam);
		mMouse.OnWheelMousePressed(point.x, point.y);
		return S_OK;
	}
	case WM_MBUTTONUP:
	{
		const POINTS point = MAKEPOINTS(lParam);
		mMouse.OnWheelMouseReleased(point.x, point.y);
		return S_OK;
	}
	case WM_MOUSEMOVE:
	{
		const POINTS point = MAKEPOINTS(lParam);

		RECT wr;
		GetClientRect(hWnd, &wr);

		if (point.x >= wr.left && point.y >= wr.top &&
			point.x < wr.right && point.y < wr.bottom)
		{
			if (!mMouse.IsMouseInside())
			{
				SetCapture(hWnd);
				mMouse.OnMouseEnter();
			}
			mMouse.OnMouseMove(point.x, point.y);
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))
			{
				mMouse.OnMouseMove(point.x, point.y);
			}
			else 
			{
				ReleaseCapture();
				mMouse.OnMouseLeave();
			}
		}
		return S_OK;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

#pragma endregion

#pragma region WINDOW_CLASS

const wchar_t* FxWindow::_FxWindowClass::GetName() noexcept
{
	return m_pWinClassName;
}

HINSTANCE FxWindow::_FxWindowClass::GetInstance() noexcept
{
	return m_sWindowClass.m_hInstance;
}

FxWindow::_FxWindowClass::_FxWindowClass() noexcept
	: m_hInstance(GetModuleHandle(nullptr))
{
	//~ Class Instance

	WNDCLASSEX wnd{};
	wnd.cbClsExtra = 0;
	wnd.cbSize = sizeof(wnd);
	wnd.cbWndExtra = 0;
	wnd.hbrBackground = nullptr;
	wnd.hCursor = nullptr;
	wnd.hIcon = static_cast<HICON>(LoadImage(m_hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wnd.hIconSm = static_cast<HICON>(LoadImage(m_hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0));
	wnd.hInstance = m_hInstance;
	wnd.lpfnWndProc = HandleMessageSetup;
	wnd.lpszClassName = GetName();
	wnd.lpszMenuName = nullptr;
	wnd.style = CS_OWNDC;

	RegisterClassEx(&wnd);
}

FxWindow::_FxWindowClass::~_FxWindowClass()
{
	UnregisterClass(GetName(), GetInstance());
}

#pragma endregion

#pragma region WIN_EXCEPTION
FxWindow::FxWinException::FxWinException(int line, const char* file, HRESULT hr) noexcept
	: m_lHR(hr), FxException(line, file)
{

}

const wchar_t* FxWindow::FxWinException::GetType() const noexcept
{
	return L"FxWindowException";
}

void FxWindow::FxWinException::UpdateInfo() noexcept
{
	std::wostringstream woss;
	woss << GetType() << std::endl
		<< L"[Error Code] " << GetErrorCode() << std::endl
		<< L"[Description] " << GetErrorString() << std::endl
		<< GetOriginString();

	WhatBuffer = woss.str();
}

std::wstring FxWindow::FxWinException::TranslateErrorCode(HRESULT hr) noexcept
{
	wchar_t* pMsgBuffer = nullptr;

	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pMsgBuffer), 0, nullptr);

	if (nMsgLen == 0) return L"UnIdentified Error Code";

	std::wstring errorString = pMsgBuffer;
	LocalFree(pMsgBuffer);

	return errorString;
}

HRESULT FxWindow::FxWinException::GetErrorCode() const noexcept
{
	return m_lHR;
}

std::wstring FxWindow::FxWinException::GetErrorString() const noexcept
{
	return TranslateErrorCode(m_lHR);
}

#pragma endregion
