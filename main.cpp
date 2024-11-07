
#include "FoxEngine/WinSystem/Public/FxWindow.h"
#include "FoxEngine/ExceptionHandler/Public/FxException.h"

#include "FoxEngine/Application/Public/Application.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	try
	{
		FxApplication app{};
		return app.Execute();
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
