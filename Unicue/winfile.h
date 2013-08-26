/************************************************************************/
/*                                                                      */
/* A Simple File Obejct using win 32 api.                               */
/* It is simplified from CIOFile of win32-standard-expansion.           */
/* See http://code.google.com/p/win32-standard-expansion/ for detail.   */
/*                                                                      */
/* win32-standard-expansion                                             */
/* Publishedunder New BSD License.                                      */
/* Copyright memleak@darkc.at                                           */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
/* Published under Apache License 2.0                                   */
/* http://www.apache.org/licenses/LICENSE-2.0                           */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#pragma once

// file.h
#ifndef CWINFILE_H_
#define CWINFILE_H_

#include <WinBase.h>
#include <Windows.h>

class CWinFile
{
public:
    enum SeekPosition {
        begin   = FILE_BEGIN,
        current = FILE_CURRENT,
        end     = FILE_END
    };
    enum OpenFlags {
        modeRead       = (int)0x0001,
        modeWrite      = (int)0x0002,
        modeReadWrite  = (int)0x0004,
        shareDenyWrite = (int)0x0010,
        shareDenyRead  = (int)0x0020,
        shareDenyNone  = (int)0x0040,
        shareExclusive = (int)0x0080,
        modeCreate     = (int)0x0100,
        modeNoTruncate = (int)0x0200
    };
    CWinFile(const std::wstring &filename, UINT openFlags = modeRead | shareDenyWrite);
    CWinFile(LPCWSTR filename, UINT openFlags = modeRead | shareDenyWrite);
    ~CWinFile();
    BOOL open();
    UINT64 length();
    BOOL seek(INT64 offset, int position);
    DWORD read(char *dst, UINT length);
    UINT64 getPosition();
    DWORD write(const char *src, UINT length);
    BOOL flush();
    BOOL close();
    BOOL isEOF();

private:
    wchar_t*  m_filename;
    UINT      m_nFlags;
    HANDLE    m_hFile;
    void tryClose();
};

#endif // CWINFILE_H_
