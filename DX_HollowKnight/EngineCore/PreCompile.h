#pragma once

#include <EnginePlatform/PreCompile.h>

#include "Pawn.h"
#include "GameMode.h"
#include "Level.h"
#include "EngineCore.h"

#pragma comment(lib, "EngineBase.lib")
#pragma comment(lib, "EnginePlatform.lib")

#include <wrl.h>
#include <d3d11_4.h> // directx 11 버전4용 헤더
#include <d3dcompiler.h> // 쉐이더 컴파일러용 인터페이스
#include <EnginePlatform/EngineWindow.h>

// 라이브러리들
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler") 
#pragma comment(lib, "dxguid")

// GetHighPerFormanceAdapter 등등을 위해서는 모니터 정보나 그래픽카드 정보를 얻어와야하는데
// 이녀석이 그 함수들을 가지고 있다.
#pragma comment(lib, "DXGI") 