
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtFrameSkip.h"

// 
//----------------------------------------------------------------------------
nt::app::NtApplication* g_app = nullptr;
nt::fs::NtResourceManager* g_resMgr = nullptr;
nt::memory::NtLinearAllocator* g_liAllocator = nullptr;

#if _NT_DX11
nt::renderer::NtRenderer* g_renderInterface;
std::shared_ptr<nt::renderer::NtDx11Renderer> g_renderer;
#endif

// 
//----------------------------------------------------------------------------

namespace nt { 

using namespace renderer;
using namespace input;

namespace app {

NtApplication::NtApplication()
	: m_width(0)
	, m_height(0)
	, m_fullScreen(false)
	, m_minimized(false)
	, m_maximized(false)
	, m_resizing(false)
	, m_appPaused(false)
	, m_renderer(nullptr)
	, m_inputManager(nullptr)
	, m_resManager(nullptr)
{

}

bool NtApplication::Initialize(bool fullscreen, ntInt width, ntInt height)
{
	NtSetup::Setup();

	m_fullScreen = fullscreen;
	m_width = width;
	m_height = height;
	// 
	//int width = GetSystemMetrics(SM_CXSCREEN);
	//int height = GetSystemMetrics(SM_CYSCREEN);

	// set external pointer
	g_app = this;

	NtInitDelegator::ExecuteInitDelegator();

	m_hInst = GetModuleHandle(NULL);

	m_appName = L"NorthWind";

	//
	ntWchar* path = nullptr;
	NtErrorCode res = Crt::AllocEnvVariable(L"NT_PATH", &path);
	if (res != NtERROR_SUCCESS)
	{
		return false;
	}

	m_globalPath = path;
	m_globalPath.Lowercase();
	m_globalPath.Replace(L'\\', L'/');
	m_globalPath += L"/";
	
	// set the cmd lines
	m_cmdArg = Crt::GetCmdLine();

	WNDCLASSEX wc;
	DEVMODE screenSetting;

	// set the window class
	wc.style = CS_HREDRAW | CS_VREDRAW /*| CS_OWNDC*/;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_appName.Buffer();
	wc.cbSize = sizeof(WNDCLASSEX);

	// register the window class
	if (!RegisterClassEx(&wc))
	{
		MessageBox(0, L"Register Class Failed!", L"Error", 0);
		return false;
	}

	// 
	int posX = 0;
	int posY = 0;
	if (m_fullScreen)
	{
		Crt::MemSet(&screenSetting, sizeof(screenSetting));
		screenSetting.dmSize = sizeof(screenSetting);
		screenSetting.dmPelsWidth = static_cast<ntUint>(m_width);
		screenSetting.dmPelsHeight = static_cast<ntUint>(m_height);
		screenSetting.dmBitsPerPel = 32;
		screenSetting.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// change the display settings to full screen
		ChangeDisplaySettings(&screenSetting, CDS_FULLSCREEN);
	}
	else
	{
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_height) / 2;
	}

	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_appName.Buffer(), m_appName.Buffer(),
		WS_OVERLAPPEDWINDOW,
		//WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, m_width, m_height, NULL, NULL, m_hInst, NULL);


	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor
	ShowCursor(FALSE);

	// create memory Allocator
	NtObject* allocator = NtObject::CreateObject(memory::NtLinearAllocator::m_RTTI.GetObjName());
	if (allocator != nullptr)
	{
		g_liAllocator = static_cast<memory::NtLinearAllocator*>(allocator);
		if (false == g_liAllocator->Initialize(MAKE_MEGA2BYTES(2)))
		{
			return false;
		}
	}

	// 
	m_resManager = new nt::fs::NtResourceManager;
	if (false == m_resManager->Initialize(m_globalPath.Buffer()))
	{
		return false;
	}

	g_resMgr = m_resManager;

	// 
	m_inputManager = new nt::input::NtInputManager;
	if (false == m_inputManager->Initialize())
	{
		return false;
	}

