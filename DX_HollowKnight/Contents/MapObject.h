#pragma once
#include <EngineBase/EngineSerializer.h>
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include "Global.h"

class MapObjectData : public ISerializeObject
{
public:
	USpriteRenderer* SpriteRenderer;
	bool bIsCollision = false; // 충돌여부
	bool bIsInteractable = false; // 통과 여부
	

	FVector Scale;
	FVector Position;

	ELayer Layer;

};

// 설명 :
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

