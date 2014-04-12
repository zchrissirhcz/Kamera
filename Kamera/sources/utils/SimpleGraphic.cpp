#include "stdafx.h"
#include "math.h"
#include "structs.h"
#include "SimpleGraphic.h"
#include <atlstr.h>

void ChangeBrightness(unsigned char *image, int w, int h, int val)
{
	int new_val;
	for (int i=0; i<(w*h); i++)
	{
		new_val = min(max((*image)+val, 0), 255);
		*image = (unsigned char)new_val;
		image++;
	}
}

void DrawBigCross(int x, int y, int w, int h, byte *RGBPixels, int r, int g, int b)
{
	(x, y, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x-1, y-1, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x+1, y+1, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x, y-1, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x, y-2, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x, y+1, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x, y+2, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x-1, y, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x-2, y, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x+1, y, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x+2, y, w, h, RGBPixels, r, g, b);
}

void DrawRectangleRGB(RECT rect, int x, int y, byte *pixelsRGB, int w, int h, int r, int g, int b)
{
	int i;

	for (i=max(0, (rect.left + x)); i<min((rect.right+x+1), w); i++)
	{
		DrawPixel(i, y+rect.top, w, h, pixelsRGB, r, g, b);
		DrawPixel(i, y+rect.bottom, w, h, pixelsRGB, r, g, b);
	}

	for (i=max(0, (rect.top + y)); i<min((rect.bottom+y+1), h); i++)
	{
		DrawPixel(rect.left + x, i, w, h, pixelsRGB, r, g, b);
		DrawPixel(rect.right + x, i, w, h, pixelsRGB, r, g, b);
	}
}


void DrawRectangle(RECT rect, int x, int y, byte *pixels, int w, int h)
{
	int i;

	for (i=max(0, (rect.left + x)); i<min((rect.right+x+1), w); i++)
	{
		pixels[(y+rect.top)*w + i]    = 0; 
		pixels[(y+rect.bottom)*w + i] = 0; 
	}

	for (i=max(0, (rect.top + y)); i<min((rect.bottom+y+1), h); i++)
	{
		pixels[i*w + rect.left + x]    = 0; 
		pixels[i*w + rect.right + x] = 0; 
	}
}

void GetPixelRGB(int x, int y, unsigned char *RGBPixels, int w, int h, unsigned char &R, 
	unsigned char &G, unsigned char &B)
{
	int l;
	if (x<w && y<h && x>0 && y>0)
	{
		l = 3*(h-y-1)*w + x*3;
		R = RGBPixels[l];
		G = RGBPixels[l+1];
		B = RGBPixels[l+2];
	}
}

void DrawPixel(int x, int y, int w, int h, byte *RGBPixels, int r, int g, int b)
{
	int l;

	if (x<w && y<h && x>=0 && y>=0)
	{
		l = 3*(h-y-1)*w + x*3;
		RGBPixels[l]   = (unsigned char)r;
		RGBPixels[l+1] = (unsigned char)g;
		RGBPixels[l+2] = (unsigned char)b;
	}
}

void DrawPixel(int x, int y, int w, int h, byte *pixels, int val)
{
	if (x<w && y<h && x>0 && y>0)
		pixels[x+y*w]   = (byte)val;
}

void DrawLine(int xa, int ya, int xb, int yb, int w, int h, byte *pixels, int val)
{
	double arc=0;
	double wx, wy, div;
	double l;
	int x1, x2, y1, y2, sgnx=1, sgny=1, sgn=1;

	x1 = min(xa, xb);
	x2 = max(xa, xb);
	y1 = min(ya, yb);
	y2 = max(ya, yb);
	
	wx = x1 - x2;
	wy = y1 - y2;
	
	div = wy/wx;

	if (xb>xa) sgnx = -1;
	if (yb>ya) sgny = -1;

	arc = atan(div); 

	if ((sgnx==1 && sgny==1) ||
	   (sgnx==-1 && sgny==-1)) ;
	 else sgn = -1;
		 
	
	for (l=x1; l<x2; l++)
	{
		if (sgn==-1) wy = (x2-l) * tan(arc); 
		  else wy = (l-x1) * tan(arc);

		DrawPixel((int)l, y1+(int)wy, w, h, pixels, val);
	}

	
	for (l=y1; l<y2; l++)
	{
		if (sgn==-1) wx = (y2-l) / tan(arc);
		  else wx = (l-y1) / tan(arc);
		
		DrawPixel(x1+(int)wx, (int)l, w, h, pixels, val);
	}

}


