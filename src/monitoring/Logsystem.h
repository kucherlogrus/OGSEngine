#ifndef OGS_LOG_SYSTEM_H
#define OGS_LOG_SYSTEM_H

#ifndef DEBUG
    #define DEBUG 1
#endif

#ifndef DEBUG_LEVEL
    #define DEBUG_LEVEL 0
#endif    

#define DEBUG_INFO 0
#define DEBUG_DEBUG 1
#define DEBUG_WARNING 2
#define DEBUG_ERROR 3

#if defined(WINDOWS)

#if WINVER >= 0x0A00
    #define LOGCOLORRED    "[31m"
    #define LOGCOLORGREEN "[32m"
    #define LOGCOLORYELLOW "[33m"
    #define LOGCOLORRESET  "[0m"
#else
    #define LOGCOLORRED ""
    #define LOGCOLORGREEN ""
    #define LOGCOLORYELLOW ""
    #define LOGCOLORRESET ""

#endif

#else

#define LOGCOLORRED    "\x1b[031m"
#define LOGCOLORGREEN "\x1b[032m"
#define LOGCOLORYELLOW "\x1b[033m"
#define LOGCOLORRESET  "\x1b[0m"

#endif

#include <iostream>
#include "glm/glm.hpp"
#include <vector>

namespace ogs {

    static std::ostream &operator<<(std::ostream &os, const glm::mat4 &m) {
        os << std::endl;
        os << m[0][0] << " " << m[0][1] << " " << m[0][2]<< " " << m[0][3] << std::endl;
        os << m[1][0] << " " << m[1][1] << " " << m[1][2]<< " " << m[1][3] << std::endl;
        os << m[2][0] << " " << m[2][1] << " " << m[2][2]<< " " << m[2][3] << std::endl;
        os << m[3][0] << " " << m[3][1] << " " << m[3][2]<< " " << m[3][3];
        return os;
    };

    template<typename T>
    static std::ostream &operator<<(std::ostream &os, const std::vector<T>& vec) {
        os << "size of vector:" << vec.size() << std::endl;
        for (auto it = vec.begin(); it != vec.end(); it++) {
            os << *it << " dawdw " << std::endl;
        }

        os << std::endl;
        return os;
    };



};


#if defined(ANDROID)
#include <android/log.h>
#include <string>
#include <sstream>

template<typename ...Args>
std::string getstring(Args const&... args) {
    std::ostringstream stream;
    using List = int[];
    (void)List{0, ( (void)(stream << args), 0 ) ... };
    return stream.str();
};

template<typename... Args>
void loginfo(const char *tag, const char *msg, const Args &... args) {
    std::string str = getstring(args...);
    __android_log_print(ANDROID_LOG_INFO, tag, "%s %s", msg, str.c_str());
}

template<typename... Args>
void logdebug(const char *tag, const char *msg, const Args &... args) {
    std::string str = getstring(args...);
    __android_log_print(ANDROID_LOG_DEBUG, tag, "%s %s", msg, str.c_str());
}

template<typename... Args>
void logwarning(const char *tag, const char *msg, const Args &... args) {
    std::string str = getstring(args...);
    __android_log_print(ANDROID_LOG_WARN, tag, "%s %s", msg, str.c_str());
}

template<typename... Args>
void logerror(const char *tag, const char *msg, const Args &... args) {
    std::string str = getstring(args...);
    __android_log_print(ANDROID_LOG_ERROR, tag, "%s %s", msg, str.c_str());
}
#else

#include <stdarg.h>

namespace ogs {

static void __log() {}

template<typename First, typename ...Rest>
void __log(const First f, const Rest &... rest) {
    std::cout << f << " ";
    __log(rest...);
}


template<typename... Args>
void _log(int level, const char *tag, const char *msg, const Args &... args) {
    if (level >= DEBUG_LEVEL) {
        std::cout << tag << ": ";
        __log(msg, args...);
        std::cout << std::endl;
    }
}

template<typename... Args>
void loginfo(const char *tag, const char *msg, const Args &... args) {
    _log(DEBUG_INFO, tag, msg, args...);
}

template<typename... Args>
void logdebug(const char *tag, const char *msg, const Args &... args) {
    std::cout << LOGCOLORGREEN;
    _log(DEBUG_DEBUG, tag, msg, args...);
    std::cout << LOGCOLORRESET << std::endl;
}

template<typename... Args>
void logwarning(const char *tag, const char *msg, const Args &... args) {
    std::cout << LOGCOLORYELLOW;
    _log(DEBUG_WARNING, tag, msg, args...);
    std::cout << LOGCOLORRESET << std::endl;
}

template<typename... Args>
void logerror(const char *tag, const char *msg, const Args &... args) {
    std::cout << LOGCOLORRED;
    _log(DEBUG_ERROR, tag, msg, args...);
    std::cout << LOGCOLORRESET << std::endl;
}

}

#endif

#endif


