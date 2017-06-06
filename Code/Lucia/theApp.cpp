#include "stdafx.h"
#pragma hdrstop

#include "TheApp.h"

// c++ importer interface
#include "importer.hpp"
#include "scene.h"
#include "postprocess.h"

#include "NtLog.h"

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
{

}

TheApp::~TheApp()
{

}

bool TheApp::Initialize(bool fullscreen)
{
	bool result = NtApplication::Initialize(fullscreen);
	if (result == false)
	{
		return false;
	}

	const ntWchar* fname = g_resManager->GetWholePath(L"test_cube.ase");

	ntChar buf[256];

	Crt::WideStrToMultiStr(buf, Crt::StrLen(buf), fname);

	DoImport(buf);

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

