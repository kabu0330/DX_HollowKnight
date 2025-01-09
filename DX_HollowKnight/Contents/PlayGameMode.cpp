#include "PreCompile.h"
#include "PlayGameMode.h"
#include <EnginePlatform/EngineWinImage.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include "Room.h"

std::shared_ptr<ACameraActor> APlayGameMode::Camera = nullptr;
FVector APlayGameMode::MousePos = { 0.0f, 0.0f, 0.0f };
FVector APlayGameMode::KnightPos = { 0.0f, 0.0f, 0.0f };

//class DebugWindow : public UEngineGUIWindow
//{
//public:
//	void OnGUI() override
//	{
//		if (true == ImGui::Button("FreeCameraOn"))
//		{
//			GetWorld()->GetMainCamera()->FreeCameraSwitch();
//
//		}
//
//		GetCurRoom();
//		GetMousePos();
//		GetKnightInfo();
//
//		//ImGui::Button("WindowButton");
//		//ImGui::SameLine(); // 한 칸 띄우기
//	}
//
//	void GetMousePos()
//	{
//		FVector MousePos = APlayGameMode::MousePos;
//		ImGui::Text("Mouse Pos X : %.0f, Y : %.0f", MousePos.X, MousePos.Y);
//	}
//
//	void GetCurRoom()
//	{
//		if (nullptr == ARoom::GetCurRoom())
//		{
//			return;
//		}
//		std::string CurRoomName = ARoom::GetCurRoom()->GetName();
//		ImGui::Text("CurRoom Name : %s", CurRoomName.c_str());
//	}
//
//	void GetKnightInfo()
//	{
//		AKnight* Knight = AKnight::GetPawn();
//		FVector KnightPos = APlayGameMode::KnightPos;
//		ImGui::Text("Knight Pos X : %.0f, Y : %.0f", KnightPos.X, KnightPos.Y);
//
//		float ZValue = AKnight::GetPawn()->GetKnightRenderer()->GetTransformRef().RelativeLocation.Z;
//		ImGui::Text("Knight Z Vaule : %.6f", ZValue);
//
//		GetGravityForce();
//
//		ImGui::Text("Knight JumpPower : %.2f", Knight->JumpForce);
//	}
//
//	void GetGravityForce()
//	{
//		ImGui::Text("Knight GravityForce : %.2f", AKnight::GetPawn()->GetGravityForce());
//		int Result = static_cast<int>(AKnight::GetPawn()->GetIsOnGround());
//		std::string ResultString = "";
//		if (0 == Result)
//		{
//			ResultString = "false";
//		}
//		else
//		{
//			ResultString = "true";
//		}
//		ImGui::Text("Knight IsOnGround : %s", ResultString.data());
//	}
//};

APlayGameMode::APlayGameMode()
{
	SetCamera();
	CreateAndLinkCollisionGroup();

	// 맵 세팅
	std::shared_ptr<ARoom> Dirtmouth = CreateRoom("Dirtmouth", "dartmount_back.bmp", { 4148, 2500 });
	std::shared_ptr<ARoom> Entrance = CreateRoom("Entrance", "entrance_back.bmp", { 1958, 3000 });
	std::shared_ptr<ARoom> Stage1 = CreateRoom("Stage1", "stage1_back.bmp", { 4519, 2600 });

	Dirtmouth->InterLinkRoom(Entrance.get(), FVector{-(4148.0f / 2.0f + 1958.0 / 2.0f), 0.0f});
	Dirtmouth->InterLinkRoom(Stage1.get(), FVector{1300.0f, -(2500.0f/ 2.0f) - 1300.0f});

	//std::shared_ptr<ARoom> Dirtmouth = GetWorld()->SpawnActor<ARoom>();
	//Dirtmouth->SetName("Dirtmouth");
	//Rooms.push_back(Dirtmouth);
	//LoadPixelCollisionTexture(Dirtmouth.get(), &Dirtmouth->GetPixelCollisionImage(), "dartmount_back.bmp", { 4148.0f / 2.0f, -2500.0f / 2.0f });


	SetInitCurRoom(Dirtmouth);

}

void APlayGameMode::SetCamera()
{
	Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);
}

void APlayGameMode::CreateAndLinkCollisionGroup()
{
	GetWorld()->CreateCollisionProfile("Knight");
	GetWorld()->CreateCollisionProfile("Monster");
	GetWorld()->CreateCollisionProfile("Object");

	// 충돌체크 해야한다.
	GetWorld()->LinkCollisionProfile("Knight", "Monster");
}

void APlayGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	MousePos = Camera->ScreenMousePosToWorldPos();
	KnightPos = AKnight::GetPawn()->GetRootComponent()->GetTransformRef().RelativeLocation;
}

std::shared_ptr<class ARoom> APlayGameMode::CreateRoom(std::string_view _RoomName, std::string_view _FileName, FVector _Size)
{
	std::string RoomName = _RoomName.data();
	std::shared_ptr<ARoom> NewRoom = GetWorld()->SpawnActor<ARoom>();
	NewRoom->SetName(RoomName);
	NewRoom->SetSize(_Size);
	LoadPixelCollisionTexture(NewRoom.get(), &NewRoom->GetPixelCollisionImage(), _FileName, _Size);
	return NewRoom;
}

void APlayGameMode::LoadPixelCollisionTexture(ARoom* _Room, UEngineWinImage* _BmpTexture, std::string_view _FileName, FVector _Size)
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

void APlayGameMode::SetInitCurRoom(std::shared_ptr<class ARoom> _InitRoom)
{
	ARoom::GetCurRoom() = _InitRoom;
	std::string RoomName = ARoom::GetCurRoom()->GetName();
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();
}

APlayGameMode::~APlayGameMode()
{
}