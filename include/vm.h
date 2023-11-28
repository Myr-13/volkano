#ifndef VOLKANO_VM_H
#define VOLKANO_VM_H

#include "vconf.h"
#include "vtypes.h"

#define VLK_STACK_SIZE 2048

class CVolkano_VM;

VLK_API CVolkano_VM *Vlk_NewVM();
VLK_API void Vlk_CloseVM(CVolkano_VM *pVM);
VLK_API void Vlk_Execute(CVolkano_VM *pVM);

VLK_API int Vlk_LoadFromFile(CVolkano_VM *pVM, const char *pFileName);
VLK_API int Vlk_LoadFromBuffer(CVolkano_VM *pVM, const char *pBuf, size_t BufSize);

VLK_API void Vlk_PushValue(CVolkano_VM *pVM, const char *pStr);
VLK_API void Vlk_PushValue(CVolkano_VM *pVM, Vlk_Int i);
VLK_API void Vlk_PushValue(CVolkano_VM *pVM, Vlk_Float f);
VLK_API void Vlk_PushValue(CVolkano_VM *pVM, Vlk_CFunction fn);

VLK_API void Vlk_Pop(CVolkano_VM *pVM);
VLK_API int Vlk_GetTop(CVolkano_VM *pVM);

VLK_API bool Vlk_IsNull(CVolkano_VM *pVM, int Idx);
VLK_API bool Vlk_IsInt(CVolkano_VM *pVM, int Idx);
VLK_API bool Vlk_IsFloat(CVolkano_VM *pVM, int Idx);
VLK_API bool Vlk_IsFunction(CVolkano_VM *pVM, int Idx);
VLK_API bool Vlk_IsUserData(CVolkano_VM *pVM, int Idx);
VLK_API bool Vlk_IsString(CVolkano_VM *pVM, int Idx);

VLK_API Vlk_Int Vlk_GetInt(CVolkano_VM *pVM, int Idx);
VLK_API Vlk_Float Vlk_GetFloat(CVolkano_VM *pVM, int Idx);
VLK_API Vlk_UserData Vlk_GetUserData(CVolkano_VM *pVM, int Idx);
VLK_API const char *Vlk_GetString(CVolkano_VM *pVM, int Idx);

VLK_API void Vlk_SetGlobal(CVolkano_VM *pVM, const char *pKey, const char *pStr);
VLK_API void Vlk_SetGlobal(CVolkano_VM *pVM, const char *pKey, Vlk_Int i);
VLK_API void Vlk_SetGlobal(CVolkano_VM *pVM, const char *pKey, Vlk_Float f);
VLK_API void Vlk_SetGlobal(CVolkano_VM *pVM, const char *pKey, Vlk_CFunction fn);

VLK_API void Vlk_GetGlobal(CVolkano_VM *pVM, const char *pKey);

#endif // VOLKANO_VM_H
