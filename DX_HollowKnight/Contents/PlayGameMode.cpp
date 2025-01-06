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
		GetMousePos();
		GetKnightPos();
		GetKnightZValue();

		//ImGui::Button("WindowButton");
		//ImGui::SameLine(); // 한 칸 띄우기
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

};


APlayGameMode::APlayGameMode()
{
	// Test
	//{
	//	UEngineDirectory Dir;
	//	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	//	{
	//		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
	//		return;
	//	}
	//	Dir.Append("Image");
	//	//UEngineFile ImageFiles = Dir.GetFile("dartmount_front.bmp");
	//	UEngineFile ImageFiles = Dir.GetFile("block.png");

	//	// 편한 인터페이스로 안됩니다.
	//	PixelCollisionImage->Load(nullptr, ImageFiles.GetPathToString());
	//}
}

APlayGameMode::~APlayGameMode()
{
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();

	Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);

	UEngineGUI::CreateGUIWindow<DebugWindow>("DebugWindow");

	GetWorld()->SpawnActor<ARoom>();
}

void APlayGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	MousePos = Camera->ScreenMousePosToWorldPos();
	KnightPos = AKnight::GetPawn()->GetRootComponent()->GetTransformRef().WorldLocation;
}

