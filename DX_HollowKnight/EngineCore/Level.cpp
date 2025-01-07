#include "PreCompile.h"
#include "Level.h"
#include "Actor.h"
#include "Renderer.h"
#include "Collision.h"
#include "EngineCore.h"
#include "EngineCamera.h"
#include "CameraActor.h"
#include "EngineGUI.h"

std::shared_ptr<class ACameraActor> ULevel::SpawnCamera(int _Order)
{
	std::shared_ptr<ACameraActor> Camera = std::make_shared<ACameraActor>();

	if (true == Cameras.contains(_Order))
	{
		MSGASSERT("이미 존재하는 카메라 Order입니다. 다른 값을 입력해주세요. : " + std::to_string(_Order));
		return nullptr;
	}

	Camera->BeginPlay(); 

	Cameras.insert({ _Order , Camera });
	return Camera;
}

ULevel::ULevel()
{
	SpawnCamera(0); // 레벨 생성 시, 기본적으로 0번 카메라는 만든다.
}

ULevel::~ULevel()
{
	BeginPlayList.clear();
	AllActorList.clear();
	Cameras.clear();
}

void ULevel::LevelChangeStart()
{

}

void ULevel::LevelChangeEnd()
{

}

void ULevel::Tick(float _DeltaTime)
{
	// Tick을 돌리기 전에, 해당 프레임에 생성된 Actor의 BeginPlay를 먼저 호출한다.
	std::list<std::shared_ptr<class AActor>>::iterator StartIter = BeginPlayList.begin();
	std::list<std::shared_ptr<class AActor>>::iterator EndIter = BeginPlayList.end();
	for (; StartIter != EndIter; )
	{
		std::shared_ptr<AActor> CurActor = *StartIter;

		if (false == CurActor->IsActive()) // 비활성화된 액터는 일단 생략
		{
			++StartIter;
			continue;
		}

		StartIter = BeginPlayList.erase(StartIter); // BeginPlay가 호출된 액터는 해당 리스트에서 제거한다.
		// 다음 프레임에는 BeginPlay가 또 호출될 일이 없다.

		CurActor->BeginPlay(); // 호출
		AllActorList.push_back(CurActor); // BeginPlay가 호출된 Actor만 Tick이 도는 ActorList에 들어간다.
	}

	// BeginPlay가 호출된 Actor
	for (std::shared_ptr<AActor> CurActor : AllActorList)
	{
		if (false == CurActor->IsActive()) // 비활성화된 Actor는 Tick을 돌리지 않는다.
		{
			continue;
		}

		CurActor->Tick(_DeltaTime);
	}
}

void ULevel::Render(float _DeltaTime)
{
	UEngineCore::GetDevice().RenderStart(); // 백버퍼 초기화 및 OM단계에서 사용할 RTV와 DSV 설정

	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : Cameras)
	{
		Camera.second->Tick(_DeltaTime);
		Camera.second->GetCameraComponent()->Render(_DeltaTime); 
	}

	{
		std::shared_ptr<class ACameraActor> Camera = GetMainCamera();

		// 충돌체 릴리즈
		for (std::pair<const std::string, std::list<std::shared_ptr<UCollision>>>& Group : Collisions)
		{
			std::list<std::shared_ptr<UCollision>>& List = Group.second;

			for (std::shared_ptr<UCollision>& _Collision : List)
			{
				if (false == _Collision->IsActive())
				{
					continue;
				}

				_Collision->DebugRender(Camera->GetCameraComponent().get(), _DeltaTime);
			}
		}
	}

	if (true == UEngineWindow::IsApplicationOn())
	{
		UEngineGUI::GUIRender();
		// IMGUI 랜더링
	}

	UEngineCore::GetDevice().RenderEnd(); // 스왑체인이 관리하는 백버퍼와 프론트버퍼를 교환(Swap) 
	// 프론트버퍼(윈도우 창)에 출력
}



void ULevel::ChangeRenderGroup(int _CameraOrder, int _PrevGroupOrder, std::shared_ptr<URenderer> _Renderer)
{
	if (false == Cameras.contains(_CameraOrder))
	{
		// 이거 왜만들었을까?
		// 카메라라서 이렇게 했네요
		// 카메라 먼저 만들어야 니가 랜더러를 관리할수 있다.
		MSGASSERT("존재하지 않는 카메라에 랜더러를 집어넣으려고 했습니다.");
		return;
	}

	std::shared_ptr<ACameraActor> Camera = Cameras[_CameraOrder];

	Camera->GetCameraComponent()->ChangeRenderGroup(_PrevGroupOrder, _Renderer);
}

