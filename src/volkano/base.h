#ifndef VOLKANO_BASE_H
#define VOLKANO_BASE_H

#include <cstdlib>

inline void mem_copy(void *pDst, const void *pSrc, size_t Size)
{
	std::memcpy(pDst, pSrc, Size);
}

inline bool is_char(char c)
{
	return (
		(c >= 'A' && c <= 'Z') ||
		(c >= 'a' && c <= 'z') ||
		(c >= '0' && c <= '9') ||
		(c == '_')
		);
}

#endif// VOLKANO_BASE_H
