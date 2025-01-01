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
		if (UEngineInput::IsFree('Z')) // Ű�� ���� �����͸� 0�� �ؾ���
		{

		}
	}
}

void AKnight::ChangeNonCombatAnimation()
{
	if (true == CanAction())
	{
		return;
	}
	if (false == IsOnGround())
	{
		return;
	}
}

void AKnight::ChangeLookAnimation()
{
	if (false == IsOnGround())
	{
		return;
	}

	if (UEngineInput::IsPress(VK_DOWN))
	{
		FSM.ChangeState(EKnightState::LOOK_DOWN);
		return;
	}
	if (UEngineInput::IsPress(VK_UP))
	{
		FSM.ChangeState(EKnightState::LOOK_UP);
		return;
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

	ChangeLookAnimation();

	ChangeJumpAnimation();
	ChangeAttackAnimation(EKnightState::IDLE); // ���� ����
}

void AKnight::SetRun(float _DeltaTime)
{
	if (UEngineInput::IsFree(VK_LEFT) && UEngineInput::IsFree(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::RUN_TO_IDLE);
		return;
	}

	ChangeJumpAnimation();
	ChangeAttackAnimation(EKnightState::RUN); // ���� ����
}

void AKnight::SetIdleToRun(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::RUN);

	ChangeJumpAnimation();
	ChangeAttackAnimation(EKnightState::RUN); // ���� ����
}

void AKnight::SetRunToIdle(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::IDLE);

	if (UEngineInput::IsPress(VK_LEFT) || UEngineInput::IsPress(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::IDLE_TO_RUN);
		return;
	}

	ChangeJumpAnimation();
	ChangeAttackAnimation(EKnightState::RUN); // ���� ����
}

void AKnight::SetSlash(float _DeltaTime)
{
	CreateSlashEffect();
	ChangePrevAnimation();
}

void AKnight::SetUpSlash(float _DeltaTime)
{
	ChangePrevAnimation();
}

void AKnight::SetDownSlash(float _DeltaTime)
{
	ChangePrevAnimation();
}

void AKnight::SetJump(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::AIRBORN);

	ChangeAttackAnimation(EKnightState::AIRBORN);
}

void AKnight::SetAirborn(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::LAND);
}

void AKnight::SetLand(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::IDLE);
}

void AKnight::SetHardLand(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::IDLE);
}

void AKnight::SetLookDown(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::LOOK_DOWN_LOOP);
	if (UEngineInput::IsFree(VK_DOWN))
	{
		FSM.ChangeState(EKnightState::IDLE);
		return;
	}
}

void AKnight::SetLookDownLoop(float _DeltaTime)
{
	if (UEngineInput::IsFree(VK_DOWN))
	{
		FSM.ChangeState(EKnightState::IDLE);
		return;
	}
}

void AKnight::SetLookUp(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::LOOK_UP_LOOP);
	if (UEngineInput::IsFree(VK_UP))
	{
		FSM.ChangeState(EKnightState::IDLE);
		return;
	}
}

void AKnight::SetLookUpLoop(float _DeltaTime)
{
	if (UEngineInput::IsFree(VK_UP))
	{
		FSM.ChangeState(EKnightState::IDLE);
		return;
	}
}

void AKnight::SetDamage(float _DeltaTime)
{
	if (true == bIsOnGround)
	{
		ChangeNextAnimation(EKnightState::IDLE);
	}
	else
	{
		ChangeNextAnimation(EKnightState::AIRBORN);
	}
	
	// �ڷ� �з�����
	// ����Ʈ ���
}

void AKnight::SetDeath(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::DEATH_DAMAGE);
}

void AKnight::SetDeathDamage(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::DEATH_HEAD);
}

