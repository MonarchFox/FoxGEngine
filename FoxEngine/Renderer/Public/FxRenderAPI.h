#pragma once

#include "../../WinSystem/Public/FxWinCore.h"

#include <d3d11.h>
#include <dxgi.h>
#include <vector>
#include <wrl.h>

class FxRenderAPI
{
public:
	FxRenderAPI() = default;
	~FxRenderAPI() = default;

	FxRenderAPI(const FxRenderAPI&)			   = delete;
	FxRenderAPI& operator=(const FxRenderAPI&) = delete;

	void Init(HWND hWnd);

	void ClearRenderTarget(const float color[4]);
	void PresentFrame();

	//~ Render Functionality
	void InitDrawTriangle();
	void DrawTriangleTest();

private:
	void CreateDeviceAndSwapChain(HWND hWnd);
	void CreateRenderTarget();
	void SetViewPort(HWND hWnd);

private:
	Microsoft::WRL::ComPtr<ID3D11Device>		   mDxDevice		{ nullptr };
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>	   mDxDeviceContext { nullptr };
	Microsoft::WRL::ComPtr<IDXGISwapChain>		   mDxgiSwapChain	{ nullptr };
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView{ nullptr };
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer{ nullptr };
};
