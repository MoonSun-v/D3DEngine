#include "PhysicsTestScript.h"
#include "Object/GameObject.h"
#include "System/InputSystem.h"
#include "Object/Component.h"
#include "Components/PhysicsComponent.h"
#include "Components/CharacterControllerComponent.h"


RTTR_REGISTRATION
{
    rttr::registration::class_<PhysicsTestScript>("PhysicsTestScript")
    .constructor<>()
        (rttr::policy::ctor::as_std_shared_ptr);
}


void PhysicsTestScript::OnInitialize() // editor
{
}

void PhysicsTestScript::OnStart() // ?? 아직 사용하지X 
{

}

void PhysicsTestScript::OnUpdate(float delta)
{
    // OnStart() 대용 
    if (!isApply)
    {
        auto rigid = GetOwner()->GetComponent<PhysicsComponent>();
        if (rigid != nullptr)
        {
            rigid->CreateDynamicBox({ 20, 30, 20 }, 5);
            // rigid->CreateStaticBox({ 20, 30, 20 });
            // rigid->SetLayer(CollisionLayer::Player);
            rigid->SyncToPhysics();
        }

        isApply = true;
    }
}

void PhysicsTestScript::OnCollisionEnter(PhysicsComponent* other) 
{
    if (!other || !other->GetOwner()) return;

    auto name = this->GetOwner()->GetName();
    auto otherName = other->GetOwner()->GetName();

    cout << endl << "[Rigid]" << name << " - OnCollisionEnter : " << otherName << endl << endl;

    std::wstring wname(otherName.begin(), otherName.end());
    OutputDebugStringW((L"[Rigid] OnCollisionEnter : " + wname + L"\n").c_str());
}

//void PhysicsTestScript::OnCollisionStay(PhysicsComponent* other)
//{
//    if (!other || !other->GetOwner()) return;
//
//    auto name = other->GetOwner()->GetName();
//
//    cout << endl << "[Rigid] OnCollisionStay : " << name << endl << endl;
//
//    std::wstring wname(name.begin(), name.end());
//    OutputDebugStringW((L"[Rigid] OnCollisionStay : " + wname + L"\n").c_str());
//}

void PhysicsTestScript::OnCCTCollisionEnter(CharacterControllerComponent* cct)
{
    if (!cct || !cct->GetOwner()) return;

    auto name = cct->GetOwner()->GetName();

    cout << endl << "[Rigid] OnCCTCollisionEnter : " << name << endl << endl;

    std::wstring wname(name.begin(), name.end());
    OutputDebugStringW((L"[Rigid] OnCCTCollisionEnter : " + wname + L"\n").c_str());
}
