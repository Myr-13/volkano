#ifndef VOLKANO_PARSER_H
#define VOLKANO_PARSER_H

#include "lexer.h"

#include <string>
#include <vector>
#include <vtypes.h>

enum
{
	ASN_FUNC,   // func a()
	ASN_CALL,   // a()
	ASN_ASSIGN, // a = 10
	ASN_COMPOUND,
	ASN_VAR,    // a
};

struct SParserNode
{
	int m_Type;
	std::string m_Value;

	// For parameter
	int m_ParamType;

	std::vector<SParserNode> m_vNodes;
};

class CParser
{
	CLexer *m_pLexer;
	SToken m_Token;

	void Eat(int Type);

	SParserNode ParseID();
	SParserNode ParseList();
	SParserNode ParseInt();
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
