#include "PreCompile.h"
#include "ContentsCore.h"
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

	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
			return;
		}
		Dir.Append("Image/Knight_Run");

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

void UContentsCore::CreateLevel()
{
	UEngineCore::CreateLevel<ATitleGameMode, APawn>("Title");
	UEngineCore::OpenLevel("Title");
	UEngineCore::CreateLevel<APlayGameMode, AKnight>("Play");
	UEngineCore::OpenLevel("Play");
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