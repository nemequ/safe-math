/* Overflow-safe math functions
 * <https://github.com/nemequ/safe-math>
 * Copyright (c) 2016 Evan Nemerson <evan@nemerson.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#if !defined(SAFE_H)
#define SAFE_H

#if !defined(SAFE_FORCE_PORTABLE)
#  if defined(__clang__)
#    if __has_builtin(__builtin_add_overflow)
#      define SAFE_HAVE_BUILTIN_OVERFLOW
#    endif
#  elif defined(__GNUC__) && (__GNUC__ >= 5)
#    define SAFE_HAVE_BUILTIN_OVERFLOW
#  endif
#endif /* !defined(SAFE_FORCE_PORTABLE) */

#if defined(__GNUC__)
#  define SAFE_LIKELY(expr)   __builtin_expect(!!(expr), 1)
#  define SAFE_UNLIKELY(expr) __builtin_expect(!!(expr), 0)
#endif /* defined(__GNUC__) */

#if !defined(SAFE_NO_FIXED)
#  include <stdint.h>
#endif /* !defined(SAFE_NO_FIXED) */
#include <limits.h>

/* If there is a type larger than the one we're concerned with it's
 * likely much faster to simply promote the operands, perform the
 * requested operation, verify that the result falls within the
 * original type, then cast the result back to the original type. */

#if !defined(SAFE_NO_PROMOTIONS)

