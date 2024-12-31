#pragma once
#include <EngineBase/EngineMath.h>

enum class EKnightState
{
	IDLE,
	RUN,
	IDLE_TO_RUN,
	RUN_TO_IDLE,
	JUMP,
	AIRBORN,
	SLASH,
	UP_SLASH,
	DOWN_SLASH,
	DAMAGED,
	DEATH,
	DEATH_HEAD,
	MAX
};
