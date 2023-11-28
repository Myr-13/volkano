#include "lexer.h"
#include "base.h"

#include <regex>

void CLexer::Init(const std::string &Data)
{
	m_C = Data[0];
	m_Offset = 0;
	m_Data = Data;
}

char CLexer::Peek(size_t Offset)
{
	return m_Data[m_Offset + Offset];
}

void CLexer::NextChar()
{
	if(m_Offset < m_Data.size() && m_Data[m_Offset] != '\0')
	{
		m_Offset++;
		m_C = m_Data[m_Offset];
	}
}

void CLexer::SkipWhitespace()
{
	while(isspace(m_C))
		NextChar();
}

const char *CLexer::GetTokenName(SToken &Token)
{
	return gs_apTokenNames[Token.m_Type];
}

SToken CLexer::Parse()
{
	std::string Buf;

	SkipWhitespace();
	while(is_char(m_C))
	{
		Buf += m_C;
		NextChar();
	}

	if(Buf.empty())
		return { TKN_UNK };
	if(Buf == "func")
		return { TKN_FUNC };
	return { TKN_ID, Buf };
}

SToken CLexer::ParseString()
{
	std::string Buf;

	while(m_C != '\"')
	{
		Buf += m_C;
		NextChar();
	}

	std::regex pat("\\\\n");
	Buf = std::regex_replace(Buf, pat, "\n");

	return { TKN_STRING, Buf };
}

SToken CLexer::NextToken()
{
	SToken Token = Parse();

	if(Token.m_Type != TKN_UNK)
		return Token;

	SkipWhitespace();

	switch(m_C)
	{
		case '(': Token.m_Type = TKN_LPAREN; break;
		case ')': Token.m_Type = TKN_RPAREN; break;
		case '{': Token.m_Type = TKN_LBRACE; break;
		case '}': Token.m_Type = TKN_RBRACE; break;
		case '\"': NextChar(); Token = ParseString(); break;
		case ',': Token.m_Type = TKN_COMMA; break;
		case '\0': Token.m_Type = TKN_EOF; break;
	}

	NextChar();

	return Token;
}
