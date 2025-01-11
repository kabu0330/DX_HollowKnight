#include "PreCompile.h"
#include "KnightEffect.h"

AKnightEffect::AKnightEffect()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	BodyRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	float ZSort = static_cast<float>(EZOrder::SKILL_FRONT);
	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });

	float FrameTime = 0.03f;

	std::string SlashEffect = "SlashEffect";
	BodyRenderer->CreateAnimation(SlashEffect, SlashEffect, 0, 5, FrameTime, false);

	std::string UpSlashEffect = "UpSlashEffect";
	BodyRenderer->CreateAnimation(UpSlashEffect, UpSlashEffect, 0, 5, FrameTime, false);

	std::string DownSlashEffect = "DownSlashEffect";
	BodyRenderer->CreateAnimation(DownSlashEffect, DownSlashEffect, 0, 5, FrameTime, false);

	std::string FocusEffect = "FocusEffect";
	BodyRenderer->CreateAnimation(FocusEffect, FocusEffect, 0, 12, FrameTime, false);

	std::string FocusEffectEnd = "FocusEffectEnd";
	BodyRenderer->CreateAnimation(FocusEffectEnd, FocusEffectEnd, 0, 1, FrameTime, false);

	BodyRenderer->ChangeAnimation(SlashEffect);
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
	this;
	CheckKnightPos();
	Release();
}

void AKnightEffect::CheckKnightPos()
{
	//KnightPos = AKnight::GetPawn()->GetRootComponent()->GetTransformRef().RelativeLocation;
}

void AKnightEffect::ChangeEffect(std::string_view _AnimationName)
{
	BodyRenderer->ChangeAnimation(_AnimationName);
}

void AKnightEffect::Release()
{
	if (nullptr == BodyRenderer)
	{
		return;
	}

	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		Destroy();
	}

}



