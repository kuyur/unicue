

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Tue Nov 18 18:41:25 2014
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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __TravellerExt_h__
#define __TravellerExt_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITravellerMenuExt_FWD_DEFINED__
#define __ITravellerMenuExt_FWD_DEFINED__
typedef interface ITravellerMenuExt ITravellerMenuExt;
#endif 	/* __ITravellerMenuExt_FWD_DEFINED__ */


#ifndef __ITravellerBgMenuExt_FWD_DEFINED__
#define __ITravellerBgMenuExt_FWD_DEFINED__
typedef interface ITravellerBgMenuExt ITravellerBgMenuExt;
#endif 	/* __ITravellerBgMenuExt_FWD_DEFINED__ */


#ifndef __TravellerMenuExt_FWD_DEFINED__
#define __TravellerMenuExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class TravellerMenuExt TravellerMenuExt;
#else
typedef struct TravellerMenuExt TravellerMenuExt;
#endif /* __cplusplus */

#endif 	/* __TravellerMenuExt_FWD_DEFINED__ */


#ifndef __TravellerBgMenuExt_FWD_DEFINED__
#define __TravellerBgMenuExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class TravellerBgMenuExt TravellerBgMenuExt;
#else
typedef struct TravellerBgMenuExt TravellerBgMenuExt;
#endif /* __cplusplus */

#endif 	/* __TravellerBgMenuExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __ITravellerMenuExt_INTERFACE_DEFINED__
#define __ITravellerMenuExt_INTERFACE_DEFINED__

/* interface ITravellerMenuExt */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ITravellerMenuExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("1D16BFEE-C651-47C0-A5AA-B610B63D2B23")
    ITravellerMenuExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ITravellerMenuExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITravellerMenuExt * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITravellerMenuExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITravellerMenuExt * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITravellerMenuExt * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITravellerMenuExt * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITravellerMenuExt * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITravellerMenuExt * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ITravellerMenuExtVtbl;

    interface ITravellerMenuExt
    {
        CONST_VTBL struct ITravellerMenuExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITravellerMenuExt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITravellerMenuExt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITravellerMenuExt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITravellerMenuExt_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITravellerMenuExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITravellerMenuExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITravellerMenuExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITravellerMenuExt_INTERFACE_DEFINED__ */


#ifndef __ITravellerBgMenuExt_INTERFACE_DEFINED__
#define __ITravellerBgMenuExt_INTERFACE_DEFINED__

/* interface ITravellerBgMenuExt */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ITravellerBgMenuExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EBBC8177-C739-4E36-A2DC-DAC4406AE236")
    ITravellerBgMenuExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ITravellerBgMenuExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITravellerBgMenuExt * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITravellerBgMenuExt * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITravellerBgMenuExt * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITravellerBgMenuExt * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITravellerBgMenuExt * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITravellerBgMenuExt * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITravellerBgMenuExt * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ITravellerBgMenuExtVtbl;

    interface ITravellerBgMenuExt
    {
        CONST_VTBL struct ITravellerBgMenuExtVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITravellerBgMenuExt_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITravellerBgMenuExt_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITravellerBgMenuExt_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITravellerBgMenuExt_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITravellerBgMenuExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITravellerBgMenuExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITravellerBgMenuExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ITravellerBgMenuExt_INTERFACE_DEFINED__ */



#ifndef __TravellerExtLib_LIBRARY_DEFINED__
#define __TravellerExtLib_LIBRARY_DEFINED__

/* library TravellerExtLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_TravellerExtLib;

EXTERN_C const CLSID CLSID_TravellerMenuExt;

#ifdef __cplusplus

class DECLSPEC_UUID("66276779-39DB-40A5-8F32-BB36A2B8F698")
TravellerMenuExt;
#endif

EXTERN_C const CLSID CLSID_TravellerBgMenuExt;

#ifdef __cplusplus

class DECLSPEC_UUID("086E6038-7710-4399-B85C-B89981E01D41")
TravellerBgMenuExt;
#endif
#endif /* __TravellerExtLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


