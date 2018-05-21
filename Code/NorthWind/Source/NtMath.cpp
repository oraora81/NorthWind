
#include "NtCoreLib.h"
#include "NtMath.h"
#include <float.h>

using namespace nt;

template <> const float NtMath<float>::EPSILON  = FLT_EPSILON;
template <> const float NtMath<float>::TOLERANCE = 1e-06f;
template <> const float NtMath<float>::MAX_VALUE = FLT_MAX;
//template <> const float NtMath<float>::PI = (float)(4.0 * atan(1.0));
template <> const float NtMath<float>::PI = 3.1415926535f;
template <> const float NtMath<float>::HALF_PI = 0.5f * NtMath<float>::PI;
template <> const float NtMath<float>::TWO_PI = 2 * NtMath<float>::PI;
template <> const float NtMath<float>::INV_PI = 1.0f / NtMath<float>::PI;
template <> const float NtMath<float>::DEG_TO_RAD = NtMath<float>::PI / 180.0f;
template <> const float NtMath<float>::RAD_TO_DEG = 180.0f / NtMath<float>::PI;

template <> const double NtMath<double>::EPSILON  = DBL_EPSILON;
template <> const double NtMath<double>::TOLERANCE = 1e-08f;
template <> const double NtMath<double>::MAX_VALUE = DBL_MAX;
//template <> const double NtMath<double>::PI = 4.0 * atan(1.0);
template <> const double NtMath<double>::PI = 3.1415926535;
template <> const double NtMath<double>::HALF_PI = 0.5 * NtMath<double>::PI;
template <> const double NtMath<double>::TWO_PI = 2 * NtMath<double>::PI;
template <> const double NtMath<double>::INV_PI = 1.0f / NtMath<double>::PI;
template <> const double NtMath<double>::DEG_TO_RAD = NtMath<double>::PI / 180.0f;
template <> const double NtMath<double>::RAD_TO_DEG = 180.0f / NtMath<double>::PI;
