#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class Direct3D
{
public:
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> deviceContext;
    ComPtr<IDXGISwapChain> swapChain;
    ComPtr<ID3D11RenderTargetView> backBufferView;

    bool Initialize(HWND hwnd, int width, int hight);

    ComPtr<ID3D11VertexShader> spriteVS = nullptr;
    ComPtr<ID3D11PixelShader> spritePS = nullptr;
    ComPtr<ID3D11InputLayout> spriteInputLayout = nullptr;

    static void CreateInstance()
    {
        DeleteInstance();

        s_instance = new Direct3D();
    }

    static void DeleteInstance()
    {
        if (s_instance != nullptr)
        {
            delete s_instance;
            s_instance = nullptr;
        }
    }

    static Direct3D& GetInstance()
    {
        return *s_instance;
    }

private:
    static inline Direct3D* s_instance;

    Direct3D() {}


};

// Direct3Dの唯一のインスタンスを簡単に取得するためのマクロ
#define D3D Direct3D::GetInstance()