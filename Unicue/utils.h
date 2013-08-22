
#pragma once

// utils.h
#ifndef UTILS_H_
#define UTILS_H_

#include <string.h>
#include <string>

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
inline size_t strlen_t(const T* str)
{
    if (!str) return 0;
    size_t i = 0;
    while (str[i] != 0) ++i;
    return i;
}

#endif // UTILS_H_
