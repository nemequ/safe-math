#include "safe.h"
#include "munit/munit.h"

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
static MunitResult
test_safe_generic(const MunitParameter params[], void* user_data) {
  (void) params;
  (void) user_data;
  unsigned long long a = 1, b = 1, res;

  do {
    if (!safe_add(&res, a, b)) {
      munit_logf(MUNIT_LOG_DEBUG, "%llu + %llu overflows", a, b);
      break;
    }

    munit_logf(MUNIT_LOG_DEBUG, "%llu + %llu = %llu", a, b, res);
    a = b;
    b = res;
  } while (1);

  return MUNIT_OK;
}
#endif

#define DEFINE_SAFE_TESTS(T, name, min, max) \
  static MunitResult \
  test_safe_##name##_add(const MunitParameter params[], void* user_data) { \
    (void) params; \
    (void) user_data; \
    T result; \
    munit_assert_true(safe_##name##_add(&result, 0, 1)); \
    munit_assert_cmp_##name(result, ==, 1); \
    munit_assert_true(safe_##name##_add(&result, 1, 1)); \
    munit_assert_cmp_##name(result, ==, 2);		       \
    munit_assert_true(safe_##name##_add(&result, 1, max - 1)); \
    munit_assert_cmp_##name(result, ==, max); \
    munit_assert_false(safe_##name##_add(&result, max, 1)); \
    return MUNIT_OK; \
  } \
  \
  static MunitResult \
  test_safe_##name##_sub(const MunitParameter params[], void* user_data) { \
    (void) params; \
    (void) user_data; \
    T result; \
    munit_assert_true(safe_##name##_sub(&result, 1, 1)); \
    munit_assert_cmp_##name(result, ==, 0); \
    munit_assert_false(safe_##name##_sub(&result, min, 1)); \
    munit_assert_true(safe_##name##_sub(&result, max, 1)); \
    munit_assert_cmp_##name(result, ==, max - 1); \
    return MUNIT_OK; \
  } \
  \
  static MunitResult \
  test_safe_##name##_mul(const MunitParameter params[], void* user_data) { \
    (void) params; \
    (void) user_data; \
    T result; \
    munit_assert_true(safe_##name##_mul(&result, 2, 2)); \
    munit_assert_cmp_##name(result, ==, 4); \
    munit_assert_true(safe_##name##_mul(&result, 0, max)); \
    munit_assert_cmp_##name(result, ==, 0); \
    munit_assert_true(safe_##name##_mul(&result, max, 0)); \
    munit_assert_cmp_##name(result, ==, 0); \
    munit_assert_true(safe_##name##_mul(&result, min, 0)); \
    munit_assert_cmp_##name(result, ==, 0); \
    return MUNIT_OK; \
  } \
  \
  static MunitResult \
  test_safe_##name##_div(const MunitParameter params[], void* user_data) { \
    (void) params; \
    (void) user_data; \
    T result; \
    munit_assert_true(safe_##name##_div(&result, 2, 2)); \
    munit_assert_cmp_##name(result, ==, 1); \
    munit_assert_false(safe_##name##_div(&result, 1, 0)); \
    munit_assert_cmp_##name(result, ==, 0); \
    return MUNIT_OK; \
  } \
  \
  static MunitResult \
  test_safe_##name##_mod(const MunitParameter params[], void* user_data) { \
    (void) params; \
    (void) user_data; \
    T result; \
    munit_assert_true(safe_##name##_mod(&result, 10, 4)); \
    munit_assert_cmp_##name(result, ==, 2); \
    munit_assert_false(safe_##name##_mod(&result, 1, 0)); \
    munit_assert_cmp_##name(result, ==, 0); \
    return MUNIT_OK; \
  }

#define DEFINE_SAFE_TESTS_S(T, name, min, max) \
  DEFINE_SAFE_TESTS(T, name, min, max) \
  static MunitResult \
  test_safe_##name##_div_signed(const MunitParameter params[], void* user_data) { \
    (void) params; \
    (void) user_data; \
    T result; \
    munit_assert_false(safe_##name##_div(&result, min, -1)); \
    munit_assert_cmp_##name(result, ==, min); \
    return MUNIT_OK; \
  } \
  \
  static MunitResult \
  test_safe_##name##_neg_signed(const MunitParameter params[], void* user_data) { \
    (void) params; \
    (void) user_data; \
    T result; \
    munit_assert_true(safe_##name##_neg(&result, max)); \
    munit_assert_cmp_##name(result, ==, -max); \
    munit_assert_false(safe_##name##_neg(&result, min)); \
    munit_assert_cmp_##name(result, ==, min); \
    return MUNIT_OK; \
  }

#define DEFINE_SAFE_TEST_ENTRY(name, op) \
  { (char*) "/" #name "/" #op, test_safe_##name##_##op, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }

#define DEFINE_SAFE_TEST_ENTRY_S(name, op) \
  { (char*) "/" #name "/" #op "/signed", test_safe_##name##_##op##_signed, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }

#define DEFINE_SAFE_TEST_ENTRIES(name) \
  DEFINE_SAFE_TEST_ENTRY(name, add), \
  DEFINE_SAFE_TEST_ENTRY(name, sub), \
  DEFINE_SAFE_TEST_ENTRY(name, mul), \
  DEFINE_SAFE_TEST_ENTRY(name, div), \
  DEFINE_SAFE_TEST_ENTRY(name, mod)

#define DEFINE_SAFE_TEST_ENTRIES_S(name) \
  DEFINE_SAFE_TEST_ENTRIES(name), \
  DEFINE_SAFE_TEST_ENTRY_S(name, div), \
  DEFINE_SAFE_TEST_ENTRY_S(name, neg)

DEFINE_SAFE_TESTS_S(char,               char,     CHAR_MIN, CHAR_MAX)
DEFINE_SAFE_TESTS  (unsigned char,      uchar,           0, UCHAR_MAX)
DEFINE_SAFE_TESTS_S(short,              short,    SHRT_MIN, SHRT_MAX)
DEFINE_SAFE_TESTS  (unsigned short,     ushort,          0, USHRT_MAX)
DEFINE_SAFE_TESTS_S(int,                int,       INT_MIN, INT_MAX)
DEFINE_SAFE_TESTS  (unsigned int,       uint,            0, UINT_MAX)
DEFINE_SAFE_TESTS_S(long,               long,     LONG_MIN, LONG_MAX)
DEFINE_SAFE_TESTS  (unsigned long,      ulong,           0, ULONG_MAX)
DEFINE_SAFE_TESTS_S(long long,          llong,   LLONG_MIN, LLONG_MAX)
DEFINE_SAFE_TESTS  (unsigned long long, ullong,          0, ULLONG_MAX)
DEFINE_SAFE_TESTS  (size_t,             size,            0, SIZE_MAX)
#if !defined(SAFE_NO_FIXED)
DEFINE_SAFE_TESTS_S(int8_t,             int8,     INT8_MIN, INT8_MAX)
DEFINE_SAFE_TESTS  (uint8_t,            uint8,           0, UINT8_MAX)
DEFINE_SAFE_TESTS_S(int16_t,            int16,   INT16_MIN, INT16_MAX)
DEFINE_SAFE_TESTS  (uint16_t,           uint16,          0, UINT16_MAX)
DEFINE_SAFE_TESTS_S(int32_t,            int32,   INT32_MIN, INT32_MAX)
DEFINE_SAFE_TESTS  (uint32_t,           uint32,          0, UINT32_MAX)
DEFINE_SAFE_TESTS_S(int64_t,            int64,   INT64_MIN, INT64_MAX)
DEFINE_SAFE_TESTS  (uint64_t,           uint64,          0, UINT64_MAX)
#endif /* !defined(SAFE_NO_FIXED) */

static MunitTest safe_test_suite_tests[] = {
  DEFINE_SAFE_TEST_ENTRIES_S(char),
  DEFINE_SAFE_TEST_ENTRIES(uchar),
  DEFINE_SAFE_TEST_ENTRIES_S(short),
  DEFINE_SAFE_TEST_ENTRIES(ushort),
  DEFINE_SAFE_TEST_ENTRIES_S(int),
  DEFINE_SAFE_TEST_ENTRIES(uint),
  DEFINE_SAFE_TEST_ENTRIES_S(long),
  DEFINE_SAFE_TEST_ENTRIES(ulong),
  DEFINE_SAFE_TEST_ENTRIES_S(llong),
  DEFINE_SAFE_TEST_ENTRIES(ullong),
  DEFINE_SAFE_TEST_ENTRIES(size),
#if !defined(SAFE_NO_FIXED)
  DEFINE_SAFE_TEST_ENTRIES_S(int8),
  DEFINE_SAFE_TEST_ENTRIES(uint8),
  DEFINE_SAFE_TEST_ENTRIES_S(int16),
  DEFINE_SAFE_TEST_ENTRIES(uint16),
  DEFINE_SAFE_TEST_ENTRIES_S(int32),
  DEFINE_SAFE_TEST_ENTRIES(uint32),
  DEFINE_SAFE_TEST_ENTRIES_S(int64),
  DEFINE_SAFE_TEST_ENTRIES(uint64),
#endif /* !defined(SAFE_NO_FIXED) */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
  { "/generic", test_safe_generic, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
#endif
  { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite test_suite = {
  (char*) "/safe",
  safe_test_suite_tests,
  NULL,
  1,
  MUNIT_SUITE_OPTION_NONE
};

int main(int argc, const char* argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
  return munit_suite_main(&test_suite, NULL, argc, argv);
}
