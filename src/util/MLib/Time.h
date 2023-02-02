/*****************************************************************************
* File:
* Description:
* Author: Malcolm Hall
* Date:
* Version: 1
*
******************************************************************************/

#ifndef MLIB_TIME_H
#define MLIB_TIME_H

#include <chrono>
#include <type_traits>

#ifndef ASSERT_IS_TIME_TYPE
#define ASSERT_IS_TIME_TYPE(T)                                        \
    static_assert(std::is_same_v<T, std::chrono::hours> ||            \
                      std::is_same_v<T, std::chrono::minutes> ||      \
                      std::is_same_v<T, std::chrono::seconds> ||      \
                      std::is_same_v<T, std::chrono::milliseconds> || \
                      std::is_same_v<T, std::chrono::microseconds> || \
                      std::is_same_v<T, std::chrono::nanoseconds>,    \
                  "Type should be an std::chrono time type.");
#endif

#define NS_PER_MS (1000000)
#define MS_PER_SECOND (1000)
#define SECONDS_PER_MINUTE (60)
#define MINUTES_PER_HOUR (60)

#ifndef MLIB_CLOCK
#define MLIB_CLOCK std::chrono::high_resolution_clock
#endif

/*****************************************************************************/
class Time {
public:
    /*************************************************************************/
    template <class T = std::chrono::milliseconds>
    static size_t Now() {
        ASSERT_IS_TIME_TYPE(T);
        auto duration = MLIB_CLOCK::now().time_since_epoch();
        return std::chrono::duration_cast<T>(duration).count();
    }

    /*************************************************************************/
    static size_t Seconds() {
        return Now<std::chrono::seconds>();
    }

    /*************************************************************************/
    static size_t Milliseconds() {
        return Now<std::chrono::milliseconds>();
    }

    /*************************************************************************/
    static size_t Microseconds() {
        return Now<std::chrono::microseconds>();
    }

    /*************************************************************************/
    static size_t Nanoseconds() {
        return Now<std::chrono::nanoseconds>();
    }

    /*************************************************************************/
    static std::string Stamp() {
        return "";
    }
};

#endif  //MLIB_TIME_H