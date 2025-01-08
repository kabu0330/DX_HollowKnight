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
	BackgroundRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BackgroundRenderer->SetupAttachment(RootComponent);
	BackgroundRenderer->SetAutoScaleRatio(1.0f);

	float ZSort = 100.0f;

	BackgroundRenderer->SetTexture("dartmount_front2.bmp", true, 1.0f);
	BackgroundRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });


	// Test
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image");
		UEngineFile ImageFiles = Dir.GetFile("dartmount_front2.bmp");

		PixelCollisionImage.Load(nullptr, ImageFiles.GetPathToString());
	}

	float X = 0.0f;
	float Y = 100.0f;
	UColor Color00 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(  0) });
	UColor Color0  = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(100) });
	UColor Color1  = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(200) });
	UColor Color2  = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(300) });
	UColor Color3  = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(400) });
	UColor Color4  = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(500) });
	UColor Color5  = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(600) });
	UColor Color6  = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(700) });
	UColor Color7  = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(800) });
	UColor Color8  = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(900) });
	UColor Color9  = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(1000) });
	UColor Color10 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(1100) });
	UColor Color11 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(1200) });
	UColor Color12 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(1300) });
	UColor Color13 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(1400) });
	UColor Color14 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(1500) });
	UColor Color15 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(1600) });
	UColor Color16 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(1700) });
	UColor Color17 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(1800) });
	UColor Color18 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(1900) });
	UColor Color19 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(2000) });
	UColor Color20 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(2100) });
	UColor Color21 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(2200) });
	UColor Color22 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(2300) });
	UColor Color23 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(2400) });
	UColor Color24 = PixelCollisionImage.GetColor({ static_cast<float>(X), static_cast<float>(2499) });

	int a = 0;
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
	
}

bool ARoom::IsLinking(std::shared_ptr<ARoom> _Room)
{
	for (std::shared_ptr<ARoom> Room : Rooms)
	{
		if (Room == _Room)
		{
			return true;
		}
	}
	return false;
}

bool ARoom::InterLinkRoom(std::shared_ptr<ARoom> _Room, FVector _WorldPos)
{
	this->LinkRoom(_Room);
	_Room->LinkRoom(static_cast<std::shared_ptr<ARoom>>(this));
	_Room->SetActorLocation(_WorldPos);

	return true;
}

std::shared_ptr<ARoom> ARoom::LinkRoom(std::shared_ptr<ARoom> _Room)
{
	if (_Room == static_cast<std::shared_ptr<ARoom>>(this))
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
	FVector NextPos = APlayGameMode::KnightPos + _MovePos;
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

