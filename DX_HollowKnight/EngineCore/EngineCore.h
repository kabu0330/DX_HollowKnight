#pragma once
#include <EngineBase/EngineDefine.h>
#include <EngineBase/EngineTimer.h>
#include <EnginePlatform/EngineWindow.h>
#include "EngineGraphicDevice.h"
#include "IContentsCore.h"
#include "Level.h"
#include <memory>


// 설명 : 게임 루프를 돌릴 핵심 클래스
class UEngineCore
{
public:
	// constrcuter destructer
	ENGINEAPI UEngineCore();
	ENGINEAPI virtual ~UEngineCore() = 0;

	ENGINEAPI static void EngineStart(HINSTANCE _Instance, std::string_view _DllName);

	template<typename GameModeType, typename MainPawnType>
	static class std::shared_ptr<class ULevel> CreateLevel(std::string_view _Name)
	{
		std::shared_ptr<ULevel> NewLevel = NewLevelCreate(_Name);

		NewLevel->SpawnActor<GameModeType>();
		NewLevel->SpawnActor<MainPawnType>();

		// 2가 됩니다
		return NewLevel;
	}

	ENGINEAPI static void OpenLevel(std::string_view _Name);


	ENGINEAPI static FVector GetScreenScale();

	ENGINEAPI static UEngineGraphicDevice& GetDevice();

	ENGINEAPI static UEngineWindow& GetMainWindow();

protected:

private:
	ENGINEAPI static UEngineWindow MainWindow;

	ENGINEAPI static UEngineGraphicDevice Device;

	static HMODULE ContentsDLL;
	static std::shared_ptr<IContentsCore> Core;
	static UEngineInitData Data;

	static UEngineTimer Timer; // Delta Timer

	static void WindowInit(HINSTANCE _Instance);
	static void LoadContents(std::string_view _DllName);

	static void EngineFrame();
	static void EngineEnd();

	ENGINEAPI static std::shared_ptr<ULevel> NewLevelCreate(std::string_view _Name);

	static std::map<std::string, std::shared_ptr<class ULevel>> LevelMap;
	static std::shared_ptr<class ULevel> CurLevel; // 현재 Tick을 돌 레벨
	static std::shared_ptr<class ULevel> NextLevel; // 현재 Level을 종료하고 다음 프레임에 실행될 Level
};

