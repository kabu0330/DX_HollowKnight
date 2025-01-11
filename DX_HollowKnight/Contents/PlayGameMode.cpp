#include "PreCompile.h"
#include "PlayGameMode.h"
#include <EnginePlatform/EngineWinImage.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include "Room.h"
#include "CollisionManager.h"
#include "RoomManager.h"

std::shared_ptr<ACameraActor> APlayGameMode::Camera = nullptr;
FVector APlayGameMode::MousePos = { 0.0f, 0.0f, 0.0f };
FVector APlayGameMode::KnightPos = { 0.0f, 0.0f, 0.0f };

APlayGameMode::APlayGameMode()
{
	SetCamera();
	CreateAndLinkCollisionGroup();
	RoomManager::CreateAndLinkRoom(this);

	//std::shared_ptr<ARoom> Dirtmouth = GetWorld()->SpawnActor<ARoom>();
	//Dirtmouth->SetName("Dirtmouth");
	//Rooms.push_back(Dirtmouth);
	//LoadPixelCollisionTexture(Dirtmouth.get(), &Dirtmouth->GetPixelCollisionImage(), "dartmount_back.bmp", { 4148.0f / 2.0f, -2500.0f / 2.0f });

}

void APlayGameMode::SetCamera()
{
	Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);
}

void APlayGameMode::CreateAndLinkCollisionGroup()
{
	UCollisionManager::CreateCollisionProfile(this);
	UCollisionManager::LinkCollision(this);
}

void APlayGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	CheckInfo();
}

void APlayGameMode::CheckInfo()
{
	MousePos = Camera->ScreenMousePosToWorldPos();
	KnightPos = AKnight::GetPawn()->GetRootComponent()->GetTransformRef().RelativeLocation;
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();
}

APlayGameMode::~APlayGameMode()
{
}