/*** Autogenerated by WIDL 7.7 from include/windows.media.idl - Do not edit ***/

#ifdef _WIN32
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif
#include <rpc.h>
#include <rpcndr.h>
#endif

#ifndef COM_NO_WINDOWS_H
#include <windows.h>
#include <ole2.h>
#endif

#ifndef __windows_media_h__
#define __windows_media_h__

/* Forward declarations */

#ifndef ____x_ABI_CWindows_CMedia_CIMediaMarker_FWD_DEFINED__
#define ____x_ABI_CWindows_CMedia_CIMediaMarker_FWD_DEFINED__
typedef interface __x_ABI_CWindows_CMedia_CIMediaMarker __x_ABI_CWindows_CMedia_CIMediaMarker;
#ifdef __cplusplus
#define __x_ABI_CWindows_CMedia_CIMediaMarker ABI::Windows::Media::IMediaMarker
namespace ABI {
    namespace Windows {
        namespace Media {
            interface IMediaMarker;
        }
    }
}
#endif /* __cplusplus */
#endif

/* Headers for imported files */

#include <inspectable.h>
#include <windows.foundation.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ____x_ABI_CWindows_CMedia_CIMediaControl_FWD_DEFINED__
#define ____x_ABI_CWindows_CMedia_CIMediaControl_FWD_DEFINED__
typedef interface __x_ABI_CWindows_CMedia_CIMediaControl __x_ABI_CWindows_CMedia_CIMediaControl;
#ifdef __cplusplus
#define __x_ABI_CWindows_CMedia_CIMediaControl ABI::Windows::Media::IMediaControl
namespace ABI {
    namespace Windows {
        namespace Media {
            interface IMediaControl;
        }
    }
}
#endif /* __cplusplus */
#endif

/*****************************************************************************
 * IMediaMarker interface
 */
#if WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION >= 0x10000
#ifndef ____x_ABI_CWindows_CMedia_CIMediaMarker_INTERFACE_DEFINED__
#define ____x_ABI_CWindows_CMedia_CIMediaMarker_INTERFACE_DEFINED__

