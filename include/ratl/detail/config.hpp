/**
 * Copyright (c) 2018-2022 Hamish Cook
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _ratl_detail_config_
#define _ratl_detail_config_

// platform
// RATL_CPP_PLATFORM_CYGWIN
// RATL_CPP_PLATFORM_WINDOWS
// RATL_CPP_PLATFORM_LINUX
// RATL_CPP_PLATFORM_MACOS

#if defined(__CYGWIN__)
#    define RATL_CPP_PLATFORM_CYGWIN
#elif defined(_WIN32)
#    define RATL_CPP_PLATFORM_WINDOWS
#elif defined(__linux__)
#    define RATL_CPP_PLATFORM_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
#    define RATL_CPP_PLATFORM_MACOS
#else
#    error Unknown platform
#endif

// compiler
// RATL_CPP_COMPILER_CLANG
// RATL_CPP_COMPILER_GCC
// RATL_CPP_COMPILER_MSVC

#if defined(__clang__)
#    define RATL_CPP_COMPILER_CLANG
#    if defined(_MSC_VER)
#        define RATL_CPP_COMPILER_BACKEND_MSVC
#    endif
#elif defined(__GNUC__)
#    define RATL_CPP_COMPILER_GCC
#elif defined(_MSC_VER)
#    define RATL_CPP_COMPILER_MSVC
#else
#    error Unknown compiler
#endif

// architecture
// RATL_CPP_ARCH_X86
// RATL_CPP_ARCH_X86_64
// RATL_CPP_ARCH_ARM
// RATL_CPP_ARCH_AARCH64

#if defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
#    if defined(__x86_64__)
#        define RATL_CPP_ARCH_X86_64
#    elif defined(__i386)
#        define RATL_CPP_ARCH_X86
#    elif defined(__aarch64__)
#        define RATL_CPP_ARCH_AARCH64
#    elif defined(__arm__)
#        define RATL_CPP_ARCH_ARM
#    else
#        error Unknown architecture
#    endif
#elif defined(RATL_CPP_COMPILER_MSVC)
#    if defined(_M_X64)
#        define RATL_CPP_ARCH_X86_64
#    elif defined(_M_IX86)
#        define RATL_CPP_ARCH_X86
#    elif defined(_M_ARM64)
#        define RATL_CPP_ARCH_AARCH64
#    elif defined(_M_ARM)
#        define RATL_CPP_ARCH_ARM
#    else
#        error Unknown architecture
#    endif
#endif

// C++ version
// RATL_CPP_VERSION_HAS_CPP11
// RATL_CPP_VERSION_HAS_CPP14
// RATL_CPP_VERSION_HAS_CPP17
// RATL_CPP_VERSION_HAS_CPP20

#if defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
#    if __cplusplus >= 202002L
#        define RATL_CPP_VERSION_HAS_CPP20
#    endif
#    if __cplusplus >= 201703L
#        define RATL_CPP_VERSION_HAS_CPP17
#    endif
#    if __cplusplus >= 201402L
#        define RATL_CPP_VERSION_HAS_CPP14
#    endif
#elif defined(RATL_CPP_COMPILER_MSVC)
#    if !defined(_MSVC_LANG)
#        error Unsupported version of msvc; requires Visual Studio 2015 or above
#    endif
#    if _MSVC_LANG >= 202004L
#        define RATL_CPP_VERSION_HAS_CPP20
#    endif
#    if _MSVC_LANG >= 201703L
#        define RATL_CPP_VERSION_HAS_CPP17
#    endif
#    if _MSVC_LANG >= 201402L
#        define RATL_CPP_VERSION_HAS_CPP14
#    endif
#endif
#if !defined(RATL_CPP_VERSION_HAS_CPP14)
#    error Unsupported C++ version; requires C++14 or greater
#endif

// endianness
// RATL_CPP_LITTLE_ENDIAN
// RATL_CPP_BIG_ENDIAN

#if defined(RATL_CPP_COMPILER_CLANG)
#    if defined(__LITTLE_ENDIAN__)
#        define RATL_CPP_LITTLE_ENDIAN
#    elif defined(__BIG_ENDIAN__)
#        define RATL_CPP_BIG_ENDIAN
#    else
#        error Unknown endianness
#    endif
#elif defined(RATL_CPP_COMPILER_GCC)
#    if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#        define RATL_CPP_LITTLE_ENDIAN
#    elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#        define RATL_CPP_BIG_ENDIAN
#    else
#        error Unknown endianness
#    endif
#elif defined(RATL_CPP_COMPILER_MSVC)
#    if defined(RATL_CPP_ARCH_X86) || defined(RATL_CPP_ARCH_X86_64)
#        define RATL_CPP_LITTLE_ENDIAN
#    else
#        error Unknown endianness
#    endif
#endif

// likely/unlikely
// RATL_LIKELY(x)
// RATL_UNLIKELY(x)

#if defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
#    define RATL_LIKELY(x) __builtin_expect(x, 1)
#    define RATL_UNLIKELY(x) __builtin_expect(x, 0)
#else
#    define RATL_LIKELY(x) x
#    define RATL_UNLIKELY(x) x
#endif

#if defined(RATL_CPP_COMPILER_MSVC) || defined(RATL_CPP_COMPILER_BACKEND_MSVC)
#    if defined(RATL_CPP_VERSION_HAS_CPP20)
#        define RATL_USE_INT24_MEMCPY_CONVERT
#    else
#        define RATL_USE_INT24_MEMCPY_NARROWING_CAST
#    endif
#endif

#endif // _ratl_detail_config_
