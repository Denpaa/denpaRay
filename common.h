//  common.hpp
//  Contains all the functions and definitions used by other files
//  This should be included after all the standard headers and before the user-defined headers
//  Created by 電波

#pragma once

#define PI32 3.14159265359

// -----------------------------------------------
// @denpa: All the common types are typedef'd here.
// -----------------------------------------------
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef float f32;
typedef double f64;

// -----------------------------------------------
// @denpa: Detects the platform being used.
// -----------------------------------------------
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define DENPA_PLATFORM_WINDOWS 1
#ifndef _WIN64
#error "64-bit Windows is required."
#endif

#elif defined(__linux__) || defined(__gnu_linux__)
#define DENPA_PLATFORM_LINUX 1

#if defined(__ANDROID__)
#define DENPA_PLATFORM_ANDROID 1
#endif

#elif defined(__unix__)
#define DENPA_PLATFORM_UNIX 1

#elif defined(_POSIX_VERSION)
#define DENPA_PLATFORM_POSIX 1

#elif __APPLE__
#define DENPA_PLATFORM_APPLE 1
#include <TargetConditionals.h>

#if TARGET_IPHONE_SIMULATOR
#define DENPA_PLATFORM_IOS 1
#define DENPA_PLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define DENPA_PLATFORM_IOS 1
#elif TARGET_OS_MAC
#else
#error "Unknown Apple platform"
#endif

#else
#error "Unknown platform"
#endif

// -----------------------------------------------
// @denpa: Determines the appropriate keyword for assertion depending on the compiler used.
// -----------------------------------------------
#if defined(__clang__) || defined(__GNUC__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

// -----------------------------------------------
// @denpa: Determines the appropriate keyword for inlining depending on the compiler used.
// -----------------------------------------------
#if defined(__clang__) || defined(__gcc__)
#define DENPA_INLINE __attribute__((always_inline)) inline
#define DENPA_NOINLINE __attribute__((noinline))

#elif defined(_MSC_VER)
#define DENPA_INLINE __forceinline
#define DENPA_NOINLINE __declspec(noinline)

#else
#define DENPA_INLINE inline
#define DENPA_NOINLINE
#endif

#define DINLINE DENPA_INLINE
#define DNOINLINE DENPA_NOINLINE

// -----------------------------------------------
// @denpa: Used to label intent and unused functions and parameters.
// -----------------------------------------------
#define UNUSED __attribute__((unused))
#define GLOBAL_VARIABLE static
#define LOCAL_PERSIST static
#define INTERNAL static

// -----------------------------------------------
// @denpa: Some universally useful one-liner helper functions.
// -----------------------------------------------
#define DENPA_CLAMP(value, min, max) (((value) <= (min)) ? (min) : ((value) >= (max)) ? (max) : (value))
#define DENPA_MIN(x, y) ((x) < (y) ? (x) : (y))
#define DENPA_MAX(x, y) ((x) > (y) ? (x) : (y))
#define DENPA_ARRAY_SIZE(a) ((sizeof((a))) / (sizeof((a)[0])))
#define DENPA_KILOBYTES(number) ((number)*1024ull)
#define DENPA_MEGABYTES(number) (DENPA_KILOBYTES((number)) * 1024ull)
#define DENPA_GIGABYTES(number) (DENPA_MEGABYTES((number)) * 1024ull)
#define DENPA_TERABYTES(number) (DENPA_GIGABYTES((number)) * 1024ull)
#define DENPA_ALIGN16(value) (((value) + 15) & ~15)
#define DENPA_ALIGN4(value) (((value) + 3) & ~3)

// -----------------------------------------------
// @denpa: Controls the tolerance for equality between two floats.
// -----------------------------------------------
#define EPSILON 0.00001f

// -----------------------------------------------
// @denpa: Checks if two floats are equal (with some degree of inaccuracy)
// -----------------------------------------------
INTERNAL DINLINE bool areFloatsEqual(float a, float b) {
	if (abs(a-b) < EPSILON) {return true;}
	return false;
}
