
#pragma once

namespace NT
{
namespace RENDERER
{

/**
 *	@brief		NtTexture :
 */
class NtRenderer;

class NtTexture
{
public:
	NtTexture();
	~NtTexture();

	bool Initialize(const NtWChar* fileName);
	void Release();

	// get
	ID3D11ShaderResourceView* GetTexture();
	inline NtUInt		Width() const;
	inline NtUInt		Height() const;
	inline NtUShort		UseCount() const;
	inline NtSize		Size() const;
	inline NtUInt		TexHandle() const;

	// set
	inline void SetTexHandle(NtUInt handle);
	inline void IncreaseUseCount();
	inline void DecreaseUseCount();

private:
	bool CreateTextureFromDDSFile(const NtWChar* fileName);
	bool CreateTextureFromResourceFile(const NtWChar* fileName);

private:
	ID3D11ShaderResourceView* m_textureView;
	ID3D11Resource*	m_texResource;
	NtUInt		m_width;
	NtUInt		m_height;
	NtUShort	m_useCount;
	NtSize		m_size;
	NtUInt		m_handle;
};



NtUInt NtTexture::Width() const
{
	return m_width;
}

NtUInt NtTexture::Height() const
{
	return m_height;
}

NtUShort NtTexture::UseCount() const
{
	return m_useCount;
}

NtSize NtTexture::Size() const
{
	return m_size;
}

NtUInt NtTexture::TexHandle() const
{
	return m_handle;
}

void NtTexture::SetTexHandle(NtUInt handle)
{
	m_handle = handle;
}

void NtTexture::IncreaseUseCount()
{
	++m_useCount;
}

void NtTexture::DecreaseUseCount()
{
	--m_useCount;
}

/**
 *	@brief		NtTexHandle :
 */

class NtTexHandle
{
public:
	NtTexHandle() 
		: m_linkTexture(nullptr), m_texHandle(0), m_usable(true)
	{}

	bool operator ==(NtUInt handle);

	inline void			SetTexture(NtTexture* texture);
	inline NtTexture*	GetTexture() const;
	inline NtUInt		GetHandle() const;
	inline void			SetHandle(NtUInt handle);
	inline void			SetUsable(bool flag);
	inline bool			IsUsable() const;

private:
	NtTexHandle(const NtTexture& handle);
	NtTexHandle& operator=(const NtTexHandle& handle);

private:
	NtTexture*	m_linkTexture;
	NtUInt		m_texHandle;
	bool		m_usable;
};

/*inline*/void NtTexHandle::SetTexture(NtTexture* texture)
{
	m_linkTexture = texture;
	SetUsable(false);
}


/*inline*/ NtTexture* NtTexHandle::GetTexture() const
{
	return m_linkTexture;
}

/*inline*/ NtUInt NtTexHandle::GetHandle() const
{
	return m_texHandle;
}

/*inline*/ void NtTexHandle::SetHandle(NtUInt handle)
{
	m_texHandle = handle;
}

/*inline*/ void NtTexHandle::SetUsable(bool flag)
{
	m_usable = flag;
}

/*inline*/ bool NtTexHandle::IsUsable() const
{
	return m_usable;
}




}
}
