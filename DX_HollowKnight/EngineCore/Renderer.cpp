#include "PreCompile.h"
#include "Renderer.h"
#include "Level.h"
#include "EngineCore.h"

URenderer::URenderer()
{
}

URenderer::~URenderer()
{
	VertexBuffer->Release();
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
}

void URenderer::Render(float _DeltaTime)
{
	// 랜더링 파이프라인
	InputAssembler1Setting();
}

void URenderer::InputAssembler1Init()
{
	// 버텍스 버퍼를 그래픽카드에게 만들어 달라고 요청

	// 여기는 이미 ram
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

	BufferInfo.ByteWidth = sizeof(EngineVertex) * Vertexs.size();
	// 용도는 버텍스 버퍼
	BufferInfo.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// CPU에서 수정할수 있어?
	BufferInfo.CPUAccessFlags = 0;
	// 중요 메모리 위치를 그래픽카드로 할거냐 cpu로 할거야?
	// 여러가지 옵션

	// D3D11_USAGE_DEFAULT
	// GPU에서 읽기 및 쓰기 액세스가 필요한 리소스입니다.이는 가장 일반적인 사용 선택일 수 있습니다.

	// D3D11_USAGE_IMMUTABLE
	// GPU에서만 읽을 수 있는 리소스입니다.GPU에서 작성할 수 없으며 CPU에서 전혀 액세스할 수 없습니다.이 유형의 리소스는 만든 후에 변경할 수 없으므로 만들 때 초기화해야 합니다.

	// D3D11_USAGE_DYNAMIC
	// GPU(읽기 전용)와 CPU(쓰기 전용)에서 액세스할 수 있는 리소스입니다.동적 리소스는 CPU에서 프레임당 한 번 이상 업데이트되는 리소스에 적합합니다.동적 리소스를 업데이트하려면 Map 메서드를 사용합니다.

	// 비트코인 채굴할때. 
	// 	D3D11_USAGE_STAGING
	// GPU에서 CPU로의 데이터 전송(복사)을 지원하는 리소스입니다.

	BufferInfo.Usage = D3D11_USAGE_DEFAULT;


	// 초기값
	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem = &Vertexs[0];

	// GPU에 xxx 크기의 버퍼 만들어주세요는 모두 아래의 함수로 만듭니다.
	if (S_OK != UEngineCore::Device.GetDevice()->CreateBuffer(&BufferInfo, &Data, &VertexBuffer))
	{
		MSGASSERT("버텍스 버퍼 생성에 실패했습니다.");
		return;
	}
}

void URenderer::InputAssembler1Setting()
{

}