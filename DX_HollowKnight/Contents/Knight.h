#pragma once
#include <EngineCore/Pawn.h>
#include <EngineBase/FSMStateManager.h>
#include "Global.h"

// ���� : Player
class AKnight : public APawn
{
public:
	// constrcuter destructer
	AKnight();
	~AKnight();

	// delete Function
	AKnight(const AKnight& _Other) = delete;
	AKnight(AKnight&& _Other) noexcept = delete;
	AKnight& operator=(const AKnight& _Other) = delete;
	AKnight& operator=(AKnight&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void TimeElapsed(float _DeltaTime);
	void EndAnimationEffect();

	static std::shared_ptr<AKnight> GetPawn()
	{
		return MainPawn;
	}

	std::shared_ptr<USceneComponent> GetRootComponent() const
	{
		return RootComponent;
	}

protected:

private:
	static std::shared_ptr<AKnight> MainPawn;
	std::shared_ptr<class USpriteRenderer> BodyRenderer;
	std::shared_ptr<class USpriteRenderer> EffectRenderer;


	// ���� �Ǵ� �ǰ� ���� ���� ��
	bool bIsAttacking = false;
	bool bIsBeingHit = false;
	bool bIsCastingSpell = false;
	bool bIsDeath = false;

	float HitStunDuration = 1.0f;

	// Stat
	float Velocity = 300.0f;
	float InitVelocity = 0.0f;
	float DashSpeed = 0.0f;
	float NailRange = 100.0f;
	int NailDamage = 5;
	int MaxHealth = 5;


private:
	void InputCheck(float _DeltaTime);
	void Move(float _DeltaTime);

	void DebugInput();

	void CreateRenderer();

	bool CanAction();

	// Jump : ZŰ
	bool bIsOnGround = true;
	bool CanJump();
	bool IsOnGround();

	// Slash : XŰ
	bool bIsShowEffect = false;
	float AttackCooldownElapsed = 0.0f;
	void CreateSlashEffect();
	void CreateUpSlashEffect();
	void CreateDownSlashEffect();
	
	// Dash : CŰ
	bool bIsDashing = false;
	bool bCanRotation = true; // ������ȯ ����, ��� �߿��� �Ұ�
	float DashCooldownElapsed = 0.0f;
	void ChangeDash();

	// Spell : AŰ
	bool bIsEffectActive = false;
	void CastFocus();
	void CastFireball();

	// Animation
	void ChangeNextAnimation(EKnightState _NextState);
	void ChangePrevAnimation();

	void ChangeAttackAnimation(EKnightState _PrevState);
	void ChangeJumpAnimation();
	void ChangeNonCombatAnimation();
	void ChangeLookAnimation();

	// FSM
	using StateCallback = void(AKnight::*)(float);
	void CreateState(EKnightState _State, StateCallback _Callback, std::string_view _AnimationName);

	UFSMStateManager FSM;
	EKnightState NextState = EKnightState::IDLE;
	void SetFSM();
	void SetIdle(float _DeltaTime);
	void SetRun(float _DeltaTime);
	void SetIdleToRun(float _DeltaTime);
	void SetRunToIdle(float _DeltaTime);
	void SetDash(float _DeltaTime);
	void SetJump(float _DeltaTime);
	void SetAirborn(float _DeltaTime);
	void SetLand(float _DeltaTime);
	void SetHardLand(float _DeltaTime);

	void SetLookDown(float _DeltaTime);
	void SetLookDownLoop(float _DeltaTime);
	void SetLookUp(float _DeltaTime);
	void SetLookUpLoop(float _DeltaTime);

	void SetSlash(float _DeltaTime);
	void SetUpSlash(float _DeltaTime);
	void SetDownSlash(float _DeltaTime);

	void SetFocus(float _DeltaTime);
	void SetFocusGet(float _DeltaTime);
	void SetFocusEnd(float _DeltaTime);
	void SetFireballAntic(float _DeltaTime);
	void SetFireballCast(float _DeltaTime);

	void SetDamage(float _DeltaTime);
	void SetDeath(float _DeltaTime);
	void SetDeathDamage(float _DeltaTime);
	void SetDeathHead(float _DeltaTime);

	void CheckDirection();
	bool bLeftDir = true;
};

