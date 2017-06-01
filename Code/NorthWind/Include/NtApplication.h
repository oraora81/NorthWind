
#pragma once

#include "NtRenderer.h"
#include "NtInputManager.h"
#include "NtTimer.h"

namespace nt {

using namespace renderer;
using namespace INPUT;
using namespace FS;

namespace APP {

class NtApplication
{
public:
	NtApplication();
	virtual ~NtApplication() {}

	virtual bool Initialize(bool fullscreen);
	virtual void Shutdown();
	virtual bool OnResize(ntInt width, ntInt height);
	virtual bool Process();

	virtual void OnMouseDown(WPARAM buttonState, ntInt x, ntInt y) {}
	virtual void OnMouseUp(WPARAM buttonState, ntInt x, ntInt y) {}
	virtual void OnMouseMove(WPARAM buttonState, ntInt x, ntInt y) {}

	void MsgLoop();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	const HWND Handle();

	float AspectRatio() const
	{
		return static_cast<float>(m_width / m_height);
	}

private:
	NtString m_appName;
	NtString m_cmdArg;
	NtString m_globalPath;

	HWND m_hwnd;
	HINSTANCE m_hInst;
	NtTimer m_timer;

	ntUint m_width;
	ntUint m_height;
	bool m_fullScreen;
	bool m_minimized;
	bool m_maximized;
	bool m_resizing;
	bool m_appPaused;

	// dependency
	NtRenderer* m_renderer;
	NtInputManager* m_inputManager;
	NtResourceManager* m_resManager;
};

}	// namespace APP

}	// namespace nt


namespace nt {	namespace APP {
	class NtApplication;
}

namespace FS {
	class NtResourceManager;
}

} // namespace nt



extern nt::APP::NtApplication* g_app;
extern nt::FS::NtResourceManager* g_resManager;
extern nt::Memory::NtLinearAllocator* g_liAllocator;
extern nt::renderer::NtRenderer* g_renderInterface;
extern std::shared_ptr<nt::renderer::NtDx11Renderer> g_renderer;


static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		return 0;

	case WM_CLOSE:
		{
			PostQuitMessage(0);
		}
		return 0;
	}

	return g_app->MessageHandler(hwnd, message, wParam, lParam);
}
