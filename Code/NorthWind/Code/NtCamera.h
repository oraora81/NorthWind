
#pragma once

namespace NT
{
namespace RENDERER
{

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

private:
	float m_pos[3];
	float m_rot[3];
	XMMATRIX m_viewMatrix;
};

}	// RENDERER
}	// namespace NT