	//
	m_renderer = new nt::renderer::NtRenderer;
	g_renderInterface = m_renderer;
	if (false == m_renderer->Initialize(m_hwnd, m_width, m_height, m_fullScreen))
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
	SAFE_DELETE(g_liAllocator);
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

bool NtApplication::OnResize(ntInt width, ntInt height)
{
	if (m_renderer == nullptr)
	{
		return false;
	}

    if (m_renderer->Resize(width, height) == false)
    {
        return false;
    }

    m_width = width;

    m_height = height;

	return true;
}


void NtApplication::MsgLoop()
{
	MSG msg;

	m_timer.Reset();

	NtFrameSkip fs;
	fs.SetFramePerSec(60.0f);

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
			m_timer.Tick();

			float deltaTime = m_timer.DeltaTime();

			// >>>
			// 1. 가능한 빠르게 동작해야 하는 부분 
			//		카메라 이동?
			// <<<


			// >>>
			// 고정 프레임으로 동작하는 부분
			if (fs.Update(deltaTime))
			{
				if (!Process(deltaTime))
				{
					END = true;
				}
			}
			// <<<

			// >>>
			// 경우에 따라 호출되지 않아도 되는 코드
			// 렌더링 같이 시간을 많이 잡아먹는 코드를 넣는다
			if (fs.IsFrameSkip() == false)
			{
				Render();
			}
			// >>>
		}
	}
}

bool NtApplication::Process(const float deltaTime)
{
	if (m_inputManager->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	if (m_inputManager->IsKeyDown(VK_F1))
	{
		g_renderInterface->SetRenderState(NT_RS_SOLID);
	}

	if (m_inputManager->IsKeyDown(VK_F2))
	{
		g_renderInterface->SetRenderState(NT_RS_WIRE_FRAME);
	}

	if (!m_renderer->Process(deltaTime))
	{
		return false;
	}

	return true;
}

void NtApplication::Render()
{
	CalculateFrame();

	m_renderer->Draw();
}


LRESULT CALLBACK NtApplication::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
		{
			m_inputManager->KeyDown((ntUint)wParam);
		}
		return 0;

	case WM_KEYUP:
		{
			m_inputManager->KeyUp((ntUint)wParam);
		}
		return 0;

	case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				m_appPaused = true;
				m_timer.Stop();
			}
			else
			{
				m_appPaused = false;
				m_timer.Start();
			}
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		{
			m_appPaused = true;
			m_resizing  = true;
			m_timer.Stop();
		}
		return 0;

	case WM_EXITSIZEMOVE:
		{
			m_appPaused = false;
			m_resizing = false;
			m_timer.Start();
		}
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		return 0;

	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		{
			OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		return 0;

	case WM_MOUSEMOVE:
		{
			OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		return 0;

	case WM_SIZE:
		{
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			
			if (wParam == SIZE_MINIMIZED)
			{
				m_appPaused = true;
				m_minimized = true;
				m_maximized = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				m_appPaused = false;
				m_minimized = false;
				m_maximized = true;
				OnResize(width, height);
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (m_minimized)
				{
					m_appPaused = false;
					m_minimized = false;
					OnResize(width, height);
				}
				else if (m_maximized)
				{
					m_appPaused = false;
					m_maximized = false;
					OnResize(width, height);
				}
				else if (m_resizing)
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else
				{
					// API call such as SetWindowPos or m_swapChain->SetFullscreenState.
					OnResize(width, height);
				}
			}
		}
		return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

const HWND NtApplication::Handle()
{
	return m_hwnd;
}

void NtApplication::CalculateFrame()
{
	// 평균 fps계산, 한 프레임 렌더링하는데 걸리는 평균 시간 계산
	static int frameCount = 0;
	static float timeElapsed = 0.0f;

	frameCount++;

	// 1초 동안의 평균 프레임 수를 계산한다.
	if ((m_timer.TotalTime() - timeElapsed) >= 1.0f)
	{
		// fps = frameCount / 1
		float fps = (float)frameCount;
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << m_appName.Buffer() << L"      "
			<< L"FPS : " << fps << L"      "
			<< L"Frame Time : " << mspf << L" (ms)" << L"      "
			<< L"dt : " << m_timer.DeltaTime() << L"      ";

		SetWindowText(m_hwnd, outs.str().c_str());

		frameCount = 0;
		timeElapsed += 1.0f;
	}
}

const NtTimer& NtApplication::Timer() const
{
	return m_timer;
}

} }
