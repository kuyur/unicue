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
