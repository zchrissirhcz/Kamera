#pragma once

#include <string>
#include "MemoryUtils.h"

#include <DShow.h>
#include <streams.h>
#include "vfw.h"
#include "VideoCallback.h"


using namespace std;

class DeviceResolutions { 

public:
	unsigned int *x; 
	unsigned int *y; 
	GUID *color_space;
	DWORD *compression;
	int nResolutions;

	void ClearTables()
	{
		DestroyTable(&x);
		DestroyTable(&y);
		DestroyTable(&color_space);
		DestroyTable(&compression);
	}

	DeviceResolutions()
	{
		x = NULL; y = NULL;
		color_space = NULL;
		compression = NULL;
		nResolutions = 0;
	}

	void SetNResolutions(int nRes)
	{
		ClearTables();
		nResolutions = nRes;
		x = new unsigned int[nResolutions];
		y = new unsigned int[nResolutions];
		color_space = new GUID[nResolutions];
		compression = new DWORD[nResolutions];
	}

	DeviceResolutions(int nRes)
	{
		SetNResolutions(nRes);
	}


	~DeviceResolutions()
	{
		ClearTables();
	}
};

class VideoCapture
{
public:

	VideoCapture();

	unsigned int nDevices;
	string *devices_names;
	DeviceResolutions *devices_resolutions;		

	~VideoCapture();

	bool InitCamera(int iDevice, int iResolution);
	long ShowPropertyPage(HWND hwndParent);

	VideoCallback callback;
	
    IMediaControl *pControl;
	
private:
	int EnumDevices();
	void EnumResolutions();
	void ZeroVariables();
	bool BindFilter(int deviceId, IBaseFilter **pFilter);
	void SetResolution(int iDevice, int iResolution);

	IPin * GetInPin( IBaseFilter * pFilter, int nPin );
	IPin * GetOutPin( IBaseFilter * pFilter, int nPin );
	HRESULT  GetPin( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin);

	IAMStreamConfig* pConfig; 
	ISampleGrabber *pGrabber;

	CComPtr< IBaseFilter >    pSource;
    CComPtr< IBaseFilter >    pNull;
	CComPtr< IPin >			  pSourcePin;
    CComPtr< IGraphBuilder >  pGraph;
	

	bool cameraInitialized;

};