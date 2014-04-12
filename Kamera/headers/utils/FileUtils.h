#ifndef FILEUTILS
#define FILEUTILS

#include "afxwin.h"
#include "atlimage.h"

void SaveJPG(CString filename, unsigned char *data, int iWidth, int iHeight);
void LoadJPG(CString filename, unsigned char **data, int &iWidth, int &iHeight);
void skipLine(FILE *f);
bool CheckFileExistence(CString filename);
bool CheckFileExistence(char *filename);
CString* loadAllClassifiersFileNames(char *path, int &n_files);
CString* loadAllFileNames(char *path, char *filter, int &n_files, bool full_name);
int ReadCoordinates(CString filename, int *pointsx, int *pointsy);
void WriteToTmp1(char* filename, float *buffer, int element_size, bool printZeros, int n);
void MarkCodePoint(char *label, int l=0); // write to the c:\label.txt label 


#endif
