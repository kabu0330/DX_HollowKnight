#include "PreCompile.h"
#include "ContentsCore.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/Level.h>
#include <EngineCore/EngineTexture.h>
#include <EngineCore/EngineSprite.h>
#include "TitleGameMode.h"
#include "PlayGameMode.h"

// ���� ���� ������ �ش� Ŭ������ EngineCore�� �������� ��������� ������ ������ �ھ�� �����Ѵ�.
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

	UEngineCore::OpenLevel("Play");
}

void UContentsCore::LoadResourceDirectory()
{
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
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

	// Knight Effect
	LoadContentsResource("Image/Effect/Knight/SlashEffect");
	LoadContentsResource("Image/Effect/Knight/UpSlashEffect");
	LoadContentsResource("Image/Effect/Knight/DownSlashEffect");
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
	//for (size_t i = 0; i < 4; i++)
	//{
	//	if (true == UEngineInput::IsDown('1' + i))
	//	{
	//		Select = i + 1;
	//		break;
	//	}
	//}
	//
	//switch (Select)
	//{
	//case 0:
	//	//UEngineCore::OpenLevel("Play");
	//	break;
	//case 1:
	//	//UEngineCore::OpenLevel("Title");
	//default:
	//	break;
	//}
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
		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
		return;
	}
	Dir.Append(Path);
	UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
}

UContentsCore::UContentsCore()
{
}

UContentsCore::~UContentsCore()
{
}