#define SAFE_DEFINE_LARGER_BINARY_OP(T, name, op_name, op) \
  static inline safe_##name##_larger \
  safe_larger_##name##_##op_name (T a, T b) { \
    return ((safe_##name##_larger) a) op ((safe_##name##_larger) b); \
  }

#define SAFE_DEFINE_LARGER_UNARY_OP(T, name, op_name, op) \
  static inline safe_##name##_larger \
  safe_larger_##name##_##op_name (T value) { \
    return (op ((safe_##name##_larger) value)); \
  }

#define SAFE_DEFINE_LARGER_OPS(T, name) \
  SAFE_DEFINE_LARGER_BINARY_OP(T, name, add, +) \
  SAFE_DEFINE_LARGER_BINARY_OP(T, name, sub, -) \
  SAFE_DEFINE_LARGER_BINARY_OP(T, name, mul, *) \
  SAFE_DEFINE_LARGER_BINARY_OP(T, name, div, /) \
  SAFE_DEFINE_LARGER_BINARY_OP(T, name, mod, %) \
  SAFE_DEFINE_LARGER_UNARY_OP (T, name, neg, -)

#define SAFE_IS_LARGER(ORIG_MAX, DEST_MAX) ((DEST_MAX / ORIG_MAX) >= ORIG_MAX)

#if defined(__GNUC__)
#define SAFE_HAVE_128
typedef __int128  safe_int128_t;
typedef unsigned __int128 safe_uint128_t;
#endif /* defined(__GNUC__) */

#if !defined(SAFE_NO_FIXED)
#define SAFE_HAVE_INT8_LARGER
#define SAFE_HAVE_UINT8_LARGER
typedef int16_t  safe_int8_larger;
typedef uint16_t safe_uint8_larger;

#define SAFE_HAVE_INT16_LARGER
typedef int32_t  safe_int16_larger;
typedef uint32_t safe_uint16_larger;

#define SAFE_HAVE_INT32_LARGER
typedef int64_t  safe_int32_larger;
typedef uint64_t safe_uint32_larger;

#if defined(SAFE_HAVE_128)
#define SAFE_HAVE_INT64_LARGER
typedef safe_int128_t safe_int64_larger;
typedef safe_uint128_t safe_uint64_larger;
#endif /* defined(SAFE_HAVE_128) */
#endif /* !defined(SAFE_NO_FIXED) */

#define SAFE_HAVE_LARGER_CHAR
#if SAFE_IS_LARGER(CHAR_MAX, SHRT_MAX)
typedef short safe_char_larger;
#elif SAFE_IS_LARGER(CHAR_MAX, INT_MAX)
typedef int safe_char_larger;
#elif SAFE_IS_LARGER(CHAR_MAX, LONG_MAX)
typedef long safe_char_larger;
#elif SAFE_IS_LARGER(CHAR_MAX, LLONG_MAX)
typedef long safe_char_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(CHAR_MAX, INT16_MAX)
typedef int16_t safe_char_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(CHAR_MAX, INT32_MAX)
typedef int32_t safe_char_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(CHAR_MAX, INT64_MAX)
typedef int64_t safe_char_larger;
#elif !defined(SAFE_NO_FIXED) && defined(SAFE_HAVE_128) && (CHAR_MAX <= INT64_MAX)
typedef safe_int128_t safe_char_larger;
#else
#undef SAFE_HAVE_LARGER_CHAR
#endif

#define SAFE_HAVE_LARGER_UCHAR
#if SAFE_IS_LARGER(UCHAR_MAX, USHRT_MAX)
typedef unsigned short safe_uchar_larger;
#elif SAFE_IS_LARGER(UCHAR_MAX, UINT_MAX)
typedef unsigned int safe_uchar_larger;
#elif SAFE_IS_LARGER(UCHAR_MAX, ULONG_MAX)
typedef unsigned long safe_uchar_larger;
#elif SAFE_IS_LARGER(UCHAR_MAX, ULLONG_MAX)
typedef unsigned long safe_uchar_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(UCHAR_MAX, UINT16_MAX)
typedef uint16_t safe_uchar_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(UCHAR_MAX, UINT32_MAX)
typedef uint32_t safe_uchar_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(UCHAR_MAX, UINT64_MAX)
typedef uint64_t safe_uchar_larger;
#elif !defined(SAFE_NO_FIXED) && defined(SAFE_HAVE_128) && (UCHAR_MAX <= UINT64_MAX)
typedef safe_uint128_t safe_uchar_larger;
#else
#undef SAFE_HAVE_LARGER_UCHAR
#endif

#define SAFE_HAVE_LARGER_SHRT
#if SAFE_IS_LARGER(SHRT_MAX, INT_MAX)
typedef int safe_short_larger;
#elif SAFE_IS_LARGER(SHRT_MAX, LONG_MAX)
typedef long safe_short_larger;
#elif SAFE_IS_LARGER(SHRT_MAX, LLONG_MAX)
typedef long safe_short_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(SHRT_MAX, INT16_MAX)
typedef int16_t safe_short_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(SHRT_MAX, INT32_MAX)
typedef int32_t safe_short_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(SHRT_MAX, INT64_MAX)
typedef int64_t safe_short_larger;
#elif !defined(SAFE_NO_FIXED) && defined(SAFE_HAVE_128) && (SHRT_MAX <= INT64_MAX)
typedef safe_int128_t safe_short_larger;
#else
#undef SAFE_HAVE_LARGER_SHRT
#endif

#define SAFE_HAVE_LARGER_USHRT
#if SAFE_IS_LARGER(USHRT_MAX, UINT_MAX)
typedef unsigned int safe_ushort_larger;
#elif SAFE_IS_LARGER(USHRT_MAX, ULONG_MAX)
typedef unsigned long safe_ushort_larger;
#elif SAFE_IS_LARGER(USHRT_MAX, ULLONG_MAX)
typedef unsigned long safe_ushort_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(USHRT_MAX, UINT16_MAX)
typedef uint16_t safe_ushort_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(USHRT_MAX, UINT32_MAX)
typedef uint32_t safe_ushort_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(USHRT_MAX, UINT64_MAX)
typedef uint64_t safe_ushort_larger;
#elif !defined(SAFE_NO_FIXED) && defined(SAFE_HAVE_128) && (USHRT_MAX <= UINT64_MAX)
typedef safe_uint128_t safe_ushort_larger;
#else
#undef SAFE_HAVE_LARGER_USHRT
#endif

#define SAFE_HAVE_LARGER_INT
#if SAFE_IS_LARGER(INT_MAX, LONG_MAX)
typedef long safe_int_larger;
#elif SAFE_IS_LARGER(INT_MAX, LLONG_MAX)
typedef long safe_int_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(INT_MAX, INT16_MAX)
typedef int16_t safe_int_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(INT_MAX, INT32_MAX)
typedef int32_t safe_int_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(INT_MAX, INT64_MAX)
typedef int64_t safe_int_larger;
#elif !defined(SAFE_NO_FIXED) && defined(SAFE_HAVE_128) && (INT_MAX <= INT64_MAX)
typedef safe_int128_t safe_int_larger;
#else
#undef SAFE_HAVE_LARGER_INT
#endif

#define SAFE_HAVE_LARGER_UINT
#if SAFE_IS_LARGER(UINT_MAX, ULONG_MAX)
typedef unsigned long safe_uint_larger;
#elif SAFE_IS_LARGER(UINT_MAX, ULLONG_MAX)
typedef unsigned long safe_uint_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(UINT_MAX, UINT16_MAX)
typedef uint16_t safe_uint_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(UINT_MAX, UINT32_MAX)
typedef uint32_t safe_uint_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(UINT_MAX, UINT64_MAX)
typedef uint64_t safe_uint_larger;
#elif !defined(SAFE_NO_FIXED) && defined(SAFE_HAVE_128) && (UINT_MAX <= UINT64_MAX)
typedef safe_uint128_t safe_uint_larger;
#else
#undef SAFE_HAVE_LARGER_UINT
#endif

#define SAFE_HAVE_LARGER_LONG
#if SAFE_IS_LARGER(LONG_MAX, LLONG_MAX)
typedef long safe_long_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(LONG_MAX, INT16_MAX)
typedef int16_t safe_long_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(LONG_MAX, INT32_MAX)
typedef int32_t safe_long_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(LONG_MAX, INT64_MAX)
typedef int64_t safe_long_larger;
#elif !defined(SAFE_NO_FIXED) && defined(SAFE_HAVE_128) && (LONG_MAX <= INT64_MAX)
typedef safe_int128_t safe_long_larger;
#else
#undef SAFE_HAVE_LARGER_LONG
#endif

#define SAFE_HAVE_LARGER_ULONG
#if SAFE_IS_LARGER(ULONG_MAX, ULLONG_MAX)
typedef unsigned long safe_ulong_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(ULONG_MAX, UINT16_MAX)
typedef uint16_t safe_ulong_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(ULONG_MAX, UINT32_MAX)
typedef uint32_t safe_ulong_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(ULONG_MAX, UINT64_MAX)
typedef uint64_t safe_ulong_larger;
#elif !defined(SAFE_NO_FIXED) && defined(SAFE_HAVE_128) && (ULONG_MAX <= UINT64_MAX)
typedef safe_uint128_t safe_ulong_larger;
#else
#undef SAFE_HAVE_LARGER_ULONG
#endif

#define SAFE_HAVE_LARGER_LLONG
#if !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(LLONG_MAX, INT16_MAX)
typedef int16_t safe_llong_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(LLONG_MAX, INT32_MAX)
typedef int32_t safe_llong_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(LLONG_MAX, INT64_MAX)
typedef int64_t safe_llong_larger;
#elif !defined(SAFE_NO_FIXED) && defined(SAFE_HAVE_128) && (LLONG_MAX <= INT64_MAX)
typedef safe_int128_t safe_llong_larger;
#else
#undef SAFE_HAVE_LARGER_LLONG
#endif

#define SAFE_HAVE_LARGER_ULLONG
#if !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(ULLONG_MAX, UINT16_MAX)
typedef uint16_t safe_ullong_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(ULLONG_MAX, UINT32_MAX)
typedef uint32_t safe_ullong_larger;
#elif !defined(SAFE_NO_FIXED) && SAFE_IS_LARGER(ULLONG_MAX, UINT64_MAX)
typedef uint64_t safe_ullong_larger;
#elif !defined(SAFE_NO_FIXED) && defined(SAFE_HAVE_128) && (ULLONG_MAX <= UINT64_MAX)
typedef safe_uint128_t safe_ullong_larger;
#else
#undef SAFE_HAVE_LARGER_ULLONG
#endif

#if defined(SAFE_HAVE_LARGER_CHAR)
SAFE_DEFINE_LARGER_OPS(char, char)
#endif

#if defined(SAFE_HAVE_LARGER_UCHAR)
SAFE_DEFINE_LARGER_OPS(unsigned char, uchar)
#endif

#if defined(SAFE_HAVE_LARGER_SHORT)
SAFE_DEFINE_LARGER_OPS(short, short)
#endif

#if defined(SAFE_HAVE_LARGER_USHORT)
SAFE_DEFINE_LARGER_OPS(unsigned short, ushort)
#endif

#if defined(SAFE_HAVE_LARGER_INT)
SAFE_DEFINE_LARGER_OPS(int, int)
#endif

#if defined(SAFE_HAVE_LARGER_UINT)
SAFE_DEFINE_LARGER_OPS(unsigned int, uint)
#endif

#if defined(SAFE_HAVE_LARGER_LONG)
SAFE_DEFINE_LARGER_OPS(long, long)
#endif

#if defined(SAFE_HAVE_LARGER_ULONG)
SAFE_DEFINE_LARGER_OPS(unsigned long, ulong)
#endif

#if defined(SAFE_HAVE_LARGER_LLONG)
SAFE_DEFINE_LARGER_OPS(long long, llong)
#endif

#if defined(SAFE_HAVE_LARGER_ULLONG)
SAFE_DEFINE_LARGER_OPS(unsigned long long, ullong)
#endif

#if !defined(SAFE_NO_FIXED)
SAFE_DEFINE_LARGER_OPS(int8_t,   int8)
SAFE_DEFINE_LARGER_OPS(uint8_t,  uint8)
SAFE_DEFINE_LARGER_OPS(int16_t,  int16)
SAFE_DEFINE_LARGER_OPS(uint16_t, uint16)
SAFE_DEFINE_LARGER_OPS(int32_t,  int32)
SAFE_DEFINE_LARGER_OPS(uint32_t, uint32)
#if defined(SAFE_HAVE_128)
SAFE_DEFINE_LARGER_OPS(int64_t,  int64)
SAFE_DEFINE_LARGER_OPS(uint64_t, uint64)
#endif
#endif

#endif /* !defined(SAFE_NO_PROMOTIONS) */

#define SAFE_DEFINE_BUILTIN_BINARY_OP(T, name, op_name) \
  static inline _Bool \
  safe_##name##_##op_name(T* res, T a, T b) { \
    return !__builtin_##op_name##_overflow(a, b, res); \
  }

#define SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(T, name, op_name, min, max) \
  static inline _Bool \
  safe_##name##_##op_name(T* res, T a, T b) { \
    const safe_##name##_larger r = safe_larger_##name##_##op_name(a, b); \
    *res = r; \
    return (r >= min) && (r <= max); \
  }

#define SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(T, name, op_name, max) \
  static inline _Bool \
  safe_##name##_##op_name(T* res, T a, T b) { \
    const safe_##name##_larger r = safe_larger_##name##_##op_name(a, b); \
    *res = r; \
    return (r <= max); \
  }

