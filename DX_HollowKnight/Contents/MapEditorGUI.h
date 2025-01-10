#pragma once
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
// ���� :
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

	bool& IsPreviewRef()
	{
		return bIsPreview;
	}
protected:
	void OnGUI() override;

	void ShowSpriteImageButton();
	void RenderPreview();

	void ShowActorListBox();
	void DeleteAllActors();
	void SpawnActor();

	void SetActorLocationButton();

	void SaveFile();
	void LoadFile();
	void LoadTexture();

private:
	int SelectItem = 0;
	int SelectButtonIndex = 0;
	int ObjectItem = -1;
	std::shared_ptr<UEngineSprite> Sprite = nullptr;
	UEngineTexture* Texture = nullptr;
	ImTextureID TextureSRV = 0;

	int ZValue = 3999;
	int XValue = 0;
	int YValue = 0;

	ImTextureID SelectedTexture = 0;
	bool bIsPreview = false;
	FVector TextureScale = FVector::ZERO;
	float ScaleRatio = 1.0f;

};