void DrawLine(int xa, int ya, int xb, int yb, int w, int h, byte *pixels, unsigned char r, unsigned char g,
				 unsigned char b)
{
	double arc=0;
	double wx, wy, div;
	double l;
	int x1, x2, y1, y2, sgnx=1, sgny=1, sgn=1;

	x1 = min(xa, xb);
	x2 = max(xa, xb);
	y1 = min(ya, yb);
	y2 = max(ya, yb);
	
	wx = x1 - x2;
	wy = y1 - y2;
	
	div = wy/wx;

	if (xb>xa) sgnx = -1;
	if (yb>ya) sgny = -1;

	arc = atan(div); 

	if ((sgnx==1 && sgny==1) ||
	   (sgnx==-1 && sgny==-1)) ;
	 else sgn = -1;
		 
	
	for (l=x1; l<x2; l++)
	{
		if (sgn==-1) wy = (x2-l) * tan(arc); 
		  else wy = (l-x1) * tan(arc);

		DrawPixel((int)l, y1+(int)wy, w, h, pixels, r, g, b);
	}

	
	for (l=y1; l<y2; l++)
	{
		if (sgn==-1) wx = (y2-l) / tan(arc);
		  else wx = (l-y1) / tan(arc);
		
		DrawPixel(x1+(int)wx, (int)l, w, h, pixels, r, g, b);
	}

}

RECT DrawFaceBoundingRotated(int x, int xres, int y, int yres, int width, int height, 
							 unsigned char* pixels, double angle, int r, int g, int b, bool paint)
{
	RECT boundingBox;

	double sin_a,cos_a,Sxd,Syd,S2x,S2y;
	int x1, y1, x2, y2, x3, y3;

	sin_a=sin(angle);  	  
    cos_a=cos(angle);

	Sxd=x+0.5*(cos_a*(xres-1)-sin_a*(yres-1));
    Syd=y+0.5*(cos_a*(yres-1)+sin_a*(xres-1));	  	  
    S2x=(xres-1)/2.0;
    S2y=(yres-1)/2.0;	

    x1=(int)(cos_a*(xres-1-S2x)+sin_a*S2y+Sxd);
    y1=(int)(sin_a*(xres-1-S2x)-cos_a*S2y+Syd);

	if (paint) DrawLine(x, y, x1, y1, width, height, pixels, (byte)b, (byte)g, (byte)r);

	x2=(int)(cos_a*(xres-1-S2x)-sin_a*(yres-1-S2y)+Sxd);
  	y2=(int)(sin_a*(xres-1-S2x)+cos_a*(yres-1-S2y)+Syd);
	
    if (paint) DrawLine(x1, y1, x2, y2, width, height, pixels, (byte)b, (byte)g, (byte)r);
		  		  
    x3=(int)(-cos_a*S2x-sin_a*(yres-1-S2y)+Sxd);
	y3=(int)(-sin_a*S2x+cos_a*(yres-1-S2y)+Syd);
		  
	if (paint)DrawLine(x2, y2, x3, y3, width, height, pixels, (byte)b, (byte)g, (byte)r);
	if (paint)DrawLine(x, y, x3, y3, width, height, pixels, (byte)b, (byte)g, (byte)r);

	boundingBox.top = max(0, min(y, y1) - 5);
	boundingBox.left = max(0, min(x, x3) - 5);
	boundingBox.bottom = min(max(y2, y3) + 5, height-1);
	boundingBox.right = min(max(x2, x1) + 5, width-1);


	return boundingBox;
}

