#ifndef VOLKANO_OPCODES_H
#define VOLKANO_OPCODES_H

enum
{
	// Terms:
	// i32 - 32-bits signed int
	// str - null-terminated string
	//
	// 0x00 is reserved for null-terminating
	OP_CALL = 0x01, // [0x01] Call global function                         | call func
	OP_CALL_C,      // [0x02] Call function from C                         | callc func
	OP_PUSH_STR,    // [0x03] Push str value to stack                      | pushs str
	OP_POP,         // [0x04] Pop value from stack                         | pop
	OP_RET,         // [0x05] Return from function (Marks end of function) | ret
	OP_PUSH_INT,    // [0x06] Push i32 to stack                            | pushi 57
	OP_SET_INT,     // [0x07] Set stacks value to i32                      | seti str, 57
};

#endif // VOLKANO_OPCODES_H
