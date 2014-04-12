#include "stdafx.h"
#include "ImageProcessing.h"

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

ImageProcessing::ImageProcessing()
{
	ZeroVariables();
	// add initializations here
}

void ImageProcessing::Mode1Processing(unsigned char *rgb_image, int w, int h)
{
	unsigned char R=0, G=255, B=0;
	if (last_rect.top!=-1)
		DrawRectangleRGB(last_rect, 0, 0, rgb_image, w, h, B, G, R);
}

void ImageProcessing::Mode2Processing(unsigned char *rgb_image, int w, int h)
{
	int deltaW = static_cast<int>(w/16);
	int deltaH = static_cast<int>(h/16) - 1;
	for (int i = 0; i < 16; ++i) {
		for (int k = 0; k < 16; ++k) {
			for (int j = i*deltaH; j < (i+1)*deltaH; ++j) {
				for (int m = k*deltaW; m < (k+1)*deltaW; ++m) {
					DrawPixel(m, j, w, h, rgb_image, LNorm [i] [k] * 255, LNorm [i] [k] * 255, LNorm [i] [k] * 255);
				}
			}		
		}
	}
	/*unsigned char R=0, G=0, B=0;

	unsigned char **rgb_pixels = new unsigned char*[h];

	// assign every rgb_pixels[y] to one row
	for (int y=0; y<h; y++)
		rgb_pixels[y] = rgb_image + y*w*3;

	// setting BGR pixel map
	for (int x=0; x<w; x++)
		for (int y=0; y<h; y++)
		{
			DrawPixel(x, y, w, h, rgb_image, B, G, R);

			B++;
			if (B==255) { B=0; G++; }
			if (G==255) { G=0; R++; }
			if (R==255) R=0;
		}

	delete []rgb_pixels;*/
}

void ImageProcessing::Mode3Processing(unsigned char *rgb_image, int w, int h)
{
	bool** binImg;
	binImg = new bool*[h + 2];
	for(int i = 0; i < h + 2; ++i) {
		binImg[i] = new bool[w + 2];
		for (int j = 0; j < w + 2; ++j)
			binImg[i][j] = 0;
	}
	short **et;
	et = new short*[h + 2];
	for (int i = 0; i < h + 2; ++i) {
		et[i] =  new short[w + 2];
		for (int j = 0; j < w + 2; ++j)
			et[i][j] = 0;
	}
	//tworzenie obrazu binarnego
	unsigned char R, G, B;
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			GetPixelRGB(x, y, rgb_image, w, h, R, G, B);
			float r = static_cast<float>(R)/(static_cast<float>(R) + static_cast<float>(G) + static_cast<float>(B));
			float b = static_cast<float>(B)/(static_cast<float>(R) + static_cast<float>(G)  + static_cast<float>(B));
			int ir = static_cast<int>(min(15,r*16));
			int ib = static_cast<int>(min(15,b*16));

			if(LNorm[ib][ir] * 255 > yt) 
				binImg [y + 1] [x + 1] = 1;
			//DrawPixel(x, y, w, h, rgb_image, binImg [y] [x] * 255, binImg [y] [x] * 255, binImg [y] [x] * 255);
		}
	}
//nadawanie etykiet
//pierwsza maska
	for (int i = 1; i < h + 1; ++i) {
		for (int j = 1; j < w + 1; ++j) {
			if(binImg[i][j]) { //jesli piksel jest czarny
				///////////////////////////
				if (binImg[i-1][j-1] || binImg[i-1][j] || binImg[i-1][j+1] || binImg[i][j-1]) {
					unsigned short min = currE + 1;
					if(et[i-1][j-1] && (et[i-1][j-1] < min)) 
						min = et[i-1][j-1];
					if(et[i-1][j] && (et[i-1][j] < min)) 
						min = et[i-1][j];
					if(et[i-1][ j + 1] && (et[i-1][ j+1] < min)) 
						min = et[i-1][ j + 1];
					if(et[i][j-1] && (et[i][j -1] < min)) 
						min = et[i][j -1];
					et[i][j] = min;
				}
				else
					et[i][j] = ++currE;
			}
		}
	}

