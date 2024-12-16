#pragma once

#include <EngineBase/Object.h>

#include "EngineCore.h"

// ���� :
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

	// ���� ���� ����Ǵ� ������ �Ǿ�����
	void LevelChangeStart();
	// ���� ��ü ��������
	void LevelChangeEnd();


	void Tick(float _DeltaTime);

	template<typename ActorType>
	std::shared_ptr<ActorType> SpawnActor()
	{
		// AMonster : public AActor
		// SpawnActor<AMonster>();

		std::shared_ptr<ActorType> NewActor = std::make_shared<ActorType>();

		//�����Ϸ��� �װ� �𸥴�.
		BeginPlayList.push_back(NewActor);

		return NewActor;
	}

protected:

private:
	std::list<std::shared_ptr<class AActor>> BeginPlayList;

	std::list<std::shared_ptr<class AActor>> AllActorList;

};

