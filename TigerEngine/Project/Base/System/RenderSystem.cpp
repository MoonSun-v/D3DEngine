#include "RenderSystem.h"

void RenderSystem::Register(RenderComponent* comp)
{
    comps.push_back(comp);
}

void RenderSystem::UnRegister(RenderComponent* comp)
{
    for (auto it = comps.begin(); it != comps.end(); it++)
    {
        if (*it == comp)
        {
            comps.erase(it);
            return;
        }
    }
}

void RenderSystem::Render(ComPtr<ID3D11DeviceContext>& context)
{
    for (auto& e : comps)
    {
        e->OnRender(context);
    }
}