#include "stdafx.h"
#include "FileUtils.h"
#include "structs.h"

BITMAPINFO* PrepareBitmapInfo(int w, int h)
{
	BITMAPINFO *bip;
    bip = (BITMAPINFO*) new unsigned char[sizeof(BITMAPINFOHEADER)];
    bip->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bip->bmiHeader.biWidth = w;
    bip->bmiHeader.biHeight = h;
    bip->bmiHeader.biPlanes = 1;
    bip->bmiHeader.biBitCount = 24;
    bip->bmiHeader.biCompression = BI_RGB;
    bip->bmiHeader.biSizeImage = 0;
    bip->bmiHeader.biXPelsPerMeter = 0;
    bip->bmiHeader.biYPelsPerMeter = 0;
    bip->bmiHeader.biClrUsed = 0;
    bip->bmiHeader.biClrImportant = 0;

	return bip;
}

void SaveJPG(CString filename, unsigned char *data, int iWidth, int iHeight)
{
	CImage im;

	BITMAPINFO *bip = PrepareBitmapInfo(iWidth, iHeight);

	im.Create(iWidth, iHeight, 24);
	HDC hdc = im.GetDC();

	StretchDIBits(hdc, 0, 0, iWidth, iHeight, 0, 0, iWidth, iHeight, 
				  data, bip, DIB_RGB_COLORS, SRCCOPY);

	im.Save(filename);
	im.ReleaseDC();
	delete bip;
}

void LoadJPG(CString filename, unsigned char **data, 
			 int &iWidth, int &iHeight)
{
	CImage im;
    int pitch, bpp;
    byte *pdata;

    im.Load(filename);

    pitch = im.GetPitch();
    pdata = (byte*)im.GetBits();

    iWidth = im.GetWidth();
    iHeight = im.GetHeight();

    bpp = abs(pitch/iWidth);
    (*data) = new unsigned char[iWidth*iHeight*3];


    for (int i=0; i<iHeight; i++)
    {
        pdata = (byte*)im.GetBits() + pitch*i;
        for (int j=0; j<iWidth; j++) 
        {
            (*data)[((iHeight-1-i)*iWidth)*3 + 3*j] = *pdata;
            (*data)[((iHeight-1-i)*iWidth)*3 + 3*j+1] = *(pdata+1);
            (*data)[((iHeight-1-i)*iWidth)*3 + 3*j+2] = *(pdata+2);
/*            (*data)[(i*iWidth)*3 + 3*j] = *pdata;
            (*data)[(i*iWidth)*3 + 3*j+1] = *(pdata+1);
            (*data)[(i*iWidth)*3 + 3*j+2] = *(pdata+2);
  */        pdata+=bpp;
        }
    }
}

void skipLine(FILE *f)
{
	int c = -1;

	while (c!='\n')
		c = getc(f);
}

bool CheckFileExistence(CString filename)
{
	FILE *f;

	if (fopen_s(&f, filename, "rt")!=0)
		return false;

	fclose(f);
	return true;
}

bool CheckFileExistence(char *filename)
{
	FILE *f;

	if (fopen_s(&f, filename, "rt")!=0)
		return false;

	fclose(f);
	return true;
}

void WriteToTmp1(char* filename, float *buffer, int element_size, bool printZeros, int n)
{
	float *tmp;
	FILE *f;
	fopen_s(&f, filename, "wt");

	tmp = buffer;

	int cnt=0;
	for (int el=0; el<n; el++)
	{	
		if (el%element_size==0) 
		{
			fprintf(f, "\n\n");
			cnt = 0;
		}
		if (printZeros || tmp[el]!=0) 
		{
			fprintf(f, "%f ", /*cnt, */tmp[el]);
			cnt++;
		}
	}

	fclose(f);
}



int ReadCoordinates(CString filename, int *pointsx, int *pointsy)
{
	int xcounter = 0;

	filename.Replace(".pgm", ".txt");
	filename.Replace(".jpg", ".txt");

	FILE *f;

	fopen_s(&f, filename, "rt");

	if (f!=NULL)
	{
	  for (int i=0; i<N_POINTS; i++)
	    if (fscanf_s(f, "%d %d ", &pointsx[i], &pointsy[i])==EOF)
		{
			pointsx[i] = -1;
			pointsy[i] = -1;

			xcounter = 0;
		} else xcounter++;
	  fclose(f);
	} else 
	for (int i=0; i<N_POINTS; i++)
	{
		pointsx[i] = -1;
		pointsy[i] = -1;
	}

	return xcounter;
}


CString* loadAllClassifiersFileNames(char *path, int &n_files)
{
	CString *sFileNames;
	WIN32_FIND_DATA fdata;
	HANDLE hnd;
	bool found = true;
	SetCurrentDirectory(path);
	char tmp[255];

	n_files=0;

// finding number of files
	hnd = FindFirstFile("cascade*.scs", &fdata);
	if (hnd == INVALID_HANDLE_VALUE){ return 0; }

	while (found)
	{
		n_files++;
		found = (FindNextFile(hnd, &fdata)!=0);
	}
	found = true;
	sFileNames = new CString[n_files];

// loading filenames 
	n_files = 0;
	hnd = FindFirstFile("cascade*.scs", &fdata);

	while (found)
	{
		sprintf_s(tmp, "cascade_stage%d.scs",  n_files);
		sFileNames[n_files] = tmp;
		n_files++;
		found = (FindNextFile(hnd, &fdata)!=0);
	}


	return sFileNames;
}


CString* loadAllFileNames(char *path, char *filter, int &n_files, bool full_name)
{
	CString *sFileNames;
	WIN32_FIND_DATA fdata;
	HANDLE hnd;
	bool found = true;
	SetCurrentDirectory(path);

	n_files=0;

// finding number of files
	hnd = FindFirstFile(filter, &fdata);
	if (hnd == INVALID_HANDLE_VALUE){ return NULL; }

	while (found)
	{
		n_files++;
		found = (FindNextFile(hnd, &fdata)!=0);
	}

	if (n_files==0) return NULL;

	found = true;

	sFileNames = new CString[n_files];

// loading filenames 
	n_files = 0;
	hnd = FindFirstFile(filter, &fdata);

	while (found)
	{
		if (full_name) sFileNames[n_files] = path; else sFileNames[n_files].Format("");
		sFileNames[n_files].Append(fdata.cFileName);
		n_files++;
		found = (FindNextFile(hnd, &fdata)!=0);
	}


	return sFileNames;
}


void MarkCodePoint(char *label, int l) // write to the c:\label.txt label 
{
	char tmp[256];
	sprintf_s(tmp, 256, "c:\\label_%d.txt", l);
	FILE *f;
	fopen_s(&f, tmp, "wt");

	fprintf(f, "%s", label);

	fclose(f);
}
