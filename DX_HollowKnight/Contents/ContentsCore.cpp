#include "PreCompile.h"
#include "ContentsCore.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/Level.h>
#include <EngineCore/EngineTexture.h>
#include <EngineCore/EngineSprite.h>
#include "TitleGameMode.h"
#include "PlayGameMode.h"
#include "MapEditorMode.h"

// 상위 엔진 레벨에 해당 클래스를 EngineCore에 간접적인 제어권한을 가지는 컨텐츠 코어로 설정한다.
CreateContentsCoreDefine(UContentsCore);

void UContentsCore::EngineStart(UEngineInitData& _Data)
{
	SetWindowSize(_Data);
	LoadResourceDirectory();
	LoadFolder();
	LoadSprite();

	CreateLevel();
}

void UContentsCore::SetWindowSize(UEngineInitData& _Data)
{
	_Data.WindowPos = { 100, 100 };
	_Data.WindowSize = { 1280, 720 };
}

void UContentsCore::CreateLevel()
{
	UEngineCore::CreateLevel<ATitleGameMode, APawn>("Title");
	UEngineCore::CreateLevel<APlayGameMode, AKnight>("Play");
	UEngineCore::CreateLevel<AMapEditorMode, APawn>("MapEditor");

	//UEngineCore::OpenLevel("Title");
	//UEngineCore::OpenLevel("MapEditor");
	UEngineCore::OpenLevel("Play");
}

void UContentsCore::LoadResourceDirectory()
{
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image");
		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineTexture::Load(FilePath);
		}
	}

	{
		// 맵 리소스
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory("ContentsResources");
		Dir.Append("MapObjectResources");

		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile(true, { ".PNG", ".BMP", ".JPG" });
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineTexture::Load(FilePath);
		}

	}
	LoadContentsResource("MapObjectResources");

	// Knight
	LoadContentsResource("Image/Knight/Run");
	LoadContentsResource("Image/Knight/Jump");
	LoadContentsResource("Image/Knight/RunToIdle");
	LoadContentsResource("Image/Knight/IdleToRun");
	LoadContentsResource("Image/Knight/Slash");
	LoadContentsResource("Image/Knight/UpSlash");
	LoadContentsResource("Image/Knight/DownSlash");
	LoadContentsResource("Image/Knight/Damage");
	LoadContentsResource("Image/Knight/Death");
	LoadContentsResource("Image/Knight/DeathDamage");
	LoadContentsResource("Image/Knight/DeathHead");
	LoadContentsResource("Image/Knight/Airborn");
	LoadContentsResource("Image/Knight/Land");
	LoadContentsResource("Image/Knight/HardLand");
	LoadContentsResource("Image/Knight/LookDown");
	LoadContentsResource("Image/Knight/LookDownLoop");
	LoadContentsResource("Image/Knight/LookUp");
	LoadContentsResource("Image/Knight/LookUpLoop");
	LoadContentsResource("Image/Knight/Dash");
	LoadContentsResource("Image/Knight/Focus");
	LoadContentsResource("Image/Knight/FocusGet");
	LoadContentsResource("Image/Knight/FocusEnd");
	LoadContentsResource("Image/Knight/FireballAntic");
	LoadContentsResource("Image/Knight/FireballCast");

	// Knight Effect
	LoadContentsResource("Image/Effect/Knight/SlashEffect");
	LoadContentsResource("Image/Effect/Knight/UpSlashEffect");
	LoadContentsResource("Image/Effect/Knight/DownSlashEffect");
	LoadContentsResource("Image/Effect/Knight/FocusEffect");
	LoadContentsResource("Image/Effect/Knight/FocusEffectEnd");
}

void UContentsCore::LoadFolder()
{
	UEngineDirectory TitleMain;
	TitleMain.MoveParentToDirectory("ContentsResources//Image//Title");
	TitleMain.Append("TitleBackGround");
	//UImageManager::GetInst().LoadFolder(TitleMain.GetPathToString());
}

void UContentsCore::LoadSprite()
{
	UEngineSprite::CreateSpriteToMeta("Knight_Idle.png", ".smeta");

}

void UContentsCore::EngineTick(float _DeltaTime)
{
	CheckInput();
	SwitchGameMode();
}

void UContentsCore::EngineEnd()
{
}

void UContentsCore::LoadContentsResource(std::string_view _Path)
{
	std::string Path = _Path.data();
	UEngineDirectory Dir;
	if (false == Dir.MoveParentToDirectory("ContentsResources"))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}
	Dir.Append(Path);
	UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
}

void UContentsCore::CheckInput()
{
	for (int i = 0; i < 4; i++)
	{
		if (true == UEngineInput::IsDown('1' + i))
		{
			Select = i + 1;
			break;
		}
	}
}

void UContentsCore::SwitchGameMode()
{
	switch (Select)
	{
	case 0:
	{
		break;
	}
	default:
		break;
	}
}

UContentsCore::UContentsCore()
{
}

UContentsCore::~UContentsCore()
{
}