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

#pragma once

#ifndef STRINGELEMENTTRAITX_H_
#define STRINGELEMENTTRAITX_H_

#include <windows.h>
#include <atlcoll.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>

class CStringElementTraitX : public CElementTraitsBase<WTL::CString>
{
public:
    typedef const WTL::CString& INARGTYPE;
    typedef WTL::CString& OUTARGTYPE;

    static ULONG Hash(_In_ INARGTYPE str)
    {
        ULONG nHash = 0;
        const wchar_t* pch = str;
        ATLENSURE( pch != NULL );
        while (*pch != 0)
        {
            nHash = (nHash<<5)+nHash+(*pch); // case sensitive
            pch++;
        }
        return nHash;
    };

    static bool CompareElements(_In_ INARGTYPE str1, _In_ INARGTYPE str2) throw()
    {
        return (wcscmp(str1, str2) == 0);
    };

    static int CompareElementsOrdered(_In_ INARGTYPE str1, _In_ INARGTYPE str2) throw()
    {
        return wcscmp(str1, str2);
    };
};

#endif // STRINGELEMENTTRAITX_H_
