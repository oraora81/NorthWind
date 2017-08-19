#pragma once

#include "NtApplication.h"

class Box;
class Hills;
class Shapes;
class Skull;
class WaveModel;
class Points;
class Pyramid;

class TheApp : public nt::app::NtApplication
{
public:
	TheApp();

	virtual ~TheApp() override;

	virtual bool Initialize(bool fullscreen, ntInt width, ntInt height) override;

	virtual void OnMouseDown(WPARAM buttonState, ntInt x, ntInt y) override;

	virtual void OnMouseUp(WPARAM buttonState, ntInt x, ntInt y) override;

	virtual void OnMouseMove(WPARAM buttonState, ntInt x, ntInt y) override;

private:
	POINT m_lastMousePos;
	//Box* m_model;
	//Hills* m_model;
    //Shapes* m_model;
	//Skull* m_model;
	//WaveModel* m_model;
    //Points* m_model;
    Pyramid* m_model;
};