void DrawSmallCross(int x, int y, int w, int h, byte *RGBPixels, int r, int g, int b)
{
	DrawPixel(x, y, w, h, RGBPixels, r, g, b);
	DrawPixel(x+1, y, w, h, RGBPixels, r, g, b);
	DrawPixel(x+2, y, w, h, RGBPixels, r, g, b);
	DrawPixel(x+3, y, w, h, RGBPixels, r, g, b);
	DrawPixel(x, y-1, w, h, RGBPixels, r, g, b);
	DrawPixel(x, y-2, w, h, RGBPixels, r, g, b);
	DrawPixel(x, y-3, w, h, RGBPixels, r, g, b);
	DrawPixel(x-1, y, w, h, RGBPixels, r, g, b);
	DrawPixel(x-2, y, w, h, RGBPixels, r, g, b);
	DrawPixel(x-3, y, w, h, RGBPixels, r, g, b);
	DrawPixel(x, y+1, w, h, RGBPixels, r, g, b);
	DrawPixel(x, y+2, w, h, RGBPixels, r, g, b);
	DrawPixel(x, y+3, w, h, RGBPixels, r, g, b);
}

void DrawCross(int x, int y, int w, int h, byte *RGBPixels, int r, int g, int b)
{
	DrawSmallCross(x, y, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x-1, y-1, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x-2, y-2, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x, y-1, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x, y-2, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x-1, y, w, h, RGBPixels, r, g, b);
	DrawSmallCross(x-2, y, w, h, RGBPixels, r, g, b);
}
void DrawCrossSimple(int x, int y, int w, int h, byte* pixels,byte val)
{
	DrawPixel(x, y, w, h, pixels, val);
	DrawPixel(x-1, y, w, h, pixels, val);
	DrawPixel(x-2, y, w, h, pixels, val);
	DrawPixel(x-3, y, w, h, pixels, val);
	DrawPixel(x+1, y, w, h, pixels, val);
	DrawPixel(x+2, y, w, h, pixels, val);
	DrawPixel(x+3, y, w, h, pixels, val);
	DrawPixel(x, y-1, w, h, pixels, val);
	DrawPixel(x, y-2, w, h, pixels, val);
	DrawPixel(x, y-3, w, h, pixels, val);
	DrawPixel(x, y+1, w, h, pixels, val);
	DrawPixel(x, y+2, w, h, pixels, val);
	DrawPixel(x, y+3, w, h, pixels, val);

}

void CopyRGBToGrayscale(byte *dest, byte *RGBSource, int w, int h)
{
	int x, y, diff;
	byte *ptrs, *ptrd;
	for (y=0; y<h; y++)
	{
	  diff = h-y-1;
	  
	  ptrs = RGBSource + y*w*3;
	  ptrd = dest      + diff*w;

	  for (x=0; x<w; x++)
 	  {
//		*ptrd = (0.11*(*ptrs)+0.30*(*(ptrs+1))+.59*(*(ptrs+2)));
		*ptrd = ((*ptrs)+*(ptrs+1)+*(ptrs+2))/3;
		ptrs+=3;
		ptrd+=1;
	  }
	}

}

void CopyRGBToGrayscale(byte *dest, byte *RGBSource, int w, int h, int extra)
{
	int x, y, diff;
	byte *ptrs, *ptrd;
	for (y=0; y<h; y++)
	{
	  diff = h-y-1;
	  
	  ptrs = RGBSource + y*(w+extra)*3;
	  ptrd = dest      + diff*w;

	  for (x=0; x<w; x++)
 	  {
//		*ptrd = (0.11*(*ptrs)+0.30*(*(ptrs+1))+.59*(*(ptrs+2)));
		*ptrd = ((*ptrs)+*(ptrs+1)+*(ptrs+2))/3;
		ptrs+=3;
		ptrd+=1;
	  }
	}

}

