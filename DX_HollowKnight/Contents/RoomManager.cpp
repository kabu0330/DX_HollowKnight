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
	std::string Background = "_back.png";
	std::string PixelCollision = "_pixel.bmp";
	std::string DirtmouthStr = "Dirtmouth";
	std::string ForgottenCrossroadsStr1 = "ForgottenCrossroads1";
	std::string ForgottenCrossroadsStr2 = "ForgottenCrossroads2";
	std::string ForgottenCrossroadsStr3 = "ForgottenCrossroads3";
	FVector ForgottenCrossroadsSize = { 12838, 14928 };
	// �� ����
	std::shared_ptr<ARoom> Dirtmouth = CreateRoom(DirtmouthStr, DirtmouthStr + Background, DirtmouthStr + PixelCollision, { 14172, 3806 });
	//std::shared_ptr<ARoom> ForgottenCrossroads1 = CreateRoom(ForgottenCrossroadsStr1, ForgottenCrossroadsStr1 + Background, ForgottenCrossroadsStr1 + Background, ForgottenCrossroadsSize);
	//std::shared_ptr<ARoom> ForgottenCrossroads2 = CreateRoom(ForgottenCrossroadsStr2, ForgottenCrossroadsStr2 + Background, ForgottenCrossroadsStr2 + Background, ForgottenCrossroadsSize);
	//std::shared_ptr<ARoom> ForgottenCrossroads3 = CreateRoom(ForgottenCrossroadsStr3, ForgottenCrossroadsStr3 + Background, ForgottenCrossroadsStr3 + Background, ForgottenCrossroadsSize);
	
	//Dirtmouth->InterLinkRoom(ForgottenCrossroads.get(), FVector{ -(14172.0f / 2.0f + 3806.0f / 2.0f), 0.0f });
	//Dirtmouth->InterLinkRoom(Stage1.get(), FVector{ 1300.0f, -(2500.0f / 2.0f) - 1300.0f });

	//SetInitCurRoom(Dirtmouth);
}

std::shared_ptr<class ARoom> RoomManager::CreateRoom(std::string_view _RoomName, std::string_view _BackgroundName, std::string_view _PixelCollisionName, FVector _Size, float _ScaleRatio/* = 1.0f*/)
{
	std::string RoomName = _RoomName.data();
	std::shared_ptr<ARoom> NewRoom = GameMode->GetWorld()->SpawnActor<ARoom>();
	NewRoom->SetName(RoomName);
	NewRoom->SetSize(_Size);

	NewRoom->CreateTexture(_BackgroundName, _ScaleRatio);
	LoadPixelCollisionTexture(NewRoom.get(), &NewRoom->GetPixelCollisionImage(), _PixelCollisionName, _Size, _ScaleRatio);
	return NewRoom;
}

void RoomManager::SetInitCurRoom(std::shared_ptr<class ARoom> _InitRoom)
{
	ARoom::GetCurRoom() = _InitRoom;
	std::string RoomName = ARoom::GetCurRoom()->GetName();
}

void RoomManager::LoadPixelCollisionTexture(ARoom* _Room, UEngineWinImage* _BmpTexture, std::string_view _FileName, FVector _Size, float _ScaleRatio)
{
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	{
		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
		return;
	}
	Dir.Append("Image");
	UEngineFile ImageFiles = Dir.GetFile(_FileName);

	_BmpTexture->Load(nullptr, ImageFiles.GetPathToString());

	_Room->CreatePixelCollisionTexture(_FileName, _ScaleRatio);

}