//laczenie obszarow wspolnych
	bool change = true;
	while(change) {
		change = false;
		for (int i = 1; i < h+1; ++i) {
			for (int j = 1; j < w+1; ++j) {
				if(et[i][j]) { //jesli piksel jest czarny
					unsigned short min = et[i][j];
					if(et[i-1][j] && et[i-1][j] < min)
						min = et[i-1][j];
					if(et[i-1][j-1] && et[i-1][j-1] < min)
						min = et[i-1][j-1];
					if(et[i][j-1] && et[i][j-1] < min)
						min = et[i][ j-1];
					if(et[i+1][j] && et[i+1][j] < min)
						min = et[i+1][j];
					if(et[i+1][j-1] && et[i+1][j-1] < min)
						min = et[i+1][j-1];
					if(et[i-1][ j + 1] && et[i-1][ j + 1] < min)
						min = et[i-1][ j + 1];
					if(et[i][j+1] && et[i][j+1] < min)
						min = et[i][j+1];
					if(et[i+1][j+1] && et[i+1][j+1] < min)
						min = et[i+1][j+1];				
					if(min < et[i][j]) {
						et[i][j] = min;
						change = true;
					}
				}
			}
		}
	}
	
	//szukanie etykiety najwiekszego obszaru
	unsigned short maxEt = 0;
	unsigned short maxX = 0;
	unsigned short minX = w;
	unsigned short maxY = 0;
	unsigned short minY = h;
	unsigned short* etNum = new unsigned short[currE+2];
	for (int i = 0; i < currE+2; ++i)
		etNum[i] = 0;

	for (int i = 1; i < h + 1; ++i) {
			for (int j = 1; j < w + 1; ++j) {
				if(et[i][j]) 
					etNum[et[i][j]] ++;
			}
	}
	
	for (int i = 0; i < currE + 2; ++i) {
		if(etNum[i] > etNum[maxEt])
			maxEt = i;
	}

	for (int i = 1; i < h + 1; ++i) {
			for (int j = 1; j < w + 1; ++j) {
				if(et[i][j] == maxEt) {
					if(i < minY)
						minY = i;
					if (i > maxY)
						maxY = i;
					if( j < minX)
						minX = j;
					if (j > maxX)
						maxX = j;
				}
			}
	}
	R=0, G=255, B=0;
	CRect rect;
	rect.SetRect(minX, minY, maxX, maxY);

	if (rect.top!=-1)
		DrawRectangleRGB(rect, 0, 0, rgb_image, w, h, B, G, R);
	currE = 0;
	
	for(int i = 0; i < h + 2; ++i) {
		delete binImg[i];
		delete et[i];
	}
	delete binImg;
	delete et;
	delete []etNum;

}

void ImageProcessing::ProcessRGBImage(int mode, unsigned char *rgb_image, int w, int h)
{
	switch (mode)
	{
	case 0	: Mode1Processing(rgb_image, w, h);
		break;
	case 1	: Mode2Processing(rgb_image, w, h);
		break;
	case 2	: Mode3Processing(rgb_image, w, h);
		break;
	}
}

void ImageProcessing::ServiceDrawnRectangle(CRect rect, unsigned char *rgb_image, int w, int h)
{
	last_rect = rect;
	
	// tutaj umieœæ kod co program ma wykonaæ w momencie zaznaczanie prostok¹ta

	++K;
	//tworzenie histogramu nieznormalizowanego
	unsigned char R, G, B;	
	float sum = 0;
	for (int i = last_rect.top + 2; i < last_rect.bottom - 1; ++i) { //od gory do dolu
		for (int k = last_rect.left + 2; k < last_rect.right - 1; ++k) { //od lewej do prawej
			GetPixelRGB(k, i, rgb_image, w, h, R, G, B);
			float r = static_cast<float>(R)/(static_cast<float>(R) + static_cast<float>(G) + static_cast<float>(B));
			float b = static_cast<float>(B)/(static_cast<float>(R) + static_cast<float>(G)  + static_cast<float>(B));

			int ir = static_cast<int>(min(15,r*16));
			int ib = static_cast<int>(min(15,b*16));

			L[ib] [ir] += 1;
			sum ++;
		}
	}
	for (int i = 0; i < 16; ++i) {
		for (int k = 0; k < 16; ++k) {
			LNorm [i] [k] = L [i] [k] / sum;
		}
	}
	//ustalanie progu
	yt = 0.7*sum / 255; 
}

void ImageProcessing::ZeroVariables()
{
	last_rect.SetRect(-1, -1, -1, -1);
	// assign initial variables values 
	yt = 0;
	K = 0;
	currE = 0;
	for (int i = 0; i <16; ++i) {
		for (int k = 0; k < 16; ++k) {
			L[i][k] = 0;
			LNorm[i][k] = 0;
		}
	}
}

ImageProcessing::~ImageProcessing()
{
	// delete all allocated memory
}
