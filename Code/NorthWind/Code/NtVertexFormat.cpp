
#include "NtCoreLib.h"

#include "NtVertexFormat.h"

namespace NT
{
namespace RENDERER
{

struct VertexFormatUsage
{
	ntWchar*	keyword;
	ntInt		usage;
};

const static VertexFormatUsage g_vertexUsageList[] = 
{
	L"pos",			0,	// USAGE_POSITION,
	L"normal",		1,	// USAGE_NORMAL,
	L"tan",			2,	// USAGE_TANGENT,
	L"binormal",	3,	// USAGE_BINORMAL,
	L"tex",			4,	// USAGE_TEXCOORD,
	L"color",		5,	// USAGE_COLOR,
	L"blendindex",	6,	// USAGE_BLENDINDICES,
	L"blendweight",	7,	// USAGE_BLENTWEIGHT,
	L"fog",			8,	// USAGE_FOGCOORD,
};
//--

struct sVertexFormatType
{
	ntWchar*	keyword;
	ntInt		size;
	ntInt		offset;
};

const static sVertexFormatType g_vertexFormatType[] =
{
	L"",			0,		0,
	L"float1",		4,		1,
	L"float2",		8,		2,
	L"float3",		12,		3,
	L"float4",		16,		4,
							
	L"word1",		2,		1,
	L"word2",		4,		2,
	L"word3",		6,		3,
	L"word4",		8,		4,
							
	L"dword",		4,		1,
							
	L"short1",		2,		1,
	L"short2",		4,		2,
	L"short3",		8,		4,
};
//--

NtVertexFormat::NtVertexFormat()
{
	Crt::MemSet(m_element, sizeof(m_element));
}

NtVertexFormat::~NtVertexFormat()
{

}


bool NtVertexFormat::SetFormatType( const ntWchar* usage, const ntWchar* type )
{
	ntInt size = _countof(g_vertexUsageList);
	ntInt usageIndex = -1;
	for (ntInt i = 0; i < size; ++i)
	{
		if (Crt::StrCmp(usage, g_vertexUsageList[i].keyword) == 0)
		{
			m_element[i].formatUsage = (eFormatUsage)g_vertexUsageList[i].usage;
			usageIndex = i;
			break;
		}
	}

	if (usageIndex == -1)
	{
		return false;
	}

	size = _countof(g_vertexFormatType);
	for (ntInt i = 0; i < size; ++i)
	{
		if (Crt::StrCmp(type, g_vertexFormatType[i].keyword) == 0)
		{
			m_element[usageIndex].formatType = (eFormatType)i;
			m_element[usageIndex].offset = g_vertexFormatType[i].offset;
			return true;
		}
	}

	return true;
}

NT::ntInt NtVertexFormat::GetSumOffset()
{
	ntInt sum = 0;
	ntInt size = _countof(m_element);
	for (ntInt i = 0; i < size; ++i)
	{
		if (m_element[i].formatType == 0)
		{
			continue;
		}

		sum += m_element[i].offset;
	}

	return sum;
}


}	// namespace RENDERER
}	// namespace NT