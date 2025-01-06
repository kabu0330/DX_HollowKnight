#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class ARoom : public AActor
{
public:
	// constrcuter destructer
	ARoom();
	~ARoom();

	// delete Function
	ARoom(const ARoom& _Other) = delete;
	ARoom(ARoom&& _Other) noexcept = delete;
	ARoom& operator=(const ARoom& _Other) = delete;
	ARoom& operator=(ARoom&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	UEngineWinImage& GetPixelCollisionImage()
	{
		return PixelCollisionImage;
	}
	static std::shared_ptr<ARoom>& GetCurRoom()
	{
		return CurRoom;
	}
	
	bool IsLinking(std::shared_ptr<ARoom> _Room);
	bool InterLinkRoom(std::shared_ptr<ARoom> _Room, FVector _WorldPos);
	std::shared_ptr<ARoom> LinkRoom(std::shared_ptr<ARoom> _Room);

	void CheckGround(FVector _MovePos);

protected:

private:
	static std::shared_ptr<ARoom> CurRoom;
	UEngineWinImage PixelCollisionImage;

	std::shared_ptr<class USpriteRenderer> BackgroundRenderer;

	std::vector<std::shared_ptr<ARoom>> Rooms;
};

