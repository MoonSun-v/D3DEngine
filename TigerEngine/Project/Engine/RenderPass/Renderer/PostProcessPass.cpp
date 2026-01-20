#include "PostProcessPass.h"
#include "../../Manager/ShaderManager.h"

// [ PostProcess Pass ]
// ToneMapping(LDR) + PostProcess
// Tone Mapping 패스는 화면을 덮는 FullScreen 사각형을 그리면서,
// HDR SRV를 샘플링해 색을 계산하고, 그 결과를 BackBuffer에 기록하는 단계
void PostProcessPass::Execute(ComPtr<ID3D11DeviceContext>& context, RenderQueue& queue, Camera* cam)
{
    auto& sm = ShaderManager::Instance();

    // clear
    ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
    context->PSSetShaderResources(14, 1, nullSRV);
    context->PSSetShaderResources(16, 1, nullSRV);

    ID3D11RenderTargetView* nullRTVs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = {};
    context->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, nullRTVs, nullptr);

    // RTV
    context->RSSetViewports(1, &sm.viewport_screen);
    context->OMSetRenderTargets(1, sm.backBufferRTV.GetAddressOf(), nullptr);
    context->ClearRenderTargetView(sm.backBufferRTV.Get(), clearColor);

    // IA
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(nullptr);

    // Shader
    context->VSSetShader(sm.VS_FullScreen.Get(), NULL, 0);
    context->PSSetShader(sm.PS_PostProcess.Get(), NULL, 0);

    // Sampler
    context->PSSetSamplers(0, 1, sm.linearSamplerState.GetAddressOf());

    // SRV
    context->PSSetShaderResources(14, 1, sm.sceneHDRSRV.GetAddressOf());
    context->PSSetShaderResources(16, 1, sm.finalBloomSRV.GetAddressOf());

    // Draw Call
    context.Get()->Draw(3, 0);

    // cleanup
    context->PSSetShaderResources(14, 1, nullSRV);
}