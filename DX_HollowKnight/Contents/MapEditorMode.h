#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class AMapEditorMode : public AActor
{
public:
	// constrcuter destructer
	AMapEditorMode();
	~AMapEditorMode();

	// delete Function
	AMapEditorMode(const AMapEditorMode& _Other) = delete;
	AMapEditorMode(AMapEditorMode&& _Other) noexcept = delete;
	AMapEditorMode& operator=(const AMapEditorMode& _Other) = delete;
	AMapEditorMode& operator=(AMapEditorMode&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	FVector MousePos = { 0.0f, 0.0f };

protected:

private:

};

