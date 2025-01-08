#pragma once
#include <EngineCore/GameMode.h>

// Ό³Έν :
class APlayGameMode : public AGameMode
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
	void SetCamera();
	std::shared_ptr<class ARoom> CreateRoom(std::string_view _RoomName, std::string_view _FileName, FVector _Size);
	void SetInitCurRoom(std::shared_ptr<class ARoom> _InitRoom);
	void LoadPixelCollisionTexture(class ARoom* _Room, UEngineWinImage* _BmpTexture, std::string_view _FileName, FVector _Size);
	
	class UEngineWinImage* PixelCollisionImage = nullptr;

};

