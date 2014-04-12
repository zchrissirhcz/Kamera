#pragma once

#include "afxwin.h"
//#include "cublas.h"
#include "structs.h"
#include "math.h"


float GetEditBoxFloat(CEdit &box);
float* GetMultiplicationResult(float *A, float *B, bool isBInDevice, int m, int n, int k);
//float* GetMultiplicationResult2(float *A, float *B, bool isBInDevice, int m, int n, int k);

bool RectangleIntersection(RECT r1, RECT r2);
float RectangleIntersectionRatio(RECT r1, RECT r2);

inline bool checkIfAngleHigherThan45Degrees(int x1, int y1, int x2, int y2)
{   if (abs(x2-x1)<abs(y2-y1)) return true;	else return false;  }

inline float FindFloatDistance(feature f1, feature f2)
{   
	int dist1 = f1.X - f2.X;
	int dist2 = f1.Y - f2.Y;
	dist1*=dist1;   dist2*=dist2;
	return sqrt((float)(dist1+dist2));
}

inline float FindFloatDistance(int x1, int y1, feature f2)
{   
	int dist1 = x1 - f2.X;
	int dist2 = y1 - f2.Y;
	dist1*=dist1;   dist2*=dist2;
	return sqrt((float)(dist1+dist2));
}

inline float FindFloatDistance(int x1, int y1, int x2, int y2)
{   
	int dist1 = x1 - x2;
	int dist2 = y1 - y2;
	dist1*=dist1;   dist2*=dist2;
	return sqrt((float)(dist1+dist2));
}