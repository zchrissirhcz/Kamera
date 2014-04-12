#pragma once

#define __IDxtCompositor_INTERFACE_DEFINED__
#define __IDxtAlphaSetter_INTERFACE_DEFINED__
#define __IDxtJpeg_INTERFACE_DEFINED__
#define __IDxtKey_INTERFACE_DEFINED__

#include "qedit.h"
#include "dshow.h"
#include "structs.h"

// Implementation of CSampleGrabberCB object
//
// Note: this object is a SEMI-COM object, and can only be created statically.

class VideoCallback : public ISampleGrabberCB 
{

public:
    long im_width;
    long im_height;

	bool fSampleReady;

	CALLBACKINFO cbInfo;

    // Fake out any COM ref counting
    //
    STDMETHODIMP_(ULONG) AddRef() { return 2; }
    STDMETHODIMP_(ULONG) Release() { return 1; }

    // Fake out any COM QI'ing
    //
    STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
    {
        if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown ) 
        {
            *ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
            return NOERROR;
        }    

        return E_NOINTERFACE;
    }

    // We don't implement this one
    //
    STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample )
    {
        return 0;
    }


    // The sample grabber is calling us back on its deliver thread.
    // This is NOT the main app thread!
    //
	STDMETHODIMP BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize )
    {
        if (!pBuffer)
            return E_POINTER;

		if (fSampleReady==false)
		{
			if (!pBuffer)
		     return E_POINTER;
		
			memcpy(cbInfo.pBuffer, pBuffer, lBufferSize);

			fSampleReady = true;

		}
        return 0;
    }
};
