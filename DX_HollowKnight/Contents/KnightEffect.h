#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class AKnightEffect : public AActor
{
public:
	// constrcuter destructer
	AKnightEffect();
	~AKnightEffect();

	// delete Function
	AKnightEffect(const AKnightEffect& _Other) = delete;
	AKnightEffect(AKnightEffect&& _Other) noexcept = delete;
	AKnightEffect& operator=(const AKnightEffect& _Other) = delete;
	AKnightEffect& operator=(AKnightEffect&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	void ChangeEffect(std::string_view _AnimationName);

protected:

private:
	std::shared_ptr<class USpriteRenderer> EffectRenderer;
};

