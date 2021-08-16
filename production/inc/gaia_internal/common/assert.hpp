/////////////////////////////////////////////
// Copyright (c) Gaia Platform LLC
// All rights reserved.
/////////////////////////////////////////////

#pragma once

#include <sstream>
#include <string>

#include "gaia/exception.hpp"

namespace gaia
{
/**
 * \addtogroup Gaia
 * @{
 */
namespace common
{
/**
 * \addtogroup Common
 * @{
 */

// The ASSERT macros defined in this file are used for internal validation checks
// that are meant to be performed in both debug and retail builds.
//
// These ASSERTs provide a mechanism for failing execution as soon as an issue is detected,
// which should surface errors early on and should prevent more expensive investigations.
//
// A way to disable these ASSERTs (via corresponding defines) is provided mainly as a mechanism
// for determining if they contribute any negative execution impact.

// ASSERT_PRECONDITION is meant for validating conditions that should hold when a function is called.
//
// This should be used to validate that internal functions are called with the correct parameters
// and in the correct context.
//
// NOTE: This assert should not be used within public functions. Public functions should expect to
// be called incorrectly and should handle such incorrect calls with regular errors or exceptions.
#ifdef DISABLE_ASSERTS
#define ASSERT_PRECONDITION(c, m) ((void)0)
#else
#define ASSERT_PRECONDITION(c, m) gaia::common::assert_do_not_call_directly(static_cast<bool>(c), m, __FILE__, __LINE__, __func__)
#endif

// ASSERT_INVARIANT is meant for validating conditions that should hold internally,
// during the execution of a function.
//
// This is arguably the most important ASSERT to use, because it can surface
// algorithmic errors which would otherwise be very difficult to detect.
#ifdef DISABLE_ASSERTS
#define ASSERT_INVARIANT(c, m) ((void)0)
#else
#define ASSERT_INVARIANT(c, m) gaia::common::assert_do_not_call_directly(static_cast<bool>(c), m, __FILE__, __LINE__, __func__)
#endif

// ASSERT_POSTCONDITION is meant for validating conditions that should hold after a function
// has completed execution.
//
// This should be used to validate that functions produce the expected outcome.
#ifdef DISABLE_ASSERTS
#define ASSERT_POSTCONDITION(c, m) ((void)0)
#else
#define ASSERT_POSTCONDITION(c, m) gaia::common::assert_do_not_call_directly(static_cast<bool>(c), m, __FILE__, __LINE__, __func__)
#endif

// ASSERT_UNREACHABLE is meant for validating that a section of code can never be reached.
//
// Because ASSERT_UNREACHABLE results in an unconditional failure,
// there should never be a need to disable such an assert - even for debugging purposes.
#define ASSERT_UNREACHABLE(m) gaia::common::assert_do_not_call_directly(false, m, __FILE__, __LINE__, __func__)

// The following DEBUG_ASSERT macros are equivalent to the ASSERT macros, except that they are disabled by default in non-debug builds.
// They are intended for validation code that is too expensive to enable in production.
// These macros are disabled if the NDEBUG macro is defined, which is consistent with the behavior of assert() in the C and C++ standards.

#ifdef NDEBUG
#define DEBUG_ASSERT_PRECONDITION(c, m) ((void)0)
#else
#define DEBUG_ASSERT_PRECONDITION(c, m) ASSERT_PRECONDITION(c, m)
#endif

#ifdef NDEBUG
#define DEBUG_ASSERT_INVARIANT(c, m) ((void)0)
#else
#define DEBUG_ASSERT_INVARIANT(c, m) ASSERT_INVARIANT(c, m)
#endif

#ifdef NDEBUG
#define DEBUG_ASSERT_POSTCONDITION(c, m) ((void)0)
#else
#define DEBUG_ASSERT_POSTCONDITION(c, m) ASSERT_POSTCONDITION(c, m)
#endif

#ifdef NDEBUG
#define DEBUG_ASSERT_UNREACHABLE(m) ((void)0)
#else
#define DEBUG_ASSERT_UNREACHABLE(m) ASSERT_UNREACHABLE(m)
#endif

/**
 * Thrown when an assert check has failed.
 */
class assertion_failure : public gaia_exception
{
public:
    explicit assertion_failure(const std::string& message)
        : gaia_exception(message)
    {
    }
};

/**
 * Asserts are meant for important conditions that indicate internal errors.
 * They help us surface issues early, at the source.
 *
 * This function should only be called through the various assert macros,
 * so that it gets passed the correct information about the point of call.
 */
inline void assert_do_not_call_directly(
    bool condition, const std::string& message, const char* file, size_t line, const char* function)
{
    if (!condition)
    {
        std::stringstream message_stream;
        message_stream << "Assertion failed in " << file << "::" << function << "(): line " << line << ": " << message;
        throw assertion_failure(message_stream.str());
    }
}

/*@}*/
} // namespace common
/*@}*/
} // namespace gaia
