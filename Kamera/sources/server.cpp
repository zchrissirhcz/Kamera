#include "stdafx.h"
#include "..\Detektor DCCDlg.h"
#include "SocketCS.h"

#define		GET				2
#define		NORM			1
#define		WRONG_REQUEST	-1

using namespace std;

DWORD WINAPI Server(void* data) {

    CDetektorDCCDlg* dlg = (CDetektorDCCDlg*)(data);

	int request;

	char tmp[256];
    SocketServer ss(4000, 1);
    Socket *socket;

	unsigned char normalizedImage[NORM_IMAGE_W*NORM_IMAGE_H];
	memset(normalizedImage, 0 , NORM_IMAGE_W*NORM_IMAGE_H);

    while(1)
    {
        socket = ss.Accept();

        std::string receivedString;
        std::string sentString;

        CString dataString;


        while (1)
        {
            receivedString = socket->ReceiveLine();
            if (receivedString.empty()) break;

			if (receivedString.find("GET", 0 )==0) request = GET;
			else if (receivedString.find("NORM", 0 )==0) request = NORM;
			else request = WRONG_REQUEST;
  
			switch (request)
			{
			case GET :
				// get the number of faces and store current normalized image of the largest face
                while (TryEnterCriticalSection(&(dlg->CS_normalizedImage))==0) Sleep(1);

				sprintf_s(tmp, 256, "%d\n", dlg->fd->nFacesDetected);
				socket->SendBytes(tmp, (int)strlen(tmp));
			
				memcpy(normalizedImage, dlg->fd->normalized_image, NORM_IMAGE_W*NORM_IMAGE_H);
				LeaveCriticalSection(&(dlg->CS_normalizedImage));
				break;

			case NORM :
				// send stored normalized image (saved at last GET command)
				socket->SendBytes((char*)normalizedImage, NORM_IMAGE_W*NORM_IMAGE_H);
 				break;
			}

			if (dlg->suspendServerThread)
			{
				dlg->serverThreadSuspended = true;
				while (dlg->suspendServerThread) Sleep(1);
				dlg->serverThreadSuspended = false;
			} 
			request = NULL;
		}
	}

	return 0;
}
