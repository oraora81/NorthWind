
#include "NtCoreLib.h"
#include "NtMatrix2.h"

using namespace NT;

template<> const NtMatrix2<float> NtMatrix2<float>::ZERO(0.0f, 0.0f, 0.0f, 0.0f);

template<> const NtMatrix2<float> NtMatrix2<float>::IDENTITY(1.0f, 0.0f, 0.0f, 1.0f);

template<> const NtMatrix2<double> NtMatrix2<double>::ZERO(0.0, 0.0, 0.0, 0.0);

template<> const NtMatrix2<double> NtMatrix2<double>::IDENTITY(1.0, 0.0, 0.0, 1.0);