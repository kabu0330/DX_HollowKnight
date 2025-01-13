#include "PreCompile.h"
#include "Room.h"
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>
#include "PlayGameMode.h"
#include "Monster.h"

std::shared_ptr<ARoom> ARoom::CurRoom = nullptr;

ARoom::ARoom()
{
	// 항상 DefaultSceneComponent는 생성자에서만. BeginPlay에서는 만들 수 없다.
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	PixelCollisionTexture = CreateDefaultSubObject<UContentsRenderer>();
	PixelCollisionTexture->SetupAttachment(RootComponent);
	PixelCollisionTexture->SetAutoScaleRatio(1.0f);

	BackgroundRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BackgroundRenderer->SetupAttachment(RootComponent);
	//BackgroundRenderer->SetTexture("Dirtmouth_Back.png", true, 2.0f);

	//DebugNonGravity = true;
	bActiveGravity = true;
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
	float ZSort = static_cast<float>(EZOrder::BACKGROUND);
	BackgroundRenderer->SetZSort(ZSort);
}

bool ARoom::IsLinking(ARoom* _Room)
{
	for (ARoom* Room : Rooms)
	{
		if (Room == _Room)
		{
			return true;
		}
	}
	return false;
}

bool ARoom::InterLinkRoom(ARoom* _Room, FVector _WorldPos)
{
	this->LinkRoom(_Room);
	_Room->LinkRoom(this);
	_Room->SetActorLocation(this->GetActorLocation() + _WorldPos);

	return true;
}

ARoom* ARoom::LinkRoom(ARoom* _Room)
{
	if (_Room == this)
	{
		MSGASSERT("자기 자신을 연결할 수 없습니다.");
		return nullptr;
	}
	if (true == IsLinking(_Room))
	{
		MSGASSERT("이미 연결된 맵입니다.");
		return nullptr;
	}
	
	Rooms.push_back(_Room);
	return Rooms[Rooms.size() - 1];
}

void ARoom::CreateTexture(std::string_view _FileName, float _ScaleRatio)
{
	float ZSort = static_cast<float>(EZOrder::BACKGROUND);

	BackgroundRenderer->SetTexture(_FileName, true, _ScaleRatio);
	BackgroundRenderer->SetRelativeLocation({ Size.X / 2.0f * _ScaleRatio, -Size.Y / 2.0f * _ScaleRatio, ZSort });
	BackgroundRenderer->SetActive(false);
}

void ARoom::CreatePixelCollisionTexture(std::string_view _FileName, float _ScaleRatio)
{
	float ZSort = static_cast<float>(EZOrder::PIXELCOLLISION);

	PixelCollisionTexture->SetTexture(_FileName, true, _ScaleRatio);
	PixelCollisionTexture->SetRelativeLocation({ Size.X / 2.0f * _ScaleRatio, -Size.Y / 2.0f * _ScaleRatio, ZSort });
}

void ARoom::CheckPixelCollision(AActor* _Actor, class UContentsRenderer* _Renderer, FVector _Gravity)
{
	float DeltaTime = UEngineCore::GetDeltaTime();
	FVector NextPos = GravityForce * DeltaTime;

	FVector ActorPos = _Actor->GetActorLocation();
	float HalfRendererHeight = _Renderer->GetScale().Y * 0.5f;

	FVector CollisionPoint = { ActorPos.X + NextPos.X, ActorPos.Y +  NextPos.Y + HalfRendererHeight };

	// 실수오차 문제 때문에
	CollisionPoint.X = floorf(CollisionPoint.X);
	CollisionPoint.Y = floorf(CollisionPoint.Y);

	UColor CollisionColor = PixelCollisionImage.GetColor({ CollisionPoint.X, -CollisionPoint.Y }); // y축 반전

	AKnight* Knight = dynamic_cast<AKnight*>(_Actor);

	if (nullptr != Knight)
	{
		if (CollisionColor == UColor::BLACK || CollisionColor == UColor(0, 0, 0, 255))
		{
			//UEngineDebug::OutPutString("White");
			Knight->SetOnGround(true);
			return;
		}
		else if (CollisionColor == UColor::WHITE || CollisionColor == UColor(255, 255, 255, 255))
		{
			//UEngineDebug::OutPutString("Black");
			// 흰색 아니면 다 벽과 바닥으로 보겠다.
			Knight->SetOnGround(false);
			return;
		}
	}

	AMonster* Monster = dynamic_cast<AMonster*>(_Actor);
	if (nullptr != Monster)
	{
		if (CollisionColor == UColor::WHITE || CollisionColor == UColor(255, 255, 255, 255))
		{
			Monster->SetOnGround(true);
			return;
		}
		else if (CollisionColor == UColor::BLACK || CollisionColor == UColor(0, 0, 0, 255))
		{
			Monster->SetOnGround(false);
			return;
		}
	}
}

void ARoom::CheckGround(AActor* _Actor, FVector _Gravity)
{
	float DeltaTime = UEngineCore::GetDeltaTime();
	FVector CollisionPixel = _Actor->GetActorLocation() + (_Gravity * DeltaTime);

	CollisionPixel.X = floorf(CollisionPixel.X);
	CollisionPixel.Y = floorf(CollisionPixel.Y);

	UColor Color = PixelCollisionImage.GetColor({ CollisionPixel.X, -CollisionPixel.Y });

	if (Color == UColor::BLACK)
	{
		AKnight::GetPawn()->SetOnGround(true);
	}
	else if (Color == UColor::WHITE || Color == UColor(255, 255, 255, 0))
	{
		//UEngineDebug::OutPutString("White");
		AKnight::GetPawn()->SetOnGround(false);
	}

}

