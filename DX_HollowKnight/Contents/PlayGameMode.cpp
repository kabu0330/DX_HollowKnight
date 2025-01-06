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

class DebugWindow : public UEngineGUIWindow
{
public:
	void OnGUI() override
	{
		GetCurRoom();
		GetMousePos();
		GetKnightPos();
		GetKnightZValue();

		//ImGui::Button("WindowButton");
		//ImGui::SameLine(); // ÇÑ Ä­ ¶ç¿ì±â
		//ImGui::Text("test");
	}

	void GetMousePos()
	{
		FVector MousePos = APlayGameMode::MousePos;
		ImGui::Text("Mouse Pos X : %.0f, Y : %.0f", MousePos.X, MousePos.Y);
	}

	void GetKnightPos()
	{
		FVector KnightPos = APlayGameMode::KnightPos;
		ImGui::Text("Knight Pos X : %.0f, Y : %.0f", KnightPos.X, KnightPos.Y);
	}
	void GetKnightZValue()
	{
		float ZValue = AKnight::GetPawn()->GetKnightRenderer()->GetTransformRef().RelativeLocation.Z;
		ImGui::Text("Knight Z Vaule : %.6f", ZValue);
	}

	void GetCurRoom()
	{
		if (nullptr == ARoom::GetCurRoom())
		{
			return;
		}
		std::string CurRoomName = ARoom::GetCurRoom()->GetName();
		ImGui::Text("CurRoom Name : %s", CurRoomName.c_str());
	}
};


APlayGameMode::APlayGameMode()
{
	Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);

	std::shared_ptr<ARoom> Dirtmouth = GetWorld()->SpawnActor<ARoom>();
	Dirtmouth->SetName("Dirtmouth");
	Rooms.push_back(Dirtmouth);
	ARoom::GetCurRoom() = Rooms[0];
	std::string RoomName = ARoom::GetCurRoom()->GetName();
	UEngineGUI::CreateGUIWindow<DebugWindow>("DebugWindow");
}

APlayGameMode::~APlayGameMode()
{
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();
}

void APlayGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	MousePos = Camera->ScreenMousePosToWorldPos();
	KnightPos = AKnight::GetPawn()->GetRootComponent()->GetTransformRef().WorldLocation;
}

