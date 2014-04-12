// ameraDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Kamera.h"
#include "CameraDialog.h"

// CameraDialog dialog
#define N_FORMATS 13

const struct formats{
char name[8];
GUID media_format;
};

struct formats formats_table[] =
{
{ "Grey", MEDIASUBTYPE_RGB8 },
{ "BGR32", MEDIASUBTYPE_RGB32 },
{ "BGR24", MEDIASUBTYPE_RGB24 },
{ "RGB565", MEDIASUBTYPE_RGB565 },
{ "RGB555", MEDIASUBTYPE_RGB555 },
{ "YUV420P", MEDIASUBTYPE_IYUV },
{ "YUV422P", MEDIASUBTYPE_YUYV },
{ "YUV411", MEDIASUBTYPE_Y411 },
{ "YUV411P", MEDIASUBTYPE_Y41P },
{ "YUV410P", MEDIASUBTYPE_YVU9 },
{ "YUY2", MEDIASUBTYPE_YUY2 },
{ "MJPEG", MEDIASUBTYPE_MJPG },
{ "UYVY422", MEDIASUBTYPE_UYVY },
};
 
IMPLEMENT_DYNAMIC(CameraDialog, CDialog)


BOOL CameraDialog::EnumResolutions()
{
	int i;
	CString text;
	int iDevice = m_devices.GetCurSel();
	DeviceResolutions *resol = &capt.devices_resolutions[iDevice];

	m_resolutions.ResetContent();

	if (resol->nResolutions==0)
	{
		GetDlgItem(IDC_BUTTON_OK)->EnableWindow(false);
		return false;
	}
	else
		GetDlgItem(IDC_BUTTON_OK)->EnableWindow(true);

	// list all the resolutions and color spaces
	for (i=0; i<resol->nResolutions; i++)
	{	
		for (int form=0; form<N_FORMATS; form++)
			if (resol->color_space[i]==formats_table[form].media_format)
			{
				text.Format("%dx%d (%s)", resol->x[i], resol->y[i],
					formats_table[form].name);
				break;
			}

		m_resolutions.AddString(text);
	}

	m_resolutions.SetCurSel(0);

	// select first 640x480 if available 
	for (i=0; i<resol->nResolutions; i++)
	{
		if (resol->x[i]==640 && resol->y[i]==480)
		{
			m_resolutions.SetCurSel(i);
			break;
		}
	}

	return true;

}

BOOL CameraDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (capt.nDevices==0)
	{
		MessageBox("No camera found, closing ...");
		iSelectedDevice = -1;
		OnCancel();
		return TRUE;
	}

	// list devices
	for (unsigned int i=0; i<capt.nDevices; i++)
		m_devices.AddString(capt.devices_names[i].c_str());

	m_devices.SetCurSel(0);

	OnCbnSelchangeComboCamera();
	fOK = false;

	return TRUE;
}

CameraDialog::CameraDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CameraDialog::IDD, pParent)
{

}

CameraDialog::~CameraDialog()
{
}

void CameraDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CAMERA, m_devices);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_resolutions);
}


BEGIN_MESSAGE_MAP(CameraDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_CAMERA, &CameraDialog::OnCbnSelchangeComboCamera)
	ON_CBN_SELCHANGE(IDC_COMBO_RESOLUTION, &CameraDialog::OnCbnSelchangeComboResolution)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_OK, &CameraDialog::OnBnClickedButtonOk)
END_MESSAGE_MAP()


// CameraDialog message handlers

void CameraDialog::OnCbnSelchangeComboCamera()
{
	EnumResolutions();
//	Capture->CloseInterfaces();
}

void CameraDialog::OnCbnSelchangeComboResolution()
{
	// TODO: Add your control notification handler code here
}

void CameraDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

void CameraDialog::OnOK()
{
	OnBnClickedButtonOk();

}

void CameraDialog::OnCancel()
{
	fOK = false;
	CDialog::OnCancel();
}

void CameraDialog::OnBnClickedButtonOk()
{
	iSelectedDevice			=	m_devices.GetCurSel();
	iSelectedResolution		=	m_resolutions.GetCurSel();

	if (capt.devices_resolutions[iSelectedDevice].nResolutions==0)
		OnCancel();
	else
	{
		iSelectedResX = capt.devices_resolutions[iSelectedDevice].x[iSelectedResolution];
		iSelectedResY = capt.devices_resolutions[iSelectedDevice].y[iSelectedResolution];
		fOK = true;
		CDialog::OnOK();
	}
}
