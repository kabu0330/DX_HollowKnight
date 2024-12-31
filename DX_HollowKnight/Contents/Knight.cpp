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
	EndAnimationEffect();
	//InputCheck(_DeltaTime);
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

void AKnight::EndAnimationEffect()
{
	if (nullptr == EffectRenderer)
	{
		return; 
	}

	if (true == EffectRenderer->IsActive())
	{
		if (true == EffectRenderer->IsCurAnimationEnd())
		{
			EffectRenderer->SetActive(false);
		}
	}
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

bool AKnight::CanJump()
{
	if (false == CanAction())
	{
		return false;
	}
	if (false == IsOnGround())
	{
		return false;
	}
	return true;
}

bool AKnight::IsOnGround()
{
	if (false == bIsOnGround)
	{
		return false;
	}
	return true;
}

void AKnight::ChangeJumpAnimation()
{
	if (true == CanJump())
	{
		if (UEngineInput::IsPress('Z'))
		{
			bIsOnGround = false;
			FSM.ChangeState(EKnightState::JUMP);
			return;
		}
		if (UEngineInput::IsFree('Z')) // 키를 떼면 업벡터를 0로 해야함
		{

		}
	}
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

void AKnight::CreateSlashEffect()
{
	float SlashFrameTime = 0.1f;

	EffectRenderer = CreateDefaultSubObject<USpriteRenderer>();
	std::string SlashEffect = "SlashEffect";
	EffectRenderer->CreateAnimation(SlashEffect, SlashEffect, 0, 5, SlashFrameTime, false);
	{
		USpriteRenderer::FrameAnimation* Animation = EffectRenderer->FindAnimation(SlashEffect);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}
	EffectRenderer->ChangeAnimation(SlashEffect);
	EffectRenderer->SetActive(true);

	EffectRenderer->SetupAttachment(RootComponent);
}

void AKnight::ChangeAttackAnimation(EKnightState _PrevState)
{
	if (true == CanAction())
	{
		if (UEngineInput::IsPress('X') && UEngineInput::IsPress(VK_UP))
		{
			bIsAttacking = true;
			NextState = _PrevState;
			FSM.ChangeState(EKnightState::UP_SLASH);
			return;
		}
		if (UEngineInput::IsPress('X') && UEngineInput::IsPress(VK_DOWN))
		{
			bIsAttacking = true;
			NextState = _PrevState;
			FSM.ChangeState(EKnightState::DOWN_SLASH);
			return;
		}
		if (UEngineInput::IsPress('X'))
		{
			bIsAttacking = true;
			NextState = _PrevState;
			FSM.ChangeState(EKnightState::SLASH);
			return;
		}
	}
}

void AKnight::SetIdle(float _DeltaTime)
{
	bIsOnGround = true;

	if (UEngineInput::IsPress(VK_LEFT) || UEngineInput::IsPress(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::IDLE_TO_RUN);
		return;
	}

	ChangeJumpAnimation();
	ChangeAttackAnimation(EKnightState::IDLE); // 지상 공격
}

void AKnight::SetRun(float _DeltaTime)
{
	if (UEngineInput::IsFree(VK_LEFT) && UEngineInput::IsFree(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::RUN_TO_IDLE);
		return;
	}

	ChangeJumpAnimation();
	ChangeAttackAnimation(EKnightState::RUN); // 지상 공격
}

void AKnight::SetIdleToRun(float _DeltaTime)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(EKnightState::RUN);
		return;
	}

	ChangeJumpAnimation();
	ChangeAttackAnimation(EKnightState::RUN); // 지상 공격
}

void AKnight::SetRunToIdle(float _DeltaTime)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(EKnightState::IDLE);
		return;
	}

	ChangeJumpAnimation();
	ChangeAttackAnimation(EKnightState::RUN); // 지상 공격
}

void AKnight::SetSlash(float _DeltaTime)
{
	CreateSlashEffect();
	ChangePrevAnimation();
}

void AKnight::SetUpSlash(float _DeltaTime)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(NextState);
		return;
	}
}

