#include "PreCompile.h"
#include "TitleScene.h"

ATitleScene::ATitleScene()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	BackgroundRenderer = CreateDefaultSubObject<UContentsRenderer>();

	std::string TitleBackGround = "TitleBackGround";
	BackgroundRenderer->CreateAnimation(TitleBackGround, TitleBackGround, 0, 111, 0.1f);
	GlobalFunc::AutoScale(BackgroundRenderer, TitleBackGround);
	BackgroundRenderer->ChangeAnimation(TitleBackGround);

	BackgroundRenderer->SetupAttachment(RootComponent);

	//Logo = GetWorld()->SpawnActor<ATitleLogo>();
//// Logo->SetActorLocation({ 300.0f, 0.0f, 0.0f });
//Logo->GetRenderer()->SetSpriteData(4);
//
//// 카메라를 위치조정을 무조건 해줘야 한다.
//std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
//Camera->SetActorLocation({0.0f, 0.0f, -1000.0f, 1.0f});
}

ATitleScene::~ATitleScene()
{
}

void ATitleScene::BeginPlay()
{
	AActor::BeginPlay();
}

void ATitleScene::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);


}

