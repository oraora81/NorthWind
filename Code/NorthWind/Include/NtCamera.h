
#pragma once

namespace nt { namespace renderer {

__declspec(align(16))
class NtCamera
{
public:
	NtCamera();
	~NtCamera();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	const XMFLOAT3A GetPosition();
	const XMFLOAT3A GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX& view);


	void* operator new(size_t s)
	{
		return _aligned_malloc(s, 16);
	}

		void operator delete(void* obj)
	{
		_aligned_free(obj);
	}

private:
	float m_pos[3];
	float m_rot[3];
	XMMATRIX m_viewMatrix;
};

}	// RENDERER
}	// namespace nt
