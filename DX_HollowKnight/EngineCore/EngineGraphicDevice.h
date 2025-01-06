#pragma once
#include <wrl.h>
#include <d3d11_4.h> 
#include <d3dcompiler.h>
#include <EnginePlatform/EngineWindow.h>
#include <memory>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler") 
#pragma comment(lib, "dxguid")
#pragma comment(lib, "DXGI") 

// 설명 :
class UEngineGraphicDevice
{
public:
	// constrcuter destructer
	ENGINEAPI UEngineGraphicDevice();
	ENGINEAPI ~UEngineGraphicDevice();

	// delete Function
	UEngineGraphicDevice(const UEngineGraphicDevice& _Other) = delete;
	UEngineGraphicDevice(UEngineGraphicDevice&& _Other) noexcept = delete;
	UEngineGraphicDevice& operator=(const UEngineGraphicDevice& _Other) = delete;
	UEngineGraphicDevice& operator=(UEngineGraphicDevice&& _Other) noexcept = delete;

	ENGINEAPI void CreateDeviceAndContext();

	ENGINEAPI void CreateBackBuffer(const UEngineWindow& _Window);

	// Adapter 그래픽카드의 정보를 가지고 있는 인터페이스
	// 다이렉트 x 디바이스가 되죠.
	ENGINEAPI IDXGIAdapter* GetHighPerFormanceAdapter();

	ENGINEAPI void Release();

	ENGINEAPI void RenderStart();

	ENGINEAPI void RenderEnd();

	ENGINEAPI ID3D11Device* GetDevice()
	{
		return Device.Get();
	}

	ENGINEAPI ID3D11DeviceContext* GetContext()
	{
		return Context.Get();
	}

	ENGINEAPI ID3D11RenderTargetView* GetRTV()
	{
		return RTV.Get();
	}

protected:

private:
	Microsoft::WRL::ComPtr<IDXGIAdapter> MainAdapter = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Device> Device = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context = nullptr;

	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> DXBackBufferTexture = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RTV = nullptr;

	std::shared_ptr<class UEngineTexture> DepthTex;
	
	//FVector ClearColor = FVector::BLUE;
	ENGINEAPI void DefaultResourcesInit();

	ENGINEAPI void MeshInit();
	ENGINEAPI void BlendInit();
	ENGINEAPI void ShaderInit();
	ENGINEAPI void MaterialInit();
	ENGINEAPI void RasterizerStateInit();
	ENGINEAPI void TextureInit();
	ENGINEAPI void DepthStencilInit();
};

