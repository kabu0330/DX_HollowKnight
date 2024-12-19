#include "PreCompile.h"
#include "Renderer.h"
#include "Level.h"
#include "EngineCore.h"
#include <EngineBase/EngineString.h>

URenderer::URenderer()
{
}

URenderer::~URenderer()
{
	VertexBuffer->Release();
	ShaderCodeBlob->Release();
	ErrorCodeBlob->Release();
}

void URenderer::SetOrder(int _Order)
{
	int PrevOrder = GetOrder();
	UObject::SetOrder(_Order);

	ULevel* Level = GetActor()->GetWorld();
	  
	std::shared_ptr<URenderer> RendererPtr = GetThis<URenderer>();

	Level->ChangeRenderGroup(PrevOrder, RendererPtr);
}

ENGINEAPI void URenderer::BeginPlay()
{
	SetOrder(0);

	InputAssembler1Init();
	VertexShaderInit();
}

void URenderer::Render(float _DeltaTime)
{
	// ������ ����������
	InputAssembler1Setting();
	VertexShaderSetting();
}

void URenderer::InputAssembler1Init()
{
	// ���ؽ� ���۸� �׷���ī�忡�� ����� �޶�� ��û

	// ����� �̹� ram
	// CPU
	std::vector<EngineVertex> Vertexs;
	Vertexs.resize(6);

	Vertexs[0] = EngineVertex{ FVector(-0.5f, 0.5f, -0.5f), {} };
	Vertexs[1] = EngineVertex{ FVector(0.5f, 0.5f, -0.5f), {} };
	Vertexs[2] = EngineVertex{ FVector(-0.5f, -0.5f, -0.5f), {} };
	Vertexs[3] = EngineVertex{ FVector(0.5f, 0.5f, -0.5f), {} };
	Vertexs[4] = EngineVertex{ FVector(0.5f, -0.5f, -0.5f), {} };
	Vertexs[5] = EngineVertex{ FVector(-0.5f, -0.5f, -0.5f), {} };


	D3D11_BUFFER_DESC BufferInfo = { 0 };

	BufferInfo.ByteWidth = sizeof(EngineVertex) * static_cast<int>(Vertexs.size());
	// �뵵�� ���ؽ� ����
	BufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// CPU���� �����Ҽ� �־�?
	BufferInfo.CPUAccessFlags = 0;
	// �߿� �޸� ��ġ�� �׷���ī��� �Ұų� cpu�� �Ұž�?
	// �������� �ɼ�

	// D3D11_USAGE_DEFAULT
	// GPU���� �б� �� ���� �׼����� �ʿ��� ���ҽ��Դϴ�.�̴� ���� �Ϲ����� ��� ������ �� �ֽ��ϴ�.

	// D3D11_USAGE_IMMUTABLE
	// GPU������ ���� �� �ִ� ���ҽ��Դϴ�.GPU���� �ۼ��� �� ������ CPU���� ���� �׼����� �� �����ϴ�.�� ������ ���ҽ��� ���� �Ŀ� ������ �� �����Ƿ� ���� �� �ʱ�ȭ�ؾ� �մϴ�.

	// D3D11_USAGE_DYNAMIC
	// GPU(�б� ����)�� CPU(���� ����)���� �׼����� �� �ִ� ���ҽ��Դϴ�.���� ���ҽ��� CPU���� �����Ӵ� �� �� �̻� ������Ʈ�Ǵ� ���ҽ��� �����մϴ�.���� ���ҽ��� ������Ʈ�Ϸ��� Map �޼��带 ����մϴ�.

	// ��Ʈ���� ä���Ҷ�. 
	// 	D3D11_USAGE_STAGING
	// GPU���� CPU���� ������ ����(����)�� �����ϴ� ���ҽ��Դϴ�.

	BufferInfo.Usage = D3D11_USAGE_DEFAULT;


	// �ʱⰪ
	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Vertexs[0];

	// GPU�� xxx ũ���� ���� ������ּ���� ��� �Ʒ��� �Լ��� ����ϴ�.
	if (S_OK != UEngineCore::Device.GetDevice()->CreateBuffer(&BufferInfo, &Data, &VertexBuffer))
	{
		MSGASSERT("���ؽ� ���� ������ �����߽��ϴ�.");
		return;
	}
}

