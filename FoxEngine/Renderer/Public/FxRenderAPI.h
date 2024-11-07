#pragma once

#include "../../WinSystem/Public/FxWinCore.h"

#include <d3d11.h>
#include <dxgi.h>

class FxRenderAPI
{
public:
	FxRenderAPI() = default;

	FxRenderAPI(const FxRenderAPI&)			   = delete;
	FxRenderAPI& operator=(const FxRenderAPI&) = delete;

	~FxRenderAPI();

	void Init(HWND hWnd);

	void ClearRenderTarget(const float color[4]);
	void EndFrame();

private:
	void CreateDeviceAndSwapChain(HWND hWnd);
	void CreateRenderTarget();

private:
	ID3D11Device*			mDxDevice		 { nullptr };
	ID3D11DeviceContext*	mDxDeviceContext { nullptr };
	IDXGISwapChain*			mDxgiSwapChain	 { nullptr };
	ID3D11RenderTargetView* mRenderTargetView{ nullptr };
};
