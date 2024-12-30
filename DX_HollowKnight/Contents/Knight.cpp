#include "PreCompile.h"
#include "Knight.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>

AKnight::AKnight()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	float IdleFrameTime = 0.2f;
	float RunFrameTime = 0.1f;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	//BodyRenderer->SetSprite("862.png", 0);
	BodyRenderer->CreateAnimation("Idle", "Knight_Idle.png", 0, 8, IdleFrameTime);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation("Idle");
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

	BodyRenderer->CreateAnimation("Run", "Knight_Run", 0, 7, RunFrameTime);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation("Run");
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

	BodyRenderer->ChangeAnimation("Idle");
	//BodyRenderer->SetRelativeScale3D({ -100, 100, 1.0f });
	BodyRenderer->SetupAttachment(RootComponent);

}

AKnight::~AKnight()
{
}

void AKnight::BeginPlay()
{
	AActor::BeginPlay();
}

void AKnight::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	InputCheck(_DeltaTime);

}

void AKnight::InputCheck(float _DeltaTime)
{
	if (UEngineInput::IsPress(VK_LEFT))
	{
		BodyRenderer->ChangeAnimation("Run");
		BodyRenderer->SetRotation({ 0.0f,0.0f,0.0f });
		AddRelativeLocation(FVector{ -100.0f * _DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		AddRelativeLocation(FVector{ 100.0f * _DeltaTime, 0.0f, 0.0f });
		BodyRenderer->ChangeAnimation("Run");
		BodyRenderer->SetRotation({ 0.0f,180.0f,0.0f });
	}
	if (UEngineInput::IsPress('Z'))
	{
		AddRelativeLocation(FVector{ 0.0f, 100.0f * _DeltaTime, 0.0f });
	}
	if (UEngineInput::IsDown('C'))
	{
		AddRelativeLocation(FVector{ 1000.0f * _DeltaTime, 0.0f, 0.0f });
	}
}

