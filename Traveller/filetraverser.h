
#pragma once

// filetraverser.h
#ifndef FILETRAVERSER_H__
#define FILETRAVERSER_H__

#include <vector>
#include <map>
#include <windows.h>
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
    void addFilter(const wchar_t* extension);
    void removeFilter(const wchar_t* extension);
    void setIgnoreHidden(BOOL isIgnoreHidden);
    void setIgnoredFolderName(const wchar_t *folderName);
    std::vector<WTL::CString> getFiles();

private:
    WTL::CString m_folder;
    WTL::CString m_ignoredFolderName;
    UINT         m_mode;
    BOOL         m_isIgnoreHidden;
    std::map<std::wstring, bool> m_extensions;
    void getFiles_(std::vector<WTL::CString> &vec, const WTL::CString &folder);
};

#endif // FILETRAVERSER_H__
