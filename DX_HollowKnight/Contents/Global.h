#pragma once
#include <EngineBase/EngineMath.h>

enum class EKnightState
{
	IDLE,
	RUN,
	IDLE_TO_RUN,
	RUN_TO_IDLE,
	JUMP,
	SLASH,
	UP_SLASH,
	DOWN_SLASH,
	DAMAGED,
	AIRBORN,
	DEATH,
	DEATH_HEAD,
	MAX
};
