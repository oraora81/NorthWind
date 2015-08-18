
#pragma once

namespace NT
{
namespace RENDERER
{

class NtImage
{
public:
	NtImage();
	virtual ~NtImage();

	bool Initialize(const ntWchar* fileName, ntInt screenWidth, ntInt screenHeight);
	void Release();

	
	// inline function
	inline ntInt GetIndexCount() const;

private:
	bool InitializeBuffer();

private:
	ntUint	m_texHandle;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	ntInt	m_vertexCount;
	ntInt	m_indexCount;
	NtPosition m_screenSize;
	NtPosition m_prevPos;
};


// inline implement
ntInt NtImage::GetIndexCount() const
{
	return m_indexCount;
}

}	// namespace RENDERER
}	// namespace NT
