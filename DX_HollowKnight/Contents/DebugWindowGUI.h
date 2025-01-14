#pragma once
#include <EngineCore/EngineGUIWindow.h>

// ���� :
class UDebugWindowGUI : public UEngineGUIWindow
{
public:
	// constrcuter destructer
	UDebugWindowGUI();
	~UDebugWindowGUI();

	// delete Function
	UDebugWindowGUI(const UDebugWindowGUI& _Other) = delete;
	UDebugWindowGUI(UDebugWindowGUI&& _Other) noexcept = delete;
	UDebugWindowGUI& operator=(const UDebugWindowGUI& _Other) = delete;
	UDebugWindowGUI& operator=(UDebugWindowGUI&& _Other) noexcept = delete;

	void GetMousePos();
	void GetCurRoom();
	void GetKnightInfo();
	void GetGravityForce();


protected:
	void OnGUI() override;

private:

};
