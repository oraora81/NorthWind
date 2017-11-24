#pragma once

class Waves
{
public:
	Waves();

	~Waves();

	UINT RowCount() const;

	UINT ColumnCount() const;

	UINT VertexCount() const;

	UINT TrisCount() const;

	const XMFLOAT3 operator[](int i) const
	{
		return m_currSolution[i];
	}

	void Init(UINT m, UINT n, float dx, float dt, float speed, float damping);

	void Update(float dt);

	void Disturb(UINT i, UINT j, float magnitude);

    const XMFLOAT3& Normal(ntInt i) const { return m_normals[i]; }

    const XMFLOAT3& TangentX(ntInt i) const { return m_tangentX[i]; }

private:
	UINT m_numRow;
	UINT m_numColumn;
	UINT m_vertexCount;
	UINT m_trisCount;

	float m_k1;
	float m_k2;
	float m_k3;

	float m_timeStep;
	float m_spatialStep;

	XMFLOAT3* m_prevSolution;
	XMFLOAT3* m_currSolution;
    XMFLOAT3* m_normals;
    XMFLOAT3* m_tangentX;
};
