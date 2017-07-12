#include "stdafx.h"
#pragma hdrstop

#include "TheApp.h"

// c++ importer interface
#include "importer.hpp"
#include "scene.h"
#include "postprocess.h"

#include "NtLog.h"
#include "Box.h"

using namespace nt;
using namespace nt::log;
using namespace nt::renderer;

namespace
{
	void DoImport(const ntChar* fileName)
	{
		// Create an instance of the Importer class
		Assimp::Importer importer;
		// And have it read the given file with some example postprocessing
		// Usually - if speed is not the most important aspect for you - you'll 
		// propably to request more postprocessing than we do in this example.

		std::string pFile = fileName;

		const aiScene* scene = importer.ReadFile(pFile,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		// If the import failed, report it
		if (!scene)
		{
			//DoTheErrorLogging(importer.GetErrorString());
			return;
		}
		// Now we can access the file's contents. 
		//DoTheSceneProcessing(scene);
		// We're done. Everything will be cleaned up by the importer destructor
		return;
	}
}

TheApp::TheApp()
	: NtApplication()
{
	m_lastMousePos.x = 0;
	m_lastMousePos.y = 0;
}

TheApp::~TheApp()
{

}

bool TheApp::Initialize(bool fullscreen, ntInt width, ntInt height)
{
	bool result = NtApplication::Initialize(fullscreen, width, height);
	if (result == false)
	{
		return false;
	}

	//const ntWchar* fname = g_resManager->GetPath(L"test_cube.ase");
	//ntChar buf[256];
	//Crt::WideStrToMultiStr(buf, Crt::StrLen(buf), fname);
	//DoImport(buf);

	NtModel::NtPCVertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), (const float*)&Colors::White },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), (const float*)&Colors::Black },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), (const float*)&Colors::Red },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), (const float*)&Colors::Green },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), (const float*)&Colors::Blue },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), (const float*)&Colors::Yellow },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), (const float*)&Colors::Cyan },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), (const float*)&Colors::Magenta },
	};

	ntUint indices[] = 
	{
		0, 1, 2,
		0, 2, 3,

		4, 6, 5, 
		4, 7, 6,

		4, 5, 1,
		4, 1, 0, 

		3, 2, 6,
		3, 6, 7,

		1, 5, 6,
		1, 6, 2,

		4, 0, 3,
		4, 3, 7
	};

	m_model = new Box();
	m_model->IntializeModelData(vertices, _countof(vertices), indices, _countof(indices), L"../Code/Lucia/simple_fx.fxo");


	g_renderInterface->AddModel(m_model);

	return true;
}

void TheApp::OnMouseDown(WPARAM buttonState, ntInt x, ntInt y)
{
	m_lastMousePos.x = x;
	m_lastMousePos.y = y;

	SetCapture(Handle());
}

void TheApp::OnMouseUp(WPARAM buttonState, ntInt x, ntInt y)
{
	ReleaseCapture();
}


void TheApp::OnMouseMove(WPARAM buttonState, ntInt x, ntInt y)
{
	if ((buttonState & MK_LBUTTON) != 0)
	{
		// 1픽셀이 4분의 1도(디그리)가 되게 한다.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - m_lastMousePos.x));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - m_lastMousePos.y));

		// 마우스 입력에 기초한 각도로 상자 주변의 궤도 카메라를 갱신한다.
		m_model->Theta() += dx;
		m_model->Phi() += dy;

		float phi = NtMath<float>::Clamp(m_model->Phi(), 0.1f, NtMath<float>::PI - 0.1f);
		m_model->Phi(phi);
	}
	else if ((buttonState & MK_RBUTTON) != 0)
	{
		// 1픽셀이 장면의 0.005 단위가 되게 한다.
		float dx = 0.005f * static_cast<float>(x - m_lastMousePos.x);
		float dy = 0.005f * static_cast<float>(y - m_lastMousePos.y);

		float r = m_model->Radius();
		r += (dx - dy);
		r = NtMath<float>::Clamp(r, 3.0f, 15.0f);
		m_model->Radius(r);
	}

	m_lastMousePos.x = x;
	m_lastMousePos.y = y;
}