#define SAFE_DEFINE_SIGNED_ADD(T, name, min, max) \
  static inline _Bool \
  safe_##name##_add (T* res, T a, T b) { \
    *res = (T) (a + b); \
    return !SAFE_UNLIKELY(((b > 0) && (a > (max - b))) || \
                          ((b < 0) && (a < (max - b)))); \
  }

#define SAFE_DEFINE_UNSIGNED_ADD(T, name, max) \
  static inline _Bool \
  safe_##name##_add (T* res, T a, T b) { \
    *res = (T) (a + b); \
    return !SAFE_UNLIKELY((b > 0) && (a > (max - b))); \
  }

#define SAFE_DEFINE_SIGNED_SUB(T, name, min, max) \
  static inline _Bool \
  safe_##name##_sub (T* res, T a, T b) { \
      *res = (T) (a - b); \
      return !SAFE_UNLIKELY ((b > 0 && a < min + b) || \
                             (b < 0 && a > max + b)); \
  }

#define SAFE_DEFINE_UNSIGNED_SUB(T, name, max) \
  static inline _Bool \
  safe_##name##_sub (T* res, T a, T b) { \
      *res = (T) (a - b); \
      return !SAFE_UNLIKELY(b > a); \
  }

#define SAFE_DEFINE_SIGNED_MUL(T, name, min, max) \
  static inline _Bool \
  safe_##name##_mul (T* res, T a, T b) { \
    *res = (T) (a * b); \
    if (a > 0) { \
      if (b > 0) { \
        if (a > (max / b)) { \
          return 0; \
        } \
      } else { \
        if (b < (min / a)) { \
          return 0; \
        } \
      } \
    } else { \
      if (b > 0) { \
        if (a < (min / b)) { \
          return 0; \
        } \
      } else { \
        if ( (a != 0) && (b < (max / a))) { \
          return 0; \
        } \
      } \
    } \
    return 1; \
  }

