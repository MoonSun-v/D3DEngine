#include "Components/PhysicsComponent.h"
#include "Object/GameObject.h"
#include "Components/CharacterControllerComponent.h"

#include "GroundTestScript.h"

RTTR_REGISTRATION
{
    rttr::registration::class_<GroundTestScript>("PhysicsTestScript")
        .constructor<>()
        (rttr::policy::ctor::as_std_shared_ptr);
}

void GroundTestScript::OnInitialize() // editor
{
}

void GroundTestScript::OnStart() // ?? 아직 사용하지X 
{

}

void GroundTestScript::OnUpdate(float delta)
{
    // OnStart() 대용 
    if (!isApply)
    {
        auto rigid = GetOwner()->GetComponent<PhysicsComponent>();
        if (rigid != nullptr)
        {
            rigid->CreateStaticBox({ 600, 70, 600 });
            // rigid->CreateTriggerBox({ 600, 70, 600 });
            // rigid->SetLayer(CollisionLayer::IgnoreTest); // 충돌 레이어 테스트 
            rigid->SyncToPhysics();
        }

        isApply = true;
    }
}

void GroundTestScript::OnCCTCollisionEnter(CharacterControllerComponent* cct)
{
    if (!cct || !cct->GetOwner()) return;

    auto name = this->GetOwner()->GetName();
    auto otherName = cct->GetOwner()->GetName();

    cout << endl << "[Rigid]" << name << " - OnCollisionEnter : " << otherName << endl << endl;

    std::wstring wName(name.begin(), name.end());
    std::wstring wOtherName(otherName.begin(), otherName.end());
    OutputDebugStringW((L"[Rigid] "+ wName + L" - OnCCTCollisionEnter : " + wOtherName + L"\n").c_str());
}