void URenderer::InputAssembler1Setting()
{
	UINT VertexSize = sizeof(EngineVertex);
	// �� ���ؽ� ���۰� 10��¥���ε� 3��° ���ؽ� ���� ��������.
	UINT Offset = 0;

	// �ѹ��� �������� ���ؽ� ���۸� �����Ҽ� �ִ�.
	// ���ؽ����������� 1���� �迭�� �־��ִµ�.
	// 0�����Ժ��� 1�� ����ϰڴ�.
	// ���ؽ� �ϳ��� ũ��� VertexSize
	// Offset���� ���ؽ��� ����ϰڴ�.
	UEngineCore::Device.GetContext()->IASetVertexBuffers(0, 1, &VertexBuffer, &VertexSize, &Offset);
}

void URenderer::VertexShaderInit()
{
	// FX���ϱ��� ã�ư��� �Ѵ�.
	UEngineDirectory CurDir;
	CurDir.MoveParentToDirectory("EngineShader");
	UEngineFile File = CurDir.GetFile("EngineSpriteShader.fx");


	// wchar_t*

	std::string Path = File.GetPathToString();

	std::wstring WPath = UEngineString::AnsiToUnicode(Path);

	// ���̴� �ڵ带 �׳� ���ڿ��� ���� 
	//std::string ShaderCode = "struct EngineVertex \	{ \		float4 COLOR : COLOR; \		float4 OSITION : POSITION; \	}; \	struct VertexShaderOutPut \	{ \		float4 SVPOSITION : V_POSITION; \		float4 COLOR : COLOR; \	}; \	VertexShaderOutPut VertexToWorldEngineVertex _Vertex) \	{ \		VertexShaderOutPut OutPut; \		utPut.SVPOSITION = _Vertex.POSITION; \		OutPut.COLOR = _Vertex.COLOR; \
	//	return OutPut; \	} 
	// D3DCompile()

	// ������ �����.
	std::string version = "vs_5_0";

	int Flag0 = 0;
	int Flag1 = 0;

#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif

	// ����� ����ְ� �ɰ��̴�.
	// ���� �������ٰ��ϴ� �ϴµ� �������� �����ϴ�.
	// Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	// [1][0][0][0]
	// [0][1][0][0]
	// [0][0][1][0]
	// [100][100][0][1]

	// [1][0][0][100]
	// [0][1][0][100]
	// [0][0][1][0]
	// [0][0][0][1]

	D3DCompileFromFile(
		WPath.c_str(),
		nullptr, // Define TEST ������ ��ó���⸦ ������.
		nullptr,
		"VertexToWorld",
		version.c_str(),
		Flag0,
		Flag1,
		&ShaderCodeBlob,
		&ErrorCodeBlob
	);

	if (nullptr == ShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(ErrorCodeBlob->GetBufferPointer());
		MSGASSERT("���̴� �ڵ� �߰����忡�� �����߽��ϴ�\n" + ErrString);
		return;
	}

	HRESULT Result = UEngineCore::Device.GetDevice()->CreateVertexShader(
		ShaderCodeBlob->GetBufferPointer(),
		ShaderCodeBlob->GetBufferSize(),
		nullptr,
		&VertexShader
	);

	if (S_OK != Result)
	{
		MSGASSERT("���ؽ� ���̴� ������ �����߽��ϴ�.");
	}

}

void URenderer::VertexShaderSetting()
{
	 
	UEngineCore::Device.GetContext()->VSSetShader(VertexShader, nullptr, 0);
}