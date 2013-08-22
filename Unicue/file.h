
#pragma once

// file.h
#ifndef CFILE_H_
#define CFILE_H_

#include <fstream>

class CFile
{
public:
    enum mode {
        READ  = 0x0000,
        WRITE = 0x0001
    };
    CFile(const std::wstring &filename, mode modes);
    ~CFile();
    bool open();
    long long length() const;
    bool seek(std::ios::off_type offset, std::ios::seekdir dir) const;
    bool read(char *dst, unsigned int length) const;
    bool readAt(char *dst, unsigned int length, unsigned int begin = 0) const;
    long long getPosition() const;
    bool write(const char *src, unsigned int length);
    bool close();

private:
    std::ifstream* m_in;
    std::ofstream* m_out;
    wchar_t*  m_filename;
    mode      m_modes;
};

#endif // CFILE_H_
