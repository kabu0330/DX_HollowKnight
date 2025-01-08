#include "PreCompile.h"
#include "Knight.h"

#include <EnginePlatform/EngineInput.h>
#include <EnginePlatform/EngineWinImage.h>
#include <EngineCore/SceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>

#include "KnightEffect.h"
#include "Room.h"

AKnight* AKnight::MainPawn = nullptr;

AKnight::AKnight()
{
	CreateRenderer();

	MainPawn = this;

	Velocity = 400.0f;
	InitVelocity = Velocity;
	DashSpeed = Velocity * 3.0f;
	JumpForce = InitJumpForce;

	SetActorLocation({ 1000.0f, -2185.0f });

	// Debug
	BodyRenderer->BillboardOn();
	DebugNonGravity = true;
}

void AKnight::BeginPlay()
{
	AActor::BeginPlay();
	SetFSM();
}

void AKnight::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	this;
	CheckDirection();
	FSM.Update(_DeltaTime);
	TimeElapsed(_DeltaTime);
	EndAnimationEffect();

	SetCameraPosition();
	DebugInput(_DeltaTime);

}

void AKnight::CheckGround(FVector _Gravity)
{
	if (true == DebugNonGravity)
	{
		return;
	}
	std::shared_ptr<ARoom> CurRoom = ARoom::GetCurRoom();
	if (nullptr == CurRoom)
	{
		return;
	}

	CurRoom->CheckGround(_Gravity);
}

void AKnight::Gravity(float _DeltaTime)
{
	if (true == DebugNonGravity)
	{
		return;
	}

	if (false == bIsOnGround)
	{
		AddRelativeLocation(GravityForce * _DeltaTime);
		GravityForce += FVector::DOWN * GravityValue * _DeltaTime;
	}
	else
	{
		GravityForce = FVector::ZERO;
	}
}

