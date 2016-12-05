#pragma once
//Default
#include <Windows.h>
#include <tlhelp32.h>
#include <Psapi.h>

//DX9
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9")
#pragma comment (lib , "d3dx9")
#pragma comment (lib , "dinput8.lib")
#pragma comment (lib , "dxguid.lib")

//.Thirdparty
#include "IMGUI/imgui.h"
#include "IMGUI/DX9/imgui_impl_dx9.h"

//.Externals
extern IMGUI_API LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam);

//.Typedefs
typedef HRESULT(WINAPI* tEndScene)(LPDIRECT3DDEVICE9 _ptr_Device);
typedef HRESULT(WINAPI* tReset)(LPDIRECT3DDEVICE9 _ptr_Device, D3DPRESENT_PARAMETERS* _ptr_PresentationParameters);
typedef LRESULT(WINAPI* tWndProc)(HWND, UINT, WPARAM, LPARAM);

tEndScene	oEndScene;
tReset		oReset;
tWndProc	oWndProc;

//.Prototypes
namespace WorkHere
{
	LRESULT WINAPI HookedProcessMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
}

namespace Fundamentals
{
	HRESULT WINAPI HookedEndscene(LPDIRECT3DDEVICE9 _ptr_Device);
	HRESULT WINAPI HookedReset(LPDIRECT3DDEVICE9 _ptr_Device, D3DPRESENT_PARAMETERS* _ptr_PresentationParameters);	
}



