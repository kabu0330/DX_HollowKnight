#pragma once
#include <EngineCore/GameMode.h>

// ���� :
class AMapEditorGameMode : public AGameMode 
{
public:
	// constrcuter destructer
	AMapEditorGameMode();
	~AMapEditorGameMode();

	// delete Function
	AMapEditorGameMode(const AMapEditorGameMode& _Other) = delete;
	AMapEditorGameMode(AMapEditorGameMode&& _Other) noexcept = delete;
	AMapEditorGameMode& operator=(const AMapEditorGameMode& _Other) = delete;
	AMapEditorGameMode& operator=(AMapEditorGameMode&& _Other) noexcept = delete;

	void Tick(float _DeltaTime);

protected:
	FVector NewTest = FVector::RIGHT;

private:
	std::shared_ptr<class ATitleLogo> Logo;

};

