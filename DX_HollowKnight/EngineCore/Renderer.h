#pragma once
#include "SceneComponent.h"
#include "EngineGraphicDevice.h"

struct EngineVertex
{
public:
	// ���� ��ġ
	float4 Pos;

	// ���� ����
	float4 Color;

	// FVector UV;

	// FVector AnimationRatio;
};

// ���� :
class URenderer : public USceneComponent
{
	friend class ULevel;

public:
	// constrcuter destructer
	ENGINEAPI URenderer();
	ENGINEAPI ~URenderer();

	// delete Function
	URenderer(const URenderer& _Other) = delete;
	URenderer(URenderer&& _Other) noexcept = delete;
	URenderer& operator=(const URenderer& _Other) = delete;
	URenderer& operator=(URenderer&& _Other) noexcept = delete;

	void SetOrder(int _Order) override;

protected:
	ENGINEAPI void BeginPlay() override;

private:
	virtual void Render(float _DeltaTime);

public:
	ID3D11Buffer* VertexBuffer = nullptr;
	void InputAssembler1Init();
	void InputAssembler1Setting();

	ID3DBlob* ShaderCodeBlob = nullptr;
	ID3DBlob* ErrorCodeBlob = nullptr; // �߰� ������ �����
	ID3D11VertexShader* VertexShader = nullptr;
	void VertexShaderInit();
	void VertexShaderSetting();
};

