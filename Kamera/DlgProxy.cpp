// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "Kamera.h"
#include "DlgProxy.h"
#include "KameraDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKameraDlgAutoProxy

IMPLEMENT_DYNCREATE(CKameraDlgAutoProxy, CCmdTarget)

CKameraDlgAutoProxy::CKameraDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CKameraDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CKameraDlg)))
		{
			m_pDialog = reinterpret_cast<CKameraDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CKameraDlgAutoProxy::~CKameraDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CKameraDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CKameraDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CKameraDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_IDetektorDCC to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {7B060F60-2EC4-4668-9BF3-9539D31B6E7C}
static const IID IID_IDetektorDCC =
{ 0x7B060F60, 0x2EC4, 0x4668, { 0x9B, 0xF3, 0x95, 0x39, 0xD3, 0x1B, 0x6E, 0x7C } };

BEGIN_INTERFACE_MAP(CKameraDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CKameraDlgAutoProxy, IID_IDetektorDCC, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {7A5A4DA4-F051-4981-B99D-CC06B3FFEFDE}
IMPLEMENT_OLECREATE2(CKameraDlgAutoProxy, "DetektorDCC.Application", 0x7a5a4da4, 0xf051, 0x4981, 0xb9, 0x9d, 0xcc, 0x6, 0xb3, 0xff, 0xef, 0xde)


// CKameraDlgAutoProxy message handlers
