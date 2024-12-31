#pragma once
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/TimeEvent.h>
#include <EngineCore/Actor.h>

// Ό³Έν :
class UEffectRenderer : public AActor
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

protected:

private:

};

