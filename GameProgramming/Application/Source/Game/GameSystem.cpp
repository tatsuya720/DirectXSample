#include "../../framework.h"
#include "../DirectX/Direct3D.h"
#include "GameSystem.h"
    
void GameSystem::Initialize()
{

}

void GameSystem::Execute()
{
    float color[4] = { 0.2f, 0.2f, 1.0f, 1.0f };
    D3D.deviceContext->ClearRenderTargetView(D3D.backBufferView.Get(), color);


    {
        struct VertexType
        {
            DirectX::XMFLOAT3 Pos;
        };

        VertexType v[3] = {
            {{0, 0, 0}},
            {{1, -1, 0}},
            {{-1, -1, 0}},
        };

        D3D11_BUFFER_DESC vbDesc = {};
        vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbDesc.ByteWidth = sizeof(v);
        vbDesc.MiscFlags = 0;
        vbDesc.StructureByteStride = 0;
        vbDesc.Usage = D3D11_USAGE_DEFAULT;
        vbDesc.CPUAccessFlags = 0;

        ComPtr<ID3D11Buffer> vb;
        D3D11_SUBRESOURCE_DATA initData = { &v[0], sizeof(v), 0 };

        D3D.device->CreateBuffer(&vbDesc, &initData, &vb);

        UINT stride = sizeof(VertexType);
        UINT offset = 0;
        D3D.deviceContext->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &stride, &offset);
        D3D.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


        D3D.deviceContext->VSSetShader(D3D.spriteVS.Get(), 0, 0);
        D3D.deviceContext->PSSetShader(D3D.spritePS.Get(), 0, 0);
        D3D.deviceContext->IASetInputLayout(D3D.spriteInputLayout.Get());

        D3D.deviceContext->Draw(3, 0);
    }

    D3D.swapChain->Present(1, 0);
}