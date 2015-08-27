
#pragma once

#include "NtRenderer.h"
#include "NtInputManager.h"


namespace nt {

using namespace renderer;
using namespace INPUT;
using namespace FS;

namespace APP {

class NtApplication
{
public:
	NtApplication() {}
	virtual ~NtApplication() {}

	bool Initialize(bool fullscreen);
	void Shutdown();

	void MsgLoop();
	bool Process();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	const HWND Handle();

private:
	NtString m_appName;
	NtString m_cmdArg;
	NtString m_globalPath;

	HWND m_hwnd;
	HINSTANCE m_hInst;

	bool m_fullScreen;

	// dependency
	NtRenderer* m_renderer;
	NtInputManager* m_inputManager;
	NtResourceManager* m_resManager;
};

}	// namespace APP

}	// namespace nt


namespace nt {
namespace APP {
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
extern std::shared_ptr<nt::renderer::NtDirectX11Renderer> g_renderer;


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
