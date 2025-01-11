#include "PreCompile.h"
#include "MapEditorGUI.h"

#include <EngineBase/EngineMath.h>
#include <EnginePlatform/EngineInput.h>
#include <EnginePlatform/EngineWindow.h>
#include <EngineCore/EngineCore.h>
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

void MapEditorGUI::OnGUI()
{
	//float DeltaTime = UEngineCore::GetDeltaTime();
	//std::string DeltaTimeText = "FPS : " + std::to_string(static_cast<int>(1.0f / DeltaTime));
	//ImGui::Text(DeltaTimeText.c_str());

	Sprite = UEngineSprite::Find<UEngineSprite>("MapObjectResources");
	Texture = nullptr;
	TextureSRV = 0;

	SaveFile(); // ���̺�
	ImGui::SameLine();
	LoadFile(); // �ε�
	ImGui::SameLine();
	LoadTexture();
	ImGui::SameLine();
	DeleteAllActors(); // ��ü ����

	SetActorLocationButton();

	// ���� ����Ʈ
	ShowActorListBox(); 



	ShowSpriteImageButton();

	SpawnActor();


	RenderPreview();


}

void MapEditorGUI::SetActorLocationButton()
{
	float Gap = 30.0f;
	float WidthGap = 120.0f;
	float ItemWidth = 100.0f;

	ImGui::Columns(6, nullptr, false);

	ImGui::SetColumnWidth(0, Gap);
	ImGui::Text("X :");
	ImGui::NextColumn();

	ImGui::SetColumnWidth(1, WidthGap);
	ImGui::PushItemWidth(ItemWidth);
	ImGui::InputInt("#X", &XValue);
	ImGui::PopItemWidth();
	ImGui::NextColumn();

	ImGui::SetColumnWidth(2, Gap);
	ImGui::Text("Y :");
	ImGui::NextColumn();

	ImGui::SetColumnWidth(3, WidthGap);
	ImGui::PushItemWidth(ItemWidth);
	ImGui::InputInt("#Y", &YValue);
	ImGui::PopItemWidth();
	ImGui::NextColumn();

	ImGui::SetColumnWidth(4, Gap);
	ImGui::Text("Z :");
	ImGui::NextColumn();

	ImGui::SetColumnWidth(5, WidthGap);
	ImGui::PushItemWidth(ItemWidth);
	ImGui::InputInt("#Z", &ZValue);
	ImGui::PopItemWidth();
	ImGui::NextColumn();

	ImGui::Columns(1);
}

void MapEditorGUI::ShowSpriteImageButton()
{
	ImVec2 ChildSize = { 550.0f, 500.0f };
	ImGui::BeginChild("Scroll", ChildSize, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
	for (int i = 0; i < Sprite->GetSpriteCount(); i++)
	{
		std::string Name = std::to_string(i);
		
		Texture = Sprite->GetTexture(i);
		
		// �̹����� �������� ���� SRV�� �Ѱ��ش�.
		TextureSRV = reinterpret_cast<unsigned __int64>(Texture->GetSRV());

		if (ImGui::ImageButton(Name.c_str(), TextureSRV, { 60.0f, 60.0f }/* UV */))
		{
			// ���õ� �ؽ�ó�� ��������Ʈ ��ȣ ���
			SelectButtonIndex = i;
			
			// �̸������� �ؽ�ó �̹��� 
			SelectedTexture = TextureSRV;
			TextureScale = Texture->GetTextureSize();
			bIsPreview = true;
			int a = 0;
		}

		int SameLineIndex = 7;
		if (0 == i)
		{
			i += 1;
		}
		if (i != Sprite->GetSpriteCount() - 1 && i % SameLineIndex)
		{
			ImGui::SameLine();
		}
	}
	ImGui::EndChild();
}

void MapEditorGUI::RenderPreview()
{
	// IMGUI â������ �̸����� ���Ѵ�.
	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows))
	{
		bIsPreview = false; // ImGui â �ȿ� ����
	}
	else
	{
		bIsPreview = true;  // ImGui â �ۿ� ����
	}

	if (true == bIsPreview && 0 != SelectedTexture)
	{
		ImVec2 MousePos = ImGui::GetMousePos();
		ImVec2 Scale = { TextureScale.X * ScaleRatio, TextureScale.Y * ScaleRatio };
		ImGui::SetNextWindowPos(ImVec2(MousePos.x - (Scale.x / 2), MousePos.y - (Scale.y / 2)));
		ImGui::Begin("Preview", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Image(SelectedTexture, Scale);
		ImGui::End();
	}

	// �̸����� ����� ����
	ImGuiStyle& style = ImGui::GetStyle();

	// â ������ ���� �������� ����
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // RGBA
}

