#pragma once
#include "Components/ScriptComponent.h"

/// <summary>
/// 캐릭터 컨트롤러 컴포넌트 테스트용 클래스.
/// </summary>

class CCTTest : public ScriptComponent
{
    RTTR_ENABLE(ScriptComponent)

private:
    bool isApply = false;

public:
    void OnInitialize() override;
    void OnStart() override;
    void OnUpdate(float delta) override;
    void OnFixedUpdate(float dt) override;
};