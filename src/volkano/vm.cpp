#define VLK_BUILD_LIB
#include <vm.h>
#include <vstatus.h>

#include <fstream>
#include <sstream>
#include <unordered_map>

#include "parser.h"
#include "stack.h"
#include "compiler.h"
#include "mem.h"
#include "opcodes.h"

class CVolkano_VM
{
	CStack m_Stack;
	CCompiler m_Compiler;

	std::unordered_map<std::string, Vlk_Value> m_Globals;
	std::vector<Vlk_CallInfo> m_vCallStack;

	std::vector<vlk_u8> m_vByteCode;
	size_t m_PC;

	vlk_u8 *BC() { return m_vByteCode.data(); }

	template<typename T>
	T GetType()
	{
		T Val;
		mem_copy(&Val, BC() + m_PC, sizeof(T));
		m_PC += sizeof(T);
		return Val;
	}

	std::string GetStr()
	{
		size_t Size = strlen(reinterpret_cast<const char *>(BC() + m_PC));
		std::string Str(reinterpret_cast<const char *>(BC() + m_PC), Size);
		m_PC += Size + 1;
		return Str;
	}

public:
	CVolkano_VM()
	{
		m_PC = 0;
	}

	~CVolkano_VM()
	{
		for(auto &Glob : m_Globals)
			if(Glob.second.m_Type == VLK_TYPE_STRING)
				delete[] (char *)Glob.second.m_Data.u;
	}

	CStack *Stack() { return &m_Stack; }

	int Load(const std::string &Code)
	{
		int Status = m_Compiler.Build(Code);
		if(Status != VLK_OK)
			return Status;

		// Copy byte code
		m_vByteCode = m_Compiler.m_vOutData;

		// Load header from byte code
		LoadHeader();

		return VLK_OK;
	}

	void LoadHeader()
	{
		vlk_u16 GlobalFunctionsNum = GetType<vlk_u16>();

		for(int i = 0; i < GlobalFunctionsNum; i++)
		{
			std::string Name = GetStr();
			vlk_u32 Offset = GetType<vlk_u32>();

			Vlk_CallInfo CallInfo;
			CallInfo.m_PC = Offset;

			Vlk_Value Value;
			Value.m_Data.c = CallInfo;
			Value.m_Type = VLK_TYPE_FUNCTION;

			AddGlobal(Name, Value);
		}

		std::vector<vlk_u8> vData;
		for(int i = m_PC; i < m_vByteCode.size(); i++)
			vData.push_back(m_vByteCode[i]);
		m_vByteCode = vData;
	}

	void AddGlobal(const std::string &Str, Vlk_Value Value)
	{
		auto a = m_Globals.find(Str);
		if(a != m_Globals.end())
		{
			a->second = Value;
			return;
		}

		m_Globals.insert({ Str, Value });
	}

	Vlk_Value GetGlobal(const std::string &Str)
	{
		auto Glob = m_Globals.find(Str);
		if(Glob == m_Globals.end())
			return {}; // Return null if we can't find global

		return Glob->second;
	}

	vlk_u8 ReadByte()
	{
		return m_vByteCode[m_PC++];
	}

	void Execute(Vlk_CallInfo CallInfo)
	{
		m_PC = CallInfo.m_PC;
		bool Running = true;

		while(Running)
		{
			vlk_u8 Byte = ReadByte();
			const char *pStartStr = (char *)m_vByteCode.data() + m_PC;

			switch(Byte)
			{
			case OP_CALL:
			{
				// Save current call info to callstack
				Vlk_CallInfo SaveCallInfo;
				SaveCallInfo.m_PC = m_PC;
				m_vCallStack.push_back(SaveCallInfo);

				Vlk_GetGlobal(this, pStartStr);
				Vlk_Execute(this);
			} break;
			case OP_CALL_C:
			{
				std::string Name(pStartStr, strlen(pStartStr));
				Vlk_Value Val = GetGlobal(Name);

				if(Val.m_Type != VLK_TYPE_CFUNCTION)
					break;

				Val.m_Data.fn(this);
			} break;
			case OP_RET:
			{
				Running = false;

				if(!m_vCallStack.empty())
				{
					m_PC = m_vCallStack[m_vCallStack.size() - 1].m_PC;
					m_vCallStack.pop_back();
				}
			} break;

			case OP_PUSH_STR: Vlk_PushValue(this, pStartStr); break;
			case OP_POP: Vlk_Pop(this); break;
			default: break;
			}
		}
	}
};

CVolkano_VM *Vlk_NewVM()
{
	return new CVolkano_VM;
}

void Vlk_CloseVM(CVolkano_VM *pVM)
{
	delete pVM;
}

void Vlk_Execute(CVolkano_VM *pVM)
{
	if(!Vlk_IsFunction(pVM, -1))
	{
		Vlk_Pop(pVM);
		return;
	}

	Vlk_CallInfo CallInfo = pVM->Stack()->Get(-1).m_Data.c;
	Vlk_Pop(pVM);

	pVM->Execute(CallInfo);
}

int Vlk_LoadFromFile(CVolkano_VM *pVM, const char *pFileName)
{
	std::ifstream File(pFileName);
	if(!File)
		return VLK_BAD_FILE;

	std::stringstream Str;
	Str << File.rdbuf();

	return pVM->Load(Str.str());
}

