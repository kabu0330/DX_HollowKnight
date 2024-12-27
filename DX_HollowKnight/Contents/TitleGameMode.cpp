#include "PreCompile.h"
#include "TitleGameMode.h"
#include "TitleLogo.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>

class TestWindow : public UEngineGUIWindow
{
public:
	void OnGUI() override
	{
		ImGui::Button("WindowButton");
		ImGui::SameLine(); // 한 칸 띄우기
		ImGui::Text("test");

	}
};

ATitleGameMode::ATitleGameMode()
{
	Logo = GetWorld()->SpawnActor<ATitleLogo>();
	// Logo->SetActorLocation({ 300.0f, 0.0f, 0.0f });
	Logo->GetRenderer()->SetSpriteData(4);
	
	// 카메라를 위치조정을 무조건 해줘야 한다.
	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({0.0f, 0.0f, -1000.0f, 1.0f});

	UEngineGUI::CreateGUIWindow<TestWindow>("TestWindow");
}

ATitleGameMode::~ATitleGameMode()
{

}

void ATitleGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

}