void CopyRGBToGrayscale(float *dest, byte *RGBSource, int w, int h)
{
	int x, y, diff;
	byte *ptrs;
	float *ptrd;
	for (y=0; y<h; y++)
	{
	  diff = h-y-1;
	  
	  ptrs = RGBSource + y*w*3;
	  ptrd = dest      + diff*w;

	  for (x=0; x<w; x++)
 	  {
	    
//		*ptrd = 0.11*(*ptrs)+0.30*(*(ptrs+1))+0.59*(*(ptrs+2));
		*ptrd = (float)(((*ptrs)+*(ptrs+1)+*(ptrs+2))*0.33);
		ptrs+=3;
		ptrd+=1;
	  }
	}

}

// extraWidth must be difference between w and closest number that can be divided by 8
void CopyGrayscaleToRGB(byte *RGBdest, byte *source, int w, int h, int extraWidth)
{
	int x, y;
	byte *ptrs, *ptrd;
	for (y=0; y<h; y++)
	{
	  ptrs = source + (h-y-1)*w;
	  ptrd = RGBdest + y*(w+extraWidth)*3;
	  for (x=0; x<w; x++)
 	  {
		*ptrd = *ptrs;
		*(ptrd+1) = *ptrs;
		*(ptrd+2) = *ptrs;
		ptrd+=3;
		ptrs+=1;
	  }
	
	  for (x=0; x<extraWidth; x++)
	  {
		  *ptrd = 0;
		  *(ptrd+1) = 0;
		  *(ptrd+2) = 0;
		  ptrd+=3;
	  }

	}


}

void RotatePoint(float angle, int x0, int y0, int &x, int &y)
{
	int x2, y2;
	x2 = (int)(cos(angle)*(float)(x-x0) - sin(angle)*(float)(y-y0)) + x0;
	y2 = (int)(sin(angle)*(float)(x-x0) + cos(angle)*(float)(y-y0)) + y0;

	x = x2;
	y = y2;
	     	
}

void RotateImage(float angle, int x0, int y0, int iWidth, int iHeight, unsigned char *pbData, 
				 unsigned char **pbDataRows)
{
   byte *pbRotatedImage = new byte[iWidth*iHeight];
   byte **pbRotatedImageRows = new byte*[iHeight];

   memset(pbRotatedImage, 0, iWidth*iHeight);

   int i;
   int x2, y2;

   for (i=0; i<iHeight; i++)
	   pbRotatedImageRows[i] = pbRotatedImage + i*iWidth;
                   
   for (int x1 = 0; x1 < iWidth; x1++)
      for (int y1=0; y1 < iHeight; y1++)
      {
         x2 = (int)(cos(angle)*(x1-x0) - sin(angle)*(y1-y0)+x0);
         y2 = (int)(sin(angle)*(x1-x0) + cos(angle)*(y1-y0)+y0);	 
    	      
         if (x2>-1 && x2<iWidth && y2>-1 && y2<iHeight)
    	        pbRotatedImageRows[y1][x1] = pbDataRows[y2][x2];
      }

   memcpy(pbData, pbRotatedImage, iWidth*iHeight);
   delete []pbRotatedImage;
   delete []pbRotatedImageRows;
}

// returns specified rectangle area for the image
// x, y - anchoring point
// w, h - size of the new image 
// 
unsigned char* CutImage(int x, int y, int w, int h, unsigned char *data, 
				   int &iWidth, int &iHeight, bool deleteData)
{
   int i,j;
   byte *newData = new byte[w*h];

   memset(newData, 0, w*h);

   byte *ptr1, *ptr2;

   ptr1 = newData;

   for (i = 0; i < h; i++)
   {
     if ((y+i)>=iHeight || (y+i)<=0) continue;
     ptr2 = data + x + (i+y)*iWidth;

	 if ((i+y)<iHeight && (i+y)>=0)
       for (j = 0; j < w; j++)
	   {
		 if ((j+x)>=0 && (j+x)<iWidth) *ptr1 = *ptr2;
		 ptr1++;
		 ptr2++;
	   }
   }

   iWidth = w;
   iHeight = h;
   if (deleteData) delete []data;
   return newData;
}

