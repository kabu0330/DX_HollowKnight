#include "PreCompile.h"
#include "Room.h"
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>
#include "PlayGameMode.h"


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
	BackgroundRenderer->SetAutoScaleRatio(1.0f);
	BackgroundRenderer->SetTexture("Dirtmouth_100%.png", true, 2.0f);

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

void ARoom::CheckGround(FVector _MovePos)
{
	float YValue = AKnight::GetPawn()->GetRenderer()->GetRelativeScale3D().Y;
	FVector NextPos = { APlayGameMode::KnightPos.X + _MovePos.X , APlayGameMode::KnightPos.Y + _MovePos.Y + (YValue * 0.5f)};
	NextPos.X = floorf(NextPos.X);
	NextPos.Y = floorf(NextPos.Y);

	UColor Color = PixelCollisionImage.GetColor({ NextPos.X, -NextPos.Y });
	UColor White = { 255, 255, 255, 255 };
	UColor Black = { 0, 0, 0, 0 };
	if (Color == White || Color == UColor(255, 255, 255, 0))
	{
		//UEngineDebug::OutPutString("White");
		AKnight::GetPawn()->SetIsOnGround(false);
	}
	else if (Color == Black)
	{
		//UEngineDebug::OutPutString("Black");
		// 흰색 아니면 다 벽과 바닥으로 보겠다.
		AKnight::GetPawn()->SetIsOnGround(true);
	}

	int a = 0;
}

void ARoom::CreatePixelCollisionTexture(std::string_view _FileName)
{
	float ZSort = static_cast<float>(EZOrder::PIXELCOLLISION);

	PixelCollisionTexture->SetTexture(_FileName, true, 1.0f);
	PixelCollisionTexture->SetWorldLocation({ 0.0f, 0.0f, ZSort });
}

