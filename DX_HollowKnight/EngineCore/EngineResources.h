#pragma once
#include <EngineBase/Object.h>
#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>

class UEngineResources : public UObject
{
public:
	// constrcuter destructer
	UEngineResources() {}
	~UEngineResources()	{}

	// delete Function
	UEngineResources(const UEngineResources& _Other) = delete;
	UEngineResources(UEngineResources&& _Other) noexcept = delete;
	UEngineResources& operator=(const UEngineResources& _Other) = delete;
	UEngineResources& operator=(UEngineResources&& _Other) noexcept = delete;

	static std::string ToUpperName(std::string_view _Name)
	{
		return UEngineString::ToUpper(_Name);
	}

	template<typename ResType>
	ENGINEAPI static std::shared_ptr<ResType> Find(std::string_view _Name)
	{
		const type_info& Info = typeid(ResType);
		return std::dynamic_pointer_cast<ResType>(Find(Info.name(), _Name.data()));
	}

	ENGINEAPI static std::shared_ptr<UEngineResources> Find(std::string_view _ResName, std::string_view _Name);

	static bool Contains(std::string_view _Name)
	{
		return ResMap.contains(_Name.data());
	}

	static void Release()
	{
		ResMap.clear();
	}

	template<typename ResType>
	ENGINEAPI static void PushRes(std::shared_ptr<UEngineResources> _Res, std::string_view _Name, std::string_view _Path)
	{
		const type_info& Info = typeid(ResType);
		PushRes(_Res, Info.name(), _Name, _Path);
	}

	// resources 관리 함수들은 전부 다 core에서만 사용
	ENGINEAPI static void PushRes(std::shared_ptr<UEngineResources> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path);

	ENGINEAPI UEnginePath GetPath()
	{
		return Path;
	}

protected:
	UEnginePath Path;

private:
	// 이걸 해결방법은 
	//                               텍스처                aaaa.png       데이터
	//                               버텍스버퍼             Box             데이터
	ENGINEAPI static inline std::map<std::string, std::map<std::string, std::shared_ptr<UEngineResources>>> ResMap;
};


