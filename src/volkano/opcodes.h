#ifndef VOLKANO_OPCODES_H
#define VOLKANO_OPCODES_H

enum
{
	// 0x00 is reserved for null-terminating
	OP_CALL = 0x01, // [0x01] Call global function
	OP_CALL_C,      // [0x02] Call function from C
	OP_PUSH_STR,    // [0x03] Push null-terminated string value to stack
	OP_POP,         // [0x04] Pop value from stack
	OP_RET,         // [0x05] Return from function (Marks end of function)
	OP_PUSH_INT,    // [0x06] Push 4 bytes signed int
};

#endif // VOLKANO_OPCODES_H
