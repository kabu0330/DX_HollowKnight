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
	
	bool IsLinking(ARoom* _Room);
	bool InterLinkRoom(ARoom* _Room, FVector _WorldPos);
	ARoom* LinkRoom(ARoom* _Room);
	//std::shared_ptr<ARoom> LinkRoom(ARoom* _Room);

	void CheckGround(FVector _MovePos);
	void Gravity(AActor* _Target);
	void CreateTexture(std::string_view _FileName, float _ScaleRatio);
	void CreatePixelCollisionTexture(std::string_view _FileName, float _ScaleRatio);

	FVector GetSize() const
	{
		return Size;
	}
	void SetSize(const FVector& _Size)
	{
		Size = _Size;
	}

protected:

private:
	inline static bool DebugNonGravity = false;
	FVector GravityForce = FVector::ZERO;
	float GravityValue = 1400.0f;

	static std::shared_ptr<ARoom> CurRoom;
	UEngineWinImage PixelCollisionImage;
	FVector Size = FVector::ZERO;
	FVector Pos = FVector::ZERO;

	std::shared_ptr<class UContentsRenderer> PixelCollisionTexture;
	std::shared_ptr<class UContentsRenderer> BackgroundRenderer;

	std::vector<ARoom*> Rooms;
};

