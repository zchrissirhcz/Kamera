#ifndef SIMPLEG
#define SIMPLEG

#include "structs.h"

void DrawPixel(int x, int y, int w, int h, byte *RGBPixels, int r, int g, int b);
void DrawPixel(int x, int y, int w, int h, byte *pixels, int val);
void DrawBigCross(int x, int y, int w, int h, byte *RGBPixels, int r, int g, int b);

void DrawLine(int xa, int ya, int xb, int yb, int w, int h, byte *pixels, unsigned char r, unsigned char g,
				 unsigned char b);
void DrawLine(int xa, int ya, int xb, int yb, int w, int h, byte *pixels, int val=255);
RECT DrawFaceBoundingRotated(int x, int xres, int y, int yres, int width, int height, 
							 unsigned char* pixels, double angle, int r, int g, int b, bool paint);
void DrawCross(int x, int y, int w, int h, byte *RGBPixels, int r, int g, int b);
void DrawCrossSimple(int x, int y, int w, int h, byte* pixels,byte val);
void CopyRGBToGrayscale(byte *dest, byte *RGBSource, int w, int h);
void CopyRGBToGrayscale(float *dest, byte *RGBSource, int w, int h);
void CopyRGBToGrayscale(byte *dest, byte *RGBSource, int w, int h, int extra);

// extraWidth must be difference between w and closest number that can be divided by 8
void CopyGrayscaleToRGB(byte *RGBdest, byte *source, int w, int h, int extraWidth);
void RotatePoint(float angle, int x0, int y0, int &x, int &y);
void RotateImage(float angle, int x0, int y0, int iWidth, int iHeight, unsigned char *pbData, 
				 unsigned char **pbDataRows);
// returns specified rectangle area for the image
// x, y - anchoring point
// w, h - size of the new image 
// 
unsigned char* CutImage(int x, int y, int w, int h, unsigned char *data, 
				   int &iWidth, int &iHeight, bool deleteData);
unsigned char *ScaleImage2(double scale, unsigned char *data, int &iWidth, int &iHeight);
unsigned char *ScaleImage(unsigned char *data, int &iWidth, int &iHeight);
void DrawSmallCross(int x, int y, int w, int h, byte *RGBPixels, int r, int g, int b);
void ScaleImage(double scale, unsigned char *data, int &iWidth, int &iHeight, unsigned char *newData);
int* BuildHistogram(byte *image, int w, int h);
void EqualizeHistogram(byte *image, int w, int h);
int kMeans(feature *f, int nElements, int iRectangleSize, int iWidth, int iHeight, float fScaleRatio,
		   int nScales);
int kMeans2(feature *f, int nElements, int iRectangleSize, float fScaleRatio,
			int nScales);
void GetPixelRGB(int x, int y, unsigned char *RGBPixels, int w, int h, unsigned char &R, 
	unsigned char &G, unsigned char &B);

int Distance(int x1, int y1, int x2, int y2);
float Distance(float x1, float y1, float x2, float y2);
int Distance(feature f1, feature f2);
int Distance2(feature f1, feature f2);
float DistanceExact(int x1, int y1, int x2, int y2);
float DistanceExact(feature &f1, feature &f2);

void FindIntegralImage(unsigned char *img,int xres,int yres,int **iimage);

void ChangeBrightness(unsigned char *image, int w, int h, int val);

void DrawRectangle(RECT rect, int x, int y, byte *pixels, int w, int h);
void DrawRectangleRGB(RECT rect, int x, int y, byte *pixelsRGB, int w, int h, int r, int g, int b);

#endif