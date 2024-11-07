#pragma once

#include "../../WinSystem/Public/FxWindow.h"
#include "../../WinSystem/Public/FxTimer.h"

#include <memory>


class FxApplication
{
public:
	FxApplication();

	int Execute();
	

private:
	void Tick();

private:
	FxWindow	mWindow;
	FxGameTimer mTimer;
};
