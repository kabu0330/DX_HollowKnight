#include "PreCompile.h"
#include "Knight.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>

AKnight::AKnight()
{
	CreateRenderer();
	GetWorld()->GetCamera(0);
}

void AKnight::BeginPlay()
{
	AActor::BeginPlay();
	SetFSM();
}

void AKnight::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	CheckDirection();
	FSM.Update(_DeltaTime);
	TimeElapsed(_DeltaTime);
	//InputCheck(_DeltaTime);
}

bool AKnight::CanAction()
{
	if (true == bIsDeath)
	{
		return false;
	}
	if (true == bIsCastingSpell)
	{
		return false;
	}
	if (true == bIsAttacking)
	{
		return false;
	}
	if (true == bIsBeingHit)
	{
		return false;
	}

	return true;
}

void AKnight::InputCheck(float _DeltaTime)
{
	if (UEngineInput::IsPress(VK_LEFT))
	{
		AddRelativeLocation(FVector{ -100.0f * _DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		AddRelativeLocation(FVector{ 100.0f * _DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('Z'))
	{
		AddRelativeLocation(FVector{ 0.0f, 100.0f * _DeltaTime, 0.0f });
	}
	if (UEngineInput::IsDown('C'))
	{
		AddRelativeLocation(FVector{ 1000.0f * _DeltaTime, 0.0f, 0.0f });
	}
}

void AKnight::CreateRenderer()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	float IdleFrameTime = 0.2f;
	float RunFrameTime = 0.1f;
	float ChangeFrameTime = 0.07f;
	float SlashFrameTime = 0.05f;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	//BodyRenderer->SetSprite("862.png", 0);
	std::string Idle = "Idle";
	BodyRenderer->CreateAnimation(Idle, "Knight_Idle.png", 0, 8, IdleFrameTime);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation(Idle);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

	std::string Run = "Run";
	BodyRenderer->CreateAnimation(Run, Run, 0, 7, RunFrameTime);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation(Run);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

	std::string RunToIdle = "RunToIdle";
	BodyRenderer->CreateAnimation(RunToIdle, RunToIdle, 0, 5, ChangeFrameTime, false);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation(RunToIdle);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

	std::string IdleToRun = "IdleToRun";
	BodyRenderer->CreateAnimation(IdleToRun, IdleToRun, 0, 4, ChangeFrameTime, false);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation(IdleToRun);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}
	std::string Slash = "Slash";
	BodyRenderer->CreateAnimation(Slash, Slash, 0, 6, SlashFrameTime, false);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation(Slash);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

	//BodyRenderer->ChangeAnimation("Idle");
	//BodyRenderer->SetRelativeScale3D({ -100, 100, 1.0f });
	BodyRenderer->SetupAttachment(RootComponent);
}

void AKnight::SetFSM()
{
	FSM.CreateState(EKnightState::IDLE, std::bind(&AKnight::SetIdle, this, std::placeholders::_1),
		[this]()
		{
			BodyRenderer->ChangeAnimation("Idle");
		}
	);
	FSM.CreateState(EKnightState::RUN, std::bind(&AKnight::SetRun, this, std::placeholders::_1),
		[this]()
		{
			BodyRenderer->ChangeAnimation("Run");
		}
	);
	FSM.CreateState(EKnightState::RUN_TO_IDLE, std::bind(&AKnight::SetRunToIdle, this, std::placeholders::_1),
		[this]()
		{
			BodyRenderer->ChangeAnimation("RunToIdle");
		}
	);
	FSM.CreateState(EKnightState::IDLE_TO_RUN, std::bind(&AKnight::SetIdleToRun, this, std::placeholders::_1),
		[this]()
		{
			BodyRenderer->ChangeAnimation("IdleToRun");
		}
	);
	FSM.CreateState(EKnightState::SLASH, std::bind(&AKnight::SetSlash, this, std::placeholders::_1),
		[this]()
		{
			BodyRenderer->ChangeAnimation("Slash");
		}
	);

	FSM.ChangeState(EKnightState::IDLE);
}

void AKnight::SetIdle(float _DeltaTime)
{

	if (UEngineInput::IsPress(VK_LEFT) || UEngineInput::IsPress(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::IDLE_TO_RUN);
		return;
	}

	ChangeAttackAnimation(EKnightState::IDLE); // 지상 공격
}

void AKnight::ChangeAttackAnimation(EKnightState _PrevState)
{
	if (true == CanAction())
	{
		if (UEngineInput::IsPress('Z'))
		{
			bIsAttacking = true;
			NextState = _PrevState;
			FSM.ChangeState(EKnightState::SLASH);
			return;
		}
	}
}

void AKnight::TimeElapsed(float _DeltaTime)
{
	float AttackCooldown = 0.6f;

	if (true == bIsAttacking)
	{
		AttackCooldownElapsed += _DeltaTime;
	}
	if (AttackCooldownElapsed >= AttackCooldown)
	{
		bIsAttacking = false;
		AttackCooldownElapsed = 0.0f;
	}
}

void AKnight::SetRun(float _DeltaTime)
{
	if (UEngineInput::IsFree(VK_LEFT) && UEngineInput::IsFree(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::RUN_TO_IDLE);
		return;
	}

	ChangeAttackAnimation(EKnightState::RUN); // 지상 공격
}

void AKnight::SetIdleToRun(float _DeltaTime)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(EKnightState::RUN);
		return;

	}
	ChangeAttackAnimation(EKnightState::RUN); // 지상 공격
}

void AKnight::SetRunToIdle(float _DeltaTime)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(EKnightState::IDLE);
		return;
	}
	ChangeAttackAnimation(EKnightState::RUN); // 지상 공격
}

void AKnight::SetSlash(float _DeltaTime)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		//bIsAttacking = false;
		FSM.ChangeState(NextState);
		return;
	}
}

void AKnight::SetUpSlash(float _DeltaTime)
{
}

void AKnight::SetDownSlash(float _DeltaTime)
{
}

void AKnight::SetAirborn(float _DeltaTime)
{
}

void AKnight::SetDamaged(float _DeltaTime)
{
}

void AKnight::SetDeath(float _DeltaTime)
{
}

void AKnight::CheckDirection()
{
	if (false == CanAction())
	{
		return;
	}
	if (UEngineInput::IsPress(VK_LEFT))
	{
		BodyRenderer->SetRotation({ 0.0f, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		BodyRenderer->SetRotation({ 0.0f, 180.0f, 0.0f });
	}
}

AKnight::~AKnight()
{
}
