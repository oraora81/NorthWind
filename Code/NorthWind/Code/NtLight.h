
#pragma once

namespace NT
{
namespace RENDERER
{

class NtLight
{
public:
	NtLight();
	~NtLight();

	void SetDiffuse(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);

	const XMFLOAT4A& GetDiffuse() const;
	const XMFLOAT3A& GetDirection() const;

private:
	XMFLOAT4A m_diffuse;
	XMFLOAT3A m_direction;
};

}	// namespace RENDERER
}	// namespace NT
