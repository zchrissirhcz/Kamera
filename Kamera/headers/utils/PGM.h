unsigned char *ReadPGM(char *name, int &w, int &h, bool adjustToEightMultiple);
bool WritePGM(char *name, int w, int h, unsigned char *data);
bool WritePGM(CString name, int w, int h, unsigned char *data);

int GetMagicNumber(int n);

