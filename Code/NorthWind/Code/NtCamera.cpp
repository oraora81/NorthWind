
#include "NtCoreLib.h"
#include "NtCamera.h"

using namespace NT;
using namespace RENDERER;

NtCamera::NtCamera()
{
	Crt::MemSet(m_pos, sizeof(m_pos));
	Crt::MemSet(m_rot, sizeof(m_rot));
}


NtCamera::~NtCamera()
{

}

void NtCamera::SetPosition(float x, float y, float z)
{
	m_pos[0] = x;
	m_pos[1] = y;
	m_pos[2] = z;
}


void NtCamera::SetRotation(float x, float y, float z)
{
	m_rot[0] = x;
	m_rot[1] = y;
	m_rot[2] = z;
}

const XMFLOAT3A NtCamera::GetPosition()
{
	return XMFLOAT3A(m_pos[0], m_pos[1], m_pos[2]);
}


const XMFLOAT3A NtCamera::GetRotation()
{
	return XMFLOAT3A(m_rot[0], m_rot[1], m_rot[2]);
}


void NtCamera::Render()
{
	// setup the vector that points upwards;
	XMVECTOR up;
	up.m128_f32[0] = 0.0f;
	up.m128_f32[1] = 1.0f;
	up.m128_f32[2] = 0.0f;
	up.m128_f32[3] = 0.0f;
	
	// setup the position of the camera in the world.
	XMVECTOR pos;//(m_pos[0], m_pos[1], m_pos[2]);
	pos.m128_f32[0] = m_pos[0];
	pos.m128_f32[1] = m_pos[1];
	pos.m128_f32[2] = m_pos[2];
	pos.m128_f32[3] = 0.0f;

	// setup where the camera is looking by default
	//XMVECTOR look(0.0f, 0.0f, 1.0f, 0.0f)
	XMVECTOR look;
	look.m128_f32[0] = 0.0f;
	look.m128_f32[1] = 0.0f;
	look.m128_f32[2] = 1.0f;
	look.m128_f32[3] = 0.0f;

	// set the yaw(Y axis), pitch(X), and roll(Z) rotations in radians.
	ntFloat pitch = m_rot[0] * NtMath<float>::DEG_TO_RAD;
	ntFloat yaw = m_rot[1] * NtMath<float>::DEG_TO_RAD;
	ntFloat roll = m_rot[2] * NtMath<float>::DEG_TO_RAD;

	// create the rotation matrix from the yaw, pitch and roll values
	XMMATRIX rotationMatrix;
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// transform the lookat and up vector by the rotation matrix so the view
	// is correctly rotated at the origin.
	look = XMVector3TransformCoord(look, rotationMatrix);
	//D3DXVec3TransformCoord(&look, &look, &rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);
	//D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// translate the rotated camera position to the location of the viewer.
	look = pos + look;

	// finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(pos, look, up);
	//D3DXMatrixLookAtLH(&m_viewMatrix, &pos, &look, &up);
}


void NtCamera::GetViewMatrix(XMMATRIX& view)
{
	view = m_viewMatrix;
}