void ULevel::CreateCollisionProfile(std::string_view _ProfileName)
{
	std::string UpperName = UEngineString::ToUpper(_ProfileName);

	Collisions[UpperName];
}

void ULevel::LinkCollisionProfile(std::string_view _LeftProfileName, std::string_view _RightProfileName)
{
	std::string LeftUpperName = UEngineString::ToUpper(_LeftProfileName);
	std::string RightUpperName = UEngineString::ToUpper(_RightProfileName);

	CollisionLinks[LeftUpperName].push_back(RightUpperName);
}

void ULevel::PushCollisionProfileEvent(std::shared_ptr<class URenderer> _Renderer)
{

}

void ULevel::ChangeCollisionProfileName(std::string_view _ProfileName, std::string_view _PrevProfileName, std::shared_ptr<UCollision> _Collision)
{
	if (false == Collisions.contains(_ProfileName.data()))
	{
		MSGASSERT("존재하지 않는 콜리전 그룹에 랜더러를 집어넣으려고 했습니다.");
		return;
	}

	std::string PrevUpperName = UEngineString::ToUpper(_PrevProfileName);

	if (_PrevProfileName != "")
	{
		std::list<std::shared_ptr<UCollision>>& PrevCollisionGroup = Collisions[PrevUpperName];
		PrevCollisionGroup.remove(_Collision);
	}

	std::string UpperName = UEngineString::ToUpper(_ProfileName);

	std::list<std::shared_ptr<UCollision>>& CollisionGroup = Collisions[UpperName];
	CollisionGroup.push_back(_Collision);
}

void ULevel::Collision(float _DeltaTime)
{
	// Monster Player 충돌체크 해야 한다.
	
	for (std::pair<const std::string, std::list<std::string>>& Links : CollisionLinks)
	{
		const std::string& LeftProfile = Links.first;

		std::list<std::string>& LinkSecond = Links.second;

		for (std::string& RightProfile : LinkSecond)
		{
			std::list<std::shared_ptr<class UCollision>>& LeftList = CheckCollisions[LeftProfile];
			std::list<std::shared_ptr<class UCollision>>& RightList = Collisions[RightProfile];

			for (std::shared_ptr<class UCollision>& LeftCollision : LeftList)
			{
				for (std::shared_ptr<class UCollision>& RightCollision : RightList)
				{
					LeftCollision->CollisionEventCheck(RightCollision);
				}
			}
		}
	}
}

void ULevel::Release(float _DeltaTime)
{
	for (std::pair<const int, std::shared_ptr<ACameraActor>>& Camera : Cameras)
	{
		Camera.second->GetCameraComponent()->Release(_DeltaTime);
	}

	{
		// 충돌체 릴리즈
		for (std::pair<const std::string, std::list<std::shared_ptr<UCollision>>>& Group : Collisions)
		{
			std::list<std::shared_ptr<UCollision>>& List = Group.second;

			std::list<std::shared_ptr<UCollision>>::iterator StartIter = List.begin();
			std::list<std::shared_ptr<UCollision>>::iterator EndIter = List.end();

			// 언리얼은 중간에 삭제할수 없어.
			for (; StartIter != EndIter; )
			{
				if (false == (*StartIter)->IsDestroy())
				{
					++StartIter;
					continue;
				}

				// 랜더러는 지울 필요가 없습니다.
				// (*RenderStartIter) 누가 지울 권한을 가졌느냐.
				// 컴포넌트의 메모리를 삭제할수 권한은 오로지 액터만 가지고 있다.
				StartIter = List.erase(StartIter);
			}
		}
	}

	{
		std::list<std::shared_ptr<AActor>>& List = AllActorList;

		std::list<std::shared_ptr<AActor>>::iterator StartIter = List.begin();
		std::list<std::shared_ptr<AActor>>::iterator EndIter = List.end();

		// 언리얼은 중간에 삭제할수 없어.
		for (; StartIter != EndIter; )
		{
			if (false == (*StartIter)->IsDestroy())
			{
				++StartIter;
				continue;
			}

			// 이제 delete도 필요 없다.
			StartIter = List.erase(StartIter);
		}
	}
}