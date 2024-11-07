
#include "FoxEngine/WinSystem/Public/FxWindow.h"
#include "FoxEngine/ExcepSystem/Public/FxException.h"


int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	try
	{
		RECT winSize{ 0, 0, 800, 300 };
		std::wstring name{ L"Test" };
		FxWindow window(winSize, name.c_str());

		MSG msg{};

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		return S_OK;
	}
	catch (FxException& e)
	{
		MessageBox(nullptr, e.what_w(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		std::string whatMessage = e.what();
		std::wstring wWhatMessage(whatMessage.begin(), whatMessage.end());

		MessageBox(nullptr, wWhatMessage.c_str(), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(nullptr, L"Brother your pc sucks!", L"Unkown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return S_OK;
}
