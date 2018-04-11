#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "config.h"
#include "game.h"
#include "client_card.h"
#include <stdio.h>
#include <urlmon.h>
#pragma comment (lib, "urlmon.lib")

HRESULT ( STDMETHODCALLTYPE QueryInterface )( 
    IBindStatusCallback * This,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ void **ppvObject)
{
    return E_NOTIMPL;
}

ULONG ( STDMETHODCALLTYPE AddRef )( 
    IBindStatusCallback * This)
{
    return E_NOTIMPL;
}

ULONG ( STDMETHODCALLTYPE Release )( 
    IBindStatusCallback * This)
{
    return E_NOTIMPL;
}

HRESULT ( STDMETHODCALLTYPE OnStartBinding )( 
    IBindStatusCallback * This,
    /* [in] */ DWORD dwReserved,
    /* [in] */ IBinding *pib)
{
    return E_NOTIMPL;
}

HRESULT ( STDMETHODCALLTYPE GetPriority )( 
    IBindStatusCallback * This,
    /* [out] */ LONG *pnPriority)
{
    return E_NOTIMPL;
}

HRESULT ( STDMETHODCALLTYPE OnLowResource )( 
    IBindStatusCallback * This,
    /* [in] */ DWORD reserved)
{
    return E_NOTIMPL;
}

HRESULT ( STDMETHODCALLTYPE OnProgress )( 
    IBindStatusCallback * This,
    /* [in] */ ULONG ulProgress,
    /* [in] */ ULONG ulProgressMax,
    /* [in] */ ULONG ulStatusCode,
    /* [in] */ LPCWSTR szStatusText)
{
    if (ulProgress > 0)
    {   
        printf("Downloaded %d of %d bytes\n",
            ulProgress, ulProgressMax); 
        fflush(stdout);
    }
    return S_OK;
}

HRESULT ( STDMETHODCALLTYPE OnStopBinding )( 
    IBindStatusCallback * This,
    /* [in] */ HRESULT hresult,
    /* [unique][in] */ LPCWSTR szError)
{
    return E_NOTIMPL;
}

HRESULT ( STDMETHODCALLTYPE GetBindInfo )( 
    IBindStatusCallback * This,
    /* [out] */ DWORD *grfBINDF,
    /* [unique][out][in] */ BINDINFO *pbindinfo)
{
    return E_NOTIMPL;
}

HRESULT ( STDMETHODCALLTYPE OnDataAvailable )( 
    IBindStatusCallback * This,
    /* [in] */ DWORD grfBSCF,
    /* [in] */ DWORD dwSize,
    /* [in] */ FORMATETC *pformatetc,
    /* [in] */ STGMEDIUM *pstgmed)
{
    return E_NOTIMPL;
}

HRESULT ( STDMETHODCALLTYPE OnObjectAvailable )( 
    IBindStatusCallback * This,
    /* [in] */ REFIID riid,
    /* [iid_is][in] */ IUnknown *punk)
{
    return E_NOTIMPL;
}

static const IBindStatusCallbackVtbl* IBindStatusCallback_Vtbl = {
    QueryInterface,
    AddRef,
    Release,
    OnStartBinding,
    GetPriority,
    OnLowResource,
    OnProgress,
    OnStopBinding,
    GetBindInfo,
    OnDataAvailable,
    OnObjectAvailable
};

#endif //EVENT_HANDLER_H
