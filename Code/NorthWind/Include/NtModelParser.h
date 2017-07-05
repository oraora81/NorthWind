
#pragma once

namespace nt {

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

class NtParseModeBinder
{
public:
	explicit NtParseModeBinder(const ntWchar* str);
	~NtParseModeBinder() {}
	
	ntInt FindParseKey();

private:
	NtString m_target;
};


namespace renderer
{
class NtModel;
}

class NtModelParser
{
public:
	explicit NtModelParser();
	~NtModelParser();

	bool	Execute(const ntWchar* fileName);

	void	SetPuppet(renderer::NtModel* puppet);

private:
	bool	ProcessParse();
	bool	ProcessVertexNum();
	bool	ProcessVertexType();
	bool	ProcessVertexInfo();

private:
	bool	FindKeyword();
	bool	FindKeyword(PARSE_KEYWORD parseKey);
	bool	DetermineParseMode(const ntWchar* src);
	bool	ReadVertexType();
	bool	ReadVertexInfo();

private:
	fs::NtFile		m_file;
	PARSE_KEYWORD	m_parseMode;
	renderer::NtModel* m_linkPuppet;
};

}	// namespace nt
