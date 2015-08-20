
#pragma once

namespace nt
{
	struct ntExceptionBoundary
	{
		struct ntException : public std::exception
		{
			const char* what() const throw() { return "unknown exception"; }
		};
	};
}