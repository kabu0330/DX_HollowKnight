#include "PreCompile.h"
#include "KnightEffect.h"

AKnightEffect::AKnightEffect()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	EffectRenderer = CreateDefaultSubObject<USpriteRenderer>();

	// 이동 애니메이션
	std::string SlashEffect = "SlashEffect";
	EffectRenderer->CreateAnimation(SlashEffect, SlashEffect, 0, 8, 0.1f);
	{
		USpriteRenderer::FrameAnimation* Animation = EffectRenderer->FindAnimation(SlashEffect);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

	EffectRenderer->SetupAttachment(RootComponent);
}

AKnightEffect::~AKnightEffect()
{
}

void AKnightEffect::BeginPlay()
{
	AActor::BeginPlay();
}

void AKnightEffect::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

}

void AKnightEffect::ChangeEffect(std::string_view _AnimationName)
{
}

