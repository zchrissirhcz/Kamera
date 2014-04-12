#include "stdafx.h"

#include "VideoCapture.h"

VideoCapture::VideoCapture()
{
	cameraInitialized = false;
	

	CoInitialize(NULL);
	ZeroVariables();

	if (EnumDevices()==0) throw -1;
	
	EnumResolutions();
}

bool VideoCapture::InitCamera(int iDevice, int iResolution)
{
	if (cameraInitialized) return false;

	HRESULT hr;
	bool ans;
	int iWidth = devices_resolutions[iDevice].x[iResolution];
	int iHeight = devices_resolutions[iDevice].y[iResolution];

    USES_CONVERSION;

	CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
		  IID_ISampleGrabber, (void**)&pGrabber);
	if( !pGrabber ) return false;
    
	hr = pNull.CoCreateInstance(CLSID_NullRenderer);
	
    CComQIPtr< IBaseFilter, &IID_IBaseFilter > pGrabberBase( pGrabber );

	ans = BindFilter(iDevice, &pSource);
	if( !pSource ) return false;
	
	hr = pGraph.CoCreateInstance( CLSID_FilterGraph );
	if (hr!=S_OK) return false;

	hr = pGraph->AddFilter( pSource, L"Source" );
	if (hr!=S_OK) return false;
	hr = pGraph->AddFilter( pGrabberBase, L"Grabber" );
    if (hr!=S_OK) return false;
	hr = pGraph->AddFilter( pNull, L"NullRenderer" );
	if (hr!=S_OK) return false;
	
	// Tell the grabber to grab 24-bit video. Must do this
    // before connecting it
    CMediaType GrabType;

	GrabType.SetType( &MEDIATYPE_Video );
	GrabType.SetSubtype( &MEDIASUBTYPE_RGB24 );

	hr = pGrabber->SetMediaType( &GrabType );
	if (hr!=S_OK) return false;

    CComPtr< IPin > pGrabPinIn;
    CComPtr< IPin > pGrabPinOut;
    CComPtr< IPin > pNullPinIn;

    pSourcePin = GetOutPin( pSource, 0 );
    pGrabPinIn   = GetInPin( pGrabberBase, 0 );
    pGrabPinOut  = GetOutPin( pGrabberBase, 0 );
    pNullPinIn  = GetInPin( pNull, 0 );

	CComPtr <ICaptureGraphBuilder2> pCaptB;
	pCaptB.CoCreateInstance(CLSID_CaptureGraphBuilder2);
	pCaptB->SetFiltergraph(pGraph);

	hr = pCaptB->FindInterface(&PIN_CATEGORY_CAPTURE,
		&MEDIATYPE_Video,pSource, 
		IID_IAMStreamConfig, (void**)&pConfig);
	if (hr!=S_OK) return false;

	SetResolution(iDevice, iResolution);
    // ... and connect them
    //
    hr = pGraph->Connect( pSourcePin, pGrabPinIn);
    if (hr!=S_OK) return false;
	
	hr = pGraph->Connect( pGrabPinOut, pNullPinIn );
	if (hr!=S_OK) return false;
	
	// Ask for the connection media type so we know its size
    //
    AM_MEDIA_TYPE mt;
	hr = pGrabber->GetConnectedMediaType( &mt );
	if (hr!=S_OK) return false;

    VIDEOINFOHEADER * vih = (VIDEOINFOHEADER*) mt.pbFormat;
    callback.im_width  = iWidth;
    callback.im_height = iHeight;
    FreeMediaType( mt );

	// Write the bitmap format
    //   
    memset( &(callback.cbInfo.bih), 0, sizeof( callback.cbInfo.bih ) );
    callback.cbInfo.bih.biSize = iWidth*iHeight*3;
    callback.cbInfo.bih.biWidth = iWidth;
    callback.cbInfo.bih.biHeight = iHeight;
    callback.cbInfo.bih.biPlanes = 1;
    callback.cbInfo.bih.biBitCount = 24;

	callback.cbInfo.pBuffer = new byte[callback.cbInfo.bih.biSize];
	    
	hr = pGrabber->SetBufferSamples( FALSE );
	hr = pGrabber->SetOneShot( FALSE );
    hr = pGrabber->SetCallback( &callback, 1);

	hr = pGraph->QueryInterface(IID_IMediaControl,(LPVOID *) &pControl);

	hr = pControl->Run( );

	cameraInitialized = true;
	return true;	
}

