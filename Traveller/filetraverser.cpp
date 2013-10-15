
#include "stdafx.h"
#include "filetraverser.h"

CFileTraverser::CFileTraverser(const WTL::CString &folderPath, UINT mode)
    :m_folder(folderPath), m_mode(mode)
{
}

CFileTraverser::CFileTraverser(const wchar_t* folderPath, UINT mode)
    :m_folder(folderPath), m_mode(mode)
{
}

CFileTraverser::~CFileTraverser()
{
}

std::vector<WTL::CString> CFileTraverser::getFiles(const wchar_t* extension)
{
    std::vector<WTL::CString> vec;
    if (m_folder.GetLength() > 0)
        getFiles_(vec, m_folder, extension);
    return vec;
}

void CFileTraverser::getFiles_(std::vector<WTL::CString> &vec, const WTL::CString &folder, const wchar_t* extension)
{
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
            WTL::CString &find = folder + L"\\" + FindFileData.cFileName;
            if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
            {
                if (wcscmp(FindFileData.cFileName, L".") == 0 || wcscmp(FindFileData.cFileName, L"..") == 0)
                    continue;
                if ((m_mode & CFileTraverser::FOLDER) != 0)
                    vec.push_back(find);
                getFiles_(vec, find, extension);
            }
            else
                if ((m_mode & CFileTraverser::FILE) != 0)
                {
                    if (NULL == extension)
                        vec.push_back(find);
                    else
                    {
                        if (wcslen(FindFileData.cFileName) >= wcslen(extension))
                        {
                            if (0 == wcscmp(FindFileData.cFileName + (wcslen(FindFileData.cFileName) - wcslen(extension)), extension))
                                vec.push_back(find);
                        }
                    }
                }
        }
        FindClose(hFind);
    }
}
