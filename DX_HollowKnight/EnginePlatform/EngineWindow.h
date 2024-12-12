#pragma once
#include <EngineBase/EngineDefine.h>

// Ό³Έν :
class UEngineWindow
{
public:
	// constrcuter destructer
	ENGINEAPI UEngineWindow();
	ENGINEAPI ~UEngineWindow();

	// delete Function
	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& _Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;

protected:

private:

};

