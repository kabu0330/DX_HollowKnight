#pragma once
#include "EngineResources.h"
#include "ThirdParty/DirectxTex/Inc/DirectXTex.h"

// 설명 :
class UEngineTexture : public UEngineResources
{
public:
	// constrcuter destructer
	ENGINEAPI UEngineTexture();
	ENGINEAPI ~UEngineTexture();

	// delete Function
	UEngineTexture(const UEngineTexture& _Other) = delete;
	UEngineTexture(UEngineTexture&& _Other) noexcept = delete;
	UEngineTexture& operator=(const UEngineTexture& _Other) = delete;
	UEngineTexture& operator=(UEngineTexture&& _Other) noexcept = delete;

	static std::shared_ptr<UEngineTexture> Load(std::string_view _Path)
	{
		UEnginePath EnginePath = UEnginePath(_Path);

		std::string FileName = EnginePath.GetFileName();

		return Load(FileName, _Path);
	}

	ENGINEAPI static std::shared_ptr<UEngineTexture> Load(std::string_view _Name, std::string_view _Path);

	ID3D11ShaderResourceView* GetSRV()
	{
		return SRV.Get();
	}

	FVector GetTextureSize()
	{
		return Size;
	}

protected:

private:
	ENGINEAPI void ResLoad();

	FVector Size;
	DirectX::TexMetadata Metadata;
	DirectX::ScratchImage ImageData;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D = nullptr; // 로드한 텍스처
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV = nullptr; // 텍스처를 셰이더 세팅할 수 있는권한
};
