
#pragma once

// Fundamental Type
typedef char			ntChar;
typedef wchar_t			ntWchar;
typedef unsigned char	ntUchar;
typedef short			ntShort;
typedef unsigned short	ntUshort;
typedef int				ntInt;
typedef unsigned int	ntUint;
typedef long			ntLong;
typedef unsigned long	ntUlong;
typedef __int64			ntInt64;

// Floating
typedef float			ntFloat;
typedef double			ntDouble;


// Custom Type
typedef long			ntIndex;

#if WIN64
typedef unsigned __int64 ntSize;
#else
typedef unsigned int	ntSize;
#endif


// Nt Defined Type
enum NtErrorCode
{
	NtERROR_FAIL = 0,
	NtERROR_SUCCESS,
};
