# Contributing to NewsDist

Contributions to NewsDist are welcome, but be sure about:
 1. You cannot send pull requests to GitHub repository, send patches to my email instead.
 2. You must write the code in C89, but:
    1. You can use following fixed integer types:
       1. `uint8_t`/`int8_t`
       2. `uint16_t`/`int16_t`
       3. `uint32_t`/`int32_t`
    2. You can use:
       1. `NDBOOL` instead of `bool`
       2. `NDTRUE` instead of `true`
       3. `NDFALSE` instead of `false`
       But you must define `#define INCLUDE_BOOLEAN` before including `newsdist.h`.
 3. Include standard headers before including `newsdist.h`.
