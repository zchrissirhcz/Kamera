#pragma once

#include "SimpleGraphic.h"
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <algorithm>
#include <fstream>

const unsigned int MAX_HEIGHT = 480;
const unsigned int MAX_WIDTH = 640;
class ImageProcessing
{
private:
	void ZeroVariables();
	void Mode1Processing(unsigned char *rgb_image, int w, int h);
	void Mode2Processing(unsigned char *rgb_image, int w, int h);
	void Mode3Processing(unsigned char *rgb_image, int w, int h);

	CRect last_rect;
	float L[16] [16]; //histogram nieznormalizownay
	float LNorm[16] [16]; //znormalizowany
	float yt; //prog
	int K; //liczba zaznaczen
	int currE;

public:
	ImageProcessing();
	~ImageProcessing();
	void ProcessRGBImage(int mode, unsigned char *rgb_image, int w, int h);
	void ServiceDrawnRectangle(CRect rect, unsigned char *rgb_image, int w, int h);
};