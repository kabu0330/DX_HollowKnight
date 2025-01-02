#include "PreCompile.h"
#include "MapEditorMode.h"
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineCore/EngineCore.h>
#include <EnginePlatform/EngineInput.h>


class TestWindow : public UEngineGUIWindow
{
public:
	void OnGUI() override
	{
		GetMousePos();
		ImGui::Button("WindowButton");
		ImGui::SameLine(); // ÇÑ Ä­ ¶ç¿ì±â
		ImGui::Text("test");


	}

	void GetMousePos()
	{
		FVector MousePos = UEngineCore::GetMainWindow().GetMousePos();
		ImGui::Text("Mouse Pos X : %.0f, Y : %.0f", MousePos.X, MousePos.Y);
	}
};

AMapEditorMode::AMapEditorMode()
{
	UEngineGUI::CreateGUIWindow<TestWindow>("TestWindow");

}

AMapEditorMode::~AMapEditorMode()
{
}

void AMapEditorMode::BeginPlay()
{
	AActor::BeginPlay();
}

void AMapEditorMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);


	if (true == UEngineInput::IsPress(VK_LBUTTON))
	{

	}
	if (true == UEngineInput::IsPress(VK_RBUTTON))
	{

	}
}

