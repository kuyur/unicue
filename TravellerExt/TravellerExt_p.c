

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for TravellerExt.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#include "ndr64types.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "TravellerExt.h"

#define TYPE_FORMAT_STRING_SIZE   3                                 
#define PROC_FORMAT_STRING_SIZE   1                                 
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _TravellerExt_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } TravellerExt_MIDL_TYPE_FORMAT_STRING;

typedef struct _TravellerExt_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } TravellerExt_MIDL_PROC_FORMAT_STRING;

typedef struct _TravellerExt_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } TravellerExt_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

static const RPC_SYNTAX_IDENTIFIER  _NDR64_RpcTransferSyntax = 
{{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}};



extern const TravellerExt_MIDL_TYPE_FORMAT_STRING TravellerExt__MIDL_TypeFormatString;
extern const TravellerExt_MIDL_PROC_FORMAT_STRING TravellerExt__MIDL_ProcFormatString;
extern const TravellerExt_MIDL_EXPR_FORMAT_STRING TravellerExt__MIDL_ExprFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ITravellerMenuExt_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ITravellerMenuExt_ProxyInfo;


extern const MIDL_STUB_DESC Object_StubDesc;


extern const MIDL_SERVER_INFO ITravellerBgMenuExt_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO ITravellerBgMenuExt_ProxyInfo;



#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const TravellerExt_MIDL_PROC_FORMAT_STRING TravellerExt__MIDL_ProcFormatString =
    {
        0,
        {

			0x0
        }
    };

static const TravellerExt_MIDL_TYPE_FORMAT_STRING TravellerExt__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */

			0x0
        }
    };


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ITravellerMenuExt, ver. 0.0,
   GUID={0x1D16BFEE,0xC651,0x47C0,{0xA5,0xAA,0xB6,0x10,0xB6,0x3D,0x2B,0x23}} */

#pragma code_seg(".orpc")
static const unsigned short ITravellerMenuExt_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };



/* Object interface: ITravellerBgMenuExt, ver. 0.0,
   GUID={0xEBBC8177,0xC739,0x4E36,{0xA2,0xDC,0xDA,0xC4,0x40,0x6A,0xE2,0x36}} */

#pragma code_seg(".orpc")
static const unsigned short ITravellerBgMenuExt_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0
    };



#endif /* defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 12:14:07 2038
 */
/* Compiler settings for TravellerExt.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)




#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif


#include "ndr64types.h"
#include "pshpack8.h"


typedef 
NDR64_FORMAT_UINT32
__midl_frag1_t;
extern const __midl_frag1_t __midl_frag1;

static const __midl_frag1_t __midl_frag1 =
(NDR64_UINT32) 0 /* 0x0 */;


#include "poppack.h"



/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: ITravellerMenuExt, ver. 0.0,
   GUID={0x1D16BFEE,0xC651,0x47C0,{0xA5,0xAA,0xB6,0x10,0xB6,0x3D,0x2B,0x23}} */

#pragma code_seg(".orpc")
static const FormatInfoRef ITravellerMenuExt_Ndr64ProcTable[] =
    {
    (FormatInfoRef)(LONG_PTR) -1,
    (FormatInfoRef)(LONG_PTR) -1,
    (FormatInfoRef)(LONG_PTR) -1,
    (FormatInfoRef)(LONG_PTR) -1,
    0
    };


static const MIDL_SYNTAX_INFO ITravellerMenuExt_SyntaxInfo [  2 ] = 
    {
    {
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    TravellerExt__MIDL_ProcFormatString.Format,
    &ITravellerMenuExt_FormatStringOffsetTable[-3],
    TravellerExt__MIDL_TypeFormatString.Format,
    0,
    0,
    0
    }
    ,{
    {{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}},
    0,
    0 ,
    (unsigned short *) &ITravellerMenuExt_Ndr64ProcTable[-3],
    0,
    0,
    0,
    0
    }
    };

static const MIDL_STUBLESS_PROXY_INFO ITravellerMenuExt_ProxyInfo =
    {
    &Object_StubDesc,
    TravellerExt__MIDL_ProcFormatString.Format,
    &ITravellerMenuExt_FormatStringOffsetTable[-3],
    (RPC_SYNTAX_IDENTIFIER*)&_RpcTransferSyntax,
    2,
    (MIDL_SYNTAX_INFO*)ITravellerMenuExt_SyntaxInfo
    
    };


static const MIDL_SERVER_INFO ITravellerMenuExt_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    TravellerExt__MIDL_ProcFormatString.Format,
    (unsigned short *) &ITravellerMenuExt_FormatStringOffsetTable[-3],
    0,
    (RPC_SYNTAX_IDENTIFIER*)&_NDR64_RpcTransferSyntax,
    2,
    (MIDL_SYNTAX_INFO*)ITravellerMenuExt_SyntaxInfo
    };
