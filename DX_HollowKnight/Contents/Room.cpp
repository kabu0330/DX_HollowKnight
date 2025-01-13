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
	//BackgroundRenderer->SetTexture("Dirtmouth_Back.png", true, 2.0f);

	DebugNonGravity = true;
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
	float YValue = AKnight::GetPawn()->GetRenderer()->GetScale().Y;
	FVector NextPos = { APlayGameMode::KnightPos.X + _MovePos.X , APlayGameMode::KnightPos.Y + _MovePos.Y + (YValue * 0.5f)};
	NextPos.X = floorf(NextPos.X);
	NextPos.Y = floorf(NextPos.Y);

	UColor Color = PixelCollisionImage.GetColor({ NextPos.X, -NextPos.Y });
	UColor White = { 255, 255, 255, 255 };
	UColor Black = { 0, 0, 0, 0 };

	if (Color == White || Color == UColor(255, 255, 255, 0))
	{
		//UEngineDebug::OutPutString("White");
		AKnight::GetPawn()->IsOnGroundRef(false);
	}
	else if (Color == Black)
	{
		//UEngineDebug::OutPutString("Black");
		// 흰색 아니면 다 벽과 바닥으로 보겠다.
		AKnight::GetPawn()->IsOnGroundRef(true);
	}

	int a = 0;
}

void ARoom::Gravity(AActor* _Target)
{
	if (true == DebugNonGravity)
	{
		return;
	}

	AKnight* Target = dynamic_cast<AKnight*>(_Target);
	if (nullptr == Target)
	{
		return;
	}

	if (false == Target->IsOnGround())
	{
		float DeltaTime = UEngineCore::GetDeltaTime();
		GravityForce += FVector::DOWN * GravityValue * DeltaTime;
		AddRelativeLocation(GravityForce * DeltaTime);
	}
	else
	{
		GravityForce = FVector::ZERO;
	}
}

void ARoom::CreateTexture(std::string_view _FileName, float _ScaleRatio)
{
	float ZSort = static_cast<float>(EZOrder::BACKGROUND);

	BackgroundRenderer->SetTexture(_FileName, true, _ScaleRatio);
	BackgroundRenderer->SetRelativeLocation({ Size.X / 2.0f * _ScaleRatio, -Size.Y / 2.0f * _ScaleRatio, ZSort });
	//BackgroundRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });
	//BackgroundRenderer->SetActive(false);
}

void ARoom::CreatePixelCollisionTexture(std::string_view _FileName, float _ScaleRatio)
{
	float ZSort = static_cast<float>(EZOrder::PIXELCOLLISION);

	PixelCollisionTexture->SetTexture(_FileName, true, _ScaleRatio);
	PixelCollisionTexture->SetRelativeLocation({ Size.X / 2.0f * _ScaleRatio, -Size.Y / 2.0f * _ScaleRatio, ZSort });
	//PixelCollisionTexture->SetWorldLocation({ 0.0f, 0.0f, ZSort });
}