#define SAFE_DEFINE_UNSIGNED_MUL(T, name, max) \
  static inline _Bool \
  safe_##name##_mul (T* res, T a, T b) { \
    *res = (T) (a * b); \
    return !SAFE_UNLIKELY((a > 0) && (b > 0) && (a > (max / b))); \
  }

#define SAFE_DEFINE_SIGNED_DIV(T, name, min, max)   \
  static inline _Bool \
  safe_##name##_div (T* res, T a, T b) { \
    if (SAFE_UNLIKELY(b == 0)) { \
      *res = 0; \
      return 0; \
    } else if (SAFE_UNLIKELY(a == min && b == -1)) {    \
      *res = min; \
      return 0; \
    } else { \
      *res = (T) (a / b); \
      return 1; \
    } \
  }

#define SAFE_DEFINE_UNSIGNED_DIV(T, name, max) \
  static inline _Bool \
  safe_##name##_div (T* res, T a, T b) { \
    if (SAFE_UNLIKELY(b == 0)) { \
      *res = 0; \
      return 0; \
    } else { \
      *res = a / b; \
      return 1; \
    } \
  }

#define SAFE_DEFINE_SIGNED_MOD(T, name, min, max) \
  static inline _Bool \
  safe_##name##_mod (T* res, T a, T b) { \
    if (SAFE_UNLIKELY(b == 0)) { \
      *res = 0; \
      return 0; \
    } else if (SAFE_UNLIKELY(a == min && b == -1)) { \
      *res = min; \
      return 0; \
    } else { \
      *res = (T) (a % b); \
      return 1; \
    } \
  }

