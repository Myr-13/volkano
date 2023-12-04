#ifndef VOLKANO_VTYPES_H
#define VOLKANO_VTYPES_H

typedef unsigned char vlk_u8;
typedef unsigned short vlk_u16;
typedef unsigned int vlk_u32;

class CVolkano_VM;

typedef int (*Vlk_CFunction)(CVolkano_VM *pVM);
typedef int Vlk_Int;
typedef float Vlk_Float;
typedef void *Vlk_UserData;

enum
{
	VLK_TYPE_NULL,
	VLK_TYPE_INT,
	VLK_TYPE_FLOAT,
	VLK_TYPE_FUNCTION,
	VLK_TYPE_CFUNCTION,
	VLK_TYPE_USERDATA,
	VLK_TYPE_STRING,

	VLK_TYPES_NUM
};

#endif // VOLKANO_VTYPES_H
