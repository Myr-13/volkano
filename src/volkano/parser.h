#ifndef VOLKANO_PARSER_H
#define VOLKANO_PARSER_H

#include "lexer.h"

#include <string>
#include <vector>

enum
{
	ASN_FUNC,   // func a()
	ASN_CALL,   // a()
	ASN_ASSIGN, // a = 10
	ASN_COMPOUND
};

struct SParserNode
{
	int m_Type;
	std::string m_Value;

	// For parameter
	int m_ParamType;

	std::vector<SParserNode> m_vNodes;
};

struct SParserFunc
{
	std::string m_Name;
	std::vector<SParserNode> m_vNodes;
	bool m_NeedClose; // false - no function in process
};

class CParser
{
	CLexer *m_pLexer;
	SToken m_Token;

	void Eat(int Type);

	SParserNode ParseID();
	SParserNode ParseList();
	SParserNode ParseString();
	SParserNode ParseFactor();
	SParserNode ParseTerm();
	SParserNode ParseExpr();
	SParserNode ParseCompound();
	SParserNode ParseFunction();

public:
	void Init(CLexer *pLexer);

	int Process(SParserNode &Node);
};

#endif // VOLKANO_PARSER_H
