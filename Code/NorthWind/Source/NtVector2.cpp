
#include "NtCoreLib.h"
#include "NtVector2.h"

using namespace nt;

template<> const NtVector2<float> NtVector2<float>::ZERO(0.0f, 0.0f);
template<> const NtVector2<float> NtVector2<float>::UNIT_X(1.0f, 0.0f);
template<> const NtVector2<float> NtVector2<float>::UNIT_Y(0.0f, 1.0f);

template<> const NtVector2<double> NtVector2<double>::ZERO(0.0, 0.0);
template<> const NtVector2<double> NtVector2<double>::UNIT_X(1.0, 0.0);
template<> const NtVector2<double> NtVector2<double>::UNIT_Y(0.0, 1.0);