void AKnight::SetDownSlash(float _DeltaTime)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(NextState);
		return;
	}
}

void AKnight::SetJump(float _DeltaTime)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(EKnightState::AIRBORN);
		return;
	}

	ChangeAttackAnimation(EKnightState::AIRBORN);
}

void AKnight::SetAirborn(float _DeltaTime)
{
	// 땅에 닿으면 땅에 닿는 모션 이후에 Idle로 전환해야 할 듯
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(EKnightState::IDLE);
		return;
	}
}

void AKnight::SetDamage(float _DeltaTime)
{
}

void AKnight::SetDeath(float _DeltaTime)
{
}

void AKnight::ChangePrevAnimation()
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(NextState);
		return;
	}
}

void AKnight::CreateRenderer()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	float IdleFrameTime = 0.3f;
	float RunFrameTime = 0.1f;
	float ChangeFrameTime = 0.07f;
	float SlashFrameTime = 0.05f;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();

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

	std::string Jump = "Jump";
	BodyRenderer->CreateAnimation(Jump, Jump, 0, 7, RunFrameTime, false);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation(Jump);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

	std::string Airborn = "Airborn";
	BodyRenderer->CreateAnimation(Airborn, Airborn, 0, 2, RunFrameTime);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation(Airborn);
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

	std::string UpSlash = "UpSlash";
	BodyRenderer->CreateAnimation(UpSlash, UpSlash, 0, 6, SlashFrameTime, false);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation(UpSlash);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

	std::string DownSlash = "DownSlash";
	BodyRenderer->CreateAnimation(DownSlash, DownSlash, 0, 6, SlashFrameTime, false);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation(DownSlash);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

	std::string Damage = "Damage";
	BodyRenderer->CreateAnimation(Damage, Damage, 0, 0, HitStunDuration, false);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation(Damage);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

	float DeathFrameTime = 0.2f;

	std::string Death = "Death";
	BodyRenderer->CreateAnimation(Death, Death, 0, 12, DeathFrameTime, false);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation(Death);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

	std::string DeathDamage = "DeathDamage";
	BodyRenderer->CreateAnimation(DeathDamage, DeathDamage, 0, 6, DeathFrameTime, false);
	{
		USpriteRenderer::FrameAnimation* Animation = BodyRenderer->FindAnimation(DeathDamage);
		Animation->IsAutoScale = true;
		Animation->AutoScaleRatio = 1.0f;
	}

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
	FSM.CreateState(EKnightState::JUMP, std::bind(&AKnight::SetJump, this, std::placeholders::_1),
		[this]()
		{
			BodyRenderer->ChangeAnimation("Jump");
		}
	);
	FSM.CreateState(EKnightState::AIRBORN, std::bind(&AKnight::SetAirborn, this, std::placeholders::_1),
		[this]()
		{
			BodyRenderer->ChangeAnimation("Airborn");
		}
	);
	FSM.CreateState(EKnightState::SLASH, std::bind(&AKnight::SetSlash, this, std::placeholders::_1),
		[this]()
		{
			BodyRenderer->ChangeAnimation("Slash");
		}
	);
	FSM.CreateState(EKnightState::UP_SLASH, std::bind(&AKnight::SetUpSlash, this, std::placeholders::_1),
		[this]()
		{
			BodyRenderer->ChangeAnimation("UpSlash");
		}
	);
	FSM.CreateState(EKnightState::DOWN_SLASH, std::bind(&AKnight::SetDownSlash, this, std::placeholders::_1),
		[this]()
		{
			BodyRenderer->ChangeAnimation("DownSlash");
		}
	);
	FSM.CreateState(EKnightState::DAMAGED, std::bind(&AKnight::SetDamage, this, std::placeholders::_1),
		[this]()
		{
			BodyRenderer->ChangeAnimation("Damage");
		}
	);
	FSM.CreateState(EKnightState::DEATH, std::bind(&AKnight::SetDeath, this, std::placeholders::_1),
		[this]()
		{
			BodyRenderer->ChangeAnimation("Death");
		}
	);

	FSM.ChangeState(EKnightState::IDLE);
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
