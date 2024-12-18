#pragma once
// Os Header
#include <Windows.h>

// std Header
#include <map>
#include <string>
#include <functional>

// user header
#include <EngineBase/EngineMath.h>

// Ό³Έν :
class UEngineWindow
{
public:
	ENGINEAPI static void EngineWindowInit(HINSTANCE _Instance);
	ENGINEAPI static void CreateWindowClass(const WNDCLASSEXA& _Class);
	ENGINEAPI static int WindowMessageLoop(std::function<void()> _StartFunction, std::function<void()> _FrameFunction, std::function<void()> _EndFunction = nullptr);

	// constrcuter destructer
	ENGINEAPI UEngineWindow();
	ENGINEAPI ~UEngineWindow();

	// delete Function
	UEngineWindow(const UEngineWindow& _Other) = delete;
	UEngineWindow(UEngineWindow&& _Other) noexcept = delete;
	UEngineWindow& operator=(const UEngineWindow& _Other) = delete;
	UEngineWindow& operator=(UEngineWindow&& _Other) noexcept = delete;

	ENGINEAPI void Create(std::string_view _TitleName, std::string_view _ClassName = "Default");
	ENGINEAPI void Open(std::string_view _TitleName = "Window");

	inline FVector GetWindowSize() const
	{
		return WindowSize;
	}

	inline void SetWindowTitle(std::string_view Text)
	{
		SetWindowTextA(WindowHandle, Text.data());
	}

	ENGINEAPI void SetWindowPosAndScale(FVector _Pos, FVector _Scale);

	ENGINEAPI FVector GetMousePos();

	ENGINEAPI void static ApplicationOff()
	{
		LoopActive = false;
	}

	ENGINEAPI HWND GetWindowHandle() const
	{
		return WindowHandle;
	}

protected:

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	ENGINEAPI static HINSTANCE hInstance;
	ENGINEAPI static std::map<std::string, WNDCLASSEXA> WindowClasss;
	inline static bool LoopActive = true;

	FVector WindowSize;
	HWND WindowHandle = nullptr;
};


