#pragma once

// dxgi 어뎁터 조회용
#include <dxgi1_6.h>
#include <psapi.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "psapi.lib")

#include "pch.h"
#include "Renderer/IRenderer.h"
#include "../Scene/Scene.h"
#include "../RenderPass/IRenderPass.h"
#include "../Components/Camera.h"

/// <summary>
/// DirectX11 기능을 사용하는 클래스
/// </summary>
class DirectX11Renderer : public IRenderer
{
public:
	void Initialize(HWND hwnd, int width, int height) override;
	void OnResize(int width, int height) override;
	void BeginRender() override;
	void EndRender() override;

	// 매 랜더링 시 호출되는 함수
	void ProcessScene(RenderQueue& queue, 
					  IRenderPass& renderPass,
					  Camera* cam);

	ComPtr<ID3D11Device> GetDevice() const;
	ComPtr<ID3D11DeviceContext> GetDeviceContext() const;
	ComPtr<ID3D11RenderTargetView> GetBackBufferRTV() const;
	ComPtr<ID3D11DepthStencilView> GetDepthStencilView() const;
	ComPtr<ID3D11DepthStencilView> GetDepthStencilReadOnlyView() const;
	ComPtr<ID3D11ShaderResourceView> GetDepthSRV() const;
	D3D11_VIEWPORT GetRenderViewPort() const;
	
private:
	ComPtr<ID3D11Device>			device{};			
	ComPtr<ID3D11DeviceContext>		deviceContext{};	
	ComPtr<IDXGISwapChain>			swapChain{};
    ComPtr<ID3D11Texture2D>         backbufferTex;
	ComPtr<ID3D11RenderTargetView>	backBufferRTV{};	

    ComPtr<ID3D11Texture2D>           depthStencilTexture;
	ComPtr<ID3D11DepthStencilView>	  depthStencilView{};	
    ComPtr<ID3D11DepthStencilView>    depthStencilReadOnlyView;
    ComPtr<ID3D11ShaderResourceView>  depthSRV;

    D3D11_VIEWPORT					renderViewport{};
};