void VideoCapture::SetResolution(int iDevice, int iResolution)
{
	VIDEO_STREAM_CONFIG_CAPS caps;
    AM_MEDIA_TYPE *pmt;
	HRESULT res;

	if( pConfig->GetStreamCaps(iResolution, &pmt, reinterpret_cast<BYTE*>(&caps)) == S_OK ) 
	{
		VIDEOINFOHEADER *pVih = 
            reinterpret_cast<VIDEOINFOHEADER*>(pmt->pbFormat);

		pVih->bmiHeader.biWidth  = devices_resolutions[iDevice].x[iResolution];
		pVih->bmiHeader.biHeight = devices_resolutions[iDevice].y[iResolution];
		pVih->bmiHeader.biSizeImage = 
			pVih->bmiHeader.biWidth * pVih->bmiHeader.biHeight 
			* pVih->bmiHeader.biBitCount/8;

		res = pConfig->SetFormat(pmt);
	}
	
	DeleteMediaType(pmt);
}

IPin *  VideoCapture::GetInPin( IBaseFilter * pFilter, int nPin )
{
    CComPtr<IPin> pComPin=0;
    GetPin(pFilter, PINDIR_INPUT, nPin, &pComPin);
    return pComPin;
}


IPin *  VideoCapture::GetOutPin( IBaseFilter * pFilter, int nPin )
{
    CComPtr<IPin> pComPin=0;
    GetPin(pFilter, PINDIR_OUTPUT, nPin, &pComPin);
    return pComPin;
}

HRESULT  VideoCapture::GetPin( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin)
{
    CComPtr< IEnumPins > pEnum;
    *ppPin = NULL;

    HRESULT hr = pFilter->EnumPins(&pEnum);
    if(FAILED(hr)) return hr;

    ULONG ulFound;
    IPin *pPin;
    hr = E_FAIL;

    while(S_OK == pEnum->Next(1, &pPin, &ulFound))
    {
        PIN_DIRECTION pindir = (PIN_DIRECTION)3;

        pPin->QueryDirection(&pindir);
        if(pindir == dirrequired)
        {
            if(iNum == 0)
            {
                *ppPin = pPin;  // Return the pin's interface
                hr = S_OK;      // Found requested pin, so clear error
                break;
            }
            iNum--;
        } 

        pPin->Release();
    } 

    return hr;
}

void VideoCapture::ZeroVariables()
{
	nDevices = 0;
	devices_names = NULL;
	devices_resolutions = NULL;
	
	pGrabber = NULL;
	pConfig = NULL;
	pControl = NULL;

	callback.cbInfo.pBuffer = NULL;
}

void VideoCapture::EnumResolutions()
{
	int iCount, iSize, iChosen=-1;
	IBaseFilter *pSource;
   	CComPtr <ICaptureGraphBuilder2> pCaptB;
	VIDEO_STREAM_CONFIG_CAPS caps;
	HRESULT hr;
	bool response;

	IAMStreamConfig *pConfig;

	devices_resolutions = new DeviceResolutions[nDevices];

	pCaptB.CoCreateInstance(CLSID_CaptureGraphBuilder2);

	for (unsigned int iDevice=0; iDevice<nDevices; iDevice++)
	{
		response = BindFilter(iDevice, &pSource);

		hr = pCaptB->FindInterface(
        &PIN_CATEGORY_CAPTURE,
        &MEDIATYPE_Video,
        pSource,
        IID_IAMStreamConfig,
        (void**)&pConfig);

		if (!SUCCEEDED(hr))
		{
			pSource->Release();
			devices_resolutions[iDevice].nResolutions = 0;
			continue;
		}

		pConfig->GetNumberOfCapabilities(&iCount, &iSize);

		devices_resolutions[iDevice].SetNResolutions(iCount);


		for(int i=0; i < iCount; i++) {
			AM_MEDIA_TYPE *pmt;
			if( pConfig->GetStreamCaps(i, &pmt, reinterpret_cast<BYTE*>(&caps)) == S_OK ) {

				VIDEOINFOHEADER *pVih = 
					reinterpret_cast<VIDEOINFOHEADER*>(pmt->pbFormat);
				
				devices_resolutions[iDevice].x[i] = caps.InputSize.cx;
				devices_resolutions[iDevice].y[i] = caps.InputSize.cy;
				devices_resolutions[iDevice].color_space[i] = pmt->subtype;
				devices_resolutions[iDevice].compression[i] = pVih->bmiHeader.biCompression;
				DeleteMediaType(pmt);
			}
		}

		pSource->Release();
		pConfig->Release();

		pSource = 0;
	}
}

