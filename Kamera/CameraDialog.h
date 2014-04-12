#pragma once
#include "afxwin.h"
//#include "VMR_Capture.h"
#include "VideoCapture.h"

// CameraDialog dialog

class CameraDialog : public CDialog
{
	DECLARE_DYNAMIC(CameraDialog)

public:
	CameraDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CameraDialog();

	int iSelectedDevice, iSelectedResolution;

	int iSelectedResX, iSelectedResY;
//	int iResX[1024], iResY[1024];

// Dialog Data
	enum { IDD = IDD_CAMERADIALOG };
	
	BOOL EnumResolutions();
	

	VideoCapture capt;
//	CVMR_Capture *Capture;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_devices;
	CComboBox m_resolutions;
	bool fOK;
	afx_msg void OnCbnSelchangeComboCamera();
	afx_msg void OnCbnSelchangeComboResolution();
	afx_msg void OnClose();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnBnClickedButtonOk();
};
