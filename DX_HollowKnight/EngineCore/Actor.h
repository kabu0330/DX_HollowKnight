#pragma once
#include <EngineBase/Object.h>


// ���ϱ����� �̾߱��� ���ô�.
// ���� :
// �𸮾󿡼� Actor�� ���� Ʈ�������� ������ �ʴ´�.
class AActor : public UObject
{
	friend class ULevel;

public:
	// constrcuter destructer
	ENGINEAPI AActor();
	ENGINEAPI ~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	ENGINEAPI virtual void BeginPlay();
	ENGINEAPI virtual void Tick(float _DeltaTime);

	virtual void LevelChangeStart() {}
	virtual void LevelChangeEnd() {}


	// �̳༮ �� ���� 
	void CreateDefaultSubObject()
	{

	}

	ULevel* GetWorld()
	{
		return World;
	}

protected:

private:
	// �ʱ�ȭ �ϸ� �ȵ˴ϴ�.
	ULevel* World;

	// std::list<std::shared_ptr<class UActorComponent>>
	std::shared_ptr<class USceneComponent> RootComponent;
	// std::list<std::shared_ptr<class USceneComponent>> SceneComponentLists;

	// ���� ó������ ����� SceneComponent ������ Root�� �˴ϴ�.
	// �¸� ActorComponent�� �и��Ұų�.
	std::list<std::shared_ptr<class UActorComponent>> ActorComponentList;
};

