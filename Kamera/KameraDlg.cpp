// Detektor DCCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Kamera.h"
#include "KameraDlg.h"
#include "DlgProxy.h"
#include "PracticalSocket.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std; 

IMPLEMENT_DYNAMIC(CKameraDlg, CDialog);

DWORD WINAPI VideoThreadFunction(void* _dlg)
{
	CKameraDlg *dlg = (CKameraDlg*) _dlg;

	int w;
	int h;

	ClickableStatic *image = &dlg->image;

	while (1)
	{
		w = dlg->im_width;
		h = dlg->im_height;
		
		if (image && !image->last_rect_serviced)
		{
			image->last_rect_serviced = true;
			EnterCriticalSection(&(dlg->CS_rgbBuffer));
			dlg->image_processing.ServiceDrawnRectangle(image->last_rect, dlg->rgbImage, w, h);
			LeaveCriticalSection(&(dlg->CS_rgbBuffer));
		}

		if (WaitForSingleObject(dlg->terminateAllEvent, 0)==WAIT_OBJECT_0)
		{
			// do all the thread cleaning if needed and quit thread
			return 0;
		}

		if (dlg->capt->callback.fSampleReady)
		{
			EnterCriticalSection(&(dlg->CS_rgbBuffer));
			memcpy(dlg->rgbImage, dlg->capt->callback.cbInfo.pBuffer, w*h*3);
			dlg->image_processing.ProcessRGBImage(dlg->mode, dlg->rgbImage, w, h);
			LeaveCriticalSection(&(dlg->CS_rgbBuffer));

			dlg->PaintRGBImage();

			dlg->capt->callback.fSampleReady = false;
		} else
		{
			Sleep(1);
		}
	}

	// should never get here...
	return -1;
}

CKameraDlg::CKameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKameraDlg::IDD, pParent)
{
	mode = 0;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;
}


void CKameraDlg::ClearVariables()
{
	DWORD ret_val;

	if (terminateAllEvent!=NULL)
		SetEvent(terminateAllEvent);
	
	if (VideoThread!=NULL)
	{
		while (GetExitCodeThread(VideoThread, &ret_val)) 
			if (ret_val!=STILL_ACTIVE) break;
		CloseHandle(VideoThread);
		VideoThread = NULL;
	}

	DestroyObject(&capt);
	DestroyTable(&rgbImage);
	DestroyTable(&rgbImageInv);

	DestroyObject(&bip);

	if (criticalsInitialized)
		DeleteCriticalSection(&CS_rgbBuffer);
	
	criticalsInitialized = false;

	if (terminateAllEvent!=NULL) 
	{ 
			CloseHandle(terminateAllEvent); 
			terminateAllEvent = NULL;
	}
}

CKameraDlg::~CKameraDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;
	
}

void CKameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKameraDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_VIDEOOPTIONS_CHOOSEDEVICE, &CKameraDlg::OnVideooptionsChoosedevice)
	ON_COMMAND(ID_VIDEOOPTIONS_CAMERAPROPERTIES, &CKameraDlg::OnVideooptionsCameraproperties)
	ON_WM_NCMOUSEMOVE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_COMMAND(ID_MODES_MODE1, &CKameraDlg::OnModesMode1)
	ON_COMMAND(ID_MODES_MODE2, &CKameraDlg::OnModesMode2)
	ON_COMMAND(ID_MODES_MODE3, &CKameraDlg::OnModesMode3)
END_MESSAGE_MAP()

void CKameraDlg::SetImageWindowSize(int w, int h)
{
    im_width  = w;
    im_height = h;

	SetWindowPos(&wndTop, 0, 0, w+8, h+25,
		SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOOWNERZORDER);
	image.SetWindowPos(&wndTop, 0, 0, w+8, h+25,
		SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOOWNERZORDER);
}

void CKameraDlg::PrepareBitmapInfo()
{
    bip = (BITMAPINFO*) new unsigned char[sizeof(BITMAPINFOHEADER)];
    bip->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bip->bmiHeader.biWidth = im_width;
    bip->bmiHeader.biHeight = im_height;
    bip->bmiHeader.biPlanes = 1;
    bip->bmiHeader.biBitCount = 24;
    bip->bmiHeader.biCompression = BI_RGB;
    bip->bmiHeader.biSizeImage = 0;
    bip->bmiHeader.biXPelsPerMeter = 0;
    bip->bmiHeader.biYPelsPerMeter = 0;
    bip->bmiHeader.biClrUsed = 0;
    bip->bmiHeader.biClrImportant = 0;
}

void CKameraDlg::ZeroVariables()
{
	im_width = -1;
	im_height = -1;
	bip = NULL;
	rgbImage = NULL;
	rgbImageInv = NULL;
	criticalsInitialized = false;
	fCamera = false;
	terminateAllEvent = NULL;
	VideoThread = NULL;
	capt = NULL;
}

