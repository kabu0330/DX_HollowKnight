#include "PreCompile.h"
#include "ContentsCore.h"

// #define�� �׳� ������ ����
CreateContentsCoreDefine(UContentsCore);
//STDAPI_(__declspec(dllexport) INT_PTR) CreateContentsCore(std::shared_ptr<IContentsCore>& _Test)
//{ 
//	_Test = std::make_shared<UContentsCore>();
//	if (nullptr == _Test) 
//	{ 
//		MSGASSERT("������ ��� ������ �����߽��ϴ�."); 
//	} 
//	return 0; 
//}

UContentsCore::UContentsCore()
{
}

UContentsCore::~UContentsCore()
{
}

void UContentsCore::EngineStart(UEngineInitData& _Data)
{
	// mainwindow�� �ƹ��� �ǵ�� �ȵȴ�.
	// �� �������ݾ� ������ �����ϴ� �������°� �����ϴ����� ������Ѵ�.

	_Data.WindowPos = { 400, 100 };
	_Data.WindowSize = { 600, 600 };

	// ������ ũ�� ����
}

void UContentsCore::EngineTick(float _DeltaTime)
{

}

void UContentsCore::EngineEnd()
{

}