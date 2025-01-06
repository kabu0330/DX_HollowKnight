#include "PreCompile.h"
#include "Room.h"
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>

std::shared_ptr<ARoom> ARoom::CurRoom = nullptr;

ARoom::ARoom()
{
	// 항상 DefaultSceneComponent는 생성자에서만. BeginPlay에서는 만들 수 없다.
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	// std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	BackgroundRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BackgroundRenderer->SetupAttachment(RootComponent);
	BackgroundRenderer->SetAutoScaleRatio(1.0f);

	float ZSort = 100.0f;

	BackgroundRenderer->SetTexture("dartmount_sample.bmp", true, 1.2f);
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

bool ARoom::InterLinkRoom(std::shared_ptr<ARoom> _Room)
{
	return false;
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

