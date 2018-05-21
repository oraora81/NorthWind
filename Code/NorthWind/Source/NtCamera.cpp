
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
	// up벡터 설정
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	
	// 월드에서의 카메라 위치 설정
	// XMVECTOR pos = XMVectorSet(m_pos[0], m_pos[1], m_pos[2], 0.0f); ???
	 XMVECTOR pos = XMVectorSet(m_pos[0], m_pos[1], m_pos[2], 1.0f);
	
	// 기본 카메라 방향 설정
	XMVECTOR look = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// yaw(Y axis), pitch(X), roll(Z) 라디안으로 회전값 설정
	float pitch = m_rot[0] * NtMathf::DEG_TO_RAD;
	float yaw = m_rot[1] * NtMathf::DEG_TO_RAD;
	float roll = m_rot[2] * NtMathf::DEG_TO_RAD;

	// yaw, pitch, roll 값으로 회전 행렬 설정
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	
	// look과 up벡터를 행렬을 이용해 변환
	look = XMVector3TransformCoord(look, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);
	
	// 회전된 카메라 위치를 뷰어의 위치로 이동
	look = pos + look;

	// 뷰 행렬 생성
	m_viewMatrix = XMMatrixLookAtLH(pos, look, up);
}


void NtCamera::GetViewMatrix(XMMATRIX& view)
{
	view = m_viewMatrix;
}

}
}
