#include "../Public/FxRenderAPI.h"

#include <stdexcept>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")


FxRenderAPI::~FxRenderAPI()
{
	if (mDxDevice) mDxDevice->Release();
	if (mDxDeviceContext) mDxDeviceContext->Release();
	if (mDxgiSwapChain) mDxgiSwapChain->Release();
	if (mRenderTargetView) mRenderTargetView->Release();
}

void FxRenderAPI::Init(HWND hWnd)
{
	CreateDeviceAndSwapChain(hWnd);
	CreateRenderTarget();
}

void FxRenderAPI::ClearRenderTarget(const float color[4])
{
	mDxDeviceContext->ClearRenderTargetView(mRenderTargetView, color);
}

void FxRenderAPI::EndFrame()
{
	mDxgiSwapChain->Present(1u, 0u);
}

void FxRenderAPI::CreateDeviceAndSwapChain(HWND hWnd)
{
	RECT wr; GetClientRect(hWnd, &wr);

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = wr.right - wr.left;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.Height = wr.bottom - wr.top;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.Windowed = TRUE;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.Flags = 0;
	sd.OutputWindow = HWND(21);
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(nullptr,
								  D3D_DRIVER_TYPE_HARDWARE,
								  nullptr, D3D11_CREATE_DEVICE_DEBUG,
								  nullptr, 0, D3D11_SDK_VERSION,
								  &sd, &mDxgiSwapChain,
								  &mDxDevice, nullptr,
								  &mDxDeviceContext);

	if (FAILED(hr)) throw std::runtime_error("Failed To Create Device!");
}

void FxRenderAPI::CreateRenderTarget()
{
	ID3D11Resource* pBackbuffer = nullptr;
	HRESULT hr = mDxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Resource),
		reinterpret_cast<void**>(&pBackbuffer));
	//TODO: Write Exception for RenderApi
	if (FAILED(hr)) throw std::runtime_error("Failed To Create backbuffer!");

	hr = mDxDevice->CreateRenderTargetView(pBackbuffer, nullptr, &mRenderTargetView);

	if (FAILED(hr)) throw std::runtime_error("Failed To Create RenderTargetView!");

	pBackbuffer->Release();
}
