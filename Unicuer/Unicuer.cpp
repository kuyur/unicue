// Unicuer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "filetraverser.h"

int _tmain(int argc, _TCHAR* argv[])
{
    WTL::CString folder(L"");
    if (argc >= 2)
    {
        folder += argv[1];
    }
    if (folder.GetLength() > 0)
    {
        CFileTraverser t(folder, CFileTraverser::FILE);
        std::vector<WTL::CString> &files = t.getFiles(L".cue");
        std::vector<WTL::CString>::iterator iter;
        std::wcout.imbue(std::locale("jpn"));
        for (iter = files.begin(); iter != files.end(); iter++)
        {
            std::wcout<<(LPCWSTR)*iter<<std::endl;
        }
    }
	return 0;
}