unsigned char *ScaleImage2(double scale, unsigned char *data, int &iWidth, int &iHeight)
{
   int iWOrg = iWidth;
   iWidth = (int)((double)iWidth/scale);
   iHeight = (int)((double)iHeight/scale);

   double countX=0, countY=0;
   
   byte *newData = new byte[iWidth*iHeight];
   byte *ptr1, *ptr2;    

   ptr1 = newData;
   ptr2 = data;
   for (int j=0; j<iHeight; j++)
   {
	   for (int i=0; i<iWidth; i++)
	   {
			*ptr1 = *ptr2;
			ptr1++;
			ptr2 = data + int(countX) + (int(countY)) * iWOrg;
			countX+=scale;
	   }
	 countY+=scale;
	 countX=0;
   }
   	 

 //  delete []data;

   return newData;
}

unsigned char *ScaleImage(unsigned char *data, int &iWidth, int &iHeight)
{
   unsigned char *temp = new unsigned char[SIZE_X * SIZE_Y];
   int x, y, sum, i, j, n, m;
   float nel;
   float scaleX = (float)iWidth/(float)SIZE_X;
   float scaleY = (float)iHeight/(float)SIZE_Y;
  
   unsigned char *pos;

   for (x=0; x<SIZE_X; x++)
     for (y=0; y<SIZE_Y; y++)
     {
       sum = 0;
       nel = 0;
    
       for (j=0; j<max(1, scaleY); j++)
       {
         n = (int)(y*scaleY)+j;
         m = (int)(x*scaleX);

         if (n>=iHeight)
            n = iHeight-1;

         pos = data + n*iWidth; 

         for (i=0; i<max(1, scaleX); i++)
         {
           if (m>=iWidth) m = iWidth-1;
           sum+=(*(pos+m));
           m++;
           nel++;

         }
       }
       temp[x + y*SIZE_X] = (unsigned char)((float)sum/nel);
     }

   iWidth = SIZE_X;
   iHeight = SIZE_Y;
   return temp;
}


void ScaleImage(double scale, unsigned char *data, int &iWidth, int &iHeight, unsigned char *newData)
{
   int iWOrg = iWidth;
   iWidth = (int)((double)iWidth/scale);
   iHeight = (int)((double)iHeight/scale);

   double countX=0, countY=0;
   
// byte *newData = new byte[iWidth*iHeight];
   byte *ptr1, *ptr2;    

   ptr1 = newData;
   ptr2 = data;
   for (int j=0; j<iHeight; j++)
   {
	   for (int i=0; i<iWidth; i++)
	   {
			*ptr1 = *ptr2;
			ptr1++;
			ptr2 = data + int(countX) + (int(countY)) * iWOrg;
			countX+=scale;
	   }
	 countY+=scale;
	 countX=0;
   }
   	 

   delete []data;

// return newData;
}

int* BuildHistogram(byte *image, int w, int h)
{
	int *histogram = new int[256];
	int i, j;

	for (i=0; i<256; i++)
		histogram[i] = 0;

	for (i=0; i<w; i++)
		for (j=0; j<h; j++)
			histogram[image[i + j*w]]++;

	return histogram;

}

void EqualizeHistogram(byte *image, int w, int h)
{
	int *histogram, *cumulative;
	int i, sum, tempval;
	byte *levels;

	cumulative = new int[256];
	levels = new byte[256];
	histogram = BuildHistogram(image, w, h);

	sum = 0;
	for (i=0; i<255; i++)
	{
		sum+=histogram[i];
		tempval = (int)floor((double)(255*sum)/(double)(w*h)+0.00001);
		tempval = min(tempval, 255);
		tempval = max(tempval, 0);

		levels[i] = (byte)tempval;
	}	

	for (i=0; i<w*h; i++)
		image[i] = levels[image[i]];
 
	delete []cumulative;
	delete []levels;
	delete []histogram;
	return;
}

