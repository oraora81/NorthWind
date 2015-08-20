
#pragma once

namespace nt
{
	class IRenderer
	{
	public:
		~IRenderer() {}

		// Init Render Setting
		virtual bool Initialize(HWND hwnd) = 0;
		virtual bool Release() = 0;
		virtual void Render() = 0;
		virtual void Process(const ntFloat& delta) = 0;

		inline void	SetWidth(ntInt width) { NtAsserte(width > 0); m_screenWidth = width; }
		inline void SetHeight(ntInt height) { NtAsserte(height > 0); m_screenHeight = height; }

		HWND	m_hwnd;
		int		m_screenWidth;
		int		m_screenHeight;
	};
}	// namespace nt
