
#include "NtCoreLib.h"
#pragma hdrstop

#include "NtModelParser.h"
#include "NtVertexFormat.h"
#include "NtPuppet.h"

namespace nt {

const ntWchar* m_keywordList[MAX_PARSE_KEY] = 
{
	L"",

	L"*vtxnum",
	L"*vtxtype",
	L"*vtxinfo",

	// 
	L"{",
	L"}",
};



NtParseModeBinder::NtParseModeBinder( const ntWchar* str )
	: m_target(str)
{

}

nt::ntInt NtParseModeBinder::FindParseKey()
{
	ntInt index = -1;

	NtString& src = m_target;
	auto findRes = std::find_if(std::begin(m_keywordList), std::end(m_keywordList), [&src, &index](const ntWchar* keyword) -> bool
	{
		++index;
		if (src == keyword)
		{
			return true;
		}

		return false;
	});
	
	return findRes != std::end(m_keywordList) ? index : -1;
}


//

NtModelParser::NtModelParser()
	: m_linkPuppet(nullptr)
{

}


NtModelParser::~NtModelParser()
{

}

bool NtModelParser::Execute(const ntWchar* fileName)
{
	bool res = m_file.Execute(fileName, FS::IO_READ);
	if (false == res)
	{
		return false;
	}

	while(!m_file.IsEOF())
	{
		if (FindKeyword() == false)
		{
			continue;
		}

		bool parseRes = ProcessParse();
		if (false == parseRes)
		{
			return false;
		}
	}

	return true;
}

bool NtModelParser::ProcessParse()
{
	switch(m_parseMode)
	{
	case PARSE_VERTEX_NUM:
		return ProcessVertexNum();
	case PARSE_VERTEX_TYPE:
		return ProcessVertexType();
	case PARSE_VERTEX_INFO:
		return ProcessVertexInfo();
	}

	return true;
}


bool NtModelParser::ProcessVertexNum()
{
	while (!m_file.IsEOF())
	{
		if (FindKeyword(PARSE_BODY_START) == false)
		{
			continue;
		}

		if (m_file.ReadTag() == false)
		{
			return false;
		}

		ntInt vertexCount = Crt::StringToNumber(m_file.GetData());
		if (vertexCount == 0)
		{
			return false;
		}

		m_linkPuppet->SetVertexCount(vertexCount);

		if (FindKeyword(PARSE_BODY_END) == false)
		{
			return false;
		}
		break;
	}

	return true;
}


bool NtModelParser::ProcessVertexType()
{
	while(!m_file.IsEOF())
	{
		if (FindKeyword(PARSE_BODY_START) == false)
		{
			continue;
		}

		bool res = ReadVertexType();
		if (res == false)
		{
			return false;
		}
		else
		{
			break;
		}
	}

	return true;
}


bool NtModelParser::ProcessVertexInfo()
{
	while(!m_file.IsEOF())
	{
		if (FindKeyword(PARSE_BODY_START) == false)
		{
			continue;
		}

		ReadVertexInfo();

		if (FindKeyword(PARSE_BODY_END) == false)
		{
			return false;
		}
		break;
	}

	return true;
}


bool NtModelParser::FindKeyword()
{
	if (m_file.ReadTag() == false)
	{
		return false;
	}

	if (DetermineParseMode(m_file.GetData()) == false)
	{
		return false;
	}

	return true;
}

bool NtModelParser::FindKeyword( PARSE_KEYWORD parseKey )
{
	if (parseKey >= MAX_PARSE_KEY)
	{
		return false;
	}

	if (!m_file.ReadTag())
	{
		return false;
	}

	if (Crt::StrCmp(m_file.GetData(), m_keywordList[parseKey]) == 0)
	{
		return true;
	}

	return false;
}

bool NtModelParser::DetermineParseMode(const ntWchar* src)
{
	NtParseModeBinder binder(src);

	ntInt index = binder.FindParseKey();
	if (index == -1)
	{
		return false;
	}

	m_parseMode = (PARSE_KEYWORD)index;

	return true;
}


bool NtModelParser::ReadVertexType()
{
	ntWchar usage[64] = {0, };
	ntWchar type[64] = {0, };

	while(!m_file.IsEOF())
	{
		Crt::MemSet(usage, sizeof(usage));
		Crt::MemSet(type, sizeof(type));

		if (!m_file.ReadLine())	return false;
		if (Crt::StrLen(m_file.GetData()) == 1)
		{
			continue;
		}

		if (Crt::StrCmp(m_file.GetData(), L"") == 0)
		{
			continue;
		}

		ntInt scanfCount = swscanf_s(m_file.GetData(), L"%s %s", usage, _countof(usage), type, _countof(type));
		if (scanfCount == 2)
		{
			m_linkPuppet->SetFormatType(usage, type);
			continue;
		}

		switch(scanfCount)
		{
		case 0:				// 오류 처리
			return false;
		case 1:				// 종료 처리
			{
				if (m_file.GetData()[0] == L'}')
				{
					return true;
				}
			}
			break;
		}
	}

	return true;
}

bool NtModelParser::ReadVertexInfo()
{
	ntInt elemNum = m_linkPuppet->GetSumOffset();
	ntInt vtxCount = m_linkPuppet->GetVertexCount();
	ntInt loopCounter = 0;

	ntFloat* vtxBuf = new ntFloat[elemNum * vtxCount];

	float a, b, c, d, e, f, g, i;
	while(!m_file.IsEOF() && (loopCounter < vtxCount))
	{
		if (!m_file.ReadLine())		return false;

		if (Crt::StrLen(m_file.GetData()) <= 1)
		{
			continue;
		}

		// 차후 이 부분을 파라미터화 하여 함수로 변경하자.
		ntInt scanfCount = swscanf_s(m_file.GetData(), L"%f %f %f %f %f %f %f %f",
			&a,
			&b,
			&c,
			&d,
			&e,
			&f,
			&g,
			&i);


		vtxBuf[elemNum * loopCounter]		= a;
		vtxBuf[elemNum * loopCounter + 1]	= b;
		vtxBuf[elemNum * loopCounter + 2]	= c;
		vtxBuf[elemNum * loopCounter + 3]	= d;
		vtxBuf[elemNum * loopCounter + 4]	= e;
		vtxBuf[elemNum * loopCounter + 5]	= f;
		vtxBuf[elemNum * loopCounter + 6]	= g;
		vtxBuf[elemNum * loopCounter + 7]	= i;

		if (scanfCount != elemNum)
		{
			return false;
		}
		
		++loopCounter;
	}

	m_linkPuppet->SetVertexInfo(vtxBuf);

	return true;
}

void NtModelParser::SetPuppet(renderer::NtPuppet* puppet)
{
	m_linkPuppet = puppet;
}


}	// namespace nt
