#ifndef VOLKANO_COMPILER_H
#define VOLKANO_COMPILER_H

#include "lexer.h"
#include "parser.h"

#include <unordered_map>

#include <vtypes.h>

class CCompiler
{
	CLexer m_Lexer;
	CParser m_Parser;

	size_t m_Offset;
	std::vector<vlk_u8> m_vData;
	std::vector<vlk_u8> m_vHeaderData;

	int m_StackPos;
	std::unordered_map<std::string, int> m_vVariables;

	std::unordered_map<std::string, size_t> m_Functions;

	void AddStr(const std::string &Str);
	template<typename T>
	void AddType(T Value);
	void AddOp(int OpCode);

	void AddStrHeader(const std::string &Str);
	template<typename T>
	void AddTypeHeader(T Value);

	void AddPushStr(const std::string &Str);
	void AddPop();
	void AddCall(const std::string &Str);
	void AddCCall(const std::string &Str);
	void AddRet();
	void AddSetInt(int Pos, int Value);
	void AddSetStr(int Pos, const std::string &Str);
	void AddDup(int Pos);

	size_t GetFunctionOffset(const std::string &Str, bool &Found);

	void BuildCall(const SParserNode &Node);
	void BuildFunc(const SParserNode &Node);
	void BuildTree(const SParserNode &Node);
	void BuildAssign(const SParserNode &Node);

	void CreateHeader();

public:
	int Build(const std::string &Code);

	std::vector<vlk_u8> m_vOutData;
};

#endif // VOLKANO_COMPILER_H
