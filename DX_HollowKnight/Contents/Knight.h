#pragma once
#include <EngineCore/Pawn.h>

// Ό³Έν : Player
class AKnight : public APawn
{
public:
	// constrcuter destructer
	AKnight();
	~AKnight();

	// delete Function
	AKnight(const AKnight& _Other) = delete;
	AKnight(AKnight&& _Other) noexcept = delete;
	AKnight& operator=(const AKnight& _Other) = delete;
	AKnight& operator=(AKnight&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void InputCheck(float _DeltaTime);

protected:

private:
	std::shared_ptr<class USpriteRenderer> BodyRenderer;
};

