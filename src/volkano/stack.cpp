#include "stack.h"

Vlk_Value CStack::Get(int i)
{
	// Negative values means we are taking value from top of stack
	if(i < 0)
		i = Top() - (i + 1);

	return m_vStack[i];
}

int CStack::Top()
{
	return m_vStack.size() - 1;
}

void CStack::Push(Vlk_Value Value)
{
	m_vStack.push_back(Value);
}

void CStack::Pop()
{
	Vlk_Value &Value = m_vStack[m_vStack.size() - 1];

	// Delete string
	if(Value.m_Type == VLK_TYPE_STRING)
		delete[] (char *)Value.m_Data.u;

	m_vStack.pop_back();
}

bool CStack::IsType(int i, int Type)
{
	return Get(i).m_Type == Type;
}