int kMeans2(feature *f, int nElements, int iRectangleSize, float fScaleRatio,
			int nScales)
{
    int i, j, scale, sumx, sumy, meanx, meany;
    int actualI;	
    int nCenters = 0, iAttached, iCentersUpToThisScale=0;
    bool changeMade = true, fFound = true;
    int minDistance, actualDistance, iX, iY;
    float x, y, fRectangleSize, fRectangleSize2;

    int *labels = new int[nElements];

    int *iScaleStart = new int[nScales];
    int *iScaleWidth = new int[nScales];
    int iScale = -1;


    fRectangleSize = (float)iRectangleSize;

    if (nElements==0) return 0;

    feature *centers = new feature[nElements];

    i=0;

    for (scale=0; scale<nScales; scale++)
    {
        nCenters = 0;
        sumx=0; sumy=0; iAttached=0; fRectangleSize2 = fRectangleSize/2;

        while (f[i].Sn==scale && i<nElements)
        {
            x = (float)(f[i].X);
            y = (float)(f[i].Y);
            actualI = iCentersUpToThisScale + nCenters;

            iX = (int)(floor(x/fRectangleSize)*fRectangleSize + fRectangleSize2);
            iY = (int)(floor(y/fRectangleSize)*fRectangleSize + fRectangleSize2);

            fFound = false;
            for (j=iCentersUpToThisScale; j<(iCentersUpToThisScale+nCenters); j++)
                if (centers[j].X == iX && centers[j].Y == iY)
                {
                    j = iCentersUpToThisScale+nCenters;
                    fFound = true;
                }

                if (fFound==false) 
                {
                    centers[actualI].X = iX;
                    centers[actualI].Y = iY;
                    centers[actualI].Sn = scale;
                    centers[actualI].F = false;
                    centers[actualI].S = f[i].S;
                    nCenters++;
                }

                i++;

        }

        iCentersUpToThisScale+=nCenters;
        fRectangleSize*=fScaleRatio;

    }

    nCenters = iCentersUpToThisScale;

    for (i=0; i<nCenters; i++)
    {
        if (centers[i].Sn!=iScale)
        {
            iScale = centers[i].Sn;
            iScaleStart[iScale] = i;
            iScaleWidth[iScale] = 0;
        }
        iScaleWidth[iScale]++;
    }

    while (changeMade)
    {
        for (i=0; i<nElements; i++)
        {
            iX = f[i].X;
            iY = f[i].Y;
            iScale = f[i].Sn;
            minDistance=99999999;

            for (j=iScaleStart[iScale]; j<(iScaleStart[iScale]+iScaleWidth[iScale]); j++)
                if ((actualDistance=(abs(iX-centers[j].X) + abs(iY-centers[j].Y)))<minDistance)
                {
                    minDistance = actualDistance;
                    labels[i] = j;
                }
        }

        changeMade = false;

        for(i=0; i<nCenters; i++)
        {
            iAttached = 0;
            sumx = 0;  sumy = 0;
            for (j=0; j<nElements; j++)
                if (labels[j]==i)
                {
                    iAttached++;
                    sumx+=f[j].X; 
                    sumy+=f[j].Y;
                }

                if (iAttached!=0)
                {
                    meanx = sumx/iAttached;
                    meany = sumy/iAttached;

                    if (meanx!=centers[i].X || meany!=centers[i].Y)
                    {
                        changeMade = true;
                        centers[i].X = meanx;
                        centers[i].Y = meany;
                    }
                }
        }
    }

    memcpy(f, centers, nCenters*sizeof(feature));

	delete []iScaleStart;
	delete []iScaleWidth;

	delete []centers;
    delete []labels;
    return nCenters;
}

int Distance(feature f1, feature f2)
{
    return (int)sqrt(float((f1.X-f2.X)*(f1.X-f2.X) + (f1.Y-f2.Y)*(f1.Y-f2.Y)));
}

float DistanceExact(feature &f1, feature &f2)
{
    return sqrt(float((f1.X-f2.X)*(f1.X-f2.X) + (f1.Y-f2.Y)*(f1.Y-f2.Y)));
}


