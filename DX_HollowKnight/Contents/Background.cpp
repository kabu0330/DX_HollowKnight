#include "PreCompile.h"
#include "Background.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/Collision.h>
#include <EngineCore/DefaultSceneComponent.h>

ABackground::ABackground()
{
	MapObjectTypeValue = EMapObjectType::BACKGROUND_OBJECT;

	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Renderer = CreateDefaultSubObject<USpriteRenderer>();
	Renderer->SetupAttachment(RootComponent);
	//Renderer->SetSprite("MapObjectResources", 0);
	Renderer->SetAutoScale(true);

	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionProfileName("Object");
	Collision->SetScale3D({ 6.0f, 6.0f });
}

ABackground::~ABackground()
{
}

void ABackground::BeginPlay()
{
	AActor::BeginPlay();
}

void ABackground::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	//std::shared_ptr<class ACameraActor> Camera = GetWorld()->GetCamera(0);

	//UEngineCore::GetMainWindow().GetMousePos();
}

void ABackground::Serialize(UEngineSerializer& _Ser)
{
	_Ser << GetActorLocation();
	_Ser << Index;
}

void ABackground::DeSerialize(UEngineSerializer& _Ser)
{
	FVector SavePos = FVector::ZERO;
	_Ser >> SavePos;
	_Ser >> Index;

	SetActorLocation(SavePos);
	SetSpriteIndex(Index);
}
