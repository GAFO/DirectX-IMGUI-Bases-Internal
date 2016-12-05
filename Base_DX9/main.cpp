/*
Note:
VC++ Directories -> General:
Change the following
- Include Directories
- Libary Directories

.. if needed.

*/

//.Includes
#include "Base Includes.h"

//.Namespaces

//.Defines
#define WindowName "Either Add the Window name for the Target here or replace the function with your own."

//.Code
namespace WorkHere
{
	LRESULT WINAPI HookedProcessMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		ImGui_ImplDX9_WndProcHandler(hWnd, message, wParam, lParam);

		return CallWindowProc(oWndProc, hWnd, message, wParam, lParam);
	}

	void InitializeDX9Elements(LPDIRECT3DDEVICE9 _ptr_Device)
	{
		ImGui_ImplDX9_Init(FindWindowA(0, WindowName), _ptr_Device);
	}

	void RenderDX9Elements(LPDIRECT3DDEVICE9 _ptr_Device)
	{
		ImGui_ImplDX9_NewFrame();
		/*
		Add your IMGUI Code here- exambles can be found at its 
		GitHub Site, just google it.
		*/


	}

	void PreDX9Reset(LPDIRECT3DDEVICE9 _ptr_Device, D3DPRESENT_PARAMETERS* _ptr_PresentationParameters)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void PostDX9Reset(LPDIRECT3DDEVICE9 _ptr_Device, D3DPRESENT_PARAMETERS* _ptr_PresentationParameters, HRESULT& _result)
	{
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

namespace Fundamentals
{
	HRESULT WINAPI HookedEndscene(LPDIRECT3DDEVICE9 _ptr_Device)
	{
		static bool loc_init_e = false;
		if (!loc_init_e)
		{
			WorkHere::InitializeDX9Elements(_ptr_Device);
			loc_init_e = true;
		}

		WorkHere::RenderDX9Elements(_ptr_Device);

		return oEndScene(_ptr_Device);
	}

	HRESULT WINAPI HookedReset(LPDIRECT3DDEVICE9 _ptr_Device, D3DPRESENT_PARAMETERS* _ptr_PresentationParameters)
	{
		WorkHere::PreDX9Reset(_ptr_Device, _ptr_PresentationParameters);

		HRESULT ret = oReset(_ptr_Device, _ptr_PresentationParameters);

		WorkHere::PostDX9Reset(_ptr_Device, _ptr_PresentationParameters, ret);

		return ret;
	}
	
	//~~~
	LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	DWORD GetDx9VtableFunction(int index)
	{
		WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, "DX", NULL };
		RegisterClassEx(&wc);
		HWND hWnd = CreateWindow("DX", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, GetDesktopWindow(), NULL, wc.hInstance, NULL);
		LPDIRECT3D9 pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		LPDIRECT3DDEVICE9 pd3dDevice;
		pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pd3dDevice);
		DWORD* pVTable = (DWORD*)pd3dDevice;
		pVTable = (DWORD*)pVTable[0];
		DWORD Foo = pVTable[index];
		DestroyWindow(hWnd);
		return Foo;
	}
	//~~~

	DWORD __stdcall SetUpHooks()
	{
		oWndProc = (tWndProc)SetWindowLongPtr(FindWindowA(0, WindowName),	GWL_WNDPROC, (LONG_PTR)WorkHere::HookedProcessMessages);
		/*
		Use your favorite hooking methods
		for Endscene and Reset.
		
		GetDx9VtableFunction(42) returns the Endscene	Address
		GetDx9VtableFunction(16) returns the Reset		Address
		
		
		*/
		return 0xDEADBEEF;
	}
}


BOOL APIENTRY DllMain(HINSTANCE hInst, DWORD Reason, LPVOID lpReserved)
{
	switch (Reason)
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hInst);		
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Fundamentals::SetUpHooks, 0, 0, 0);
		}
		case DLL_PROCESS_DETACH:
			break;

		case DLL_THREAD_ATTACH:
			break;

		case DLL_THREAD_DETACH:
		{
			break;
		}
	}

	return TRUE;
}