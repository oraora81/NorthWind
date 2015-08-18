
#pragma once

namespace NT
{


// Fundamental Type
// Integral
typedef bool			NtBool;
typedef char			NtChar;
typedef unsigned char	NtUChar;
typedef short			NtShort;
typedef unsigned short	NtUShort;
typedef int				NtInt;
typedef unsigned int	NtUInt;
typedef long			NtLong;
typedef unsigned long	NtULong;
typedef __int64			NtInt64;
typedef wchar_t			NtWChar;
// Floating
typedef float			NtFloat;
typedef double			NtDouble;


// Custom Type
typedef long			NtIndex;
typedef unsigned int	NtSize;


// Nt Defined Type
enum NtErrorCode
{
	NtERROR_FAIL = 0,
	NtERROR_SUCCESS,
};



//const NtInt

}	// namespace NT