int Vlk_LoadFromBuffer(CVolkano_VM *pVM, const char *pBuf, size_t BufSize)
{
	return pVM->Load(std::string(pBuf, BufSize));
}

// Helper functions
Vlk_Value Vlk_Help_CreateString(const char *pStr)
{
	int AllocSize = strlen(pStr) + 1;
	char *pBuf = new char[AllocSize];

	mem_copy(pBuf, pStr, AllocSize);

	Vlk_Value Value;
	Value.m_Data.u = pBuf;
	Value.m_Type = VLK_TYPE_STRING;

	return Value;
}

Vlk_Value Vlk_Help_CreateInt(Vlk_Int i)
{
	Vlk_Value Value;
	Value.m_Data.i = i;
	Value.m_Type = VLK_TYPE_INT;

	return Value;
}

Vlk_Value Vlk_Help_CreateFloat(Vlk_Float f)
{
	Vlk_Value Value;
	Value.m_Data.f = f;
	Value.m_Type = VLK_TYPE_FLOAT;

	return Value;
}

Vlk_Value Vlk_Help_CreateCFunction(Vlk_CFunction fn)
{
	Vlk_Value Value;
	Value.m_Data.fn = fn;
	Value.m_Type = VLK_TYPE_CFUNCTION;

	return Value;
}

// Stack
void Vlk_PushValue(CVolkano_VM *pVM, const char *pStr) { pVM->Stack()->Push(Vlk_Help_CreateString(pStr)); }
void Vlk_PushValue(CVolkano_VM *pVM, Vlk_Int i) { pVM->Stack()->Push(Vlk_Help_CreateInt(i)); }
void Vlk_PushValue(CVolkano_VM *pVM, Vlk_Float f) { pVM->Stack()->Push(Vlk_Help_CreateFloat(f)); }
void Vlk_PushValue(CVolkano_VM *pVM, Vlk_CFunction fn) { pVM->Stack()->Push(Vlk_Help_CreateCFunction(fn)); }

void Vlk_Pop(CVolkano_VM *pVM) { pVM->Stack()->Pop(); }

#define IS_TYPE(VM, i, type) (VM->Stack()->IsType(i, type))
bool Vlk_IsNull(CVolkano_VM *pVM, int Idx) { return IS_TYPE(pVM, Idx, VLK_TYPE_NULL); }
bool Vlk_IsInt(CVolkano_VM *pVM, int Idx) { return IS_TYPE(pVM, Idx, VLK_TYPE_INT); }
bool Vlk_IsFloat(CVolkano_VM *pVM, int Idx) { return IS_TYPE(pVM, Idx, VLK_TYPE_FLOAT); }
bool Vlk_IsFunction(CVolkano_VM *pVM, int Idx) { return IS_TYPE(pVM, Idx, VLK_TYPE_FUNCTION); }
bool Vlk_IsUserData(CVolkano_VM *pVM, int Idx) { return IS_TYPE(pVM, Idx, VLK_TYPE_USERDATA); }
bool Vlk_IsString(CVolkano_VM *pVM, int Idx) { return IS_TYPE(pVM, Idx, VLK_TYPE_STRING); }

#define GET(VM, i) (VM->Stack()->Get(i))
Vlk_Int Vlk_GetInt(CVolkano_VM *pVM, int Idx)
{
	if(IS_TYPE(pVM, Idx, VLK_TYPE_NULL))
		return 0;
	return GET(pVM, Idx).m_Data.i;
}

Vlk_Float Vlk_GetFloat(CVolkano_VM *pVM, int Idx)
{
	if(IS_TYPE(pVM, Idx, VLK_TYPE_NULL))
		return 0;
	return GET(pVM, Idx).m_Data.f;
}

Vlk_UserData Vlk_GetUserData(CVolkano_VM *pVM, int Idx)
{
	if(IS_TYPE(pVM, Idx, VLK_TYPE_NULL))
		return 0;
	return GET(pVM, Idx).m_Data.u;
}

const char *Vlk_GetString(CVolkano_VM *pVM, int Idx)
{
	if(IS_TYPE(pVM, Idx, VLK_TYPE_NULL))
		return 0;
	return (char *)(GET(pVM, Idx).m_Data.u);
}
#undef IS_TYPE
#undef GET

void Vlk_SetGlobal(CVolkano_VM *pVM, const char *pKey, const char *pStr) { pVM->AddGlobal(pKey, Vlk_Help_CreateString(pStr)); }
void Vlk_SetGlobal(CVolkano_VM *pVM, const char *pKey, Vlk_Int i) { pVM->AddGlobal(pKey, Vlk_Help_CreateInt(i)); }
void Vlk_SetGlobal(CVolkano_VM *pVM, const char *pKey, Vlk_Float f) { pVM->AddGlobal(pKey, Vlk_Help_CreateFloat(f)); }
void Vlk_SetGlobal(CVolkano_VM *pVM, const char *pKey, Vlk_CFunction fn) { pVM->AddGlobal(pKey, Vlk_Help_CreateCFunction(fn)); }

void Vlk_GetGlobal(CVolkano_VM *pVM, const char *pKey) { pVM->Stack()->Push(pVM->GetGlobal(pKey)); }
