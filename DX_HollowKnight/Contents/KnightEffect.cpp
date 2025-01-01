#include "PreCompile.h"
#include "KnightEffect.h"

AKnightEffect::AKnightEffect()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	KnightEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();

	// 이동 애니메이션
	std::string SlashEffect = "SlashEffect";
	KnightEffectRenderer->CreateAnimation(SlashEffect, SlashEffect, 0, 5, 0.1f, false);
	GlobalFunc::AutoScale(KnightEffectRenderer, SlashEffect);

	KnightEffectRenderer->SetupAttachment(RootComponent);
	KnightEffectRenderer->ChangeAnimation(SlashEffect);

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
	Release();
}

void AKnightEffect::ChangeEffect(std::string_view _AnimationName)
{
	KnightEffectRenderer->ChangeAnimation(_AnimationName);
}

void AKnightEffect::SetLocation(FVector _Pos, bool _Left)
{

	if (true == _Left)
	{
		KnightEffectRenderer->SetRelativeLocation(_Pos);
		KnightEffectRenderer->SetRotation({ 0.0f, 0.0f, 0.0f });
		return;
	}
	else
	{
		KnightEffectRenderer->SetRelativeLocation(-_Pos);
		KnightEffectRenderer->SetRotation({ 0.0f, 180.0f, 0.0f });
		return;
	}
}

void AKnightEffect::Release()
{
	if (nullptr == KnightEffectRenderer)
	{
		return;
	}

	Destroy(1.0f);

}



