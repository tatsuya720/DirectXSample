#include "Direct3D.h"
#include "../../framework.h"

#include <windows.h>

bool Direct3D::Initialize(HWND hWnd, int width, int height)
{
    ComPtr<IDXGIFactory> factory;

    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
    {
        return false;
    }

    //=====================================================
    //デバイス生成(主にリソース作成時に使用するオブジェクト)
    //=====================================================
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags != D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    D3D_FEATURE_LEVEL featureLevel;
    if (FAILED(D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        &device,
        &featureLevel,
        &deviceContext
    ))) {
        return false;
    }


    //=====================================================
    // スワップチェイン作成(フロントバッファに表示可能なバックバッファを持つもの)
    //=====================================================
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    scDesc.BufferDesc.Width = width;
    scDesc.BufferDesc.Height = height;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scDesc.BufferDesc.RefreshRate.Numerator = 0;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.SampleDesc.Count = 1;
    scDesc.SampleDesc.Quality = 0;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.BufferCount = 2;
    scDesc.OutputWindow = hWnd;
    scDesc.Windowed = TRUE;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    // スワップチェインの作成
    if (FAILED(factory->CreateSwapChain(device.Get(), &scDesc, &swapChain)))
    {
        return false;
    }

    // スワップチェインからバックバッファリソース取得
    ComPtr<ID3D11Texture2D> pBackBuffer;
    if (FAILED(swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer))))
    {
        return false;
    }

    // バックバッファリソース用のRTVを作成
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = scDesc.BufferDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    if (FAILED(device->CreateRenderTargetView(pBackBuffer.Get(), &rtvDesc, &backBufferView)))
    {
        return false;
    }

    //=====================================================
    // デバイスコンテキストに描画に関する設定を行っておく
    //=====================================================

    deviceContext->OMSetRenderTargets(1, backBufferView.GetAddressOf(), nullptr);

    D3D11_VIEWPORT vp = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
    deviceContext->RSSetViewports(1, &vp);


    //HRESULT hr = D3DCompileFromFile(ws, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypath.c_str(), "vs_5_0", compileFlags, 0, &blob, &pErrorBlob);

    ComPtr<ID3DBlob> pErrorBlob = NULL;
    ComPtr<ID3DBlob> compiledVS;

    auto hr = D3DCompileFromFile(L"Shader/SpriteShader.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &compiledVS, &pErrorBlob);
    if (FAILED(hr))
    {
        if (pErrorBlob != NULL)
        {
            OutputDebugStringA(static_cast<char*>(pErrorBlob->GetBufferPointer()));
        }
        return false;
    }

    ComPtr<ID3DBlob> compiledPS;
    if (FAILED(D3DCompileFromFile(L"Shader/SpriteShader.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &compiledPS, nullptr)))
    {
        return false;
    }

    if (FAILED(device->CreateVertexShader(compiledVS->GetBufferPointer(), compiledVS->GetBufferSize(), nullptr, &spriteVS)))
    {
        return false;
    }

    if (FAILED(device->CreatePixelShader(compiledPS->GetBufferPointer(), compiledPS->GetBufferSize(), nullptr, &spritePS)))
    {
        return false;
    }

    std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    if (FAILED(device->CreateInputLayout(&layout[0], layout.size(), compiledVS->GetBufferPointer(),
        compiledVS->GetBufferSize(), &spriteInputLayout
    )))
    {
        return false;
    }


    return true;
}