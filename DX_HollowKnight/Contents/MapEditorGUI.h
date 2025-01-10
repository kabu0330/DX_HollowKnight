#pragma once
#include <EngineCore/EngineGUIWindow.h>

// Ό³Έν :
class MapEditorGUI : public UEngineGUIWindow
{
public:
	// constrcuter destructer
	MapEditorGUI();
	~MapEditorGUI();

	// delete Function
	MapEditorGUI(const MapEditorGUI& _Other) = delete;
	MapEditorGUI(MapEditorGUI&& _Other) noexcept = delete;
	MapEditorGUI& operator=(const MapEditorGUI& _Other) = delete;
	MapEditorGUI& operator=(MapEditorGUI&& _Other) noexcept = delete;

protected:
	void OnGUI() override;

private:
	int SelectItem = 0;
	int SelectButtonIndex = 0;
	int ObjectItem = -1;
};

