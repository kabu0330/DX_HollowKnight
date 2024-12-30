#include "PreCompile.h"
#include "ContentsCore.h"
#include <EngineCore/Level.h>
#include <EngineCore/EngineTexture.h>
#include <EngineCore/EngineSprite.h>
#include "TitleGameMode.h"
#include "PlayGameMode.h"

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
	UEngineCore::OpenLevel("Title");
	UEngineCore::CreateLevel<APlayGameMode, AKnight>("Play");
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

	// Knight
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image/Knight/Run");
		UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image/Knight/Jump");
		UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image/Knight/RunToIdle");
		UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image/Knight/IdleToRun");
		UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image/Knight/Slash");
		UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image/Knight/UpSlash");
		UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image/Knight/DownSlash");
		UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image/Knight/Damage");
		UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image/Knight/Death");
		UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image/Knight/DeathDamage");
		UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Image/Knight/Airborn");
		UEngineSprite::CreateSpriteToFolder(Dir.GetPathToString());
	}
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
}

void UContentsCore::EngineEnd()
{
}

UContentsCore::UContentsCore()
{
}

UContentsCore::~UContentsCore()
{
}