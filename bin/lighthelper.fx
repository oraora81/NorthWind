
// globals
struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float pad;
};

struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float Range;

    float3 Att;
    float pad;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Position;
    float Range;

    float3 Direction;
    float Spot;

    float3 Att;
    float pad;
};

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;    // w = SpecPower
    float4 Reflect;
};

// ���� ���� �����Ѵ� dxå ���� 7.3 ����
void ComputeDirectionalLight(Material mat, DirectionalLight L,
    float3 normal, float3 toEye,
    out float4 ambient,
    out float4 diffuse,
    out float4 spec)
{
    // ��� ���� �ʱ�ȭ
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // �� ���ʹ� ������ ���ư��� ������ �ݴ����
    float3 lightVec = -L.Direction;

    // �ֺ��� ���� ����
    ambient = mat.Ambient * L.Ambient;

    // ���� ������ �ʰ� ǥ�鿡 �����Ѵٴ� ���� �Ͽ�
    // �л걤 �װ� �ݿ���(specular)���� ���Ѵ�
    float diffuseFactor = dot(lightVec, normal);

    // ���� �б⸦ ���ϱ� ���� ���ǹ��� ��ģ��
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
}

// ���� ���� ���� dxå ���� 7.4 ����
void ComputePointLight(Material mat, PointLight L,
    float3 pos, float3 normal, float3 toEye,
    out float4 ambient, out float4 diffuse, out float4 spec)
{
    // ��� ���� �ʱ�ȭ
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // �� ����(ǥ�� ������ ���������� ����)
    float3 lightVec = L.Position - pos;

    // ǥ�� ���� ���� ������ �Ÿ�
    float d = length(lightVec);

    // ���� ����
    if (d > L.Range)
    {
        return;
    }

    // �� ���͸� ����ȭ
    lightVec /= d;

    // �ֺ��� ��
    ambient = mat.Ambient * L.Ambient;

    // ���� ������ �ʰ� ǥ�鿡 �����Ѵٴ� ���� �Ͽ���
    // �л걤 �װ� �ݿ��� ���� ������
    float diffuseFactor = dot(lightVec, normal);

    // ���� �б⸦ ���ϱ� ���� ���ǹ��� ��ģ��.
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }

    // ����
    float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));

    diffuse *= att;
    spec *= att;
}

// ���� ���� ���� dxå ���� 7.5 ����
void ComputeSpotLight(Material mat, SpotLight L,
    float3 pos, float3 normal, float3 toEye,
    out float4 ambient, out float4 diffuse, out float4 spec)
{
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // �� ����(ǥ�� ������ ���������� ����)
    float3 lightVec = L.Position - pos;

    // ǥ�� ���� ���� ������ �Ÿ�
    float d = length(lightVec);

    // ���� ����
    if (d > L.Range)
    {
        return;
    }

    // �� ���͸� ����ȭ
    lightVec /= d;

    // �ֺ���
    ambient = mat.Ambient * L.Ambient;

    // ���� ������ �ʰ� ǥ�鿡 �����Ѵٴ� ���� �Ͽ���
    // �л걤 �װ� �ݿ��� ���� ���Ѵ�.
    float diffuseFactor = dot(lightVec, normal);

    // ���� �б⸦ ���ϱ� ���� ���ǹ��� ��ģ��
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }

    // ������ ����� ����Ѵ�.
    float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

    // ���� ���⸦ ������ ����� ����ϰ� �Ÿ��� ���� �����Ų��.
    float att = spot / dot(L.Att, float3(1.0f, d, d*d));

    ambient *= spot;
    diffuse *= att;
    spec *= att;
}


// Debug
// fxc /Fc /Od /Zi /T fx_5_0 /Fo ...fxo ...
// Release
// fxc /T fx_5_0 /Fo ...fxo ...