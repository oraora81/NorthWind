
#pragma once

namespace nt
{

enum eErrorCode
{
	ERR_SUCCESS = 0,

	ERR_CRT_START = 10000,

	ERR_FILE_OPEN_FAIL,
	ERR_FILE_POINT_OPEN_FAIL,			// 파일 포인터 작성 실패.
	ERR_FILE_SEEK_FAIL,					
	
	ERR_CRT_END = 19999,
};
typedef eErrorCode ERROR_CODE;


}	// namespace nt

#include "NtErrorHandling.inl"