void CKameraDlg::Initialize()
{
	char workpath[2048];
	CString str;
		
	im_width = iW;
	im_height = iH;

	GetModuleFileName(NULL, workpath, _MAX_PATH-1);
    str = workpath;
	sMyDir = str.Left(str.ReverseFind('\\')+1); 
	str = sMyDir;

	PrepareBitmapInfo();

	rgbImage = new unsigned char[im_width*im_height*3]; 
	rgbImageInv = new unsigned char[im_width*im_height*3]; 
	memset(rgbImage, 0, im_width * im_height * 3);

	InitializeCriticalSection(&CS_rgbBuffer);
	criticalsInitialized = true;

	terminateAllEvent = CreateEvent(NULL, true, false, TEXT("TerminateAllEvent"));

	// if camera can't be initialized don't start program threads
	if (!(fCamera = InitCamera())) 
	{
		MessageBox("Camera couldn't be initialized!\nSelect another device or resolution", "Application", MB_OK | MB_ICONERROR);	
		return;
	}

	SetImageWindowSize(iW, iH);

	if((VideoThread = CreateThread(NULL, 0, VideoThreadFunction, this, 0, &dVideoThreadId)) == NULL)
		MessageBox("Cannot create video thread", "Application", MB_OK | MB_ICONERROR); 
		
}

bool CKameraDlg::InitCamera()
{
	if (iDevice>=0) 
	{
		DestroyObject(&capt);

		capt = new VideoCapture();
		if (!capt->InitCamera(iDevice, iResolution)) 
		{
			DestroyObject(&capt);
			return false;
		}
	    return true;
	} else return false;
}
// CKameraDlg message handlers

BOOL CKameraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	image.Create("", SS_NOTIFY | WS_CHILD| WS_VISIBLE | BS_FLAT, CRect(0, 0, 100, 100), this, IDC_IMAGE);


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	ZeroVariables();
	Initialize();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKameraDlg::OnPaint()
{ 
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{

		CDialog::OnPaint();
	}
}

void CKameraDlg::PaintRGBImage()
{
		if (!image) return;

		EnterCriticalSection(&CS_rgbBuffer);

		if (image.actual_rect.left!=-1)
			DrawRectangleRGB(image.actual_rect, 0, 0, rgbImage, im_width, 
				im_height, 0, 0, 255);
/*		else
		if (image.last_rect.left!=-1)
			DrawRectangleRGB(image.last_rect, 0, 0, rgbImage, im_width, 
				im_height, 0, 0, 255);
*/

		CWnd *wnd = GetDlgItem(IDC_IMAGE);
	    CDC* dc = wnd->GetDC();	
							
		StretchDIBits(dc->m_hDC, 1, 1, im_width, im_height, 
                      0, 0, im_width, im_height, rgbImage, bip, 
                      DIB_RGB_COLORS, SRCCOPY);
		
		wnd->ReleaseDC(dc);

		LeaveCriticalSection(&CS_rgbBuffer);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CKameraDlg::OnClose()
{
	if (CanExit())
		CDialog::OnClose();
}

void CKameraDlg::OnOK()
{
	ClearVariables();
	if (CanExit())
		CDialog::OnOK();
}

void CKameraDlg::OnCancel()
{
	ClearVariables();
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CKameraDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.

	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

bool CKameraDlg::PauseCamera()
{
	if (capt!=NULL)
	{
		HRESULT res;
		res = capt->pControl->Pause();
		if (res==S_OK) return true;
	}

	return false;
}

bool CKameraDlg::StartCamera()
{
	if (capt!=NULL)
	{
		HRESULT res;
		res = capt->pControl->Run();
		if (res==S_OK) return true;
	}

	return false;
}

void CKameraDlg::OnVideooptionsChoosedevice()
{
	PauseCamera();

	CameraDialog *dlgCamera = new CameraDialog();
	INT_PTR nResponse;

	nResponse = dlgCamera->DoModal();

	if (dlgCamera->fOK==true)
	{
		int iRes = dlgCamera->iSelectedResolution;
		iDevice = dlgCamera->iSelectedDevice;

		iResolution =	iRes;

		iW = dlgCamera->iSelectedResX;
		iH = dlgCamera->iSelectedResY;

		ClearVariables();
		Initialize();
	} else
	  StartCamera();

	delete dlgCamera;
}


void CKameraDlg::OnVideooptionsCameraproperties()
{
	if (fCamera!=NULL)
		capt->ShowPropertyPage(GetSafeHwnd());
}

void CKameraDlg::SetMode(int mode)
{
	this->mode = mode;
	CMenu* mmenu = GetMenu();
    CMenu* submenu = mmenu->GetSubMenu(1);
    
    if (mode==0) submenu->CheckMenuItem(ID_MODES_MODE1, MF_CHECKED | MF_BYCOMMAND);
	else submenu->CheckMenuItem(ID_MODES_MODE1, MF_UNCHECKED | MF_BYCOMMAND);
    if (mode==1) submenu->CheckMenuItem(ID_MODES_MODE2, MF_CHECKED | MF_BYCOMMAND);
	else submenu->CheckMenuItem(ID_MODES_MODE2, MF_UNCHECKED | MF_BYCOMMAND);
    if (mode==2) submenu->CheckMenuItem(ID_MODES_MODE3, MF_CHECKED | MF_BYCOMMAND);
	else submenu->CheckMenuItem(ID_MODES_MODE3, MF_UNCHECKED | MF_BYCOMMAND);
}

void CKameraDlg::OnModesMode1()
{
	SetMode(0);
}


void CKameraDlg::OnModesMode2()
{
	SetMode(1);
}


void CKameraDlg::OnModesMode3()
{
	SetMode(2);
}
