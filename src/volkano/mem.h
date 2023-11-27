#ifndef VOLKANO_MEM_H
#define VOLKANO_MEM_H

#include <cstdlib>

inline void mem_copy(void *pDst, const void *pSrc, size_t Size)
{
	std::memcpy(pDst, pSrc, Size);
}

#endif // VOLKANO_MEM_H
