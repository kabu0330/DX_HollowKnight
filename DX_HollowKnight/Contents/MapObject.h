#pragma once
#include <EngineCore/Actor.h>

enum EMapObjectType
{
	BACKGROUND_COLOR,
	BACKGROUND_OBJECT,
	COLLISION_OBJECT,
	NONE_COLLISION_OBJECT,
	MAX
};

// Ό³Έν :
class AMapObject : public AActor
{
public:
	// constrcuter destructer
	AMapObject();
	~AMapObject();

	// delete Function
	AMapObject(const AMapObject& _Other) = delete;
	AMapObject(AMapObject&& _Other) noexcept = delete;
	AMapObject& operator=(const AMapObject& _Other) = delete;
	AMapObject& operator=(AMapObject&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	EMapObjectType MapObjectTypeValue;

	void ChangeTexture(std::string_view _TextureName)
	{
		Renderer->SetTexture(_TextureName);
	}
	void SetSpriteIndex(int _Index)
	{
		Renderer->SetSprite("MapObjectResources", _Index);
	}
	std::shared_ptr<class USpriteRenderer> GetRenderer()
	{
		return Renderer;
	}

protected:
	std::shared_ptr<class USpriteRenderer> Renderer;

private:
};