void AKnight::SetDeathHead(float _DeltaTime)
{
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

	// �̵� �ִϸ��̼�
	std::string Idle = "Idle";
	BodyRenderer->CreateAnimation(Idle, "Knight_Idle.png", 0, 8, IdleFrameTime);
	GlobalFunc::AutoScale(BodyRenderer, Idle);

	std::string Run = "Run";
	BodyRenderer->CreateAnimation(Run, Run, 0, 7, RunFrameTime);
	GlobalFunc::AutoScale(BodyRenderer, Run);

	std::string RunToIdle = "RunToIdle";
	BodyRenderer->CreateAnimation(RunToIdle, RunToIdle, 0, 5, ChangeFrameTime, false);
	GlobalFunc::AutoScale(BodyRenderer, RunToIdle);

	std::string IdleToRun = "IdleToRun";
	BodyRenderer->CreateAnimation(IdleToRun, IdleToRun, 0, 4, ChangeFrameTime, false);
	GlobalFunc::AutoScale(BodyRenderer, IdleToRun);

	std::string Jump = "Jump";
	BodyRenderer->CreateAnimation(Jump, Jump, 0, 7, RunFrameTime, false);
	GlobalFunc::AutoScale(BodyRenderer, Jump);

	std::string Airborn = "Airborn";
	BodyRenderer->CreateAnimation(Airborn, Airborn, 0, 2, RunFrameTime);
	GlobalFunc::AutoScale(BodyRenderer, Airborn);

	std::string Land = "Land";
	BodyRenderer->CreateAnimation(Land, Land, 0, 2, RunFrameTime);
	GlobalFunc::AutoScale(BodyRenderer, Land);

	std::string HardLand = "HardLand";
	BodyRenderer->CreateAnimation(HardLand, HardLand, 0, 9, RunFrameTime, false);
	GlobalFunc::AutoScale(BodyRenderer, HardLand);


	// ���� �ִϸ��̼�
	float StaticFrameTime = 0.15f;
	std::string LookDown = "LookDown";
	BodyRenderer->CreateAnimation(LookDown, LookDown, 0, 5, StaticFrameTime, false);
	GlobalFunc::AutoScale(BodyRenderer, LookDown);

	std::string LookDownLoop = "LookDownLoop";
	BodyRenderer->CreateAnimation(LookDownLoop, LookDownLoop, 0, 4, IdleFrameTime);
	GlobalFunc::AutoScale(BodyRenderer, LookDownLoop);

	std::string LookUp = "LookUp";
	BodyRenderer->CreateAnimation(LookUp, LookUp, 0, 5, StaticFrameTime, false);
	GlobalFunc::AutoScale(BodyRenderer, LookUp);

	std::string LookUpLoop = "LookUpLoop";
	BodyRenderer->CreateAnimation(LookUpLoop, LookUpLoop, 0, 4, IdleFrameTime);
	GlobalFunc::AutoScale(BodyRenderer, LookUpLoop);

	// ���� �ִϸ��̼�
	std::string Slash = "Slash";
	BodyRenderer->CreateAnimation(Slash, Slash, 0, 6, SlashFrameTime, false);
	GlobalFunc::AutoScale(BodyRenderer, Slash);

	std::string UpSlash = "UpSlash";
	BodyRenderer->CreateAnimation(UpSlash, UpSlash, 0, 6, SlashFrameTime, false);
	GlobalFunc::AutoScale(BodyRenderer, UpSlash);

	std::string DownSlash = "DownSlash";
	BodyRenderer->CreateAnimation(DownSlash, DownSlash, 0, 6, SlashFrameTime, false);
	GlobalFunc::AutoScale(BodyRenderer, DownSlash);

	std::string Damage = "Damage";
	BodyRenderer->CreateAnimation(Damage, Damage, 0, 0, HitStunDuration, false);
	GlobalFunc::AutoScale(BodyRenderer, Damage);


	float DeathFrameTime = 0.2f;

	std::string DeathDamage = "DeathDamage";
	BodyRenderer->CreateAnimation(DeathDamage, DeathDamage, 0, 6, DeathFrameTime, false);
	GlobalFunc::AutoScale(BodyRenderer, DeathDamage);

	std::string Death = "Death";
	BodyRenderer->CreateAnimation(Death, Death, 0, 12, DeathFrameTime, false);
	GlobalFunc::AutoScale(BodyRenderer, Death);

	std::string DeathHead = "DeathHead";
	BodyRenderer->CreateAnimation(DeathHead, DeathHead, 0, 0, DeathFrameTime, false);
	GlobalFunc::AutoScale(BodyRenderer, DeathHead);

	BodyRenderer->SetupAttachment(RootComponent);
}

void AKnight::SetFSM()
{
	// �̵� �ִϸ��̼�
	CreateState(EKnightState::IDLE, &AKnight::SetIdle, "Idle");
	CreateState(EKnightState::RUN, &AKnight::SetRun, "Run");
	CreateState(EKnightState::RUN_TO_IDLE, &AKnight::SetRunToIdle, "RunToIdle");
	CreateState(EKnightState::IDLE_TO_RUN, &AKnight::SetIdleToRun, "IdleToRun");
	CreateState(EKnightState::JUMP, &AKnight::SetJump, "Jump");
	CreateState(EKnightState::AIRBORN, &AKnight::SetAirborn, "Airborn");
	CreateState(EKnightState::LAND, &AKnight::SetLand, "Land");
	CreateState(EKnightState::HARD_LAND, &AKnight::SetHardLand, "HardLand");

	// ���� �ִϸ��̼�
	CreateState(EKnightState::LOOK_DOWN, &AKnight::SetLookDown, "LookDown");
	CreateState(EKnightState::LOOK_DOWN_LOOP, &AKnight::SetLookDownLoop, "LookDownLoop");
	CreateState(EKnightState::LOOK_UP, &AKnight::SetLookUp, "LookUp");
	CreateState(EKnightState::LOOK_UP_LOOP, &AKnight::SetLookUpLoop, "LookUpLoop");

	// ���� �ִϸ��̼�
	CreateState(EKnightState::SLASH, &AKnight::SetSlash, "Slash");
	CreateState(EKnightState::UP_SLASH, &AKnight::SetUpSlash, "UpSlash");
	CreateState(EKnightState::DOWN_SLASH, &AKnight::SetDownSlash, "DownSlash");

	CreateState(EKnightState::DAMAGED, &AKnight::SetDamage, "Damage");

	CreateState(EKnightState::DEATH, &AKnight::SetDeath, "Death");
	CreateState(EKnightState::DEATH_DAMAGE, &AKnight::SetDeathDamage, "DeathDamage");
	CreateState(EKnightState::DEATH_HEAD, &AKnight::SetDeathHead, "DeathHead");

	FSM.ChangeState(EKnightState::IDLE);
}

void AKnight::CreateState(EKnightState _State, StateCallback _CallbackFunc, std::string_view _AnimationName)
{
	FSM.CreateState(_State, std::bind(_CallbackFunc, this, std::placeholders::_1),
		[this, _AnimationName]()
		{
			std::string AnimationName = _AnimationName.data();
			BodyRenderer->ChangeAnimation(AnimationName);
		} );
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

// ���� �ִϸ��̼����� ����
void AKnight::ChangeNextAnimation(EKnightState _NextState)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(_NextState);
		return;
	}
}

// ���� �ִϸ��̼����� ����
void AKnight::ChangePrevAnimation()
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(NextState);
		return;
	}
}

AKnight::~AKnight()
{
}
