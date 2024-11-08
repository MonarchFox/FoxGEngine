#include "../Public/FxRenderAPI.h"

#include <stdexcept>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")


namespace wrl = Microsoft::WRL;

void FxRenderAPI::Init(HWND hWnd)
{
	CreateDeviceAndSwapChain(hWnd);
	CreateRenderTarget();
	SetViewPort(hWnd);
}

void FxRenderAPI::ClearRenderTarget(const float color[4])
{
	mDxDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), color);
}

void FxRenderAPI::PresentFrame()
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
	sd.OutputWindow = hWnd;
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
	wrl::ComPtr<ID3D11Resource> pBackbuffer = nullptr;
	HRESULT hr = mDxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackbuffer);
	//TODO: Write Exception for RenderApi
	if (FAILED(hr)) throw std::runtime_error("Failed To Create backbuffer!");

	hr = mDxDevice->CreateRenderTargetView(pBackbuffer.Get(), nullptr, &mRenderTargetView);
	if (FAILED(hr)) throw std::runtime_error("Failed To Create RenderTargetView!");

	mDxDeviceContext->OMSetRenderTargets(1u, mRenderTargetView.GetAddressOf(), nullptr);
}

void FxRenderAPI::SetViewPort(HWND hWnd)
{
	RECT rt; GetClientRect(hWnd, &rt);

	D3D11_VIEWPORT vp{};
	vp.Height = static_cast<FLOAT>(rt.bottom - rt.top);
	vp.Width = static_cast<FLOAT>(rt.right - rt.left);
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;
	vp.TopLeftX = static_cast<FLOAT>(rt.left);
	vp.TopLeftY = static_cast<FLOAT>(rt.top);
	mDxDeviceContext->RSSetViewports(1, &vp);
}

//~ Render Functionality

void FxRenderAPI::InitDrawTriangle()
{

	//~ Creating Vertex Buffer
	struct Vertex
	{
		float x;
		float y;
	};

	Vertex vertex[]
	{
		{ 0.0f, 0.5f },{ 0.5f, -0.5f },{ -0.5f, -0.5f }
	};
	UINT nVertex = ARRAYSIZE(vertex);

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.ByteWidth = sizeof(Vertex) * nVertex;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	HRESULT hr = mDxDevice->CreateBuffer(&bd, &sd, &mVertexBuffer);
	if (FAILED(hr)) throw std::runtime_error("Failed to create vertex Buffer!");

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	mDxDeviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);

	//~ Creating Vertex Shader
	wrl::ComPtr<ID3D11VertexShader> vertShader{ nullptr };
	wrl::ComPtr<ID3DBlob> vertBlob{ nullptr };

	hr = D3DReadFileToBlob(L"VertexShader.cso", &vertBlob);
	if (FAILED(hr)) throw std::runtime_error("Failed To Read VertexShader.cso!");

	hr = mDxDevice->CreateVertexShader(vertBlob->GetBufferPointer(),
		vertBlob->GetBufferSize(), nullptr, &vertShader);
	if (FAILED(hr)) throw std::runtime_error("Failed To Create VertexShader!");

	mDxDeviceContext->VSSetShader(vertShader.Get(), nullptr, 0);

	//~ Creating Pixel Shader
	wrl::ComPtr<ID3D11PixelShader> pixelShader{ nullptr };
	wrl::ComPtr<ID3DBlob> pixelBlob{ nullptr };

	hr = D3DReadFileToBlob(L"PixelShader.cso", &pixelBlob);
	if (FAILED(hr)) throw std::runtime_error("Failed To Read PixelShader.cso!");

	hr = mDxDevice->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(),
		nullptr, &pixelShader);

	mDxDeviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	//~ Setting Up Topolog
	mDxDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//~ Creating Input Layout
	wrl::ComPtr<ID3D11InputLayout> inputLayout{ nullptr };
	D3D11_INPUT_ELEMENT_DESC ed[]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0u}
	};
	mDxDevice->CreateInputLayout(ed, 1, vertBlob->GetBufferPointer(), vertBlob->GetBufferSize(),
		&inputLayout);
	mDxDeviceContext->IASetInputLayout(inputLayout.Get());

	//~ Creating Index Buffer

	WORD indices[]{
		0, 1, 2
	};
	UINT nIndices = ARRAYSIZE(indices);

	D3D11_BUFFER_DESC ibd{};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(WORD) * nIndices;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = sizeof(WORD);

	D3D11_SUBRESOURCE_DATA isd{};
	isd.pSysMem = indices;

	wrl::ComPtr<ID3D11Buffer> indexBuffer{ nullptr };

	mDxDevice->CreateBuffer(&ibd, &isd, &indexBuffer);
	mDxDeviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

void FxRenderAPI::DrawTriangleTest()
{
	//mDxDeviceContext->Draw(3u, 0u);
	mDxDeviceContext->DrawIndexed(3, 0, 0);
}
