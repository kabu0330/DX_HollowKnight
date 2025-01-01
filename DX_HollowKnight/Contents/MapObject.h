#pragma once
#include <EngineBase/EngineSerializer.h>
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include "Global.h"

class MapObjectData : public ISerializeObject
{
public:
	USpriteRenderer* SpriteRenderer;
	bool bIsCollision = false; // �浹����
	bool bIsInteractable = false; // ��� ����
	

	FVector Scale;
	FVector Position;

	ELayer Layer;

};

// ���� :
class MapObject : public AActor, public ISerializeObject
{
public:
	// constrcuter destructer
	MapObject();
	~MapObject();

	// delete Function
	MapObject(const MapObject& _Other) = delete;
	MapObject(MapObject&& _Other) noexcept = delete;
	MapObject& operator=(const MapObject& _Other) = delete;
	MapObject& operator=(MapObject&& _Other) noexcept = delete;


	void BeginPlay();
	void Tick(float _DeltaTime);

protected:

private:

};