DEFINE_GUID(IID___x_ABI_CWindows_CMedia_CIMediaMarker, 0x1803def8, 0xdca5, 0x4b6f, 0x9c,0x20, 0xe3,0xd3,0xc0,0x64,0x36,0x25);
#if defined(__cplusplus) && !defined(CINTERFACE)
} /* extern "C" */
namespace ABI {
    namespace Windows {
        namespace Media {
            MIDL_INTERFACE("1803def8-dca5-4b6f-9c20-e3d3c0643625")
            IMediaMarker : public IInspectable
            {
                virtual HRESULT STDMETHODCALLTYPE get_Time(
                    struct TimeSpan *value) = 0;

                virtual HRESULT STDMETHODCALLTYPE get_MediaMarkerType(
                    HSTRING *value) = 0;

                virtual HRESULT STDMETHODCALLTYPE get_Text(
                    HSTRING *value) = 0;

            };
        }
    }
}
extern "C" {
#ifdef __CRT_UUID_DECL
__CRT_UUID_DECL(__x_ABI_CWindows_CMedia_CIMediaMarker, 0x1803def8, 0xdca5, 0x4b6f, 0x9c,0x20, 0xe3,0xd3,0xc0,0x64,0x36,0x25)
#endif
#else
typedef struct __x_ABI_CWindows_CMedia_CIMediaMarkerVtbl {
    BEGIN_INTERFACE

    /*** IUnknown methods ***/
    HRESULT (STDMETHODCALLTYPE *QueryInterface)(
        __x_ABI_CWindows_CMedia_CIMediaMarker *This,
        REFIID riid,
        void **ppvObject);

    ULONG (STDMETHODCALLTYPE *AddRef)(
        __x_ABI_CWindows_CMedia_CIMediaMarker *This);

    ULONG (STDMETHODCALLTYPE *Release)(
        __x_ABI_CWindows_CMedia_CIMediaMarker *This);

    /*** IInspectable methods ***/
    HRESULT (STDMETHODCALLTYPE *GetIids)(
        __x_ABI_CWindows_CMedia_CIMediaMarker *This,
        ULONG *iidCount,
        IID **iids);

    HRESULT (STDMETHODCALLTYPE *GetRuntimeClassName)(
        __x_ABI_CWindows_CMedia_CIMediaMarker *This,
        HSTRING *className);

    HRESULT (STDMETHODCALLTYPE *GetTrustLevel)(
        __x_ABI_CWindows_CMedia_CIMediaMarker *This,
        TrustLevel *trustLevel);

    /*** IMediaMarker methods ***/
    HRESULT (STDMETHODCALLTYPE *get_Time)(
        __x_ABI_CWindows_CMedia_CIMediaMarker *This,
        struct __x_ABI_CWindows_CFoundation_CTimeSpan *value);

    HRESULT (STDMETHODCALLTYPE *get_MediaMarkerType)(
        __x_ABI_CWindows_CMedia_CIMediaMarker *This,
        HSTRING *value);

    HRESULT (STDMETHODCALLTYPE *get_Text)(
        __x_ABI_CWindows_CMedia_CIMediaMarker *This,
        HSTRING *value);

    END_INTERFACE
} __x_ABI_CWindows_CMedia_CIMediaMarkerVtbl;

interface __x_ABI_CWindows_CMedia_CIMediaMarker {
    CONST_VTBL __x_ABI_CWindows_CMedia_CIMediaMarkerVtbl* lpVtbl;
};

#ifdef COBJMACROS
#ifndef WIDL_C_INLINE_WRAPPERS
/*** IUnknown methods ***/
#define __x_ABI_CWindows_CMedia_CIMediaMarker_QueryInterface(This,riid,ppvObject) (This)->lpVtbl->QueryInterface(This,riid,ppvObject)
#define __x_ABI_CWindows_CMedia_CIMediaMarker_AddRef(This) (This)->lpVtbl->AddRef(This)
#define __x_ABI_CWindows_CMedia_CIMediaMarker_Release(This) (This)->lpVtbl->Release(This)
/*** IInspectable methods ***/
#define __x_ABI_CWindows_CMedia_CIMediaMarker_GetIids(This,iidCount,iids) (This)->lpVtbl->GetIids(This,iidCount,iids)
#define __x_ABI_CWindows_CMedia_CIMediaMarker_GetRuntimeClassName(This,className) (This)->lpVtbl->GetRuntimeClassName(This,className)
#define __x_ABI_CWindows_CMedia_CIMediaMarker_GetTrustLevel(This,trustLevel) (This)->lpVtbl->GetTrustLevel(This,trustLevel)
/*** IMediaMarker methods ***/
#define __x_ABI_CWindows_CMedia_CIMediaMarker_get_Time(This,value) (This)->lpVtbl->get_Time(This,value)
#define __x_ABI_CWindows_CMedia_CIMediaMarker_get_MediaMarkerType(This,value) (This)->lpVtbl->get_MediaMarkerType(This,value)
#define __x_ABI_CWindows_CMedia_CIMediaMarker_get_Text(This,value) (This)->lpVtbl->get_Text(This,value)
#else
/*** IUnknown methods ***/
static FORCEINLINE HRESULT __x_ABI_CWindows_CMedia_CIMediaMarker_QueryInterface(__x_ABI_CWindows_CMedia_CIMediaMarker* This,REFIID riid,void **ppvObject) {
    return This->lpVtbl->QueryInterface(This,riid,ppvObject);
}
static FORCEINLINE ULONG __x_ABI_CWindows_CMedia_CIMediaMarker_AddRef(__x_ABI_CWindows_CMedia_CIMediaMarker* This) {
    return This->lpVtbl->AddRef(This);
}
static FORCEINLINE ULONG __x_ABI_CWindows_CMedia_CIMediaMarker_Release(__x_ABI_CWindows_CMedia_CIMediaMarker* This) {
    return This->lpVtbl->Release(This);
}
/*** IInspectable methods ***/
static FORCEINLINE HRESULT __x_ABI_CWindows_CMedia_CIMediaMarker_GetIids(__x_ABI_CWindows_CMedia_CIMediaMarker* This,ULONG *iidCount,IID **iids) {
    return This->lpVtbl->GetIids(This,iidCount,iids);
}
static FORCEINLINE HRESULT __x_ABI_CWindows_CMedia_CIMediaMarker_GetRuntimeClassName(__x_ABI_CWindows_CMedia_CIMediaMarker* This,HSTRING *className) {
    return This->lpVtbl->GetRuntimeClassName(This,className);
}
static FORCEINLINE HRESULT __x_ABI_CWindows_CMedia_CIMediaMarker_GetTrustLevel(__x_ABI_CWindows_CMedia_CIMediaMarker* This,TrustLevel *trustLevel) {
    return This->lpVtbl->GetTrustLevel(This,trustLevel);
}
/*** IMediaMarker methods ***/
static FORCEINLINE HRESULT __x_ABI_CWindows_CMedia_CIMediaMarker_get_Time(__x_ABI_CWindows_CMedia_CIMediaMarker* This,struct __x_ABI_CWindows_CFoundation_CTimeSpan *value) {
    return This->lpVtbl->get_Time(This,value);
}
static FORCEINLINE HRESULT __x_ABI_CWindows_CMedia_CIMediaMarker_get_MediaMarkerType(__x_ABI_CWindows_CMedia_CIMediaMarker* This,HSTRING *value) {
    return This->lpVtbl->get_MediaMarkerType(This,value);
}
static FORCEINLINE HRESULT __x_ABI_CWindows_CMedia_CIMediaMarker_get_Text(__x_ABI_CWindows_CMedia_CIMediaMarker* This,HSTRING *value) {
    return This->lpVtbl->get_Text(This,value);
}
#endif
#ifdef WIDL_using_Windows_Media
#define IID_IMediaMarker IID___x_ABI_CWindows_CMedia_CIMediaMarker
#define IMediaMarkerVtbl __x_ABI_CWindows_CMedia_CIMediaMarkerVtbl
#define IMediaMarker __x_ABI_CWindows_CMedia_CIMediaMarker
#define IMediaMarker_QueryInterface __x_ABI_CWindows_CMedia_CIMediaMarker_QueryInterface
#define IMediaMarker_AddRef __x_ABI_CWindows_CMedia_CIMediaMarker_AddRef
#define IMediaMarker_Release __x_ABI_CWindows_CMedia_CIMediaMarker_Release
#define IMediaMarker_GetIids __x_ABI_CWindows_CMedia_CIMediaMarker_GetIids
#define IMediaMarker_GetRuntimeClassName __x_ABI_CWindows_CMedia_CIMediaMarker_GetRuntimeClassName
#define IMediaMarker_GetTrustLevel __x_ABI_CWindows_CMedia_CIMediaMarker_GetTrustLevel
#define IMediaMarker_get_Time __x_ABI_CWindows_CMedia_CIMediaMarker_get_Time
#define IMediaMarker_get_MediaMarkerType __x_ABI_CWindows_CMedia_CIMediaMarker_get_MediaMarkerType
#define IMediaMarker_get_Text __x_ABI_CWindows_CMedia_CIMediaMarker_get_Text
#endif /* WIDL_using_Windows_Media */
#endif

#endif

#endif  /* ____x_ABI_CWindows_CMedia_CIMediaMarker_INTERFACE_DEFINED__ */
#endif /* WINDOWS_FOUNDATION_UNIVERSALAPICONTRACT_VERSION >= 0x10000 */

/* Begin additional prototypes for all interfaces */

ULONG           __RPC_USER HSTRING_UserSize     (ULONG *, ULONG, HSTRING *);
unsigned char * __RPC_USER HSTRING_UserMarshal  (ULONG *, unsigned char *, HSTRING *);
unsigned char * __RPC_USER HSTRING_UserUnmarshal(ULONG *, unsigned char *, HSTRING *);
void            __RPC_USER HSTRING_UserFree     (ULONG *, HSTRING *);

/* End additional prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __windows_media_h__ */
