#ifndef VOLKANO_LEXER_H
#define VOLKANO_LEXER_H

#include <string>
#include <utility>
#include <vector>

enum
{
	TKN_UNK,      // Unknown
	TKN_ID,       // Name
	TKN_FUNC,     // func
	TKN_LPAREN,   // (
	TKN_RPAREN,   // )
	TKN_LBRACE,   // {
	TKN_RBRACE,   // }
	TKN_COMMA,    // ,
	TKN_EOF,      // EOF symbol (\0)

	TKN_STRING,   // "string"
	TKN_INT,   // 3333442
};

static const char *gs_apTokenNames[] = {
	"TKN_UNK",
	"TKN_ID",
	"TKN_FUNC",
	"TKN_LPAREN",
	"TKN_RPAREN",
	"TKN_LBRACE",
	"TKN_RBRACE",
	"TKN_COMMA",
	"TKN_EOF",

	"TKN_STRING",
	"TKN_INT",
};

struct SToken
{
	int m_Type;
	std::string m_Data;

	bool operator==(int Type) const { return m_Type == Type; }
	bool operator!=(int Type) const { return !(*this == Type); }
};

class CLexer
{
	std::string m_Data;
	size_t m_Offset;
	char m_C; // Current char

	char Peek(size_t Offset);
	void NextChar();
	void SkipWhitespace();

	SToken Parse();
	SToken ParseString();
	SToken ParseInt();

public:
	void Init(const std::string &Data);

	SToken NextToken();

	const char *GetTokenName(SToken &Token);
};

#endif // VOLKANO_LEXER_H
