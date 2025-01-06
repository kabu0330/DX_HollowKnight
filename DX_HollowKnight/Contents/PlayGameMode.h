#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class APlayGameMode : public AActor
{
public:
	// constrcuter destructer
	APlayGameMode();
	~APlayGameMode();

	// delete Function
	APlayGameMode(const APlayGameMode& _Other) = delete;
	APlayGameMode(APlayGameMode&& _Other) noexcept = delete;
	APlayGameMode& operator=(const APlayGameMode& _Other) = delete;
	APlayGameMode& operator=(APlayGameMode&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);

	static std::shared_ptr<class ACameraActor> Camera;
	static FVector MousePos;
	static FVector KnightPos;
protected:
	
private:
	class UEngineWinImage* PixelCollisionImage = nullptr;
};