int VideoCapture::EnumDevices()
{
	int id = 0;
	
    // enumerate all video capture devices
	CComPtr<ICreateDevEnum> pCreateDevEnum;

	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
								  IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if (hr != NOERROR)  return -1;
	
    CComPtr<IEnumMoniker> pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
								&pEm, 0);
    if (hr != NOERROR) 	return -1 ;
    
    pEm->Reset();

    IMoniker *pM;
	nDevices = 0;
    while(hr = pEm->Next(1, &pM, NULL), hr==S_OK)
		nDevices++;
	
	if (nDevices==0) return -1;

	pEm->Reset();

	devices_names = new string[nDevices];
		
	while(hr = pEm->Next(1, &pM, NULL), hr==S_OK)
    {
		IPropertyBag *pBag;
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		if(SUCCEEDED(hr)) 
		{
			VARIANT var;
			var.vt = VT_BSTR;
			hr = pBag->Read(L"FriendlyName", &var, NULL);
			if (hr == NOERROR) 
			{
				TCHAR str[2048];		

				WideCharToMultiByte(CP_ACP,0,var.bstrVal, -1, str, 2048, NULL, NULL);
				devices_names[id] = str;

				id++;

				SysFreeString(var.bstrVal);
			}
			pBag->Release();
		}
		pM->Release();
    }
	return id;
}

bool VideoCapture::BindFilter(int deviceId, IBaseFilter **pFilter)
{
	if (deviceId < 0)
		return false;
	
    // enumerate all video capture devices
	CComPtr<ICreateDevEnum> pCreateDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
			  IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if (hr != NOERROR)
		return false;

    CComPtr<IEnumMoniker> pEm;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
								&pEm, 0);
    if (hr != NOERROR) 
		return false;

    pEm->Reset();
    IMoniker *pM;

	for (int i=0; i<=deviceId; i++)
		pEm->Next(1, &pM, NULL);

	IPropertyBag *pBag;
	hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
	pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pFilter);
	pM->Release();

	return true;
}

long VideoCapture::ShowPropertyPage(HWND hwndParent)
{

	HRESULT hr;
	ISpecifyPropertyPages *pSpecify=0;
	CAUUID cauuid;

	hr = pSource->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpecify);

	if (SUCCEEDED(hr)) 
	{
		do 
		{
			hr = pSpecify->GetPages(&cauuid);

			hr = OleCreatePropertyFrame(hwndParent, 0, 0, NULL, 1,
				(IUnknown **)&pSource, cauuid.cElems,
				(GUID *)cauuid.pElems, 0, 0, NULL);

			CoTaskMemFree(cauuid.pElems);
		} while(0);
	}

	// Release interfaces
	if (pSpecify)
		pSpecify->Release();

	return hr;
}


VideoCapture::~VideoCapture()
{
	OAFilterState state = State_Running;
	int counter = 0;

	if (pControl!=NULL) 
	{
		pControl->Pause();
		pControl->Stop();

		while(state==State_Running)
		{
			if (pControl->GetState(10000, &state)==VFW_S_STATE_INTERMEDIATE)
				break;
			counter++;
		}

	}

	if (pConfig!=NULL)  { pConfig->Release(); pConfig = NULL; }
	if (pGrabber!=NULL) { pGrabber->Release(); pGrabber = NULL; }
	if (pControl!=NULL) { pControl->Release(); pControl = NULL; }

	DestroyTable(&devices_resolutions);
	DestroyTable(&devices_names);
	DestroyTable(&callback.cbInfo.pBuffer);

	CoUninitialize( );
}