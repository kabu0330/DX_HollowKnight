#include "PreCompile.h"
#include "Room.h"
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>

ARoom::ARoom()
{
	// �׻� DefaultSceneComponent�� �����ڿ�����. BeginPlay������ ���� �� ����.
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	// std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	BackgroundRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BackgroundRenderer->SetupAttachment(RootComponent);
	BackgroundRenderer->SetAutoScaleRatio(1.0f);

	float ZSort = 100.0f;

	BackgroundRenderer->SetTexture("dartmount_sample.bmp", true, 1.2f);
	BackgroundRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });

}

ARoom::~ARoom()
{
}

void ARoom::BeginPlay()
{
	AActor::BeginPlay();
}

void ARoom::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}

