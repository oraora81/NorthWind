
#include "NtCoreLib.h"
#include "NtMath.h"
#include <float.h>

using namespace nt;

template <> const ntFloat NtMath<ntFloat>::EPSILON  = FLT_EPSILON;
template <> const ntFloat NtMath<ntFloat>::TOLERANCE = 1e-06f;
template <> const ntFloat NtMath<ntFloat>::MAX_VALUE = FLT_MAX;
template <> const ntFloat NtMath<ntFloat>::PI = (ntFloat)(4.0 * atan(1.0));
template <> const ntFloat NtMath<ntFloat>::HALF_PI = 0.5f * NtMath<ntFloat>::PI;
template <> const ntFloat NtMath<ntFloat>::TWO_PI = 2 * NtMath<ntFloat>::PI;
template <> const ntFloat NtMath<ntFloat>::INV_PI = 1.0f / NtMath<ntFloat>::PI;
template <> const ntFloat NtMath<ntFloat>::DEG_TO_RAD = NtMath<ntFloat>::PI / 180.0f;
template <> const ntFloat NtMath<ntFloat>::RAD_TO_DEG = 180.0f / NtMath<ntFloat>::PI;

template <> const ntDouble NtMath<ntDouble>::EPSILON  = DBL_EPSILON;
template <> const ntDouble NtMath<ntDouble>::TOLERANCE = 1e-08f;
template <> const ntDouble NtMath<ntDouble>::MAX_VALUE = DBL_MAX;
template <> const ntDouble NtMath<ntDouble>::PI = 4.0 * atan(1.0);
template <> const ntDouble NtMath<ntDouble>::HALF_PI = 0.5 * NtMath<ntDouble>::PI;
template <> const ntDouble NtMath<ntDouble>::TWO_PI = 2 * NtMath<ntDouble>::PI;
template <> const ntDouble NtMath<ntDouble>::INV_PI = 1.0f / NtMath<ntDouble>::PI;
template <> const ntDouble NtMath<ntDouble>::DEG_TO_RAD = NtMath<ntDouble>::PI / 180.0f;
template <> const ntDouble NtMath<ntDouble>::RAD_TO_DEG = 180.0f / NtMath<ntDouble>::PI;
