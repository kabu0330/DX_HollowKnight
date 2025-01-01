#pragma once
#include <EngineCore/IContentsCore.h>

// Ό³Έν :
class UContentsCore : public IContentsCore
{
public:
	// constrcuter destructer
	UContentsCore();
	~UContentsCore();

	// delete Function
	UContentsCore(const UContentsCore& _Other) = delete;
	UContentsCore(UContentsCore&& _Other) noexcept = delete;
	UContentsCore& operator=(const UContentsCore& _Other) = delete;
	UContentsCore& operator=(UContentsCore&& _Other) noexcept = delete;

protected:
	void EngineStart(UEngineInitData& _Data);
	void EngineTick(float _DeltaTime);
	void EngineEnd();


private:
	void SetWindowSize(UEngineInitData& _Data);
	void LoadResourceDirectory();
	void LoadFolder();
	void LoadSprite();
	void CreateLevel();
	void LoadContentsResource(std::string_view _Path);
	
	void CheckInput();
	void SwitchGameMode();
	int Select = 0;

};

