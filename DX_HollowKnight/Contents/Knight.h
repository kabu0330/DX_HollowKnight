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





protected:

private:
	std::shared_ptr<class USpriteRenderer> BodyRenderer;
	std::shared_ptr<class USpriteRenderer> EffectRenderer;

	// ���� �Ǵ� �ǰ� ���� ���� ��
	bool bIsAttacking = false;
	bool bIsBeingHit = false;
	bool bIsCastingSpell = false;
	bool bIsDeath = false;

	float HitStunDuration = 1.0f;

	// ��Ÿ�� ����
	float AttackCooldownElapsed = 0.0f;

private:
	void InputCheck(float _DeltaTime);

	void CreateSlashEffect();

	void CreateRenderer();


	bool bIsOnGround = true;
	bool CanJump();
	bool IsOnGround();

	bool CanAction();

	// Animation
	void ChangeNextAnimation(EKnightState _NextState);
	void ChangePrevAnimation();

	void ChangeAttackAnimation(EKnightState _PrevState);
	void ChangeJumpAnimation();
	void ChangeNonCombatAnimation();
	void ChangeLookAnimation();

	// FSM
	UFSMStateManager FSM;
	EKnightState NextState;
	void SetFSM();
	void SetIdle(float _DeltaTime);
	void SetRun(float _DeltaTime);
	void SetIdleToRun(float _DeltaTime);
	void SetRunToIdle(float _DeltaTime);
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

	void SetDamage(float _DeltaTime);
	void SetDeath(float _DeltaTime);
	void SetDeathDamage(float _DeltaTime);
	void SetDeathHead(float _DeltaTime);

	void CheckDirection();
};

