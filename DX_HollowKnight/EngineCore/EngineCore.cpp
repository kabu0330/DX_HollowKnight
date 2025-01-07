#include "PreCompile.h"
#include "EngineCore.h"
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>
#include <EnginePlatform/EngineInput.h>
#include "IContentsCore.h"
#include "EngineResources.h"
#include "EngineConstantBuffer.h"
#include "EngineGUI.h"
#include "Level.h"

// EngineCore.dll에 생성된 메모리여야 하므로 cpp 파일에서 Getter 함수를 구현한다. 헤더에서 구현하니 복사된 메모리를 참조하더라.
UEngineGraphicDevice& UEngineCore::GetDevice()
{
	return Device;
}

UEngineWindow& UEngineCore::GetMainWindow()
{
	return MainWindow;
}

UEngineGraphicDevice UEngineCore::Device; // UEngineGraphicDevice EngienCore.dll::UEngineCore::Device;

UEngineWindow UEngineCore::MainWindow;
HMODULE UEngineCore::ContentsDLL = nullptr;
std::shared_ptr<IContentsCore> UEngineCore::Core;
UEngineInitData UEngineCore::Data;
UEngineTimer UEngineCore::Timer;

std::shared_ptr<class ULevel> UEngineCore::NextLevel;
std::shared_ptr<class ULevel> UEngineCore::CurLevel = nullptr;

std::map<std::string, std::shared_ptr<class ULevel>> UEngineCore::LevelMap;

FVector UEngineCore::GetScreenScale()
{
	return Data.WindowSize;
}

UEngineCore::UEngineCore()
{
}

UEngineCore::~UEngineCore()
{
}

void UEngineCore::WindowInit(HINSTANCE _Instance)
{
	UEngineWindow::EngineWindowInit(_Instance);
	MainWindow.Open("MainWindow");
}

void UEngineCore::LoadContents(std::string_view _DllName)
{
	UEngineDirectory Dir;

	Dir.MoveParentToDirectory("Build");
	Dir.Move("bin/x64");

	// 빌드 상황에 따라서 경로 변경
#ifdef _DEBUG
	Dir.Move("Debug");
#else
	Dir.Move("Release");
#endif

	UEngineFile File = Dir.GetFile(_DllName);
	std::string FullPath = File.GetPathToString();

	ContentsDLL = LoadLibraryA(FullPath.c_str());

	if (nullptr == ContentsDLL)
	{
		MSGASSERT("Contents dll 파일을 로드할 수 없습니다.");
		return;
	}

	INT_PTR(__stdcall * Ptr)(std::shared_ptr<IContentsCore>&) = (INT_PTR(__stdcall*)(std::shared_ptr<IContentsCore>&))GetProcAddress(ContentsDLL, "CreateContentsCore");

	if (nullptr == Ptr)
	{
		MSGASSERT("컨텐츠 코어 내부에 CreateContentsCoreDefine을 정의하지 않았습니다.");
		return;
	}

	Ptr(Core);

	if (nullptr == Core)
	{
		MSGASSERT("컨텐츠 코어 생성에 실패했습니다.");
		return;
	}
}

void UEngineCore::EngineStart(HINSTANCE _Instance, std::string_view _DllName)
{
	UEngineDebug::LeakCheck();

	WindowInit(_Instance);

	LoadContents(_DllName);

	UEngineWindow::WindowMessageLoop(
		[]()
		{
			// UEngineDebug::StartConsole();
			
			// 1. 그래픽카드 정보를 가져와서 Device와 Context를 생성하고
			Device.CreateDeviceAndContext();

			// 2. 윈도우 초기 세팅값 및 컨텐츠 리소스를 로드하고
			Core->EngineStart(Data);
			
			// 3. 윈도우 크기를 조절하고
			MainWindow.SetWindowPosAndScale(Data.WindowPos, Data.WindowSize);

			// 4. 윈도우 크기 정보를 바탕으로 백버퍼를 생성한다.
			Device.CreateBackBuffer(MainWindow);

			// 5. IMGUI 로드
			UEngineGUI::Init();
		},
		[]()
		{
			// 게임 실행
			EngineFrame();
		},
		[]()
		{
			// 게임 종료 시, 딱 한번만 호출
			EngineEnd();
		});	
}

// 헤더 순환 참조를 막기 위한 함수분리
std::shared_ptr<ULevel> UEngineCore::NewLevelCreate(std::string_view _Name)
{
	std::shared_ptr<ULevel> Ptr = std::make_shared<ULevel>();
	Ptr->SetName(_Name);

	LevelMap.insert({ _Name.data(), Ptr}); // 생성된 레벨은 모두 LevelMap에 저장

	std::cout << "NewLevelCreate" << std::endl;

	return Ptr;
}

void UEngineCore::OpenLevel(std::string_view _Name)
{
	if (false == LevelMap.contains(_Name.data()))
	{
		MSGASSERT(std::string(_Name) + " 은 생성되지 않은 레벨입니다. \n CreateLevel 함수를 사용해 레벨을 생성 후 OpenLevel 함수를 사용해야 합니다.");
		return;
	}

	NextLevel = LevelMap[_Name.data()];
}

void UEngineCore::EngineFrame()
{
	if (nullptr != NextLevel) // 레벨체인지할 Level이 존재하면
	{
		if (nullptr != CurLevel) // 현재 레벨이 종료되면서 할 일이 있으면 마무리 하고
		{
			CurLevel->LevelChangeEnd();
		}

		CurLevel = NextLevel; // 레벨을 바꾼다.

		CurLevel->LevelChangeStart(); // 새로운 레벨에서 처음 세팅할 작업을 먼저 진행하고
		NextLevel = nullptr; // NextLevel 포인터의 역할은 다했다.
		Timer.TimeStart(); // 델타 타임도 처음부터 다시 갱신한다. 혹시 모를 오류가 있을까봐
	}

	Timer.TimeCheck(); // 델타 타임 체크
	float DeltaTime = Timer.GetDeltaTime(); 

	UEngineInput::KeyCheck(DeltaTime); // 키입력
	
	// Core에서 Level이 관리하는 Actor, Renderer, Collision를 'Windows메시지루프'에서 돌려준다.
	CurLevel->Tick(DeltaTime); 
	CurLevel->Render(DeltaTime);
	// GUI랜더링은 기존 랜더링이 다 끝나고 해주는게 좋다.

	CurLevel->Collision(DeltaTime);
	CurLevel->Release(DeltaTime);
}

void UEngineCore::EngineEnd()
{
	UEngineGUI::Release();

	Device.Release();

	UEngineResources::Release();
	UEngineConstantBuffer::Release();

	CurLevel = nullptr;
	NextLevel = nullptr;
	LevelMap.clear();

	UEngineDebug::EndConsole();
}