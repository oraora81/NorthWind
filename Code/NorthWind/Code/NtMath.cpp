
#include "NtCoreLib.h"
#include "NtMath.h"
#include <float.h>

using namespace NT;

template <> const NtFloat NtMath<NtFloat>::EPSILON  = FLT_EPSILON;
template <> const NtFloat NtMath<NtFloat>::TOLERANCE = 1e-06f;
template <> const NtFloat NtMath<NtFloat>::MAX_VALUE = FLT_MAX;
template <> const NtFloat NtMath<NtFloat>::PI = (NtFloat)(4.0 * atan(1.0));
template <> const NtFloat NtMath<NtFloat>::HALF_PI = 0.5f * NtMath<NtFloat>::PI;
template <> const NtFloat NtMath<NtFloat>::TWO_PI = 2 * NtMath<NtFloat>::PI;
template <> const NtFloat NtMath<NtFloat>::INV_PI = 1.0f / NtMath<NtFloat>::PI;
template <> const NtFloat NtMath<NtFloat>::DEG_TO_RAD = NtMath<NtFloat>::PI / 180.0f;
template <> const NtFloat NtMath<NtFloat>::RAD_TO_DEG = 180.0f / NtMath<NtFloat>::PI;

template <> const NtDouble NtMath<NtDouble>::EPSILON  = DBL_EPSILON;
template <> const NtDouble NtMath<NtDouble>::TOLERANCE = 1e-08f;
template <> const NtDouble NtMath<NtDouble>::MAX_VALUE = DBL_MAX;
template <> const NtDouble NtMath<NtDouble>::PI = 4.0 * atan(1.0);
template <> const NtDouble NtMath<NtDouble>::HALF_PI = 0.5 * NtMath<NtDouble>::PI;
template <> const NtDouble NtMath<NtDouble>::TWO_PI = 2 * NtMath<NtDouble>::PI;
template <> const NtDouble NtMath<NtDouble>::INV_PI = 1.0f / NtMath<NtDouble>::PI;
template <> const NtDouble NtMath<NtDouble>::DEG_TO_RAD = NtMath<NtDouble>::PI / 180.0f;
template <> const NtDouble NtMath<NtDouble>::RAD_TO_DEG = 180.0f / NtMath<NtDouble>::PI;