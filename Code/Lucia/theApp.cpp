#include "stdafx.h"
#pragma hdrstop

#include "TheApp.h"

// c++ importer interface
#include "importer.hpp"
#include "scene.h"
#include "postprocess.h"

#include "NtLog.h"
#include "NtModel.h"

using namespace nt::log;

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
	, m_theta(1.5f * NtMath<float>::PI)
	, m_phi(0.25f * NtMath<float>::PI)
	, m_radius(5.0f)
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
		{ XMFLOAT3(+1.0f, +-1.0f, -1.0f), (const float*)&Colors::Red },
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


	NtModel model;
	model.IntializeModelData(vertices, _countof(vertices), indices, _countof(indices));

	return true;
}

void TheApp::OnMouseDown(WPARAM buttonState, ntInt x, ntInt y)
{
	NTRACE(L"Down - x : %d, y : %%d", x, y);
}

void TheApp::OnMouseUp(WPARAM buttonState, ntInt x, ntInt y)
{
	NTRACE(L"Up - x : %d, y : %%d", x, y);
}


bool TheApp::Process()
{
	float dt = DeltaTime();

	float x = m_radius * NtMath<float>::Sin(m_phi) * NtMath<float>::Cos(m_theta);
	float z = m_radius * NtMath<float>::Sin(m_phi) * NtMath<float>::Sin(m_theta);
	float y = m_radius * NtMath<float>::Cos(m_phi);

	return true;
}