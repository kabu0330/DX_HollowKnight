#include "PreCompile.h"
#include "Knight.h"

void AKnight::CreateRenderer()
{
	BodyRenderer = CreateDefaultSubObject<UContentsRenderer>();
	BodyRenderer->SetupAttachment(RootComponent);
	BodyRenderer->SetAutoScaleRatio(1.0f);
	float ZSort = static_cast<float>(EZOrder::PLAYER);
	BodyRenderer->SetWorldLocation({ 0.0f, 0.0f, ZSort });

	InitAnimation();
}

void AKnight::InitAnimation()
{
	float IdleFrameTime = 0.15f;
	float RunFrameTime = 0.06f;
	float ChangeFrameTime = 0.05f;
	float SlashFrameTime = 0.03f;

	// �̵� �ִϸ��̼�
	std::string Idle = "Idle";
	BodyRenderer->CreateAnimation(Idle, Idle, 0, 8, IdleFrameTime);

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


	// ���� �ִϸ��̼�
	float StaticFrameTime = 0.15f;
	std::string LookDown = "LookDown";
	BodyRenderer->CreateAnimation(LookDown, LookDown, 0, 5, StaticFrameTime, false);

	std::string LookDownLoop = "LookDownLoop";
	BodyRenderer->CreateAnimation(LookDownLoop, LookDownLoop, 0, 4, IdleFrameTime);

	std::string LookUp = "LookUp";
	BodyRenderer->CreateAnimation(LookUp, LookUp, 0, 5, StaticFrameTime, false);

	std::string LookUpLoop = "LookUpLoop";
	BodyRenderer->CreateAnimation(LookUpLoop, LookUpLoop, 0, 4, IdleFrameTime);


	// ���� �ִϸ��̼�
	std::string Slash = "Slash";
	BodyRenderer->CreateAnimation(Slash, Slash, 0, 6, SlashFrameTime, false);

	std::string UpSlash = "UpSlash";
	BodyRenderer->CreateAnimation(UpSlash, UpSlash, 0, 6, SlashFrameTime, false);

	std::string DownSlash = "DownSlash";
	BodyRenderer->CreateAnimation(DownSlash, DownSlash, 0, 6, SlashFrameTime, false);


	// ���� �ִϸ��̼�
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


	// �ǰ� �ִϸ��̼�
	std::string Damage = "Damage";
	BodyRenderer->CreateAnimation(Damage, Damage, 0, 0, HitStunDuration, false);


	// ��� �ִϸ��̼�
	float DeathFrameTime = 0.1f;
	std::string DeathDamage = "DeathDamage";
	BodyRenderer->CreateAnimation(DeathDamage, DeathDamage, 0, 4, DeathFrameTime, false);

	std::string Death = "Death";
	BodyRenderer->CreateAnimation(Death, Death, 0, 12, DeathFrameTime, false);

	std::string DeathHead = "DeathHead";
	BodyRenderer->CreateAnimation(DeathHead, DeathHead, 0, 1, 1.0f, false);
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