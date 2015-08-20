
#include "NtCoreLib.h"
#include "NtVector3.h"

using namespace nt;

template<> const NtVector3<float> NtVector3<float>::ZERO(0.0f, 0.0f, 0.0f);
template<> const NtVector3<float> NtVector3<float>::UNIT_X(1.0f, 0.0f, 0.0f);
template<> const NtVector3<float> NtVector3<float>::UNIT_Y(0.0f, 1.0f, 0.0f);
template<> const NtVector3<float> NtVector3<float>::UNIT_Z(0.0f, 0.0f, 1.0f);

template<> const NtVector3<double> NtVector3<double>::ZERO(0.0, 0.0, 0.0);
template<> const NtVector3<double> NtVector3<double>::UNIT_X(1.0, 0.0, 0.0);
template<> const NtVector3<double> NtVector3<double>::UNIT_Y(0.0, 1.0, 0.0);
template<> const NtVector3<double> NtVector3<double>::UNIT_Z(0.0, 0.0, 1.0);
