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

	// std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
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
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
			return;
		}
		Dir.Append("Image");
		UEngineFile ImageFiles = Dir.GetFile("dartmount_front2.bmp");

		PixelCollisionImage.Load(nullptr, ImageFiles.GetPathToString());
	}

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
		MSGASSERT("�ڱ� �ڽ��� ������ �� �����ϴ�.");
		return nullptr;
	}
	if (true == IsLinking(_Room))
	{
		MSGASSERT("�̹� ����� ���Դϴ�.");
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

	UColor Color = PixelCollisionImage.GetColor(APlayGameMode::KnightPos);
	if (Color == UColor{ 255, 255, 255, 255 })
	{
		AKnight::GetPawn()->IsGround() = false;
	}
	else // ��� �ƴϸ� �� �ٴ����� ���ڴ�.
	{
		AKnight::GetPawn()->IsGround() = true;
	}
	bool Result = AKnight::GetPawn()->IsGround();

}

