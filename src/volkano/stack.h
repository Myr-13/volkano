#ifndef VOLKANO_STACK_H
#define VOLKANO_STACK_H

#include <vtypes.h>

#include <vector>

struct Vlk_CallInfo
{
	vlk_u32 m_PC;
	int m_ArgsCount;
};

struct Vlk_Value
{
	union
	{
		Vlk_Int i; // For int type
		Vlk_Float f; // For float type
		Vlk_UserData u; // For user data type
		Vlk_CFunction fn; // For C function type
		Vlk_CallInfo c; // For function type
	} m_Data;
	int m_Type = VLK_TYPE_NULL;
};

class CStack
{
	std::vector<Vlk_Value> m_vStack;

public:
	Vlk_Value Get(int i);
	void Push(Vlk_Value Value);
	void Pop();
	int Top();

	bool IsType(int i, int Type);
};

#endif // VOLKANO_STACK_H
