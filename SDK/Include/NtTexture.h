
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

	bool Initialize(const ntWchar* fileName);
	void Release();

	// get
	ID3D11ShaderResourceView* GetTexture();
	inline ntUint		Width() const;
	inline ntUint		Height() const;
	inline ntUshort		UseCount() const;
	inline ntSize		Size() const;
	inline ntUint		TexHandle() const;

	// set
	inline void SetTexHandle(ntUint handle);
	inline void IncreaseUseCount();
	inline void DecreaseUseCount();

private:
	bool CreateTextureFromDDSFile(const ntWchar* fileName);
	bool CreateTextureFromResourceFile(const ntWchar* fileName);

private:
	ID3D11ShaderResourceView* m_textureView;
	ID3D11Resource*	m_texResource;
	ntUint		m_width;
	ntUint		m_height;
	ntUshort	m_useCount;
	ntSize		m_size;
	ntUint		m_handle;
};



ntUint NtTexture::Width() const
{
	return m_width;
}

ntUint NtTexture::Height() const
{
	return m_height;
}

ntUshort NtTexture::UseCount() const
{
	return m_useCount;
}

ntSize NtTexture::Size() const
{
	return m_size;
}

ntUint NtTexture::TexHandle() const
{
	return m_handle;
}

void NtTexture::SetTexHandle(ntUint handle)
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

	bool operator ==(ntUint handle);

	inline void			SetTexture(NtTexture* texture);
	inline NtTexture*	GetTexture() const;
	inline ntUint		GetHandle() const;
	inline void			SetHandle(ntUint handle);
	inline void			SetUsable(bool flag);
	inline bool			IsUsable() const;

private:
	NtTexHandle(const NtTexture& handle);
	NtTexHandle& operator=(const NtTexHandle& handle);

private:
	NtTexture*	m_linkTexture;
	ntUint		m_texHandle;
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

/*inline*/ ntUint NtTexHandle::GetHandle() const
{
	return m_texHandle;
}

/*inline*/ void NtTexHandle::SetHandle(ntUint handle)
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
