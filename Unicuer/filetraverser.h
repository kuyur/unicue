
#pragma once

// filetraverser.h
#ifndef FILETRAVERSER_H__
#define FILETRAVERSER_H__

#include <vector>
#include <Windows.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>

class CFileTraverser
{
public:
    enum Mode {
        FILE            = (UINT)0x0001,
        FOLDER          = (UINT)0x0002,
        FILE_AND_FOLDER = FILE | FOLDER
    };
    CFileTraverser(const wchar_t* folderPath, UINT mode);
    CFileTraverser(const WTL::CString &folderPath, UINT mode);
    ~CFileTraverser();
    std::vector<WTL::CString> getFiles(const wchar_t* extension = NULL);

private:
    WTL::CString m_folder;
    UINT         m_mode;
    void getFiles_(std::vector<WTL::CString> &vec, const WTL::CString &folder, const wchar_t* extension);
};

#endif // FILETRAVERSER_H__
