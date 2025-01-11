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

	void CheckKnightPos();

	void ChangeEffect(std::string_view _AnimationName);
	void SetLocation(USceneComponent* _Knight, FVector _Pos, bool _Left = true);
	void SetLocation(FVector _Pos, bool _Left);
	void Release();

	std::shared_ptr<class USpriteRenderer> GetRenderer()
	{
		return KnightEffectRenderer;
	}

protected:

private:
	std::shared_ptr<class USpriteRenderer> KnightEffectRenderer;
	FVector KnightPos = { 0.0f, 0.0f, 0.0f };
};

