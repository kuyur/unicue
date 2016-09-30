/************************************************************************/
/*                                                                      */
/* Win32 Common Modules Library                                         */
/*                                                                      */
/* A Simple File Obejct using win 32 api.                               */
/* It is simplified from CIOFile of win32-standard-expansion.           */
/* See http://code.google.com/p/win32-standard-expansion/ for detail.   */
/*                                                                      */
/* win32-standard-expansion                                             */
/* Published under New BSD License.                                     */
/* Copyright memleak@darkc.at                                           */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
/* Published under LGPL-3.0                                             */
/* https://www.gnu.org/licenses/lgpl-3.0.en.html                        */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#pragma once

// file.h
#ifndef WINFILE_H_
#define WINFILE_H_

#include <string>
#include <windows.h>

class CWinFile
{
public:
    static bool CopyFile(LPCWSTR srcfile, LPCWSTR destfile);
    static bool CopyFile(const std::wstring &srcfile, const std::wstring &destfile);

public:
    enum SeekPosition {
        begin   = FILE_BEGIN,
        current = FILE_CURRENT,
        end     = FILE_END
    };
    enum OpenFlags {
        modeRead         = (int)0x0001,
        modeWrite        = (int)0x0002,
        modeReadWrite    = (int)0x0004,
        shareDenyWrite   = (int)0x0010,
        shareDenyRead    = (int)0x0020,
        shareDenyNone    = (int)0x0040,
        shareExclusive   = (int)0x0080,
        openOnly         = (int)0x0100,  // open existing file only (if file not existing, open() will return FALSE)
        openCreateAlways = (int)0x0200,  // always create new file, all old content will be deleted
        openAppend       = (int)0x0400   // only create new file when not existing (for appending)
    };
    CWinFile(const std::wstring &filename, UINT openFlags = modeRead | shareDenyWrite | openOnly);
    CWinFile(LPCWSTR filename, UINT openFlags = modeRead | shareDenyWrite | openOnly);
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

#endif // WINFILE_H_
