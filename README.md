# Overflow-safe math functions

The safe.h header will define numerous functions verifying that
integer operations do not overflow.

Functions are provided for lots of types:

| Identifier | Type                 |
| ---------- | -------------------- |
| `char`     | `char`               |
| `uchar`    | `unsigned char`      |
| `short`    | `short`              |
| `ushort`   | `unsigned short`     |
| `int`      | `int`                |
| `uint`     | `unsigned int`       |
| `long`     | `long`               |
| `ulong`    | `unsigned long`      |
| `llong`    | `long long`          |
| `ullong`   | `unsigned long long` |
| `int8`     | `int8_t`             |
| `uint8`    | `uint8_t`            |
| `int16`    | `int16_t`            |
| `uint16`   | `uint16_t`           |
| `int32`    | `int32_t`            |
| `uint32`   | `uint32_t`           |
| `int64`    | `int64_t`            |
| `uint64`   | `uint64_t`           |

For each of the following operations:

 * add (addition)
 * sub (subtraction)
 * mul (multiplication)
 * div (division)
 * mod (modulus)
 * neg (negation)

For types where we know of a larger type we define the macro
`SAFE_HAVE_TYPE_LARGER`, as well as a typedef to the larger type
called `safe_type_larger`.  For example, we know that there is a type
larger than `int16_t` (`int32_t`), so there is:

```C
#define SAFE_HAVE_INT16_LARGER
typedef int32_t safe_int16_larger;
```

This gets slightly more complicated (but more useful) when dealing
with the non-fixed-length types.  For example, short may or may not be
larger than char (it's not shorter, but they may be equal).  So,
assuming char is less than 64-bits (or the compiler supports 128-bit
numbers), we will define `SAFE_HAVE_CHAR_LARGER` and
`safe_char_larger` will be a typedef to the first type which is larger
than char (the order we try is `char`, `short`, `int`, `long`, `long
long`, `int8_t`, `int16_t`, `int32_t`, `int64_t`, then the 128-bit
integer type if available).  So, `safe_char_larger` is a typedef to
`short` if `short` is larger than `char`, otherwise `int` if `int` is
larger than `char`, otherwise `long`, etc.

In addition to the typedefs, if there is a larger known type we
also generate a set of functions in the form of

```C
safe_type_larger safe_larger_type_operation(type, type)
```

This is done for every type and every operation; for example, if
`SAFE_HAVE_CHAR_LARGER` is defined, then so too will be

 * `safe_char_larger safe_larger_char_add(char, char)`
 * `safe_char_larger safe_larger_char_sub(char, char)`
 * `safe_char_larger safe_larger_char_mul(char, char)`
 * `safe_char_larger safe_larger_char_div(char, char)`
 * `safe_char_larger safe_larger_char_mod(char, char)`
 * `safe_char_larger safe_larger_char_neg(char)`

Finally, *for all types*, regardless of whether or not a larger
type is available, we define overflow-safe versions in the form of:

```C
_Bool safe_type_operation(type* res, type a, type b)
```

The function will store the result of the operation on `a` and `b` in
`*res` (except for negation, where there are only two parameters, for
obvious reasons).  The return value is used to indicate if the
operation was successful (in which case the return value is true) or
an overflow was detected.

These are implemented with compiler builtins when available.  If a
builtin is not avaialable, it falls back on a function which casts
to a larger type, performs the operation, checks that the result
will fit in the original type, and casts back to the original type.
If no larger type is available it will check to see if the
operation would overflow (see
https://www.securecoding.cert.org/confluence/display/c/INT32-C.+Ensure+that+operations+on+signed+integers+do+not+result+in+overflow
for details on how that is done).

If you compile in C11 mode, the following type-generic macros are also
available for invoking each operation on any of the types handled by
the other functions:

```C
_Bool safe_add(type* res, type a, type b);
_Bool safe_sub(type* res, type a, type b);
_Bool safe_mul(type* res, type a, type b);
_Bool safe_div(type* res, type a, type b);
_Bool safe_mod(type* res, type a, type b);
_Bool safe_neg(type* res, type v);
```

The type used is determined based on the type of the `res` argument.
