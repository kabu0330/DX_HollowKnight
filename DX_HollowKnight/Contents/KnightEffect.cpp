#include "PreCompile.h"
#include "KnightEffect.h"

AKnightEffect::AKnightEffect()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	KnightEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();

	std::string SlashEffect = "SlashEffect";
	KnightEffectRenderer->CreateAnimation(SlashEffect, SlashEffect, 0, 5, 0.05f, false);
	GlobalFunc::AutoScale(KnightEffectRenderer, SlashEffect);

	std::string UpSlashEffect = "UpSlashEffect";
	KnightEffectRenderer->CreateAnimation(UpSlashEffect, UpSlashEffect, 0, 5, 0.05f, false);
	GlobalFunc::AutoScale(KnightEffectRenderer, UpSlashEffect);

	std::string DownSlashEffect = "DownSlashEffect";
	KnightEffectRenderer->CreateAnimation(DownSlashEffect, DownSlashEffect, 0, 5, 0.05f, false);
	GlobalFunc::AutoScale(KnightEffectRenderer, DownSlashEffect);

	std::string FocusEffect = "FocusEffect";
	KnightEffectRenderer->CreateAnimation(FocusEffect, FocusEffect, 0, 12, 0.08f, false);
	GlobalFunc::AutoScale(KnightEffectRenderer, FocusEffect);

	std::string FocusEffectEnd = "FocusEffectEnd";
	KnightEffectRenderer->CreateAnimation(FocusEffectEnd, FocusEffectEnd, 0, 1, 0.07f, false);
	GlobalFunc::AutoScale(KnightEffectRenderer, FocusEffectEnd);

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

	// Release ¹Ì±¸Çö
	//Destroy(1.0f);

}



