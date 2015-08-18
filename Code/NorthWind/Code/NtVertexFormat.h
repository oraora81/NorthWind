
#pragma once

namespace NT
{
namespace RENDERER
{

class NtVertexFormat
{
public:
	enum eFormatUsage
	{
		USAGE_POSITION		,
		USAGE_NORMAL		,
		USAGE_TANGENT		,
		USAGE_BINORMAL		,
		USAGE_TEXCOORD		,
		USAGE_COLOR			,
		USAGE_BLENDINDICES	,
		USAGE_BLENTWEIGHT	,
		USAGE_FOGCOORD		,
		USAGE_PSIZE			,
		USAGE_QUANTITY,
	};

	enum eFormatType
	{
		TYPE_NONE,
		TYPE_FLOAT1,
		TYPE_FLOAT2,
		TYPE_FLOAT3,
		TYPE_FLOAT4,
		TYPE_WORD1,
		TYPE_WORD2,
		TYPE_WORD3,
		TYPE_WORD4,
		TYPE_UBYTE4,
		TYPE_SHORT1,
		TYPE_SHORT2,
		TYPE_SHORT4,
		TYPE_QUANTITY,
	};

	NtVertexFormat();
	~NtVertexFormat();

	void SetStride(NtInt stride);
	bool SetFormatType(const NtWChar* usage, const NtWChar* type);
	NtInt GetSumOffset();

protected:
	struct NtElement
	{
		//NtUInt			streamIndex;
		NtUInt			offset;
		eFormatType		formatType;
		eFormatUsage	formatUsage;
		NtUInt			usageIndex;
	};

	NtElement	m_element[USAGE_QUANTITY];
	NtFlag		m_formatUsage;
};

} // namespace RENDERER
} // namespace NT