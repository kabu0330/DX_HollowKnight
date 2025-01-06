#include "PreCompile.h"
#include "KnightEffect.h"

AKnightEffect::AKnightEffect()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	KnightEffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	float ZSort = 0.0f;
	KnightEffectRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });

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
	CheckKnightPos();
	Release();

}

void AKnightEffect::CheckKnightPos()
{
	//KnightPos = AKnight::GetPawn()->GetRootComponent()->GetTransformRef().RelativeLocation;
}

void AKnightEffect::ChangeEffect(std::string_view _AnimationName)
{
	KnightEffectRenderer->ChangeAnimation(_AnimationName);
}

void AKnightEffect::SetLocation(USceneComponent* _Knight, FVector _Pos, bool _Left)
{
	
}

void AKnightEffect::SetLocation(FVector _Pos, bool _Left)
{


	//FVector Pos = _RootComponent->GetTransformRef().RelativeLocation;
	//FVector LRPos = _OffsetPos;
	//if (true == _Left)
	//{
	//	LRPos += Pos;
	//	_Renderer->SetRelativeLocation(LRPos);
	//	_Renderer->SetRotation(_Rotation);
	//	return;
	//}
	//else
	//{
	//	float Inverse = 180.0f;
	//	LRPos -= Pos;
	//	_Renderer->SetRelativeLocation(-LRPos);
	//	_Renderer->SetRotation({ _Rotation.X, _Rotation.Y + Inverse, _Rotation.Z });
	//	return;
	//}
}

void AKnightEffect::Release()
{
	if (nullptr == KnightEffectRenderer)
	{
		return;
	}

	// Release �̱���
	//Destroy(1.0f);

}



