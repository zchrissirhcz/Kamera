#include "stdafx.h"
#include "SimpleUtils.h"


bool RectangleIntersection(RECT r1, RECT r2)
{
     if ((r1.left) > (r2.right) ||
         (r1.bottom) < (r2.top) ||
         (r1.right) < (r2.left) ||
         (r1.top) > (r2.bottom)) return false;
         return true;
}

// returns intersection ratio in respect to the r1 rectangle
float RectangleIntersectionRatio(RECT r1, RECT r2)
{
	float intersectionRatio = 0;
	RECT intersectingRect;
	int field1, field2, intersectionField;

	if (!RectangleIntersection(r1, r2)) return 1;

	field1 = (r1.right - r1.left) * (r1.bottom - r1.top);
	field2 = (r2.right - r2.left) * (r2.bottom - r2.top);
	
	intersectingRect.left = max(r1.left, r2.left);
	intersectingRect.right = min(r1.right, r2.right);
	intersectingRect.top = max(r1.top, r2.top);
	intersectingRect.bottom = min(r1.bottom, r2.bottom);

	intersectionField = (intersectingRect.right - intersectingRect.left) * (intersectingRect.bottom - intersectingRect.top);
	
	intersectionRatio = (float)intersectionField/(float)field1 - 1.0f;

	return intersectionRatio;
}


float GetEditBoxFloat(CEdit &box)
{
	CString text;
	float result;
	box.GetWindowTextA(text);
	sscanf_s(text.GetBuffer(), "%f", &result);
	text.ReleaseBuffer();
	return result;
}
/*
// result(m x n) = A(m x k) * B(k x n) 
float* GetMultiplicationResult(float *A, float *B, bool isBInDevice, int m, int n, int k)
{
	float *result;
	float *cuResult;
	float *cuA, *cuB;

	cublasAlloc(m*k, sizeof(float), (void**)&cuA);
	
	if (isBInDevice) cuB = B;
	else cublasAlloc(k*n, sizeof(float), (void**)&cuB);
	
	cublasAlloc(m*n, sizeof(float), (void**)&cuResult);

	result = new float[m*n];

	cublasSetMatrix(m, k, sizeof(float), A, m, cuA, m);
	if (!isBInDevice) cublasSetMatrix(k, n, sizeof(float), B, k, cuB, k);
	cublasSgemm('n', 'n', m, n, k, 1, cuA, m, cuB, k, 0, cuResult, m);

	cublasGetMatrix(m, n, sizeof(float), cuResult, m, 
		result, m);	

	cublasFree(cuA);
	if (!isBInDevice) cublasFree(cuB);
	cublasFree(cuResult);

	return result;
}

*/