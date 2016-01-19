/************************************************************************/
/*                                                                      */
/* Common helper functions for Unicue Project.                          */
/* The file is a part of Unicue Project.                                */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#pragma once

// utils.h
#ifndef UNICUE_UTILS_H_
#define UNICUE_UTILS_H_

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <string.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>

template <typename T>
inline void replace(std::basic_string<T> &str, const std::basic_string<T> &oldstr, const std::basic_string<T> &newstr)
{
    int pos = 0;
    int oldstr_len = oldstr.size(), newstr_len = newstr.size();
    while ((pos = str.find(oldstr, pos)) != std::basic_string<T>::npos)
    {
        str.replace(pos, oldstr_len, newstr);
        pos += newstr_len;
    }
}

template <typename T>
inline void replace(std::basic_string<T> &str, const std::basic_string<T> &oldstr, const T* newstr)
{
    if (!newstr) return;
    int pos = 0;
    int oldstr_len = oldstr.size(), newstr_len = strlen_t(newstr);
    while ((pos = str.find(oldstr, pos)) != std::basic_string<T>::npos)
    {
        str.replace(pos, oldstr_len, newstr);
        pos += newstr_len;
    }
}

template <typename T>
inline std::basic_string<T> &trim_left(std::basic_string<T> &str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return str;
}

template <typename T>
inline std::basic_string<T> &trim_right(std::basic_string<T> &str)
{
    str.erase(std::find_if(str.rbegin(), str.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), str.end());
    return str;
}

template <typename T>
inline std::basic_string<T> &trim(std::basic_string<T> &str)
{
    return trim_left(trim_right(str));
}

template <typename T>
inline size_t strlen_t(const T* str)
{
    if (!str) return 0;
    size_t i = 0;
    while (str[i] != 0) ++i;
    return i;
}

inline wchar_t _wchartolower(wchar_t chr)
{
    if (chr <= L'Z' && chr >= L'A')
        return chr + (L'z' - L'Z');
    return chr;
}

template <typename T>
inline void toLower(std::basic_string<T> &str)
{
    std::transform(str.begin(), str.end(), str.begin(), std::tolower);
}

inline void convertBEtoLE(wchar_t *bigEndianBuffer, unsigned int length)
{
    for (unsigned int i=0; i<length; ++i)
    {
        unsigned char *chr = (unsigned char *)(bigEndianBuffer + i);
        unsigned char temp = *chr;
        *chr = *(chr + 1);
        *(chr + 1) = temp;
    }
}

template <class T>
inline std::string toSTLString(const T& t)
{
  std::stringstream ss;
  ss << t;
  return ss.str();
}

inline bool beginWith(const wchar_t* prefix, const wchar_t* str)
{
    return wcsncmp(prefix, str, wcslen(prefix)) == 0;
}

#endif // UNICUE_UTILS_H_