void AKnight::Move(float _DeltaTime)
{
	if (false == bIsDashing)
	{
		Velocity = InitVelocity;

	}
	if (UEngineInput::IsDown('C'))
	{
		Velocity = DashSpeed;
	}

	if (true == bIsDashing)
	{
		return;
	}

	if (UEngineInput::IsPress(VK_LEFT))
	{
		AddRelativeLocation(FVector{ -Velocity * _DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		AddRelativeLocation(FVector{ Velocity * _DeltaTime, 0.0f, 0.0f });
	}

	// Debug Input
	if (UEngineInput::IsPress(VK_UP))
	{
		AddRelativeLocation(FVector{ 0.0f, Velocity * _DeltaTime, 0.0f });
	}
	if (UEngineInput::IsPress(VK_DOWN))
	{
		AddRelativeLocation(FVector{ 0.0f, -Velocity * _DeltaTime, 0.0f });
	}

	// 미래의 위치를 파악해서 이동을 취소시킬 방법을 생각해보기
	if (true == DebugNonGravity)
	{
		return;
	}

	while (true)
	{
		if (nullptr == ARoom::GetCurRoom())
		{
			break;
		}

		UColor GroundColor = ARoom::GetCurRoom()->GetPixelCollisionImage().GetColor({ GetActorTransform().RelativeLocation.X, -GetActorTransform().RelativeLocation.Y });
		FVector Pos = { GetActorTransform().RelativeLocation.X, -GetActorTransform().RelativeLocation.Y };
		UColor White = UColor{ 255, 255, 255, 255 };
		UColor Black = UColor{ 0, 0, 0, 0 };

		
		if (GroundColor == Black)
		{
			UEngineDebug::OutPutString("On Ground");
			GravityForce = FVector::ZERO;
			//AddRelativeLocation(FVector::UP);			
		}
		else if (GroundColor == White || GroundColor == UColor(255, 255, 255, 0))
		{
			UEngineDebug::OutPutString("Airborn");
			break;
		}
		else
		{
			break;
		}
	}
}

void AKnight::SetCameraPosition()
{
	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetCamera(0);
	FVector Pos = RootComponent->GetTransformRef().RelativeLocation;
	FVector ScreenSize = UEngineCore::GetScreenScale();
	CameraPos = { Pos.X, Pos.Y + ScreenSize.Y * 0.35f };
	Camera->SetActorLocation(CameraPos);
}

void AKnight::TimeElapsed(float _DeltaTime)
{
	float AttackCooldown = 0.6f;

	if (true == bIsAttacking)
	{
		AttackCooldownElapsed += _DeltaTime;
		if (AttackCooldownElapsed >= AttackCooldown)
		{
			bIsAttacking = false;
			bIsShowEffect = false;
			AttackCooldownElapsed = 0.0f;
		}
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

void AKnight::ChangeDash()
{
	if (false == CanAction())
	{
		return;
	}

	if (UEngineInput::IsDown('C'))
	{
		bIsDashing = true;
		FSM.ChangeState(EKnightState::DASH);
		return;
	}
}

void AKnight::CastFocus()
{
	if (false == CanAction())
	{
		return;
	}

	if (UEngineInput::IsPress('A'))
	{
		float PressTime = UEngineInput::IsPressTime('A');
		float TriggerTime = 0.5f;

		if (PressTime >= TriggerTime)
		{
			FSM.ChangeState(EKnightState::FOCUS);
			return;
		}
	}
}

void AKnight::CastFireball()
{
	if (false == CanAction())
	{
		return;
	}
	if (UEngineInput::IsUp('A'))
	{
		FSM.ChangeState(EKnightState::FIREBALL_ANTIC);
		return;
	}
}

bool AKnight::IsOnGround()
{
	if (false == bIsOnGround)
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

void AKnight::Jump(float _DeltaTime)
{
	if (true == CanAction())
	{
		if (UEngineInput::IsPress('Z'))
		{		
			float JumpAccTime = 0.4f;
			float JumpKeyDuration = UEngineInput::IsPressTime('Z');
			if (JumpAccTime >= JumpKeyDuration)
			{
				JumpForce += 200.0f;
				float JumpForceMax = 1000.0f;
				if (JumpForce >= JumpForceMax)
				{
					JumpForce = JumpForceMax;
				}
			}
			AddRelativeLocation(FVector{ 0.0f, JumpForce * _DeltaTime, 0.0f });
		}
	}
}

void AKnight::ChangeJumpAnimation()
{
	if (true == CanJump())
	{
		if (false == IsOnGround())
		{
			return;
		}
		if (UEngineInput::IsPress('Z'))
		{
			FSM.ChangeState(EKnightState::JUMP);
			return;
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
		float PressTime = UEngineInput::IsPressTime(VK_DOWN);
		float TriggerTime = 0.5f;

		if (PressTime >= TriggerTime)
		{
			FSM.ChangeState(EKnightState::LOOK_DOWN);
			return;
		}
	}
	//if (UEngineInput::IsPress(VK_UP))
	//{
	//	float PressTime = UEngineInput::IsPressTime(VK_UP);
	//	float TriggerTime = 0.5f;

	//	if (PressTime >= TriggerTime)
	//	{
	//		FSM.ChangeState(EKnightState::LOOK_UP);
	//		return;
	//	}
	//}
}

void AKnight::DebugInput(float _DeltaTime)
{
	if (UEngineInput::IsPress('V'))
	{
		FSM.ChangeState(EKnightState::DEATH_DAMAGE);
	}

	float ZValue = BodyRenderer->GetTransformRef().RelativeLocation.Z;
	int a = 0;

	if (UEngineInput::IsPress('W'))
	{
		BodyRenderer->AddRelativeLocation({ 0.0f, 0.0f, 1.0f * _DeltaTime });
	}

	if (UEngineInput::IsPress('S'))
	{
		BodyRenderer->AddRelativeLocation({ 0.0f, 0.0f, -1.0f * _DeltaTime });
	}
}

void AKnight::CreateSlashEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}

	std::shared_ptr<AKnightEffect> SlashEffect = GetWorld()->SpawnActor<AKnightEffect>();
	SlashEffect->ChangeEffect("SlashEffect");
	FVector LRPos = FVector{ -100.0f, 0.0f, 0.0f };
	bIsShowEffect = true;
	
	FVector Pos = RootComponent->GetTransformRef().RelativeLocation;
	GlobalFunc::SetLocation(RootComponent, SlashEffect->GetKnightEffectRenderer(), bLeftDir, LRPos);

	return;
}

void AKnight::CreateUpSlashEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}

	std::shared_ptr<AKnightEffect> SlashEffect = GetWorld()->SpawnActor<AKnightEffect>();
	SlashEffect->ChangeEffect("UpSlashEffect");
	FVector LRPos = FVector{ 0.0f, 50.0f, 0.0f };
	GlobalFunc::SetLocationTB(RootComponent, SlashEffect->GetKnightEffectRenderer(), bLeftDir, LRPos);
	bIsShowEffect = true;
}

void AKnight::CreateDownSlashEffect()
{
	if (true == bIsShowEffect)
	{
		return;
	}

	std::shared_ptr<AKnightEffect> SlashEffect = GetWorld()->SpawnActor<AKnightEffect>();
	SlashEffect->ChangeEffect("DownSlashEffect");
	FVector LRPos = FVector{ 0.0f, -100.0f, 0.0f };
	GlobalFunc::SetLocationTB(RootComponent, SlashEffect->GetKnightEffectRenderer(), bLeftDir, LRPos);
	bIsShowEffect = true;
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
		if (UEngineInput::IsPress('X') && UEngineInput::IsPress(VK_DOWN) && false == bIsOnGround)
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
	Move(_DeltaTime);
	CheckGround(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	bCanRotation = true;

	if (UEngineInput::IsPress(VK_LEFT) || UEngineInput::IsPress(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::IDLE_TO_RUN);
		return;
	}


	ChangeJumpAnimation();  // 점프
	ChangeDash(); // 대시

	ChangeLookAnimation(); // 위 아래 쳐다보기

	ChangeAttackAnimation(EKnightState::IDLE); // 지상 공격
	CastFocus(); // 집중
	CastFireball(); // 파이어볼

}

void AKnight::SetRun(float _DeltaTime)
{
	Move(_DeltaTime);
	CheckGround(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	bCanRotation = true;

	if (UEngineInput::IsFree(VK_LEFT) && UEngineInput::IsFree(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::RUN_TO_IDLE);
		return;
	}

	ChangeJumpAnimation();
	ChangeDash(); // 대시

	ChangeAttackAnimation(EKnightState::RUN); // 지상 공격
}

void AKnight::SetIdleToRun(float _DeltaTime)
{
	Move(_DeltaTime);
	CheckGround(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	bCanRotation = true;

	ChangeNextAnimation(EKnightState::RUN);

	ChangeJumpAnimation();
	ChangeDash(); // 대시

	ChangeAttackAnimation(EKnightState::RUN); // 지상 공격
}

void AKnight::SetRunToIdle(float _DeltaTime)
{
	Move(_DeltaTime);
	CheckGround(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	bCanRotation = true;
	bIsDashing = false;


	ChangeNextAnimation(EKnightState::IDLE);

	if (UEngineInput::IsPress(VK_LEFT) || UEngineInput::IsPress(VK_RIGHT))
	{
		FSM.ChangeState(EKnightState::IDLE_TO_RUN);
		return;
	}

	ChangeJumpAnimation();
	ChangeDash(); // 대시

	ChangeAttackAnimation(EKnightState::RUN); // 지상 공격
}

void AKnight::SetJump(float _DeltaTime)
{
	Jump(_DeltaTime);

	Move(_DeltaTime);
	CheckGround(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	bCanRotation = true;

	ChangeDash(); // 대시

	if (true == bIsOnGround)
	{
		FSM.ChangeState(EKnightState::LAND);
	}
	else
	{
		ChangeNextAnimation(EKnightState::AIRBORN);
		ChangeAttackAnimation(EKnightState::AIRBORN);
	}
}

void AKnight::SetAirborn(float _DeltaTime)
{
	Move(_DeltaTime);
	CheckGround(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	bCanRotation = true;

	ChangeDash(); // 대시

	if (true == bIsOnGround)
	{
		FSM.ChangeState(EKnightState::LAND);
	}

}

void AKnight::SetLand(float _DeltaTime)
{
	Move(_DeltaTime);
	CheckGround(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	float InitJumpForce = 600.0f;
	JumpForce = InitJumpForce;

	ChangeNextAnimation(EKnightState::IDLE);
}

void AKnight::SetHardLand(float _DeltaTime)
{
	CheckGround(GravityForce * _DeltaTime);
	Gravity(_DeltaTime);

	float InitJumpForce = 600.0f;
	JumpForce = InitJumpForce;

	ChangeNextAnimation(EKnightState::IDLE);
}

void AKnight::SetDash(float _DeltaTime)
{
	bCanRotation = false;
	bIsDashing = true;

	if (true == bLeftDir)
	{
		AddRelativeLocation(FVector{ -Velocity * _DeltaTime, 0.0f, 0.0f });
	}
	else
	{
		AddRelativeLocation(FVector{ Velocity * _DeltaTime, 0.0f, 0.0f });
	}

	if (true == IsOnGround())
	{
		ChangeNextAnimation(EKnightState::RUN_TO_IDLE);
		return;
	}
	else
	{
		ChangeNextAnimation(EKnightState::AIRBORN);
		return;
	}

}

void AKnight::SetSlash(float _DeltaTime)
{
	Move(_DeltaTime);

	CreateSlashEffect();
	ChangePrevAnimation();
}

void AKnight::SetUpSlash(float _DeltaTime)
{
	Move(_DeltaTime);

	CreateUpSlashEffect();
	ChangePrevAnimation();
}

void AKnight::SetDownSlash(float _DeltaTime)
{
	Move(_DeltaTime);

	CreateDownSlashEffect();
	ChangePrevAnimation();
}

void AKnight::SetFocus(float _DeltaTime)
{

	//if (UEngineInput::IsFree('A'))
	//{
	//	ChangeNextAnimation(EKnightState::IDLE);
	//	return;
	//}

	ChangeNextAnimation(EKnightState::FOCUS_GET);
}

void AKnight::SetFocusGet(float _DeltaTime)
{
	if (false == bIsEffectActive)
	{
		std::shared_ptr<AKnightEffect> FocusEffect = GetWorld()->SpawnActor<AKnightEffect>();
		FocusEffect->ChangeEffect("FocusEffect");
		GlobalFunc::SetLocation(RootComponent, FocusEffect->GetKnightEffectRenderer());
		bIsEffectActive = true;
	}

	//if (UEngineInput::IsFree('A'))
	//{
	//	ChangeNextAnimation(EKnightState::IDLE);
	//	return;
	//}
	ChangeNextAnimation(EKnightState::FOCUS_END);
}

void AKnight::SetFocusEnd(float _DeltaTime)
{
	if (UEngineInput::IsPress('A'))
	{
		FSM.ChangeState(EKnightState::FOCUS);
		bIsEffectActive = false;
		return;
	}
	else // 스킬 시전 종료
	{
		if (true == bIsEffectActive)
		{
			std::shared_ptr<AKnightEffect> FocusEffect = GetWorld()->SpawnActor<AKnightEffect>();
			FocusEffect->ChangeEffect("FocusEffectEnd");
			GlobalFunc::SetLocation(RootComponent, FocusEffect->GetKnightEffectRenderer());
			bIsEffectActive = false;
		}
		ChangeNextAnimation(EKnightState::IDLE);
	}
}

void AKnight::SetFireballAntic(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::FIREBALL_CAST);
}

void AKnight::SetFireballCast(float _DeltaTime)
{
	if (true == bIsOnGround)
	{
		ChangeNextAnimation(EKnightState::IDLE);
	}
	else
	{
		ChangeNextAnimation(EKnightState::AIRBORN);
	}
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
	
	// 뒤로 밀려나고
	// 이펙트 출력
}

void AKnight::SetDeathDamage(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::DEATH);
}

void AKnight::SetDeath(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::DEATH_HEAD);
	
}

void AKnight::SetDeathHead(float _DeltaTime)
{
	ChangeNextAnimation(EKnightState::IDLE);
}

void AKnight::CreateRenderer()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;


	float IdleFrameTime = 0.15f;
	float RunFrameTime = 0.06f;
	float ChangeFrameTime = 0.05f;
	float SlashFrameTime = 0.03f;

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	float ZSort = -100.0f;
	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });

	// 이동 애니메이션
	std::string Idle = "Idle";
	BodyRenderer->CreateAnimation(Idle, "Knight_Idle.png", 0, 8, IdleFrameTime);

	std::string Run = "Run";
	BodyRenderer->CreateAnimation(Run, Run, 0, 7, RunFrameTime);

	std::string RunToIdle = "RunToIdle";
	BodyRenderer->CreateAnimation(RunToIdle, RunToIdle, 0, 5, ChangeFrameTime, false);

	std::string IdleToRun = "IdleToRun";
	BodyRenderer->CreateAnimation(IdleToRun, IdleToRun, 0, 4, ChangeFrameTime, false);

	std::string Dash = "Dash";
	BodyRenderer->CreateAnimation(Dash, Dash, 0, 6, SlashFrameTime, false);

	std::string Jump = "Jump";
	BodyRenderer->CreateAnimation(Jump, Jump, 0, 7, RunFrameTime, false);

	std::string Airborn = "Airborn";
	BodyRenderer->CreateAnimation(Airborn, Airborn, 0, 2, RunFrameTime);

	std::string Land = "Land";
	BodyRenderer->CreateAnimation(Land, Land, 0, 2, ChangeFrameTime);

	std::string HardLand = "HardLand";
	BodyRenderer->CreateAnimation(HardLand, HardLand, 0, 9, RunFrameTime, false);


	// 정적 애니메이션
	float StaticFrameTime = 0.15f;
	std::string LookDown = "LookDown";
	BodyRenderer->CreateAnimation(LookDown, LookDown, 0, 5, StaticFrameTime, false);

	std::string LookDownLoop = "LookDownLoop";
	BodyRenderer->CreateAnimation(LookDownLoop, LookDownLoop, 0, 4, IdleFrameTime);

	std::string LookUp = "LookUp";
	BodyRenderer->CreateAnimation(LookUp, LookUp, 0, 5, StaticFrameTime, false);

	std::string LookUpLoop = "LookUpLoop";
	BodyRenderer->CreateAnimation(LookUpLoop, LookUpLoop, 0, 4, IdleFrameTime);


	// 전투 애니메이션
	std::string Slash = "Slash";
	BodyRenderer->CreateAnimation(Slash, Slash, 0, 6, SlashFrameTime, false);

	std::string UpSlash = "UpSlash";
	BodyRenderer->CreateAnimation(UpSlash, UpSlash, 0, 6, SlashFrameTime, false);

	std::string DownSlash = "DownSlash";
	BodyRenderer->CreateAnimation(DownSlash, DownSlash, 0, 6, SlashFrameTime, false);


	// 스펠 애니메이션
	std::string Focus = "Focus";
	BodyRenderer->CreateAnimation(Focus, Focus, 0, 6, RunFrameTime, false);

	std::string FocusGet = "FocusGet";
	BodyRenderer->CreateAnimation(FocusGet, FocusGet, 0, 10, RunFrameTime, false);

	std::string FocusEnd = "FocusEnd";
	BodyRenderer->CreateAnimation(FocusEnd, FocusEnd, 0, 2, RunFrameTime, false);

	std::string FireballAntic = "FireballAntic";
	BodyRenderer->CreateAnimation(FireballAntic, FireballAntic, 0, 2, RunFrameTime, false);

	std::string FireballCast = "FireballCast";
	BodyRenderer->CreateAnimation(FireballCast, FireballCast, 0, 5, RunFrameTime, false);


	// 피격 애니메이션
	std::string Damage = "Damage";
	BodyRenderer->CreateAnimation(Damage, Damage, 0, 0, HitStunDuration, false);


	// 사망 애니메이션
	float DeathFrameTime = 0.1f;
	std::string DeathDamage = "DeathDamage";
	BodyRenderer->CreateAnimation(DeathDamage, DeathDamage, 0, 4, DeathFrameTime, false);

	std::string Death = "Death";
	BodyRenderer->CreateAnimation(Death, Death, 0, 12, DeathFrameTime, false);

	std::string DeathHead = "DeathHead";
	BodyRenderer->CreateAnimation(DeathHead, DeathHead, 0, 0, 1.0f, false);
}

void AKnight::SetFSM()
{
	// 이동 애니메이션
	CreateState(EKnightState::IDLE, &AKnight::SetIdle, "Idle");
	CreateState(EKnightState::RUN, &AKnight::SetRun, "Run");
	CreateState(EKnightState::RUN_TO_IDLE, &AKnight::SetRunToIdle, "RunToIdle");
	CreateState(EKnightState::IDLE_TO_RUN, &AKnight::SetIdleToRun, "IdleToRun");
	CreateState(EKnightState::DASH, &AKnight::SetDash, "Dash");
	CreateState(EKnightState::JUMP, &AKnight::SetJump, "Jump");
	CreateState(EKnightState::AIRBORN, &AKnight::SetAirborn, "Airborn");
	CreateState(EKnightState::LAND, &AKnight::SetLand, "Land");
	CreateState(EKnightState::HARD_LAND, &AKnight::SetHardLand, "HardLand");

	// 정적 애니메이션
	CreateState(EKnightState::LOOK_DOWN, &AKnight::SetLookDown, "LookDown");
	CreateState(EKnightState::LOOK_DOWN_LOOP, &AKnight::SetLookDownLoop, "LookDownLoop");
	CreateState(EKnightState::LOOK_UP, &AKnight::SetLookUp, "LookUp");
	CreateState(EKnightState::LOOK_UP_LOOP, &AKnight::SetLookUpLoop, "LookUpLoop");

	// 전투 애니메이션
	// 일반공격
	CreateState(EKnightState::SLASH, &AKnight::SetSlash, "Slash");
	CreateState(EKnightState::UP_SLASH, &AKnight::SetUpSlash, "UpSlash");
	CreateState(EKnightState::DOWN_SLASH, &AKnight::SetDownSlash, "DownSlash");

	// 스펠
	CreateState(EKnightState::FOCUS, &AKnight::SetFocus, "Focus");
	CreateState(EKnightState::FOCUS_GET, &AKnight::SetFocusGet, "FocusGet");
	CreateState(EKnightState::FOCUS_END, &AKnight::SetFocusEnd, "FocusEnd");
	CreateState(EKnightState::FIREBALL_ANTIC, &AKnight::SetFireballAntic, "FireballAntic");
	CreateState(EKnightState::FIREBALL_CAST, &AKnight::SetFireballCast, "FireballCast");

	// 피격
	CreateState(EKnightState::DAMAGED, &AKnight::SetDamage, "Damage");

	// 사망
	CreateState(EKnightState::DEATH, &AKnight::SetDeath, "Death");
	CreateState(EKnightState::DEATH_DAMAGE, &AKnight::SetDeathDamage, "DeathDamage");
	CreateState(EKnightState::DEATH_HEAD, &AKnight::SetDeathHead, "DeathHead");

	FSM.ChangeState(EKnightState::IDLE);
}

void AKnight::CreateState(EKnightState _State, StateCallback _Callback, std::string_view _AnimationName)
{
	FSM.CreateState(_State, std::bind(_Callback, this, std::placeholders::_1),
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
	if (false == bCanRotation)
	{
		return;
	}
	if (UEngineInput::IsPress(VK_LEFT))
	{
		bLeftDir = true;
		BodyRenderer->SetRotation({ 0.0f, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress(VK_RIGHT))
	{
		bLeftDir = false;
		BodyRenderer->SetRotation({ 0.0f, 180.0f, 0.0f });
	}
}

// 다음 애니메이션으로 변경
void AKnight::ChangeNextAnimation(EKnightState _NextState)
{
	if (true == BodyRenderer->IsCurAnimationEnd())
	{
		FSM.ChangeState(_NextState);
		return;
	}
}

// 이전 애니메이션으로 변경
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
	//MainPawn = nullptr;
}
