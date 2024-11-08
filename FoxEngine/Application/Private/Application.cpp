#include "../Public/Application.h"


#include <optional>
#include <sstream>
#include <iomanip>
#include <math.h>


FxApplication::FxApplication()
	:mWindow({0, 0, 1280, 720}, L"Fox Application")
{
	mTimer.Reset();
}

int FxApplication::Execute()
{
	while (true)
	{
		if (const auto ecode = FxWindow::ProcessMessages(); ecode.has_value())
		{
			return *ecode;
		}
		Tick();
	}
	return 0;
}

void FxApplication::Tick()
{
	//~ Testing Game Timer Class
	const float totalTime = mTimer.TotalTime();
	std::wostringstream oss;
	oss << L"Fox Engine "
		<< L"Time Elapsed: " << std::setprecision(1) << std::fixed << totalTime << "s";
	mWindow.SetTitle(oss.str());

	//~ Testing RenderAPI

	const float c = sin(mTimer.TotalTime()) / 3.0f + 0.32f;
	const float color[]{ std::max(1.f, c + 0.2f), std::max(1.f - c, 0.0f), .41212f, c };

	mWindow.RenderAPI().ClearRenderTarget(color);
	mWindow.RenderAPI().InitDrawTriangle();
	mWindow.RenderAPI().DrawTriangleTest();
	//~ Ticking Game Timer.
	mTimer.Tick();
	mWindow.RenderAPI().PresentFrame();
}

