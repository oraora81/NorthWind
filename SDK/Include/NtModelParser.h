
#pragma once

namespace NT
{

enum eParseKeyword
{
	PARSE_NONE,

	PARSE_VERTEX_NUM,
	PARSE_VERTEX_TYPE,
	PARSE_VERTEX_INFO,

	PARSE_BODY_START,
	PARSE_BODY_END,

	MAX_PARSE_KEY,
};
typedef eParseKeyword PARSE_KEYWORD;

class FS::NtBuffer;

class NtParseModeBinder
{
public:
	explicit NtParseModeBinder(const NtWChar* str);
	~NtParseModeBinder() {}
	
	NtInt FindParseKey();

private:
	NtString m_target;
};


namespace RENDERER
{
class NtPuppet;
}

class NtModelParser
{
public:
	explicit NtModelParser();
	~NtModelParser();

	bool	Execute(const NtWChar* fileName);

	void	SetPuppet(RENDERER::NtPuppet* puppet);

private:
	bool	ProcessParse();
	bool	ProcessVertexNum();
	bool	ProcessVertexType();
	bool	ProcessVertexInfo();

private:
	bool	FindKeyword();
	bool	FindKeyword(PARSE_KEYWORD parseKey);
	bool	DetermineParseMode(const NtWChar* src);
	bool	ReadVertexType();
	bool	ReadVertexInfo();

private:
	FS::NtFile		m_file;
	PARSE_KEYWORD	m_parseMode;
	RENDERER::NtPuppet* m_linkPuppet;
};

}	// namespace NT
