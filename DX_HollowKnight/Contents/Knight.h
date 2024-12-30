#pragma once
#include <EngineCore/Pawn.h>
#include <EngineBase/FSMStateManager.h>
#include "Global.h"

// 설명 : Player
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

	bool CanAction();
	void ChangeAttackAnimation(EKnightState _PrevState);
	void TimeElapsed(float _DeltaTime);

protected:

private:
	std::shared_ptr<class USpriteRenderer> BodyRenderer;
	std::shared_ptr<class UEffectRenderer> EffectRenderer;

	// FSM
	UFSMStateManager FSM;
	EKnightState NextState;

	// 공격 또는 피격 동작 중일 때
	bool bIsAttacking = false;
	bool bIsBeingHit = false;
	bool bIsCastingSpell = false;
	bool bIsDeath = false;

	// 쿨타임 관련
	float AttackCooldownElapsed = 0.0f;

private:
	void InputCheck(float _DeltaTime);

	void CreateSlashEffect();

	void CreateRenderer();
	void SetFSM();
	void SetIdle(float _DeltaTime);
	void SetRun(float _DeltaTime);
	void SetIdleToRun(float _DeltaTime);
	void SetRunToIdle(float _DeltaTime);
	void SetSlash(float _DeltaTime);
	void SetUpSlash(float _DeltaTime);
	void SetDownSlash(float _DeltaTime);
	void SetAirborn(float _DeltaTime);
	void SetDamaged(float _DeltaTime);
	void SetDeath(float _DeltaTime);

	void CheckDirection();
};

