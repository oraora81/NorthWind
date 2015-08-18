
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

	bool Initialize(const NtWChar* fileName, NtInt screenWidth, NtInt screenHeight);
	void Release();

	
	// inline function
	inline NtInt GetIndexCount() const;

private:
	bool InitializeBuffer();

private:
	NtUInt	m_texHandle;
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	NtInt	m_vertexCount;
	NtInt	m_indexCount;
	NtPosition m_screenSize;
	NtPosition m_prevPos;
};


// inline implement
NtInt NtImage::GetIndexCount() const
{
	return m_indexCount;
}

}	// namespace RENDERER
}	// namespace NT