CINTERFACE_PROXY_VTABLE(7) _ITravellerMenuExtProxyVtbl = 
{
    0,
    &IID_ITravellerMenuExt,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION ITravellerMenuExt_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _ITravellerMenuExtStubVtbl =
{
    &IID_ITravellerMenuExt,
    &ITravellerMenuExt_ServerInfo,
    7,
    &ITravellerMenuExt_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};


/* Object interface: ITravellerBgMenuExt, ver. 0.0,
   GUID={0xEBBC8177,0xC739,0x4E36,{0xA2,0xDC,0xDA,0xC4,0x40,0x6A,0xE2,0x36}} */

#pragma code_seg(".orpc")
static const FormatInfoRef ITravellerBgMenuExt_Ndr64ProcTable[] =
    {
    (FormatInfoRef)(LONG_PTR) -1,
    (FormatInfoRef)(LONG_PTR) -1,
    (FormatInfoRef)(LONG_PTR) -1,
    (FormatInfoRef)(LONG_PTR) -1,
    0
    };


static const MIDL_SYNTAX_INFO ITravellerBgMenuExt_SyntaxInfo [  2 ] = 
    {
    {
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    TravellerExt__MIDL_ProcFormatString.Format,
    &ITravellerBgMenuExt_FormatStringOffsetTable[-3],
    TravellerExt__MIDL_TypeFormatString.Format,
    0,
    0,
    0
    }
    ,{
    {{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}},
    0,
    0 ,
    (unsigned short *) &ITravellerBgMenuExt_Ndr64ProcTable[-3],
    0,
    0,
    0,
    0
    }
    };

static const MIDL_STUBLESS_PROXY_INFO ITravellerBgMenuExt_ProxyInfo =
    {
    &Object_StubDesc,
    TravellerExt__MIDL_ProcFormatString.Format,
    &ITravellerBgMenuExt_FormatStringOffsetTable[-3],
    (RPC_SYNTAX_IDENTIFIER*)&_RpcTransferSyntax,
    2,
    (MIDL_SYNTAX_INFO*)ITravellerBgMenuExt_SyntaxInfo
    
    };


static const MIDL_SERVER_INFO ITravellerBgMenuExt_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    TravellerExt__MIDL_ProcFormatString.Format,
    (unsigned short *) &ITravellerBgMenuExt_FormatStringOffsetTable[-3],
    0,
    (RPC_SYNTAX_IDENTIFIER*)&_NDR64_RpcTransferSyntax,
    2,
    (MIDL_SYNTAX_INFO*)ITravellerBgMenuExt_SyntaxInfo
    };
CINTERFACE_PROXY_VTABLE(7) _ITravellerBgMenuExtProxyVtbl = 
{
    0,
    &IID_ITravellerBgMenuExt,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */
};


static const PRPC_STUB_FUNCTION ITravellerBgMenuExt_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION
};

CInterfaceStubVtbl _ITravellerBgMenuExtStubVtbl =
{
    &IID_ITravellerBgMenuExt,
    &ITravellerBgMenuExt_ServerInfo,
    7,
    &ITravellerBgMenuExt_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    TravellerExt__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x60001, /* Ndr library version */
    0,
    0x801026e, /* MIDL Version 8.1.622 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x2000001, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };

const CInterfaceProxyVtbl * const _TravellerExt_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ITravellerBgMenuExtProxyVtbl,
    ( CInterfaceProxyVtbl *) &_ITravellerMenuExtProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _TravellerExt_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ITravellerBgMenuExtStubVtbl,
    ( CInterfaceStubVtbl *) &_ITravellerMenuExtStubVtbl,
    0
};

PCInterfaceName const _TravellerExt_InterfaceNamesList[] = 
{
    "ITravellerBgMenuExt",
    "ITravellerMenuExt",
    0
};

const IID *  const _TravellerExt_BaseIIDList[] = 
{
    &IID_IDispatch,
    &IID_IDispatch,
    0
};


#define _TravellerExt_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _TravellerExt, pIID, n)

int __stdcall _TravellerExt_IID_Lookup( const IID * pIID, int * pIndex )
{
    IID_BS_LOOKUP_SETUP

    IID_BS_LOOKUP_INITIAL_TEST( _TravellerExt, 2, 1 )
    IID_BS_LOOKUP_RETURN_RESULT( _TravellerExt, 2, *pIndex )
    
}

const ExtendedProxyFileInfo TravellerExt_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _TravellerExt_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _TravellerExt_StubVtblList,
    (const PCInterfaceName * ) & _TravellerExt_InterfaceNamesList,
    (const IID ** ) & _TravellerExt_BaseIIDList,
    & _TravellerExt_IID_Lookup, 
    2,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

