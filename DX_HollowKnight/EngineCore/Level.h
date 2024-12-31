#pragma once

#include <EngineBase/Object.h>
#include <EngineBase/EngineDebug.h>

// 설명 :
class ULevel : public UObject
{
public:
	// constrcuter destructer
	ENGINEAPI ULevel();
	ENGINEAPI ~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

	void LevelChangeStart();
	void LevelChangeEnd();


	void Tick(float _DeltaTime);
	void Render(float _DeltaTime);

	std::shared_ptr<class ACameraActor> GetMainCamera()
	{
		return GetCamera(0);
	}

	std::shared_ptr<class ACameraActor> GetCamera(int _Order)
	{
		if (false == Cameras.contains(_Order))
		{
			MSGASSERT("존재하지 않는 카메라를 사용하려고 했습니다.");
		}

		return Cameras[_Order];
	}

	template<typename EnumType>
	std::shared_ptr<class ACameraActor> SpawnCamera(EnumType _Order)
	{
		return SpawnCamera(static_cast<int>(_Order));
	}

	std::shared_ptr<class ACameraActor> SpawnCamera(int _Order);

	template<typename ActorType>
	std::shared_ptr<ActorType> SpawnActor()
	{
		static_assert(std::is_base_of_v<AActor, ActorType>, "액터를 상속받지 않은 클래스를 SpawnActor하려고 했습니다.");

		if (false == std::is_base_of_v<AActor, ActorType>)
		{
			MSGASSERT("액터를 상속받지 않은 클래스를 SpawnActor하려고 했습니다.");
			return nullptr;
		}

		char* ActorMemory = new char[sizeof(ActorType)];


		AActor* ActorPtr = reinterpret_cast<ActorType*>(ActorMemory);
		ActorPtr->World = this;

		ActorType* NewPtr = reinterpret_cast<ActorType*>(ActorMemory);

		std::shared_ptr<ActorType> NewActor(NewPtr = new(ActorMemory) ActorType());

		BeginPlayList.push_back(NewActor);

		return NewActor;
	}

	//                           0              100그룹
	void ChangeRenderGroup(int _CameraOrder, int _PrevGroupOrder, std::shared_ptr<class URenderer> _Renderer);

protected:

private:
	std::list<std::shared_ptr<class AActor>> BeginPlayList;

	std::list<std::shared_ptr<class AActor>> AllActorList;

	std::map<int, std::shared_ptr<class ACameraActor>> Cameras; // 0 : 메인카메라
};