int Distance2(feature f1, feature f2)
{
    return (f1.X-f2.X)*(f1.X-f2.X) + (f1.Y-f2.Y)*(f1.Y-f2.Y);
}

int Distance(int x1, int y1, int x2, int y2)
{
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}

float DistanceExact(int x1, int y1, int x2, int y2)
{
    return sqrt((float)((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)));
}

float Distance(float x1, float y1, float x2, float y2)
{
    return (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
}



/*
int kMeans(feature *f, int nElements, int iRectangleSize, int iWidth, int iHeight, float fScaleRatio,
		   int nScales)
{
    int i, j, scale, k, sumx, sumy, meanx, meany;	
    int nCenters = 0, iAttached;
    bool changeMade = true;
    byte flag=0;
    int minDistance, actualDistance;
    float s=0;

    if (nElements==0) return 0;

    feature *tempCenters = new feature[nElements];

	for (scale=0; scale<nScales; scale++)
	{
		for (i=0; i<iWidth; i+=iRectangleSize)
			for (j=0; j<iHeight; j+=iRectangleSize)
			{
				sumx=0; sumy=0; iAttached = 0;

				for (k=0; k<nElements; k++)
					if (f[k].Sn==scale && f[k].X>i && f[k].X<(i+iRectangleSize)
						&& f[k].Y>j && f[k].Y<(j+iRectangleSize))
					{
						iAttached++;
						sumx+=f[k].X;
						sumy+=f[k].Y;
						s = f[k].S;
						flag = f[k].F;
					}

					if (sumx>0 && sumy>0)
					{
						tempCenters[nCenters].X = sumx/iAttached;
						tempCenters[nCenters].Y = sumy/iAttached;
						tempCenters[nCenters].Sn = scale;
						tempCenters[nCenters].S = s;
						if (flag==1)
							tempCenters[nCenters].F = true;
						else tempCenters[nCenters].F = false;

						nCenters++;
					}
			}
			// for the moment without scaling (for comparison)
			iRectangleSize=(int)(iRectangleSize*fScaleRatio);
	}

    int *labels = new int[nElements];
    feature *centers = new feature[nCenters];
    for (i=0; i<nCenters; i++) centers[i]=tempCenters[i];

    delete []tempCenters;

    while (changeMade)
    {
        for (i=0; i<nElements; i++)
        {
            minDistance=99999999;
            for (j=0; j<nCenters; j++)
                if ((f[i].Sn==centers[j].Sn) &&
                    (actualDistance=Distance(f[i].X, f[i].Y, centers[j].X, centers[j].Y))<minDistance)
                {
                    minDistance = actualDistance;
                    labels[i] = j;
                }
        }

        changeMade = false;

        for(i=0; i<nCenters; i++)
        {
            iAttached = 0;
            sumx = 0;  sumy = 0;
            for (j=0; j<nElements; j++)
                if (labels[j]==i)
                {
                    iAttached++;
                    sumx+=f[j].X; 
                    sumy+=f[j].Y;
                }

                if (iAttached!=0)
                {
                    meanx = sumx/iAttached;
                    meany = sumy/iAttached;

                    if (meanx!=centers[i].X || meany!=centers[i].Y)
                    {
                        changeMade = true;
                        centers[i].X = meanx;
                        centers[i].Y = meany;
                    }
                }
        }
    }

    for (i=0; i<nCenters; i++) f[i] = centers[i];

    delete []centers;
    delete []labels;
    return nCenters;
}
*/
void FindIntegralImage(unsigned char *img,int xres,int yres,int **iimage)
{		
    int y_pos, y;

    for(int x=0;x<=xres;x++) iimage[0][x]=0;
    for(y=0;y<=yres;y++) iimage[y][0]=0;		
    for(y=1;y<=yres;y++) {
        y_pos=(y-1)*xres;			
        for(int x=1;x<=xres;x++) iimage[y][x]=iimage[y][x-1]+iimage[y-1][x]-iimage[y-1][x-1]+img[x-1+y_pos];
    }		

}


