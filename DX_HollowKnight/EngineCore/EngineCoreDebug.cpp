#include "PreCompile.h"
#include "EngineCoreDebug.h"
#include <EnginePlatform/EngineWinDebug.h>
#include "EngineCore.h"

namespace UEngineDebug
{
	class DebugTextInfo
	{
	public:
		std::string Text;
		FVector Pos;
	};


	// ����
	std::vector<DebugTextInfo> DebugTexts;


// #ifdef _DEBUG
	//FVector EngineTextPos = FVector::ZERO;
	//FVector EngineTextBotPos = FVector::ZERO;
// #endif

#ifdef _DEBUG
	bool IsDebug = true;
#else
	bool IsDebug = false;
#endif

	void SetIsDebug(bool _IsDebug)
	{
		IsDebug = _IsDebug;
	}

	void SwitchIsDebug()
	{
		IsDebug = !IsDebug;
	}

	void CoreOutPutString(std::string_view _Text)
	{
		//if (false == IsDebug)
		//{
		//	return;
		//}
		//// #ifdef _DEBUG
		//		// �ٷ� ������� �ʴ´�.
		//DebugTexts.push_back({ _Text.data(), EngineTextPos });
		//EngineTextPos.Y += 20;
		//// endif 
	}

	void CoreOutPutString(std::string_view _Text, FVector _Pos)
	{
		if (false == IsDebug)
		{
			return;
		}
// #ifdef _DEBUG
		DebugTexts.push_back({ _Text.data(), _Pos });
// #endif
	}

	class DebugRenderInfo
	{
	public:
		FTransform Trans;
		EDebugPosType Type;
	};

	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);

	std::vector<DebugRenderInfo> DebugPoses;

	void CoreDebugRender(FTransform _Trans, EDebugPosType _Type)
	{
		if (false == IsDebug)
		{
			return;
		}
		DebugPoses.push_back({ _Trans, _Type });
	}

	void PrintEngineDebugRender()
	{
		//if (false == IsDebug)
		//{
		//	return;
		//}

		//// void WinAPIOutPutString(UEngineWinImage * _Image, std::string_view _Text, FVector2D _Pos);
		//UEngineWinImage* BackBuffer = UEngineCore::GetDevice().;
		//HDC BackHDC = BackBuffer->GetDC();

		//for (size_t i = 0; i < DebugTexts.size(); i++)
		//{
		//	DebugTextInfo& Debug = DebugTexts[i];
		//	WinAPIOutPutString(BackBuffer, Debug.Text, Debug.Pos);
		//}

		//EngineTextPos = FVector::ZERO;
		//DebugTexts.clear();

		//FTransform Trans;
		//Trans.Scale = FVector(6, 6);

		//for (size_t i = 0; i < DebugPoses.size(); i++)
		//{
		//	EDebugPosType Type = DebugPoses[i].Type;

		//	FVector LT = DebugPoses[i].Trans.CenterLeftTop();
		//	FVector RB = DebugPoses[i].Trans.CenterRightBottom();

		//	switch (Type)
		//	{
		//	case UEngineDebug::EDebugPosType::Rect:
		//	{
		//		HBITMAP OldBitMapPen = static_cast<HBITMAP>(SelectObject(BackHDC, hPen));
		//		DeleteObject(OldBitMapPen);
		//		HBITMAP OldBitMapBrush = static_cast<HBITMAP>(SelectObject(BackHDC, hBrush));
		//		DeleteObject(OldBitMapBrush);

		//		Rectangle(BackHDC, LT.iX(), LT.iY(), RB.iX(), RB.iY());

		//		break;
		//	}
		//	case UEngineDebug::EDebugPosType::Circle:
		//	{
		//		HBITMAP OldBitMapPen = static_cast<HBITMAP>(SelectObject(BackHDC, hPen));
		//		DeleteObject(OldBitMapPen);
		//		HBITMAP OldBitMapBrush = static_cast<HBITMAP>(SelectObject(BackHDC, hBrush));
		//		DeleteObject(OldBitMapBrush);

		//		Ellipse(BackHDC, LT.iX(), LT.iY(), RB.iX(), RB.iY());

		//		break;
		//	}
		//	default:
		//		break;
		//	}
		//}

		//DeleteObject(hPen);
		//DeleteObject(hBrush);
		//DebugPoses.clear();
	}

	void CoreDebugBox(FTransform _Trans)
	{

	}

}