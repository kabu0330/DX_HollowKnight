#include "PreCompile.h"
#include "Room.h"
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>
#include "PlayGameMode.h"

std::shared_ptr<ARoom> ARoom::CurRoom = nullptr;

ARoom::ARoom()
{
	// �׻� DefaultSceneComponent�� �����ڿ�����. BeginPlay������ ���� �� ����.
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	BackgroundRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BackgroundRenderer->SetupAttachment(RootComponent);
	BackgroundRenderer->SetAutoScaleRatio(1.0f);
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

bool ARoom::IsLinking(ARoom* _Room)
{
	for (std::shared_ptr<ARoom> Room : Rooms)
	{
		if (Room.get() == _Room)
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
		MSGASSERT("�ڱ� �ڽ��� ������ �� �����ϴ�.");
		return nullptr;
	}
	if (true == IsLinking(_Room))
	{
		MSGASSERT("�̹� ����� ���Դϴ�.");
		return nullptr;
	}
	
	// �����Ѵ�
	Rooms.push_back(_Room);
	return Rooms[Rooms.size() - 1].get();
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
		// ��� �ƴϸ� �� ���� �ٴ����� ���ڴ�.
		AKnight::GetPawn()->SetIsOnGround(true);
	}

	int a = 0;
}

void ARoom::CreatePixelCollisionTexture(std::string_view _FileName)
{
	float ZSort = 100.0f;

	BackgroundRenderer->SetTexture(_FileName, true, 1.0f);
	BackgroundRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });
}