void MapEditorGUI::ShowActorListBox()
{
	std::vector<std::shared_ptr<AMapObject>> AllMonsterList = GetWorld()->GetAllActorArrayByClass<AMapObject>();

	std::vector<std::string> ArrString;
	for (std::shared_ptr<class AActor> Actor : AllMonsterList)
	{
		std::string Text = "";
		Text += Actor->GetName() + " ";
		Text += Actor->GetActorLocation().ToString();

		ArrString.push_back(Text);
	}

	std::vector<const char*> Arr;
	for (size_t i = 0; i < ArrString.size(); i++)
	{
		Arr.push_back(ArrString[i].c_str());
	}

	static int LastSelectedItem = -1; // ������ ���õ� �׸� ���� ����

	if (0 < Arr.size())
	{
		ImGui::PushItemWidth(450);
		ImGui::ListBox("AllActorList", &ObjectItem, &Arr[0], static_cast<int>(Arr.size()));

		//AllMonsterList[SelectItem]->GetRenderer()->ColorData.MulColor += {0.0f, 1.0f, 0.0f, 1.0f};

		// ���õ� �׸��� �ٲ���� ���
		if (LastSelectedItem != ObjectItem)
		{
			// ������ ���õ� ��ü�� ���� ����
			if (LastSelectedItem != -1)
			{
				AllMonsterList[LastSelectedItem]->GetRenderer()->ColorData.MulColor = { 1.0f, 1.0f, 1.0f, 1.0f }; // �⺻ ����
			}

			// ���õ� ��ü�� ���� ����
			if (ObjectItem != -1)
			{
				AllMonsterList[ObjectItem]->GetRenderer()->ColorData.MulColor += {0.0f, 1.0f, 0.0f, 1.0f}; // �ʷϻ�

			}

			// ���� ���� ������Ʈ
			LastSelectedItem = ObjectItem;
		}


		if (ObjectItem != -1)
		{
			if (true == ImGui::Button("Pos Update"))
			{
				// ���õ� ��ü�� ��ġ ����
				FVector Pos = AllMonsterList[ObjectItem]->GetActorLocation();
				if (0 != XValue)
				{
					Pos.X = static_cast<float>(XValue);
					AllMonsterList[ObjectItem]->SetActorLocation(Pos);
				}
				if (0 != YValue)
				{
					Pos.Y = static_cast<float>(YValue);
					AllMonsterList[ObjectItem]->SetActorLocation(Pos);
				}
			}

			if (true == ImGui::Button("Select Delete"))
			{
				ObjectItem = -1;
				LastSelectedItem = -1;
		
			}

			// ���� ��ư Ȱ��ȭ
			// AllMonsterList[ObjectItem]->
			if (true == ImGui::Button("Delete"))
			{
				AllMonsterList[ObjectItem]->Destroy();

				ObjectItem = -1;
				LastSelectedItem = -1; // ���� ���� �ʱ�ȭ
			}
		}
	}
}

void MapEditorGUI::DeleteAllActors()
{
	if (ImGui::Button("All Delete"))
	{
		std::list<std::shared_ptr<AMapObject>> AllMonsterList = GetWorld()->GetAllActorListByClass<AMapObject>();
		for (std::shared_ptr<AMapObject> Mon : AllMonsterList)
		{
			Mon->Destroy();
		}

	}
}

void MapEditorGUI::SpawnActor()
{
	if (true == UEngineInput::IsDown(VK_RBUTTON))
	{
		ESpawnList Select = static_cast<ESpawnList>(SelectItem);
		std::shared_ptr<class ACameraActor> Camera = GetWorld()->GetMainCamera();

		FVector Pos = Camera->ScreenMousePosToWorldPos();
		if (0 != XValue)
		{
			Pos.X = static_cast<float>(XValue);
		}
		if (0 != YValue)
		{
			Pos.Y = static_cast<float>(YValue);
		}

		ZValue -= 1;
		Pos.Z = static_cast<float>(ZValue);

		switch (Select)
		{
		case ESpawnList::Background:
			AMapEditorGameMode::GetMapObject() = GetWorld()->SpawnActor<ABackground>("Background");
			break;
		case ESpawnList::Object:
			AMapEditorGameMode::GetMapObject() = GetWorld()->SpawnActor<ABackground>("Background");
			break;
		default:
			break;
		}

		AMapEditorGameMode::GetMapObject()->SetSpriteIndex(SelectButtonIndex);

		// ������ ���?
		//FVector Scale = AMapEditorGameMode::GetMapObject()->GetRenderer()->GetScale();


		AMapEditorGameMode::GetMapObject()->GetRenderer()->SetAutoScaleRatio(ScaleRatio);
		AMapEditorGameMode::GetMapObject()->SetActorLocation(Pos);

		// �̸����� ����
		bIsPreview = true;

	}
}

void MapEditorGUI::SaveFile()
{
	if (true == ImGui::Button("Save"))
	{
		// 1. ������ ���� ��ġ �����ϰ�
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
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

			// ������ �������� ũ�⸸ŭ 
			Ser << static_cast<int>(AllMapObjectList.size());

			for (std::shared_ptr<AMapObject> Actor : AllMapObjectList)
			{
				// ���⼭ ������ �� �ִ� �����ʹ� ���⼭ �����ϰ�
				Ser << static_cast<int>(Actor->MapObjectTypeValue); 

				// Actor�� ������ �ִ� �����ʹ� Ŭ������ Serialize���� �����Ѵ�.
				Actor->Serialize(Ser);
			}

			UEngineFile NewFile = Dir.GetFile(ofn.lpstrFile);

			NewFile.FileOpen("wb");
			NewFile.Write(Ser);
		}
	}
}

void MapEditorGUI::LoadFile()
{
	if (true == ImGui::Button("Load"))
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("ContentsResources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
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
				int Index = -1;
				
				// ���⼭ ���� �����ʹ� ���� ������ ����
				Ser >> TypeValue;

				EMapObjectType ObjectType = static_cast<EMapObjectType>(TypeValue);


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

				// �������� ���⼭ ������ȭ�� �Ѵ�.
				AMapEditorGameMode::GetMapObject()->DeSerialize(Ser);
			}
		}
	}
}

void MapEditorGUI::LoadTexture()
{

}

MapEditorGUI::MapEditorGUI()
{
}

MapEditorGUI::~MapEditorGUI()
{
}
