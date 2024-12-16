#pragma once
#include <EngineBase/Object.h>

// C
// #include <memory.h>
// C++
#include <memory>

// 설명 :
class UActorComponent : public UObject
{
public:
	// constrcuter destructer
	UActorComponent();
	~UActorComponent();

	// delete Function
	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;

	std::shared_ptr<class AActor> GetActor();

protected:

private:

	// 액터에 들어갈수 있다. 개념
	// std::shared_ptr<class AActor> 
	// std::weak_ptr
	class AActor* Actor;
};

