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
// RATL_CPP_COMPILER_MSVC
// RATL_CPP_COMPILER_CLANG
// RATL_CPP_COMPILER_GCC

#if defined(_MSC_VER)
#    define RATL_CPP_COMPILER_MSVC
#elif defined(__clang__)
#    define RATL_CPP_COMPILER_CLANG
#elif defined(__GNUC__)
#    define RATL_CPP_COMPILER_GCC
#else
#    error Unknown compiler
#endif

// architecture
// RATL_CPP_ARCH_X86
// RATL_CPP_ARCH_X86_64
// RATL_CPP_ARCH_ARM

#if defined(RATL_CPP_COMPILER_MSVC)
#    if defined(_M_X64)
#        define RATL_CPP_ARCH_X86_64
#    elif defined(_M_IX86)
#        define RATL_CPP_ARCH_X86
#    elif defined(_M_ARM) || defined(_M_ARM64)
#        define RATL_CPP_ARCH_ARM
#    else
#        error Unknown architecture
#    endif
#elif defined(RATL_CPP_COMPILER_CLANG) || defined(RATL_CPP_COMPILER_GCC)
#    if defined(__x86_64__)
#        define RATL_CPP_ARCH_X86_64
#    elif defined(__i386)
#        define RATL_CPP_ARCH_X86
#    elif defined(__arm__)
#        define RATL_CPP_ARCH_ARM
#    else
#        error Unknown architecture
#    endif
#endif

// endianness
// RATL_CPP_LITTLE_ENDIAN
// RATL_CPP_BIG_ENDIAN

#if defined(RATL_CPP_COMPILER_MSVC)
#    if defined(RATL_CPP_ARCH_X86) || defined(RATL_CPP_ARCH_X86_64)
#        define RATL_CPP_LITTLE_ENDIAN
#    else
#        error Unknown endianness
#    endif
#elif defined(RATL_CPP_COMPILER_CLANG)
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
#endif

#if defined(__has_builtin)
#    if __has_builtin(__type_pack_element)
#        define RATL_CPP_USE_TYPE_PACK_ELEMENT_INTRINSIC
#    endif
#endif

// likely/unlikely
// RATL_LIKELY(x)
// RATL_UNLIKELY(x)

#if defined(RATL_CPP_COMPILER_MSVC)
#    define RATL_LIKELY(x) (x)
#    define RATL_UNLIKELY(x) (x)
#elif defined(RATL_CPP_COMPILER_CLANG)
#    define RATL_LIKELY(x) (__builtin_expect((x), 1))
#    define RATL_UNLIKELY(x) (__builtin_expect((x), 0))
#elif defined(RATL_CPP_COMPILER_GCC)
#    define RATL_LIKELY(x) (__builtin_expect((x), 1))
#    define RATL_UNLIKELY(x) (__builtin_expect((x), 0))
#endif

#endif // _ratl_detail_config_
