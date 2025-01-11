#pragma once

// Ό³Έν :
class RoomManager
{
public:
	// constrcuter destructer
	RoomManager();
	virtual	~RoomManager() = 0;

	// delete Function
	RoomManager(const RoomManager& _Other) = delete;
	RoomManager(RoomManager&& _Other) noexcept = delete;
	RoomManager& operator=(const RoomManager& _Other) = delete;
	RoomManager& operator=(RoomManager&& _Other) noexcept = delete;

	static void CreateAndLinkRoom(class AGameMode* _GameMode);
protected:

private:
	inline static class AGameMode* GameMode = nullptr;
	static std::shared_ptr<class ARoom> CreateRoom(std::string_view _RoomName, std::string_view _FileName, FVector _Size);
	static void SetInitCurRoom(std::shared_ptr<class ARoom> _InitRoom);
	static void LoadPixelCollisionTexture(class ARoom* _Room, UEngineWinImage* _BmpTexture, std::string_view _FileName, FVector _Size);

	class UEngineWinImage* PixelCollisionImage = nullptr;
};

