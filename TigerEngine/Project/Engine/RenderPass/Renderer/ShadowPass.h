#pragma once
#include "../IRenderPass.h"
#include "pch.h"

/*
    [ Shadow Renderer ]

    ShadowMap 생성을 위한 Stage를 맞추고, 전달받은 model들로 depth only pass를 진행합니다.
    Light 시점에서 바라본 Scene을 렌더링하며 Depth(ShadowMap)을 기록합니다.


    ** Shadow Renderer의 Renderable 객체 **
     Model Mesh
*/

class ShadowPass : public IRenderPass
{
public:
    void Init() {};
    void Execute(ComPtr<ID3D11DeviceContext>& context,
        RenderQueue& queue, Camera* cam) override;
    void End(ComPtr<ID3D11DeviceContext>& context) override {}
};

