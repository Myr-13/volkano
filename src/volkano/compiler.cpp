#include "compiler.h"
#include "opcodes.h"
#include "mem.h"

#include <vstatus.h>

#include <fstream>
#include <sstream>
#include <cstdlib>

void CCompiler::AddStr(const std::string &Str)
{
	for(char c : Str)
		m_vData.push_back(c);
	m_vData.push_back(0x00); // Null-terminating
	m_Offset += Str.size() + 1;
}

template<typename T>
void CCompiler::AddType(T Value)
{
	vlk_u8 *pStart = (vlk_u8 *)&Value;

	for(int i = 0; i < sizeof(T); i++)
		m_vData.push_back(pStart[i]);

	m_Offset += sizeof(T);
}

void CCompiler::AddOp(int OpCode)
{
	m_vData.push_back(OpCode);
	m_Offset++;
}

void CCompiler::AddStrHeader(const std::string &Str)
{
	for(char c : Str)
		m_vHeaderData.push_back(c);
	m_vHeaderData.push_back(0x00); // Null-terminating
	m_Offset += Str.size() + 1;
}

template<typename T>
void CCompiler::AddTypeHeader(T Value)
{
	vlk_u8 *pStart = (vlk_u8 *)&Value;
	for(int i = 0; i < sizeof(T); i++)
		m_vHeaderData.push_back(pStart[i]);
	m_Offset += sizeof(T);
}

void CCompiler::AddPushStr(const std::string &Str)
{
	AddOp(OP_PUSH_STR);
	AddStr(Str);
}

void CCompiler::AddPop()
{
	AddOp(OP_POP);
}

void CCompiler::AddCall(const std::string &Str)
{
	AddOp(OP_CALL);
	AddStr(Str);
}

size_t CCompiler::GetFunctionOffset(const std::string &Str, bool &Found)
{
	auto f = m_Functions.find(Str);
	if(f == m_Functions.end())
	{
		Found = false;
		return 0;
	}

	Found = true;
	return f->second;
}

void CCompiler::AddCCall(const std::string &Str)
{
	AddOp(OP_CALL_C);
	AddStr(Str);
}

void CCompiler::AddRet()
{
	AddOp(OP_RET);
}

void CCompiler::BuildCall(const SParserNode &Node)
{
	size_t ArgCount = Node.m_vNodes.size();

	// Add all arguments
	for(const SParserNode &Param : Node.m_vNodes)
	{
		switch(Param.m_ParamType)
		{
		case VLK_TYPE_STRING: AddPushStr(Param.m_Value); break;
		}
	}

	// Call function
	bool Found;
	GetFunctionOffset(Node.m_Value, Found);

	if(Found)
		AddCall(Node.m_Value);
	else
		AddCCall(Node.m_Value);

	// Pop arguments
	for(int i = 0; i < ArgCount; i++)
		AddPop();
}

void CCompiler::BuildFunc(const SParserNode &Node)
{
	m_Functions.find(Node.m_Value)->second = m_Offset;

	BuildTree(Node.m_vNodes[0]); // 0 is compound
	AddRet();
}

void CCompiler::BuildTree(const SParserNode &Node)
{
	for(const SParserNode &n : Node.m_vNodes)
	{
		switch(n.m_Type)
		{
		case ASN_FUNC:
			BuildFunc(n);
			break;
		case ASN_CALL:
			BuildCall(n);
			break;
		}
	}
}

void CCompiler::CreateHeader()
{
	// Count of jmp index
	vlk_u16 Size = (vlk_u16)m_Functions.size();
	AddTypeHeader(Size);

	for(auto &Func : m_Functions)
	{
		AddStrHeader(Func.first); // Name
		AddTypeHeader((vlk_u32)Func.second); // Offset
	}
}

int CCompiler::Build(const std::string &Code)
{
	// Init
	m_Lexer.Init(Code);
	m_Parser.Init(&m_Lexer);

	m_Offset = 0;

	SParserNode Root;

	// Run parser
	int Status = m_Parser.Process(Root);
	if(Status != VLK_OK)
		return Status;

	// Load global functions
	for(SParserNode &Node : Root.m_vNodes)
	{
		if(Node.m_Type != ASN_FUNC)
			continue;

		// TODO: Function overloads
		auto Func = m_Functions.find(Node.m_Value);
		if(Func != m_Functions.end())
			return VLK_REDEFINITION;
		m_Functions.insert({ Node.m_Value, 0 });
	}

	// Create all code
	BuildTree(Root);
	CreateHeader();

	m_vOutData = m_vHeaderData;
	m_vOutData.insert(m_vOutData.end(), m_vData.begin(), m_vData.end());

	for(vlk_u8 b : m_vOutData)
		printf("%02X ", b);
	printf("\n");

	return VLK_OK;
}
