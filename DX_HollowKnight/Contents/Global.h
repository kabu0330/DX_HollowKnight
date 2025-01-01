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

enum class ELayer
{
	BACKGROUND = 0, // 배경
	MIDDLEGROUND = 100, // 배경과 오브젝트 사이
	COLLISION = 500, // 바닥, 벽
	FOREGROUND = 1000 // 플레이어를 가리는 오브젝트
};

enum class EKnightState
{
	IDLE,
	RUN,
	IDLE_TO_RUN,
	RUN_TO_IDLE,
	DASH,
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
	FOCUS,
	FOCUS_GET,
	FOCUS_END,
	FIREBALL_ANTIC,
	FIREBALL_CAST,
	DAMAGED,
	DEATH,
	DEATH_DAMAGE,
	DEATH_HEAD,
	MAX
};


