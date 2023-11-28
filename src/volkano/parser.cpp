#include "parser.h"

#include <vstatus.h>
#include <vtypes.h>

#define TKN_T(Type) (m_Token.m_Type == Type)

void CParser::Init(CLexer *pLexer)
{
	m_pLexer = pLexer;
	m_Token = pLexer->NextToken();
}

void CParser::Eat(int Type)
{
	if(m_Token.m_Type != Type)
		exit(VLK_INVALID_TOKEN);

	m_Token = m_pLexer->NextToken();
}

SParserNode CParser::ParseID()
{
	SParserNode Node;
	Node.m_Value = m_Token.m_Data;

	Eat(TKN_ID);

	// Call
	if(TKN_T(TKN_LPAREN))
	{
		Node.m_Type = ASN_CALL;
		Node.m_vNodes = ParseList().m_vNodes;
	}

	return Node;
}

SParserNode CParser::ParseList()
{
	SParserNode Node;
	Eat(TKN_LPAREN);

	if(!TKN_T(TKN_RPAREN))
	{
		Node.m_vNodes.push_back(ParseExpr());

		while(TKN_T(TKN_COMMA))
		{
			Eat(TKN_COMMA);
			Node.m_vNodes.push_back(ParseExpr());
		}
	}

	Eat(TKN_RPAREN);

	return Node;
}

SParserNode CParser::ParseInt()
{
	SParserNode Node;
	Node.m_Value = m_Token.m_Data;
	Node.m_ParamType = VLK_TYPE_INT;
	Eat(TKN_INT);

	return Node;
}

SParserNode CParser::ParseString()
{
	SParserNode Node;
	Node.m_Value = m_Token.m_Data;
	Node.m_ParamType = VLK_TYPE_STRING;
	Eat(TKN_STRING);

	return Node;
}

SParserNode CParser::ParseFactor()
{
	switch(m_Token.m_Type) {
		case TKN_ID: return ParseID();
		case TKN_LPAREN: return ParseList();
		case TKN_STRING: return ParseString();
		case TKN_INT: return ParseInt();
	}

    return {};
}

SParserNode CParser::ParseTerm()
{
	SParserNode Node = ParseFactor();

	// TODO: Add mul & div

	return Node;
}

SParserNode CParser::ParseExpr()
{
	SParserNode Node = ParseTerm();

	// TODO: Add add & sub

	return Node;
}

SParserNode CParser::ParseFunction()
{
	Eat(TKN_FUNC);

	SParserNode Node;
	Node.m_Type = ASN_FUNC;
	Node.m_Value = ParseID().m_Value;

	Node.m_vNodes.push_back(ParseCompound());

	return Node;
}

SParserNode CParser::ParseCompound()
{
	bool ShouldClose = false;

	if(TKN_T(TKN_LBRACE))
	{
		Eat(TKN_LBRACE);
		ShouldClose = true;
	}

	SParserNode Node;
	Node.m_Type = ASN_COMPOUND;
	while(!TKN_T(TKN_EOF) && !TKN_T(TKN_RBRACE))
	{
		switch(m_Token.m_Type) {
		case TKN_FUNC: Node.m_vNodes.push_back(ParseFunction()); break;
       		default: Node.m_vNodes.push_back(ParseExpr()); break;
		}
	}

	if(ShouldClose)
		Eat(TKN_RBRACE);

	return Node;
}

int CParser::Process(SParserNode &Node)
{
	Node = ParseCompound();

	return VLK_OK;
}
