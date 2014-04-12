#pragma once

#include "consts.h"

float DistanceExact(int x1, int y1, int x2, int y2);

struct Point
{
	int x;
	int y;
	bool set;
	int index;
	float s;

	Point()
	{	x=0; y=0; set = false; index = 0; s = 0;  }
};

typedef struct _callbackinfo 
{
    double dblSampleTime;
    long lBufferSize;
    unsigned char *pBuffer;
    BITMAPINFOHEADER bih;
	DWORD biSize;

} CALLBACKINFO;


struct feature
{
	int X, Y;	// anchoring point
	float S;	// scale
	int Sn;		// scale index
	bool F;		// flag (no specified usage)
	int weight; // weight of the ff truncated to int

	void Set(int _x, int _y, float _S, int _Sn=-1, bool _F=true, int _weight=1)
	{	X = _x; Y = _y; S = _S; Sn = _Sn; F = _F, weight = _weight; }
	
	feature()
	{
		X = 0; Y = 0; S = 0; Sn = 0; F = 0; weight = 0;
	}

};