#define SAFE_DEFINE_UNSIGNED_MOD(T, name, max) \
  static inline _Bool \
  safe_##name##_mod (T* res, T a, T b) { \
    if (SAFE_UNLIKELY(b == 0)) { \
      *res = 0; \
      return 0; \
    } else { \
      *res = a % b; \
      return 1; \
    } \
  }

#define SAFE_DEFINE_SIGNED_NEG(T, name, min, max) \
  static inline _Bool \
  safe_##name##_neg (T* res, T value) { \
    *res = -value; \
    return !SAFE_UNLIKELY(value == min); \
  }

#if defined(SAFE_HAVE_BUILTIN_OVERFLOW)
SAFE_DEFINE_BUILTIN_BINARY_OP(char, char, add)
SAFE_DEFINE_BUILTIN_BINARY_OP(char, char, sub)
SAFE_DEFINE_BUILTIN_BINARY_OP(char, char, mul)
#elif defined(SAFE_HAVE_LARGER_CHAR)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(char, char, add, CHAR_MIN, CHAR_MAX)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(char, char, sub, CHAR_MIN, CHAR_MAX)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(char, char, mul, CHAR_MIN, CHAR_MAX)
#else
SAFE_DEFINE_SIGNED_ADD(char, char, CHAR_MIN, CHAR_MAX)
SAFE_DEFINE_SIGNED_SUB(char, char, CHAR_MIN, CHAR_MAX)
SAFE_DEFINE_SIGNED_MUL(char, char, CHAR_MIN, CHAR_MAX)
#endif
SAFE_DEFINE_SIGNED_DIV(char, char, CHAR_MIN, CHAR_MAX)
SAFE_DEFINE_SIGNED_MOD(char, char, CHAR_MIN, CHAR_MAX)
SAFE_DEFINE_SIGNED_NEG(char, char, CHAR_MIN, CHAR_MAX)

#if defined(SAFE_HAVE_BUILTIN_OVERFLOW)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned char, uchar, add)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned char, uchar, sub)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned char, uchar, mul)
#elif defined(SAFE_HAVE_LARGER_UCHAR)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned char, uchar, add, UCHAR_MAX)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned char, uchar, sub, UCHAR_MAX)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned char, uchar, mul, UCHAR_MAX)
#else
SAFE_DEFINE_UNSIGNED_ADD(unsigned char, uchar, UCHAR_MAX)
SAFE_DEFINE_UNSIGNED_SUB(unsigned char, uchar, UCHAR_MAX)
SAFE_DEFINE_UNSIGNED_MUL(unsigned char, uchar, UCHAR_MAX)
#endif
SAFE_DEFINE_UNSIGNED_DIV(unsigned char, uchar, UCHAR_MAX)
SAFE_DEFINE_UNSIGNED_MOD(unsigned char, uchar, UCHAR_MAX)

