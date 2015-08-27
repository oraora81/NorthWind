
#include "NtCoreLib.h"
#include "NtLight.h"

namespace nt { namespace renderer {


NtLight::NtLight()
{

}

NtLight::~NtLight()
{

}

void NtLight::SetDiffuse( float r, float g, float b, float a )
{
	m_diffuse = XMFLOAT4A(r, g, b, a);
}

void NtLight::SetDirection( float x, float y, float z )
{
	m_direction = XMFLOAT3A(x, y, z);
}

const XMFLOAT4A& NtLight::GetDiffuse() const
{
	return m_diffuse;
}

const XMFLOAT3A& NtLight::GetDirection() const
{
	return m_direction;
}


}	// namespace renderer
}	// namespace nt
