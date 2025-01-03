#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineCore/SpriteRenderer.h>

class GlobalFunc
{
public:
	static void AutoScale(std::shared_ptr<USpriteRenderer> _Renderer, std::string_view _AnimationName)
	{
		USpriteRenderer::FrameAnimation* Animation = _Renderer->FindAnimation(_AnimationName);
		//Animation->IsAutoScale = true;
		//Animation->AutoScaleRatio = 1.0f;
		_Renderer->SetAutoScaleRatio(1.0f);
	}

	// 방향전환이 필요한 경우
	static void SetLocation(std::shared_ptr<class USceneComponent> _RootComponent, std::shared_ptr<USpriteRenderer> _Renderer, FVector _OffsetPos , bool _Left, FVector _Rotation = { 0.0f, 0.0f, 0.0f })
	{
		FVector Pos = _RootComponent->GetTransformRef().RelativeLocation;
		FVector LRPos = _OffsetPos;
		if (true == _Left)
		{
			LRPos += Pos;
			_Renderer->SetRelativeLocation(LRPos);
			_Renderer->SetRotation(_Rotation);
			return;
		}
		else
		{
			LRPos -= Pos;
			_Renderer->SetRelativeLocation(-LRPos);
			_Renderer->SetRotation({ _Rotation.X, _Rotation.Y + 180.0f, _Rotation.Z });
			return;
		}
	}

	// 방향전환이 필요없는 경우
	static void SetLocationOffset(std::shared_ptr<class USceneComponent> _RootComponent, std::shared_ptr<USpriteRenderer> _Renderer, FVector _OffsetPos = {0.0f, 0.0f, 0.0f} , FVector _Rotation = {0.0f, 0.0f, 0.0f})
	{
		FVector Pos = _RootComponent->GetTransformRef().RelativeLocation;
		FVector LRPos = _OffsetPos;
		LRPos += Pos;

		_Renderer->SetRelativeLocation(LRPos);
		_Renderer->SetRotation(_Rotation);
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


