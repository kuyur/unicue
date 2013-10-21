

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Oct 21 18:31:50 2013
 */
/* Compiler settings for TravellerExt.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_ITravellerMenuExt,0x1D16BFEE,0xC651,0x47C0,0xA5,0xAA,0xB6,0x10,0xB6,0x3D,0x2B,0x23);


MIDL_DEFINE_GUID(IID, IID_ITravellerBgMenuExt,0xEBBC8177,0xC739,0x4E36,0xA2,0xDC,0xDA,0xC4,0x40,0x6A,0xE2,0x36);


MIDL_DEFINE_GUID(IID, LIBID_TravellerExtLib,0x86369FD7,0x46EA,0x45D3,0xA9,0x72,0x98,0x1B,0x0B,0x52,0xA8,0xAC);


MIDL_DEFINE_GUID(CLSID, CLSID_TravellerMenuExt,0x66276779,0x39DB,0x40A5,0x8F,0x32,0xBB,0x36,0xA2,0xB8,0xF6,0x98);


MIDL_DEFINE_GUID(CLSID, CLSID_TravellerBgMenuExt,0x086E6038,0x7710,0x4399,0xB8,0x5C,0xB8,0x99,0x81,0xE0,0x1D,0x41);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



