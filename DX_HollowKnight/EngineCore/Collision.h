#pragma once
#include "SceneComponent.h"
#include "EngineSprite.h"
#include "RenderUnit.h"

// ���� : � �������̵� �Ҽ� �մ� ������ ����ڴ�.
// �������̶� �������� �ϴ°� �ƴմϴ�. 3D
class UCollision : public USceneComponent
{
public:
	// constrcuter destructer
	ENGINEAPI UCollision();
	ENGINEAPI ~UCollision();

	// delete Function
	UCollision(const UCollision& _Other) = delete;
	UCollision(UCollision&& _Other) noexcept = delete;
	UCollision& operator=(const UCollision& _Other) = delete;
	UCollision& operator=(UCollision&& _Other) noexcept = delete;

	ENGINEAPI void SetCollisionProfileName(std::string_view _ProfileName);
	ENGINEAPI void BeginPlay() override;
	// ENGINEAPI void DebugRender(UEngineCamera* _Camera, float _DeltaTime);

	std::string GetCollisionProfileName()
	{
		return ProfileName;
	}

	void SetRadius(float _Value);

	// vector�ΰ� �� vector�� �ϳ�
	// unreal���� vector
	ENGINEAPI bool CollisionCheck(std::string_view _OtherName, std::vector<UCollision*>& _Vector);

	void SetCollisionType(ECollisionType _Type)
	{
		CollisionType = _Type;
	}


private:

public:
	ECollisionType CollisionType = ECollisionType::OBB2D;

	// ���� Monster �׷��̴�.
	std::string ProfileName; 
};

