
#pragma once

namespace nt
{

namespace _dx8
{

class DxInput
{
public:
	DxInput() {}
	~DxInput() {}

	bool Initialize(HINSTANCE hInst, HWND hwnd);
	bool Release();

	void Process(const ntFloat& delta);

	bool GetKey(int key);
	bool GetLeftDown();
	bool GetRightDown();

private:
	IDirectInputDevice8* m_directKeyboard;
	IDirectInputDevice8* m_directMouse;

	LPDIRECTINPUT8		m_directInput;

	BYTE				m_keyboardState[256];
	DIMOUSESTATE		m_mouseCurrState;
	DIMOUSESTATE		m_mouseLastState;
};

}	// namespace _dx8

}	// namespace nt
