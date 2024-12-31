#pragma once
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/TimeEvent.h>

// Ό³Έν :
class UEffectRenderer : public USpriteRenderer
{
public:
	// constrcuter destructer
	UEffectRenderer();
	~UEffectRenderer();

	// delete Function
	UEffectRenderer(const UEffectRenderer& _Other) = delete;
	UEffectRenderer(UEffectRenderer&& _Other) noexcept = delete;
	UEffectRenderer& operator=(const UEffectRenderer& _Other) = delete;
	UEffectRenderer& operator=(UEffectRenderer&& _Other) noexcept = delete;

	void PlayAnimationAndDeActive(std::string_view _AnimationName, bool _Force = false)
	{
		ChangeAnimation(_AnimationName, _Force);
		
	}

protected:

private:

};

