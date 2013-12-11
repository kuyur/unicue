/************************************************************************/
/*                                                                      */
/* Traveller 1.2                                                        */
/* A batch tool to convert files from ansi code-page to Unicode.        */
/* It is a part of Unicue Project.                                      */
/*                                                                      */
/* Author:  kuyur (kuyur@kuyur.info)                                    */
/* Published under GPLv3                                                */
/* http://www.gnu.org/licenses/gpl-3.0.en.html                          */
/*                                                                      */
/* Project URL: http://github.com/kuyur/unicue                          */
/*                                                                      */
/************************************************************************/

#include "stdafx.h"
#include "filetraverser.h"
#include "..\common\utils.h"

CFileTraverser::CFileTraverser(const WTL::CString &folderPath, UINT mode)
    :m_folder(folderPath), m_mode(mode), m_ignoredFolderName(L"")
{
}

CFileTraverser::CFileTraverser(const wchar_t* folderPath, UINT mode)
    :m_folder(folderPath), m_mode(mode), m_extensions(), m_isIgnoreHidden(FALSE)
{
}

CFileTraverser::~CFileTraverser()
{
}

void CFileTraverser::addFilter(const wchar_t* extensions)
{
    if (extensions && wcslen(extensions) != 0)
    {
        const wchar_t *last = extensions + wcslen(extensions);
        const wchar_t *begin = extensions;
        const wchar_t *end = wcschr(begin, L'|');
        do
        {
            if (end)
            {
                int len = end - begin;
                if (len > 0)
                {
                    wchar_t *ext = new wchar_t[len + 1];
                    wmemcpy(ext, begin, len);
                    ext[len] = L'\0';
                    m_extensions[trim(std::wstring(ext))] = true;
                    delete []ext;
                }
                // next
                begin = end + 1;
                end = wcschr(begin, L'|');
            }
            else
            {
                m_extensions[trim(std::wstring(begin))] = true;
                begin = last;
                end = NULL;
            }
        } while(begin != last);
    }
}

void CFileTraverser::removeFilter(const wchar_t* extensions)
{
    if (extensions && wcslen(extensions) != 0)
    {
        const wchar_t *last = extensions + wcslen(extensions);
        const wchar_t *begin = extensions;
        const wchar_t *end = wcschr(begin, L'|');
        do
        {
            if (end)
            {
                int len = end - begin;
                if (len > 0)
                {
                    wchar_t *ext = new wchar_t[len + 1];
                    wmemcpy(ext, begin, len);
                    ext[len] = L'\0';
                    m_extensions.erase(trim(std::wstring(ext)));
                    delete []ext;
                }
                // next
                begin = end + 1;
                end = wcschr(begin, L'|');
            }
            else
            {
                m_extensions.erase(trim(std::wstring(begin)));
                begin = last;
                end = NULL;
            }
        } while(begin != last);
    }
}

void CFileTraverser::setIgnoreHidden(BOOL isIgnoreHidden)
{
    m_isIgnoreHidden = isIgnoreHidden;
}

void CFileTraverser::setIgnoredFolderName(const wchar_t *folderName)
{
    m_ignoredFolderName = folderName;
}

std::vector<WTL::CString> CFileTraverser::getFiles()
{
    if (m_isIgnoreHidden && (GetFileAttributes(m_folder) & FILE_ATTRIBUTE_HIDDEN))
        return std::vector<WTL::CString>();

    std::vector<WTL::CString> vec;
    if (m_folder.GetLength() > 0)
        getFiles_(vec, m_folder);
    return vec;
}

void CFileTraverser::getFiles_(std::vector<WTL::CString> &vec, const WTL::CString &folder)
{
    if ((m_ignoredFolderName.GetLength() > 0) && (folder.Right(m_ignoredFolderName.GetLength()) == m_ignoredFolderName))
        return;

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    WTL::CString spec(folder);
    spec += L"\\*";

    hFind = FindFirstFile(spec, &FindFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        FindClose(hFind);
        return;
    }
    else
    {
        while (FindNextFile(hFind, &FindFileData))
        {
            if (m_isIgnoreHidden && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) continue;
            WTL::CString &find = folder + L"\\" + FindFileData.cFileName;
            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
            {
                if (wcscmp(FindFileData.cFileName, L".") == 0 || wcscmp(FindFileData.cFileName, L"..") == 0)
                    continue;
                if ((m_mode & CFileTraverser::FOLDER) != 0)
                    vec.push_back(find);
                getFiles_(vec, find);
            }
            else
                if ((m_mode & CFileTraverser::FILE) != 0)
                {
                    if (m_extensions.size() <= 0)
                        vec.push_back(find);
                    else
                    {
                        wchar_t *ext = wcsrchr(FindFileData.cFileName, L'.');
                        if (ext && *(ext+1))
                        {
                            if (m_extensions.find(std::wstring(ext+1)) != m_extensions.end())
                            {
                                vec.push_back(find);
                            }
                        }
                    }
                }
        }
        FindClose(hFind);
    }
}
