#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include "config.h"
#include "game.h"
#include "client_card.h"
#include <stdio.h>
#include <urlmon.h>
#pragma comment (lib, "urlmon.lib")

class CBindCallback : public IBindStatusCallback  
{
public:
CBindCallback();
virtual ~CBindCallback();

     CUrlDownloadToFileCallbackTestDlg* m_pdlg; 
    
STDMETHOD(OnStartBinding)
( DWORD dwReserved,
IBinding __RPC_FAR *pib)
{ return E_NOTIMPL; }

STDMETHOD(GetPriority)
( LONG __RPC_FAR *pnPriority)
{ return E_NOTIMPL; }

STDMETHOD(OnLowResource)
( DWORD reserved)
{ return E_NOTIMPL; }

STDMETHOD(OnProgress)
( ULONG ulProgress,    
ULONG ulProgressMax,
ULONG ulStatusCode,
LPCWSTR wszStatusText);

STDMETHOD(OnStopBinding)
( HRESULT hresult,
LPCWSTR szError)
{ return E_NOTIMPL; }

STDMETHOD(GetBindInfo)
( DWORD __RPC_FAR *grfBINDF,
BINDINFO __RPC_FAR *pbindinfo)
{ return E_NOTIMPL; }

STDMETHOD(OnDataAvailable)
( DWORD grfBSCF,
DWORD dwSize,
FORMATETC __RPC_FAR *pformatetc,
STGMEDIUM __RPC_FAR *pstgmed)
{ return E_NOTIMPL; }

STDMETHOD(OnObjectAvailable)
( REFIID riid,
IUnknown __RPC_FAR *punk)
{ return E_NOTIMPL; }

STDMETHOD_(ULONG,AddRef)()
{ return 0; }

STDMETHOD_(ULONG,Release)()
{ return 0; }

STDMETHOD(QueryInterface)
( REFIID riid,
void __RPC_FAR *__RPC_FAR *ppvObject)
{ return E_NOTIMPL; }
};

CBindCallback::CBindCallback()
{

}

CBindCallback::~CBindCallback()
{

}

LRESULT CBindCallback::OnProgress(ULONG ulProgress,
ULONG ulProgressMax,
ULONG ulSatusCode,
LPCWSTR szStatusText)
{
CProgressCtrl* m_prg = (CProgressCtrl*)m_pdlg->GetDlgItem(IDC_PROGRESS);
m_prg->SetRange32(0,ulProgressMax);
m_prg->SetPos(ulProgress);

CString szText;
szText.Format("已下载%d%%", (int)(ulProgress * 100.0 / ulProgressMax));
(m_pdlg->GetDlgItem(IDC_STATUS))->SetWindowText(szText);

return S_OK;
}

void CUrlDownloadToFileCallbackTestDlg::DownloadThread()
{
CBindCallback cbc;
cbc.m_pdlg = this;
CString szURL;
m_url.GetWindowText(szURL);
m_url.EnableWindow(FALSE);
(this->GetDlgItem(IDC_START))->EnableWindow(FALSE);
CString szRand;
szRand.Format(_T("?skq=%d"),GetTickCount());
szUrl += szRand;
if(S_OK == URLDownloadToFile(NULL,szURL,szPath,0,&cbc))
MessageBox("finished");
}

#endif //EVENT_HANDLER_H
