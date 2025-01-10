#include "PreCompile.h"
#include "MapEditorGUI.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/SceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>
#include <EngineCore/EngineCamera.h>

#include "MapObject.h"
#include "Background.h"
#include "MapEditorGameMode.h"

enum class ESpawnList
{
	Background,
	Object,
};

MapEditorGUI::MapEditorGUI()
{
}

MapEditorGUI::~MapEditorGUI()
{
}

void MapEditorGUI::OnGUI()
{
	std::shared_ptr<UEngineSprite> Sprite = UEngineSprite::Find<UEngineSprite>("MapObjectResources");
	UEngineTexture* Texture = nullptr;
	ImTextureID Id = 0;


	for (int i = 0; i < Sprite->GetSpriteCount(); i++)
	{
		std::string Name = std::to_string(i);

		Texture = Sprite->GetTexture(i);

		// 이미지를 가져오기 위해 SRV를 넘겨준다.
		Id = reinterpret_cast<unsigned __int64>(Texture->GetSRV());

		if (ImGui::ImageButton(Name.c_str(), Id, { 60.0f, 60.0f }/* UV */))
		{
			SelectButtonIndex = i;
			int a = 0;
		}

		int SameLineIndex = 5;
		if (0 == i)
		{
			i += 1;
		}
		if (i != Sprite->GetSpriteCount() - 1 && i % SameLineIndex)
		{
			ImGui::SameLine();
		}
	}

	if (true == UEngineInput::IsDown(VK_LBUTTON))
	{
		ESpawnList Select = static_cast<ESpawnList>(SelectItem);
		std::shared_ptr<class ACameraActor> Camera = GetWorld()->GetMainCamera();
		FVector Pos = Camera->ScreenMousePosToWorldPos();
		Pos.Z = 0.0f;



		switch (Select)
		{
		case ESpawnList::Background:
			AMapEditorGameMode::GetMapObject() = GetWorld()->SpawnActor<ABackground>("Background");
			break;
		case ESpawnList::Object:
			AMapEditorGameMode::GetMapObject() = GetWorld()->SpawnActor<ABackground>("Object");
			break;
		default:
			break;
		}
		AMapEditorGameMode::GetMapObject()->SetSpriteIndex(SelectButtonIndex);

	}

	if (true == UEngineInput::IsDown(VK_RBUTTON))
	{
		ESpawnList Select = static_cast<ESpawnList>(SelectItem);
		std::shared_ptr<class ACameraActor> Camera = GetWorld()->GetMainCamera();
		FVector Pos = Camera->ScreenMousePosToWorldPos();
		Pos.Z = 0.0f;

		switch (Select)
		{
		case ESpawnList::Background:
			AMapEditorGameMode::GetMapObject() = GetWorld()->SpawnActor<ABackground>("Background");
			break;
		case ESpawnList::Object:
			AMapEditorGameMode::GetMapObject() = GetWorld()->SpawnActor<ABackground>("Object");
			break;
		default:
			break;
		}

		AMapEditorGameMode::GetMapObject()->SetSpriteIndex(SelectButtonIndex);

		FVector Scale = AMapEditorGameMode::GetMapObject()->GetRenderer()->GetScale();


		AMapEditorGameMode::GetMapObject()->SetActorLocation(Pos);

	}

	{
		std::vector<std::shared_ptr<AMapObject>> AllMonsterList = GetWorld()->GetAllActorArrayByClass<AMapObject>();

		std::vector<std::string> ArrString;
		for (std::shared_ptr<class AActor> Actor : AllMonsterList)
		{
			ArrString.push_back(Actor->GetName());
		}

		std::vector<const char*> Arr;
		for (size_t i = 0; i < ArrString.size(); i++)
		{
			Arr.push_back(ArrString[i].c_str());
		}


		if (0 < Arr.size())
		{
			ImGui::ListBox("AllActorList", &ObjectItem, &Arr[0], Arr.size());

			// AllMonsterList[SelectItem]->Destroy();

			if (ObjectItem != -1)
			{
				// AllMonsterList[ObjectItem]->
				if (true == ImGui::Button("Delete"))
				{
					AllMonsterList[ObjectItem]->Destroy();
					ObjectItem = -1;
				}
			}

		}

		if (ImGui::Button("EditObjectDelete"))
		{
			std::list<std::shared_ptr<AMapObject>> AllMonsterList = GetWorld()->GetAllActorListByClass<AMapObject>();
			for (std::shared_ptr<AMapObject> Mon : AllMonsterList)
			{
				Mon->Destroy();
			}

		}

		if (true == ImGui::Button("Save"))
		{
			// 1. 저장할 폴더 위치 지정하고
			UEngineDirectory Dir;
			if (false == Dir.MoveParentToDirectory("ContentsResources"))
			{
				MSGASSERT("리소스 폴더를 찾지 못했습니다.");
				return;
			}
			Dir.Append("MapData");
			std::string InitPath = Dir.GetPathToString();

			OPENFILENAME ofn;       // common dialog box structure
			char szFile[260] = { 0 };       // if using TCHAR macros
			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = nullptr;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = ("All\0*.*\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrDefExt = "MapData";
			ofn.lpstrInitialDir = InitPath.c_str();
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetSaveFileNameA(&ofn) == TRUE)
			{
				std::list<std::shared_ptr<AMapObject>> AllMapObjectList = GetWorld()->GetAllActorListByClass<AMapObject>();

				UEngineSerializer Ser;

				Ser << static_cast<int>(AllMapObjectList.size());

				for (std::shared_ptr<AMapObject> Actor : AllMapObjectList)
				{

					Ser << static_cast<int>(Actor->MapObjectTypeValue);
					// 여기 저장된다는 이야기
					Actor->Serialize(Ser);
				}

				UEngineFile NewFile = Dir.GetFile(ofn.lpstrFile);

				NewFile.FileOpen("wb");
				NewFile.Write(Ser);
			}
		}

		if (true == ImGui::Button("Load"))
		{
			UEngineDirectory Dir;
			if (false == Dir.MoveParentToDirectory("ContentsResources"))
			{
				MSGASSERT("리소스 폴더를 찾지 못했습니다.");
				return;
			}
			Dir.Append("MapData");
			std::string InitPath = Dir.GetPathToString();

			OPENFILENAME ofn;       // common dialog box structure
			char szFile[260] = { 0 };       // if using TCHAR macros
			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = nullptr;
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = ("All\0*.*\0Text\0*.MapData\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = InitPath.c_str();
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileNameA(&ofn) == TRUE)
			{
				UEngineFile NewFile = Dir.GetFile(ofn.lpstrFile);
				UEngineSerializer Ser;

				NewFile.FileOpen("rb");
				NewFile.Read(Ser);

				int Count = 0;

				Ser >> Count;

				for (size_t i = 0; i < Count; i++)
				{
					int TypeValue = 0;
					Ser >> TypeValue;

					EMapObjectType ObjectType = static_cast<EMapObjectType>(TypeValue);

					//std::shared_ptr<AMapObject> MapObject = nullptr;

					switch (ObjectType)
					{
					case BACKGROUND_COLOR:
						AMapEditorGameMode::GetMapObject() = GetWorld()->SpawnActor<ABackground>();
						break;
					case BACKGROUND_OBJECT:
						AMapEditorGameMode::GetMapObject() = GetWorld()->SpawnActor<ABackground>();
						break;
					case COLLISION_OBJECT:
						break;
					case NONE_COLLISION_OBJECT:
						break;
					case MAX:
						break;
					default:
						break;
					}

					AMapEditorGameMode::GetMapObject()->DeSerialize(Ser);
				}
			}
		}
	}
}


