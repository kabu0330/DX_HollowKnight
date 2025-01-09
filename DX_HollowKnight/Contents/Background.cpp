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
	//Renderer->SetTexture("_0171_Layer_423.png");
	//Renderer->SetTexture("BG_bug_corpse_01.png");
	//Renderer->SetTexture("BG_bug_corpse_05.png");
	//Renderer->SetTexture("BG_bush_set.png");
	//Renderer->SetTexture("BG_egg_05.png");
	//Renderer->SetTexture("BG_far_0006_a.png");
	//Renderer->SetTexture("BG_lamp.png");
	//Renderer->SetTexture("break_floor_0002_c4.png");
	//Renderer->SetTexture("break0000.png");
	//Renderer->SetTexture("town_build_05.png");
	//Renderer->SetSprite("MapObjectResources", 0);
	Renderer->SetAutoScale(true);

	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionProfileName("Object");
	Collision->SetScale3D({ 50.0f, 50.0f });
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

	std::shared_ptr<class ACameraActor> Camera = GetWorld()->GetCamera(0);

	UEngineCore::GetMainWindow().GetMousePos();
}

void ABackground::Serialize(UEngineSerializer& _Ser)
{
	_Ser << GetActorLocation();
}

void ABackground::DeSerialize(UEngineSerializer& _Ser)
{
	FVector SavePos;
	_Ser >> SavePos;
	SetActorLocation(SavePos);
}
