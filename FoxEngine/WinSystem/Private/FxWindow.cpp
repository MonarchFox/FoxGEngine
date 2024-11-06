#include "../Public/FxWindow.h"

//~ WindowClass

FxWindow::_FxWindowClass FxWindow::_FxWindowClass::m_sWindowClass;

FxWindow::FxWindow(RECT windowRect, const wchar_t* windowName)
	: m_sRect(windowRect)
{
	// calculate window size based on desired client region size
	AdjustWindowRect(&m_sRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	// create window & get hWnd
	m_hWnd = CreateWindow(
		_FxWindowClass::GetName(),
		windowName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_sRect.right - m_sRect.left, m_sRect.bottom - m_sRect.top,
		nullptr, nullptr, _FxWindowClass::GetInstance(), this
	);

	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
	
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02;		// mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	RegisterRawInputDevices(&rid, 1, sizeof(rid));
}

FxWindow::~FxWindow()
{
	DestroyWindow(m_hWnd);
}

void FxWindow::SetTitle(const std::wstring_view& title)
{
	SetWindowText(m_hWnd, title.data());
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

LRESULT FxWindow::HandleMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
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

LRESULT FxWindow::HandleMessageThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	FxWindow* const pWnd = reinterpret_cast<FxWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->HandleMessage(hWnd, msg, wParam, lParam);
}

LRESULT FxWindow::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return S_OK;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

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
	wnd.hIcon = nullptr;
	wnd.hIconSm = nullptr;
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