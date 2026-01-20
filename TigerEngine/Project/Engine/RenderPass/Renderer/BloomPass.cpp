#include "BloomPass.h"
#include "../../Manager/ShaderManager.h"

// [ BloomProcess Pass ]
// Prefilter -> DownSample+Blur -> UpSample+Combine
void BloomPass::Execute(ComPtr<ID3D11DeviceContext>& context, RenderQueue& queue, Camera* cam)
{
    auto& sm = ShaderManager::Instance();

    // clear
    ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
    context->PSSetShaderResources(14, 1, nullSRV);
    context->PSSetShaderResources(16, 1, nullSRV);
    context->PSSetShaderResources(17, 1, nullSRV);

    ID3D11RenderTargetView* nullRTVs[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = {};
    context->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, nullRTVs, nullptr);

    // Full Screen VS
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetInputLayout(nullptr);
    context->VSSetShader(sm.VS_FullScreen.Get(), NULL, 0);

    // Sampler
    context->PSSetSamplers(2, 1, sm.linearClamSamplerState.GetAddressOf());

    // 1. Prefilter Pass ------------------------------------
    //  - HDR을 샘플링하여 BloomA의 mip0에 처리할 픽셀만 기록
    //  - sceneHDR read -> mip0 write
    {
        // View Port
        sm.SetViewportForMip(context, sm.bloomW, sm.bloomH, 0);

        // output : A.mip0
        context->OMSetRenderTargets(1, sm.bloomARTVs[0].GetAddressOf(), nullptr);
        context->ClearRenderTargetView(sm.bloomARTVs[0].Get(), clearColor);

        // input : sceneHDR
        context->PSSetShaderResources(14, 1, nullSRV);
        context->PSSetShaderResources(14, 1, sm.sceneHDRSRV.GetAddressOf());

        // CB
        UINT w0, h0;
        sm.GetMipSize(sm.bloomW, sm.bloomH, 0, w0, h0);
        sm.bloomCBData.srcMip = 0;
        sm.bloomCBData.srcTexelSize = DirectX::XMFLOAT2(1.0f / (float)w0, 1.0f / (float)h0);
        context->UpdateSubresource(sm.bloomCB.Get(), 0, nullptr, &sm.bloomCBData, 0, 0);

        // Draw Call
        context->PSSetShader(sm.PS_BloomPrefilter.Get(), NULL, 0);
        context.Get()->Draw(3, 0);

        // cleanup
        context->PSSetShaderResources(14, 1, nullSRV);
    }


    // 2. DownSample Blur Pass ------------------------------
    //  - BloomA mip0을 시작으로 Mip Chain 형성 + 블러 처리한다.
    //  - BloomA와 BloomB를 SRV와 RTV로 ping-pong하며 read & write 교대
    //  - mip(i-1) read -> mip(i) write
    {
        for (int i = 1; i < sm.bloomMipCount; ++i)
        {
            // RTV UnBind
            context->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, nullRTVs, nullptr);

            // View Port
            sm.SetViewportForMip(context, sm.bloomW, sm.bloomH, i);

            // Ping Pong
            // i 홀수 : A(SRV) -> B(RTV)
            // i 짝수 : B(SRV) -> A(RTV)
            bool AtoB = (i % 2 != 0) ? true : false;

            // input : A or B mip(i-1)
            ID3D11ShaderResourceView* bloomSRV = AtoB ? sm.bloomASRV.Get() : sm.bloomBSRV.Get();
            context->PSSetShaderResources(16, 1, nullSRV);
            context->PSSetShaderResources(16, 1, &bloomSRV);

            // output : A or B mip(i)
            ID3D11RenderTargetView* bloomRTV = AtoB ? sm.bloomBRTVs[i].Get() : sm.bloomARTVs[i].Get();
            context->OMSetRenderTargets(1, &bloomRTV, nullptr);
            context->ClearRenderTargetView(bloomRTV, clearColor);

            // CB
            UINT sw, sh;
            sm.GetMipSize(sm.bloomW, sm.bloomH, i - 1, sw, sh);
            sm.bloomCBData.srcTexelSize = DirectX::XMFLOAT2(1.0f / (float)sw, 1.0f / (float)sh);
            sm.bloomCBData.srcMip = (float)(i - 1);
            context->UpdateSubresource(sm.bloomCB.Get(), 0, nullptr, &sm.bloomCBData, 0, 0);

            // Draw Call
            context->PSSetShader(sm.PS_BloomDownsampleBlur.Get(), NULL, 0);
            context.Get()->Draw(3, 0);

            // cleanup
            context->PSSetShaderResources(16, 1, nullSRV);
        }
    }

    // 3. UpSample Combine Pass -----------------------------
    //  - mipN에서 mip0으로 올라오며 업샘플 + 가산합성하여 최종 블룸 이미지를 도출한다.
    //  - DownSample 결과의 시작 누적(accum)은 last mip이 들어있는 텍스처에서 시작한다.
    //  - BloomA, BloomB : read only
    //  - AccumA, AccumB : small(accum) read, out write
    //  - mip(i, i+1) read -> mip(i)  write
    {
        // LastMip은 bloomMipCount-1이 홀수면 B, 짝수면 A에 있음 (downpass에서 핑퐁했기 때문에)
        int lastMipLevel = (int)sm.bloomMipCount - 1;
        bool lastMipOnBloomB = (lastMipLevel % 2) != 0;

        // 첫 루프에서는 accum에 아직 누적 텍스처가 없으므로, bloom에서 big과 small 가져온다.
        bool accumOnB = false;  // bloomA를 small로 시작 (2개를 한번에 합할거라 순서 상관 x)

        for (int i = lastMipLevel - 1; i >= 0; --i)
        {
            // cleanup
            context->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, nullRTVs, nullptr);
            context->PSSetShaderResources(16, 1, nullSRV);
            context->PSSetShaderResources(17, 1, nullSRV);

            // View Port
            sm.SetViewportForMip(context, sm.bloomW, sm.bloomH, i);

            // 1) Big SRV : 더 큰 해상도의 mip이 있는 SRV
            ID3D11ShaderResourceView* bigSRV = nullptr;
            if (i == 0) bigSRV = sm.bloomASRV.Get();      // prefilter 단계에서 mip0은 BloomA에 저장함
            else bigSRV = ((i % 2) != 0) ? sm.bloomBSRV.Get() : sm.bloomASRV.Get();

            // 2) Small SRV : 가산 누적된 mip이 있는 SRV (업샘플 소스)
            // 첫 루프에서는 accum에 아직 가산한 누적 텍스처가 없으므로, small = Bloom A or B의 last mip
            // 다음부터는 small = AccumA or AccumB의 mip(i+1)
            ID3D11ShaderResourceView* smallSRV = nullptr;
            if (i == lastMipLevel - 1)
                smallSRV = lastMipOnBloomB ? sm.bloomBSRV.Get() : sm.bloomASRV.Get();
            else
                smallSRV = accumOnB ? sm.accumBSRV.Get() : sm.accumASRV.Get();

            // 3) out RTV : 현재 패스에서 기록할 texture. (smallSRV와 겹치면 안됨!)
            const bool outOnB = !accumOnB;
            ID3D11RenderTargetView* outRTV = outOnB ? sm.accumBRTVs[i].Get() : sm.accumARTVs[i].Get();

            // SRV, RTV Bind
            context->PSSetShaderResources(16, 1, &bigSRV);
            context->PSSetShaderResources(17, 1, &smallSRV);
            context->OMSetRenderTargets(1, &outRTV, nullptr);

            // CB
            UINT wi, hi;
            sm.GetMipSize(sm.bloomW, sm.bloomH, (UINT)i, wi, hi);
            sm.bloomCBData.srcTexelSize = DirectX::XMFLOAT2(1.0f / (float)wi, 1.0f / (float)hi);
            sm.bloomCBData.srcMip = (float)i;
            context->UpdateSubresource(sm.bloomCB.Get(), 0, nullptr, &sm.bloomCBData, 0, 0);

            // Draw Call
            context->PSSetShader(sm.PS_BloomUpsampleCombine.Get(), NULL, 0);
            context->Draw(3, 0);

            // cleanup
            context->PSSetShaderResources(16, 1, nullSRV);
            context->PSSetShaderResources(17, 1, nullSRV);

            // 누적 위치 갱신
            accumOnB = outOnB;
        }

        // Final Bloom Texture
        // accumOnB가 가리키는 Accum 텍스처의 mip0 -> PostProcess에 활용
        sm.finalBloomSRV = accumOnB ? sm.accumBSRV.Get() : sm.accumASRV.Get();
    }
}