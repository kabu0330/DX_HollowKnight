#pragma once

#include <d3d11_4.h> // directx 11 버전4용 헤더
#include <d3dcompiler.h> // 셰이더 컴파일러용 인터페이스
#include <EnginePlatform/EngineWindow.h>

// 라이브러리들
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler") 
#pragma comment(lib, "dxguid")

//  모니터 정보나 그래픽카드 정보를 제공하는 함수
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

	void CreateDeviceAndContext();

	void CreateBackBuffer(const UEngineWindow& _Window);

	void GetHighPerFormanceAdapter();

protected:

private:
	ID3D11Device* Device = nullptr;

	ID3D11DeviceContext* Context = nullptr;
};