#if defined(SAFE_HAVE_BUILTIN_OVERFLOW)
SAFE_DEFINE_BUILTIN_BINARY_OP(short, short, add)
SAFE_DEFINE_BUILTIN_BINARY_OP(short, short, sub)
SAFE_DEFINE_BUILTIN_BINARY_OP(short, short, mul)
#elif defined(SAFE_HAVE_LARGER_SHORT)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(short, short, add, SHRT_MIN, SHRT_MAX)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(short, short, sub, SHRT_MIN, SHRT_MAX)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(short, short, mul, SHRT_MIN, SHRT_MAX)
#else
SAFE_DEFINE_SIGNED_ADD(short, short, SHRT_MIN, SHRT_MAX)
SAFE_DEFINE_SIGNED_SUB(short, short, SHRT_MIN, SHRT_MAX)
SAFE_DEFINE_SIGNED_MUL(short, short, SHRT_MIN, SHRT_MAX)
#endif
SAFE_DEFINE_SIGNED_DIV(short, short, SHRT_MIN, SHRT_MAX)
SAFE_DEFINE_SIGNED_MOD(short, short, SHRT_MIN, SHRT_MAX)
SAFE_DEFINE_SIGNED_NEG(short, short, SHRT_MIN, SHRT_MAX)

#if defined(SAFE_HAVE_BUILTIN_OVERFLOW)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned short, ushort, add)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned short, ushort, sub)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned short, ushort, mul)
#elif defined(SAFE_HAVE_LARGER_USHORT)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned short, ushort, add, USHRT_MAX)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned short, ushort, sub, USHRT_MAX)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned short, ushort, mul, USHRT_MAX)
#else
SAFE_DEFINE_UNSIGNED_ADD(unsigned short, ushort, USHRT_MAX)
SAFE_DEFINE_UNSIGNED_SUB(unsigned short, ushort, USHRT_MAX)
SAFE_DEFINE_UNSIGNED_MUL(unsigned short, ushort, USHRT_MAX)
#endif
SAFE_DEFINE_UNSIGNED_DIV(unsigned short, ushort, USHRT_MAX)
SAFE_DEFINE_UNSIGNED_MOD(unsigned short, ushort, USHRT_MAX)

#if defined(SAFE_HAVE_BUILTIN_OVERFLOW)
SAFE_DEFINE_BUILTIN_BINARY_OP(int, int, add)
SAFE_DEFINE_BUILTIN_BINARY_OP(int, int, sub)
SAFE_DEFINE_BUILTIN_BINARY_OP(int, int, mul)
#elif defined(SAFE_HAVE_LARGER_INT)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(int, int, add, INT_MIN, INT_MAX)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(int, int, sub, INT_MIN, INT_MAX)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(int, int, mul, INT_MIN, INT_MAX)
#else
SAFE_DEFINE_SIGNED_ADD(int, int, INT_MIN, INT_MAX)
SAFE_DEFINE_SIGNED_SUB(int, int, INT_MIN, INT_MAX)
SAFE_DEFINE_SIGNED_MUL(int, int, INT_MIN, INT_MAX)
#endif
SAFE_DEFINE_SIGNED_DIV(int, int, INT_MIN, INT_MAX)
SAFE_DEFINE_SIGNED_MOD(int, int, INT_MIN, INT_MAX)
SAFE_DEFINE_SIGNED_NEG(int, int, INT_MIN, INT_MAX)

#if defined(SAFE_HAVE_BUILTIN_OVERFLOW)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned int, uint, add)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned int, uint, sub)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned int, uint, mul)
#elif defined(SAFE_HAVE_LARGER_UINT)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned int, uint, add, UINT_MAX)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned int, uint, sub, UINT_MAX)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned int, uint, mul, UINT_MAX)
#else
SAFE_DEFINE_UNSIGNED_ADD(unsigned int, uint, UINT_MAX)
SAFE_DEFINE_UNSIGNED_SUB(unsigned int, uint, UINT_MAX)
SAFE_DEFINE_UNSIGNED_MUL(unsigned int, uint, UINT_MAX)
#endif
SAFE_DEFINE_UNSIGNED_DIV(unsigned int, uint, UINT_MAX)
SAFE_DEFINE_UNSIGNED_MOD(unsigned int, uint, UINT_MAX)

