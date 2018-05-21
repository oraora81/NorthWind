
#include "NtCoreLib.h"
#include "NtCamera.h"

namespace nt { namespace renderer {

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

const XMFLOAT3A NtCamera::GetPosition() const
{
	return XMFLOAT3A(m_pos[0], m_pos[1], m_pos[2]);
}


const XMFLOAT3A NtCamera::GetRotation() const
{
	return XMFLOAT3A(m_rot[0], m_rot[1], m_rot[2]);
}


void NtCamera::Render()
{
	// up���� ����
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	
	// ���忡���� ī�޶� ��ġ ����
	// XMVECTOR pos = XMVectorSet(m_pos[0], m_pos[1], m_pos[2], 0.0f); ???
	 XMVECTOR pos = XMVectorSet(m_pos[0], m_pos[1], m_pos[2], 1.0f);
	
	// �⺻ ī�޶� ���� ����
	XMVECTOR look = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// yaw(Y axis), pitch(X), roll(Z) �������� ȸ���� ����
	float pitch = m_rot[0] * NtMathf::DEG_TO_RAD;
	float yaw = m_rot[1] * NtMathf::DEG_TO_RAD;
	float roll = m_rot[2] * NtMathf::DEG_TO_RAD;

	// yaw, pitch, roll ������ ȸ�� ��� ����
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	
	// look�� up���͸� ����� �̿��� ��ȯ
	look = XMVector3TransformCoord(look, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);
	
	// ȸ���� ī�޶� ��ġ�� ����� ��ġ�� �̵�
	look = pos + look;

	// �� ��� ����
	m_viewMatrix = XMMatrixLookAtLH(pos, look, up);
}


void NtCamera::GetViewMatrix(XMMATRIX& view)
{
	view = m_viewMatrix;
}

}
}
