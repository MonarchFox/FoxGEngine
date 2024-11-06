#include "FoxEngine/WinSystem/Public/FxWindow.h"


int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
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
