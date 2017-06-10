#pragma once

#include "NtApplication.h"

using namespace nt;

class TheApp : public nt::app::NtApplication
{
public:
	TheApp();

	virtual ~TheApp() override;

	virtual bool Initialize(bool fullscreen, ntInt width, ntInt height) override;

	virtual void OnMouseDown(WPARAM buttonState, ntInt x, ntInt y) override;

	virtual void OnMouseUp(WPARAM buttonState, ntInt x, ntInt y) override;
};
