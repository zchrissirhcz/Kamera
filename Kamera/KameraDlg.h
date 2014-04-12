// Detektor DCCDlg.h : header file
//

#pragma once

#include "resource.h"
#include "PGM.h"
#include "CameraDialog.h"
#include "ClickableStatic.h"
#include "SimpleGraphic.h"
#include "ImageProcessing.h"

class CKameraDlgAutoProxy;


// CDetektorDCCDlg dialog
class CKameraDlg : public CDialog
{
	DECLARE_DYNAMIC(CKameraDlg);
	friend class CKameraDlgAutoProxy;

// Construction
public:
	CKameraDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CKameraDlg();

	ClickableStatic image;

// Dialog Data
	enum { IDD = IDD_KAMERA_DIALOG };

	int iDevice, iResolution, ncamera, iW, iH;
    bool fCamera;

	CRITICAL_SECTION CS_rgbBuffer;

//	CVMR_Capture Capture;
	VideoCapture *capt;

	BITMAPINFO* bip;
	unsigned char *rgbImage, *rgbImageInv;
	
	int im_width, im_height;
	int mode;
	void PaintRGBImage();// Implementation

	HANDLE terminateAllEvent;

	ImageProcessing image_processing;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


	CKameraDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	void ZeroVariables();

	void ClearVariables();
	BOOL CanExit();
	void Initialize();
	void SetImageWindowSize(int w, int h);
	
	bool criticalsInitialized;
	DWORD dVideoThreadId;	
    HANDLE VideoThread;

	void PrepareBitmapInfo();

	bool InitCamera();
	CString sMyDir;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()

	bool PauseCamera();
	bool StartCamera();
	void SetMode(int mode);


public:
	afx_msg void OnVideooptionsChoosedevice();
	afx_msg void OnVideooptionsCameraproperties();
	afx_msg void OnModesMode1();
	afx_msg void OnModesMode2();
	afx_msg void OnModesMode3();
};