#if defined(SAFE_HAVE_BUILTIN_OVERFLOW)
SAFE_DEFINE_BUILTIN_BINARY_OP(long, long, add)
SAFE_DEFINE_BUILTIN_BINARY_OP(long, long, sub)
SAFE_DEFINE_BUILTIN_BINARY_OP(long, long, mul)
#elif defined(SAFE_HAVE_LARGER_LONG)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(long, long, add, LONG_MIN, LONG_MAX)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(long, long, sub, LONG_MIN, LONG_MAX)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(long, long, mul, LONG_MIN, LONG_MAX)
#else
SAFE_DEFINE_SIGNED_ADD(long, long, LONG_MIN, LONG_MAX)
SAFE_DEFINE_SIGNED_SUB(long, long, LONG_MIN, LONG_MAX)
SAFE_DEFINE_SIGNED_MUL(long, long, LONG_MIN, LONG_MAX)
#endif
SAFE_DEFINE_SIGNED_DIV(long, long, LONG_MIN, LONG_MAX)
SAFE_DEFINE_SIGNED_MOD(long, long, LONG_MIN, LONG_MAX)
SAFE_DEFINE_SIGNED_NEG(long, long, LONG_MIN, LONG_MAX)

#if defined(SAFE_HAVE_BUILTIN_OVERFLOW)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned long, ulong, add)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned long, ulong, sub)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned long, ulong, mul)
#elif defined(SAFE_HAVE_LARGER_ULONG)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned long, ulong, add, ULONG_MAX)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned long, ulong, sub, ULONG_MAX)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned long, ulong, mul, ULONG_MAX)
#else
SAFE_DEFINE_UNSIGNED_ADD(unsigned long, ulong, ULONG_MAX)
SAFE_DEFINE_UNSIGNED_SUB(unsigned long, ulong, ULONG_MAX)
SAFE_DEFINE_UNSIGNED_MUL(unsigned long, ulong, ULONG_MAX)
#endif
SAFE_DEFINE_UNSIGNED_DIV(unsigned long, ulong, ULONG_MAX)
SAFE_DEFINE_UNSIGNED_MOD(unsigned long, ulong, ULONG_MAX)

#if defined(SAFE_HAVE_BUILTIN_OVERFLOW)
SAFE_DEFINE_BUILTIN_BINARY_OP(long long, llong, add)
SAFE_DEFINE_BUILTIN_BINARY_OP(long long, llong, sub)
SAFE_DEFINE_BUILTIN_BINARY_OP(long long, llong, mul)
#elif defined(SAFE_HAVE_LARGER_LLONG)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(long long, llong, add, LLONG_MIN, LLONG_MAX)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(long long, llong, sub, LLONG_MIN, LLONG_MAX)
SAFE_DEFINE_PROMOTED_SIGNED_BINARY_OP(long long, llong, mul, LLONG_MIN, LLONG_MAX)
#else
SAFE_DEFINE_SIGNED_ADD(long long, llong, LLONG_MIN, LLONG_MAX)
SAFE_DEFINE_SIGNED_SUB(long long, llong, LLONG_MIN, LLONG_MAX)
SAFE_DEFINE_SIGNED_MUL(long long, llong, LLONG_MIN, LLONG_MAX)
#endif
SAFE_DEFINE_SIGNED_DIV(long long, llong, LLONG_MIN, LLONG_MAX)
SAFE_DEFINE_SIGNED_MOD(long long, llong, LLONG_MIN, LLONG_MAX)
SAFE_DEFINE_SIGNED_NEG(long long, llong, LLONG_MIN, LLONG_MAX)

#if defined(SAFE_HAVE_BUILTIN_OVERFLOW)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned long long, ullong, add)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned long long, ullong, sub)
SAFE_DEFINE_BUILTIN_BINARY_OP(unsigned long long, ullong, mul)
#elif defined(SAFE_HAVE_LARGER_ULLONG)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned long long, ullong, add, ULLONG_MAX)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned long long, ullong, sub, ULLONG_MAX)
SAFE_DEFINE_PROMOTED_UNSIGNED_BINARY_OP(unsigned long long, ullong, mul, ULLONG_MAX)
#else
SAFE_DEFINE_UNSIGNED_ADD(unsigned long long, ullong, ULLONG_MAX)
SAFE_DEFINE_UNSIGNED_SUB(unsigned long long, ullong, ULLONG_MAX)
SAFE_DEFINE_UNSIGNED_MUL(unsigned long long, ullong, ULLONG_MAX)
#endif
SAFE_DEFINE_UNSIGNED_DIV(unsigned long long, ullong, ULLONG_MAX)
SAFE_DEFINE_UNSIGNED_MOD(unsigned long long, ullong, ULLONG_MAX)

#endif /* !defined(SAFE_H) */
