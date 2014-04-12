// DlgProxy.h: header file
//

#pragma once

class CKameraDlg;


// CKameraDlgAutoProxy command target

class CKameraDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CKameraDlgAutoProxy)

	CKameraDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CKameraDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CKameraDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CKameraDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

