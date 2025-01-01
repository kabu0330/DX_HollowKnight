#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineCore/SpriteRenderer.h>

class GlobalFunc
{
public:
	static void AutoScale(std::shared_ptr<USpriteRenderer> _Renderer, std::string_view _AnimationName)
	{
		USpriteRenderer::FrameAnimation* Animation = _Renderer->FindAnimation(_AnimationName);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}
};

enum class EKnightState
{
	IDLE,
	RUN,
	IDLE_TO_RUN,
	RUN_TO_IDLE,
	JUMP,
	AIRBORN,
	LAND,
	HARD_LAND,
	LOOK_DOWN,
	LOOK_DOWN_LOOP,
	LOOK_UP,
	LOOK_UP_LOOP,
	SLASH,
	UP_SLASH,
	DOWN_SLASH,
	DAMAGED,
	DEATH,
	DEATH_DAMAGE,
	DEATH_HEAD,
	MAX
};
