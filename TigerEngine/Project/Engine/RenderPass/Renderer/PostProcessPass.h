#pragma once
#include "../IRenderPass.h"
#include "pch.h"

/*
    [ PostProcess Renderer ]

    최종 sceneHDR을 LDR로 톤매핑하고, 
    후처리 후 BackBuffer에 최종 렌더합니다.


    ** PostProcess Renderer의 Renderable 객체 **
     x
*/


class PostProcessPass : public IRenderPass
{
private:
    // clear color
    float clearColor[4] = { 0,0,0,1 };

public:
    void Init() {};
    void Execute(ComPtr<ID3D11DeviceContext>& context,
        RenderQueue& queue, Camera* cam) override;
    void End(ComPtr<ID3D11DeviceContext>& context) override {}
};

