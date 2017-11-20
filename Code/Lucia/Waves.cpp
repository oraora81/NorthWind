#include "stdafx.h"
#pragma hdrstop

#include "Waves.h"

Waves::Waves()
	: m_numRow(0)
	, m_numColumn(0)
	, m_vertexCount(0)
	, m_trisCount(0)
	, m_k1(0.0f)
	, m_k2(0.0f)
	, m_k3(0.0f)
	, m_timeStep(0.0f)
	, m_spatialStep(0.0f)
	, m_prevSolution(nullptr)
	, m_currSolution(nullptr)
    , m_normals(nullptr)
    , m_tangentX(nullptr)
{

}

Waves::~Waves()
{
	SAFE_DELETE_ARRAY(m_prevSolution;)
	SAFE_DELETE_ARRAY(m_currSolution);
    SAFE_DELETE_ARRAY(m_normals);
    SAFE_DELETE_ARRAY(m_tangentX);
}

UINT Waves::RowCount() const
{
	return m_numRow;
}

UINT Waves::ColumnCount() const
{
	return m_numColumn;
}

UINT Waves::VertexCount() const
{
	return m_vertexCount;
}

UINT Waves::TrisCount() const
{
	return m_trisCount;
}

void Waves::Init(UINT m, UINT n, float dx, float dt, float speed, float damping)
{
	m_numRow = m;
	m_numColumn = n;

	m_vertexCount = m * n;
	m_trisCount = (m - 1) * (n - 1) * 2;

	m_timeStep = dt;
	m_spatialStep = dx;

	float d = damping * dt + 2.0f;
	float e = (speed * speed) * (dt * dt) / (dx * dx);
	m_k1 = (damping * dt - 2.0f) / d;
	m_k2 = (4.0f - 8.0f * e) / d;
	m_k3 = (2.0f * e) / d;  

	// in case init() called again
	delete[] m_prevSolution;
	delete[] m_currSolution;

	m_prevSolution = new XMFLOAT3[m*n];
	m_currSolution = new XMFLOAT3[m*n];
    m_normals = new XMFLOAT3[m*n];
    m_tangentX = new XMFLOAT3[m*n];

	// generate grid vertices in sys mem
	float  halfWidth = (n - 1) * dx * 0.5f;
	float  halfDepth = (m - 1) * dx * 0.5f;
	for (UINT i = 0; i < m; i++)
	{
		float z = halfDepth - i * dx;
		for (UINT j = 0; j < n; j++)
		{
			float x = -halfWidth + j * dx;

			m_prevSolution[i*n + j] = XMFLOAT3(x, 0.0f, z);
			m_currSolution[i*n + j] = XMFLOAT3(x, 0.0f, z);
            m_normals[i*n + j] = XMFLOAT3(0.0f, 1.0f, 0.0f);
            m_tangentX[i*n + j] = XMFLOAT3(1.0f, 0.0f, 0.0f);
		}
	}
}

void Waves::Update(float dt)
{
	static float t = 0;

	// accumulate time
	t += dt;

	// only update the simulation at the specified time step
	if (t >= m_timeStep)
	{
		// only update interior points. we use zero boundary conditions
		for (DWORD i = 1; i < m_numRow - 1; ++i)
		{
			for (DWORD j = 1; j < m_numColumn - 1; ++j)
			{
				// After this update we will be discarding the old previous
				// buffer, so overwrite that buffer with the new update.
				// Note how we can do this inplace (read/write to same element) 
				// because we won't need prev_ij again and the assignment happens last.

				// Note j indexes x and i indexes z: h(x_j, z_i, t_k)
				// Moreover, our +z axis goes "down"; this is just to 
				// keep consistent with our row indices going down.

				m_prevSolution[i*m_numColumn + j].y =
					m_k1 * m_prevSolution[i * m_numColumn + j].y +
					m_k2 * m_currSolution[i * m_numColumn + j].y +
					m_k3 * (m_currSolution[(i + 1)*m_numColumn + j].y +
						m_currSolution[(i - 1)*m_numColumn + j].y +
						m_currSolution[i*m_numColumn + j + 1].y +
						m_currSolution[i*m_numColumn + j - 1].y);
			}
		}

		// We just overwrote the previous buffer with the new data, so
		// this data needs to become the current solution and the old
		// current solution becomes the new previous solution.
		std::swap(m_prevSolution, m_currSolution);

		t = 0.0f;

        // compute normal using finite difference scheme
        for (ntUint i = 1; i < m_numRow - 1; ++i)
        {
            for (ntUint j = 1; j < m_numColumn - 1; ++j)
            {
                float l = m_currSolution[i * m_numColumn + j - 1].y;
                float r = m_currSolution[i * m_numColumn + j + 1].y;
                float t = m_currSolution[(i - 1) * m_numColumn + j].y;
                float b = m_currSolution[(i + 1) * m_numColumn + j].y;

                m_normals[i * m_numColumn + j].x = -r + l;
                m_normals[i * m_numColumn + j].y = 2.0f * m_spatialStep;
                m_normals[i * m_numColumn + j].z = b - t;

                XMVECTOR n = XMVector3Normalize((XMLoadFloat3(&m_normals[i * m_numColumn + j])));
                XMStoreFloat3(&m_normals[i*m_numColumn + j], n);

                m_tangentX[i*m_numColumn + j] = XMFLOAT3(2.0f * m_spatialStep, r - l, 0.0f);
                XMVECTOR T = XMVector3Normalize(XMLoadFloat3(&m_tangentX[i*m_numColumn + j])));
                XMStoreFloat3(&m_tangentX[i * m_numColumn + j], T);
            }
        }
	}
}

void Waves::Disturb(UINT i, UINT j, float magnitude)
{
	// Do not disturb boundaries
	NtAsserte(i > 1 && i < m_numRow - 2);
	NtAsserte(j > 1 && i < m_numColumn - 2);

	float halfMag = 0.5f * magnitude;

	// disturb the ijth vertex height and its neighbors
	m_currSolution[i*m_numColumn + j].y += magnitude;
	m_currSolution[i*m_numColumn + j + 1].y += halfMag;
	m_currSolution[i*m_numColumn + j - 1].y += halfMag;
	m_currSolution[(i + 1)*m_numColumn + j].y += halfMag;
	m_currSolution[(i - 1)*m_numColumn + j].y += halfMag;
}
