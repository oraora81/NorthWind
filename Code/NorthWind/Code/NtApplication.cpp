
#include "NtCoreLib.h"
#pragma hdrstop

NT::APP::NtApplication* g_app = nullptr;
NT::FS::NtResourceManager* g_resManager = nullptr;
NT::Memory::NtLinearAllocator* g_linearAllocator = nullptr;

#if _NT_DX11
//NT::RENDERER::NtDirectX11Renderer* g_renderer = nullptr;
NtRenderer* g_renderInterface;
std::shared_ptr<NT::RENDERER::NtDirectX11Renderer> g_renderer;
#endif

namespace NT
{

using namespace RENDERER;
using namespace INPUT;

namespace APP
{


bool NtApplication::Initialize(bool fullscreen)
{
	NtSetup::Setup();

	m_fullScreen = fullscreen;

	WNDCLASSEX wc;
	DEVMODE screenSetting;

	// set external pointer
	g_app = this;

	NtInitDelegator::ExecuteInitDelegator();

	m_hInst = GetModuleHandle(NULL);

	m_appName = L"NorthWind";

	//
	NtWChar* path = nullptr;
	/*NtErrorCode res = Crt::AllocEnvVariable(L"NT_PATH", &path);
	if (res != NtERROR_SUCCESS)
	{
		return false;
	}*/

	path = L"D:\\src_etc\\northwind\\bin\\data";

	m_globalPath = path;
	m_globalPath.Replace(L'\\', L'/');
	m_globalPath += L"/";
	
	// set the cmd lines
	m_cmdArg = Crt::GetCmdLine();


	// set the window class
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInst;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_appName.Buffer();
	wc.cbSize = sizeof(WNDCLASSEX);

	// register the window class
	RegisterClassEx(&wc);

	// 
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);

	// 
	int posX = 0;
	int posY = 0;
	if (m_fullScreen)
	{
		Crt::MemSet(&screenSetting, sizeof(screenSetting));
		screenSetting.dmSize = sizeof(screenSetting);
		screenSetting.dmPelsWidth = static_cast<NtUInt>(width);
		screenSetting.dmPelsHeight = static_cast<NtUInt>(height);
		screenSetting.dmBitsPerPel = 32;
		screenSetting.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// change the display settings to full screen
		ChangeDisplaySettings(&screenSetting, CDS_FULLSCREEN);
	}
	else
	{
		posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_appName.Buffer(), m_appName.Buffer(),
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, width, height, NULL, NULL, m_hInst, NULL);


	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor
	ShowCursor(FALSE);

	//g_linearAllocator = static_cast<Memory::NtLinearAllocator*>(NtObject::CreateObject(Memory::NtLinearAllocator::m_RTTI.GetObjName()));
	//if (false == g_linearAllocator->Initialize(MAKE_MEGA2BYTES(2)))
	//{
	//	return false;
	//}
	
	// 
	m_resManager = new NT::FS::NtResourceManager;
	if (false == m_resManager->Initialize(m_globalPath.Buffer()))
	{
		return false;
	}

	g_resManager = m_resManager;

	// 
	m_inputManager = new NT::INPUT::NtInputManager;
	if (false == m_inputManager->Initialize())
	{
		return false;
	}

	//
	m_renderer = new NT::RENDERER::NtRenderer;
	g_renderInterface = m_renderer;
	if (false == m_renderer->Initialize(m_hwnd, width, height))
	{
		return false;
	}
	
	return true;
}

void NtApplication::Shutdown()
{
	SAFE_DELETE(m_renderer);
	SAFE_DELETE(m_inputManager);
	SAFE_DELETE(m_resManager);
	SAFE_DELETE(g_linearAllocator);
	NtInitDelegator::ExecuteReleaseDelegator();

	ShowCursor(TRUE);

	if (m_fullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	UnregisterClass(m_appName.Buffer(), m_hInst);
	m_hInst = NULL;

	g_app = nullptr;
}


void NtApplication::MsgLoop()
{
	MSG msg;

	Crt::MemSet(&msg, sizeof(MSG));
	bool END = false;
	while (!END)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 
		if (msg.message == WM_QUIT)
		{
			END = true;
		}
		else
		{
			if (!Process())
			{
				END = true;
			}
		}
	}
}

bool NtApplication::Process()
{
	if (m_inputManager->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	if (!m_renderer->Process())
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK NtApplication::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
		{
			m_inputManager->KeyDown((NtUInt)wParam);
		}
		return 0;

	case WM_KEYUP:
		{
			m_inputManager->KeyUp((NtUInt)wParam);
		}
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

const HWND NtApplication::Handle()
{
	return m_hwnd;
}


}	// namespace APP

}	// namespace NT



// default WndProc
//LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	switch(message)
//	{
//	case WM_DESTROY:
//		{
//			PostQuitMessage(0);
//		}
//		return 0;
//
//	case WM_CLOSE:
//		{
//			PostQuitMessage(0);
//		}
//		return 0;
//	}
//
//	return g_app->MessageHandler(hwnd, message, wParam, lParam);
//}