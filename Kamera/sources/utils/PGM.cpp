#include "stdafx.h"

#include "string.h"
#include "stdio.h"
#include "PGM.h"
#include <atlstr.h>


int GetMagicNumber(int n)
{
	int i=8;
	while (i<n) i+=8;
	return i;
}

bool WritePGM(CString name, int w, int h, unsigned char *data)
{
	bool returnVal;
	returnVal = WritePGM(name.GetBuffer(), w, h, data);
	name.ReleaseBuffer();
	return returnVal;
}

bool WritePGM(char *name, int w, int h, unsigned char *data)
{
  FILE *f;
  fopen_s(&f, name, "wb");

  if (f==NULL) return false;

  CString header;

  header.Format("P5 %d %d %d ", w, h, 255);
  fwrite(header.GetBuffer(), header.GetLength(), 1, f); 

  fwrite(data, w*h, 1, f);

  fclose(f);

  return true;

}

unsigned char *ReadPGM(char *name, int &w, int &h, bool adjustToEightMultiple) 
{
  FILE *f;
  char head[30];
  unsigned char *image;
  int i;
  int gray;

  if (fopen_s(&f, name, "rb")!=0) return NULL;
  
  fread(head, 1, 26, f);

// check if InfranView converted
  if (head[3]=='#' && head[5]=='C')
     fscanf_s(f, "%d %d %d", &w, &h, &gray);
  else 
  {
    rewind(f);
    fscanf_s(f, "%s %d %d %d", head, 30, &w, &h, &gray);
  }

  if (w<1 || h<1 || gray!=255) return NULL;

  image = new unsigned char[w*h];

  fread(&i, 1, 1, f);
  fread(image, 1, w*h, f);

  fclose(f);

  if (adjustToEightMultiple)
  {
	  // width and height must be 8 bytes multiple (Win bug?)
	  int w2, h2;
	  w2 = GetMagicNumber(w);
	  h2 = GetMagicNumber(h);

	  unsigned char *image2 = new unsigned char[w2*h2];
	  memset(image2, 0, w2*h2);

	  int j;
	  for (i=0; i<w; i++)
		  for (j=0; j<h; j++)
			  image2[i+j*w2] = image[i+j*w];

	  w = w2;
	  h = h2;
	  delete []image;

	  return image2;
  } else return image;
}


