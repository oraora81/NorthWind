
#include "stdafx.h"
#pragma hdrstop

#include "DxInput.h"

namespace NT
{

namespace _dx8
{

bool DxInput::Initialize(HINSTANCE hInst, HWND hwnd)
{
	HRESULT hr = DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_directInput,
		NULL);

	if (FAILED(hr))
	{
		return false;
	}

	if (m_directInput->CreateDevice(GUID_SysKeyboard, &m_directKeyboard, NULL))
	{
		return false;
	}

	if (m_directInput->CreateDevice(GUID_SysMouse, &m_directMouse, NULL))
	{
		return false;
	}

	// set keyboard
	if (m_directKeyboard->SetDataFormat(&c_dfDIKeyboard))
	{
		return false;
	}
	if (m_directKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))
	{
		return false;
	}

	// set mouse
	if (m_directMouse->SetDataFormat(&c_dfDIMouse))
	{
		return false;
	}
	if (m_directMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY))
	{
		return false;
	}

	return true;
}

bool DxInput::Release()
{
	m_directMouse->Unacquire();
	m_directKeyboard->Unacquire();
	m_directInput->Release();

	return true;
}

void DxInput::Process(const NtFloat& delta)
{
	m_directKeyboard->Acquire();
	m_directMouse->Acquire();

	m_directMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mouseCurrState);
	m_directKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);

	m_mouseLastState = m_mouseCurrState;
}

bool DxInput::GetKey(int key)
{
	NtAsserte(key < 256);

	return m_keyboardState[key] & 0x80 ? true : false;
}

bool DxInput::GetLeftDown()
{
	return m_mouseCurrState.lX != m_mouseLastState.lX;
}

bool DxInput::GetRightDown()
{
	return m_mouseCurrState.lY != m_mouseLastState.lY;
}


}

}