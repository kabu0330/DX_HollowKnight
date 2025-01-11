#include "PreCompile.h"
#include "RoomManager.h"
#include "Room.h"

RoomManager::RoomManager()
{
}

RoomManager::~RoomManager()
{
}

void RoomManager::CreateAndLinkRoom(AGameMode* _GameMode)
{
	GameMode = _GameMode;

	// 맵 세팅
	std::shared_ptr<ARoom> Dirtmouth = CreateRoom("Dirtmouth", "dartmount_back.bmp", { 4148, 2500 });
	std::shared_ptr<ARoom> Entrance = CreateRoom("Entrance", "entrance_back.bmp", { 1958, 3000 });
	std::shared_ptr<ARoom> Stage1 = CreateRoom("Stage1", "stage1_back.bmp", { 4519, 2600 });

	Dirtmouth->InterLinkRoom(Entrance.get(), FVector{ -(4148.0f / 2.0f + 1958.0 / 2.0f), 0.0f });
	Dirtmouth->InterLinkRoom(Stage1.get(), FVector{ 1300.0f, -(2500.0f / 2.0f) - 1300.0f });

	SetInitCurRoom(Dirtmouth);
}

std::shared_ptr<class ARoom> RoomManager::CreateRoom(std::string_view _RoomName, std::string_view _FileName, FVector _Size)
{
	std::string RoomName = _RoomName.data();
	std::shared_ptr<ARoom> NewRoom = GameMode->GetWorld()->SpawnActor<ARoom>();
	NewRoom->SetName(RoomName);
	NewRoom->SetSize(_Size);
	LoadPixelCollisionTexture(NewRoom.get(), &NewRoom->GetPixelCollisionImage(), _FileName, _Size);
	return NewRoom;
}

void RoomManager::SetInitCurRoom(std::shared_ptr<class ARoom> _InitRoom)
{
	ARoom::GetCurRoom() = _InitRoom;
	std::string RoomName = ARoom::GetCurRoom()->GetName();
}

void RoomManager::LoadPixelCollisionTexture(ARoom* _Room, UEngineWinImage* _BmpTexture, std::string_view _FileName, FVector _Size)
{
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}
	Dir.Append("Image");
	UEngineFile ImageFiles = Dir.GetFile(_FileName);

	_BmpTexture->Load(nullptr, ImageFiles.GetPathToString());

	_Room->CreatePixelCollisionTexture(_FileName);
	_Room->SetActorLocation({ _Size.X / 2.0f, -_Size.Y / 2.0f });
}

