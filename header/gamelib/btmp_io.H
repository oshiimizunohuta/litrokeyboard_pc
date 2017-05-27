LPBITMAPINFOHEADER adr_BInfoH;
LPBYTE adr_BMP,adr_BPixel;
LPDWORD PixData[1028];

struct BMPDATA{
	BITMAPINFO pdinfo;

	HBITMAP pdhBMP;
	HDC pdhdc;
	LPDWORD lppd;
}BMPPP;

/*BMP�o�^�[BMP�ǉ����K�{*/
char *BFName[3] = {"BMP.DAT"};
/*********/

/*���L�pDIB�錾�[BMP�ǉ����K�{*/
LPBITMAPINFO adr_BInfo;
LPDWORD adr_Pixel;
/***********/

/*�L�����N�^�[�Ǎ���*/

void bmp_regist(void){
int i,j,k,l, FNo, xItimasu=32, yItimasu=32, xNanmasu=4, yNanmasu=4;;
		getBMPAddress();

		/*32��؂�}�X*/
		xNanmasu = iWidth / 32; xItimasu = iWidth / xNanmasu;
		yNanmasu = iHeight / 32; yItimasu = iHeight / yNanmasu;
		/************/
		/* DIB�p�o�b�t�@���m�� */
		lpDIB32 = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		         sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * xNanmasu * yNanmasu));

		/* DIB�p�|�C���^���z �@BMP�ǉ����K�{*/
		lpbiInfo[FNo] = (LPBITMAPINFO)lpDIB32;

		for(i = 0; i < xNanmasu * yNanmasu; i++){
			switch(FNo){
			case 0:
			chrPix[i] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			break;

			case 1:
			mapChip[i] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			break;

			case 2:
			enePix[i] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			break;

			default: break;
			}
		}
		/* BITMAPINFO�ݒ� */

		lpbiInfo[FNo]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiInfo[FNo]->bmiHeader.biWidth = xItimasu;
		lpbiInfo[FNo]->bmiHeader.biHeight = yItimasu;
		lpbiInfo[FNo]->bmiHeader.biPlanes = 1;
		lpbiInfo[FNo]->bmiHeader.biBitCount = 32;
		lpbiInfo[FNo]->bmiHeader.biCompression = BI_RGB;

		/* BMP���̃s�N�Z�����32�r�b�g�����ăR�s�[ BMP�ǉ����K�{*/
for (l = 0 ; l < yNanmasu ; l++){////y�}�X���}�X

	for (k = 0;k < xNanmasu; k++){////x�}�X���}�X

		for (i = 0;i < yItimasu; i++){////�c�s�N�Z��

			for (j = 0;j < xItimasu; j++){////���s�N�Z��

				switch(FNo){
				case 0:
				CopyMemory(chrPix[k + (l * xNanmasu)] + j + i * xItimasu,
				     lpBMPPixel + 3 * (j + i * iWidth + k * xItimasu + (yNanmasu-1 - l) * yItimasu * iWidth), 3);
				break;

				case 1:
				CopyMemory(mapChip[k + (l * xNanmasu)] + j + i * xItimasu,
				     lpBMPPixel + 3 * (j + i * iWidth + k * xItimasu + (yNanmasu-1 - l) * yItimasu * iWidth), 3);
				break;

				case 2:
				CopyMemory(enePix[k + (l * xNanmasu)] + j + i * xItimasu,
				     lpBMPPixel + 3 * (j + i * iWidth + k * xItimasu + (yNanmasu-1 - l) * yItimasu * iWidth), 3);
				break;

				default: break;

				}
				}}}}
		/*MessageBox(NULL,"1�}�X�ǂݍ��݊�","�����[",MB_OK);*/

		/* �t�@�C���ǂݍ��݃o�b�t�@��� */
		HeapFree(GetProcessHeap(), 0, lpBMP);

	//	MessageBox(NULL,"BMP�ǂݍ��݊�",BMPName[FNo],MB_OK);
	}
/*1�I��*/

}

LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			break;

			case 1:
			mapChip[i] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			break;

			case 2:
			enePix[i] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			break;

			default: break;
			}
		}
		/* BITMAPINFO�ݒ� */

		lpbiInfo[FNo]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiInfo[FNo]->bmiHeader.biWidth = xItimasu;
		lpbiInfo[FNo]->bmiHeader.biHeight = yItimasu;
		lpbiInfo[FNo]->bmiHeader.biPlanes = 1;
		lpbiInfo[FNo]->bmiHeader.biBitCount = 32;
		lpbiInfo[FNo]->bmiHeader.biCompression = BI_RGB;

		/* BMP���̃s�N�Z�����32�r�b�g�����ăR�s�[ BMP�ǉ����K�{*/
for (l = 0 ; l < yNanmasu ; l++){////y�}�X���}�X

	for (k = 0;k < xNanmasu; k++){////x�}�X���}�X

		for (i = 0;i < yItimasu; i++){////�c�s�N�Z��

			for (j = 0;j < xItimasu; j++){////���s�N�Z��

				switch(FNo){
				case 0:
				CopyMemory(chrPix[k + (l * xNanmasu)] + j + i * xItimasu,
				     lpBMPPixel + 3 * (j + i * iWidth + k * xItimasu + (yNanmasu-1 - l) * yItimasu * iWidth), 3);
				break;

				case 1:
				CopyMemory(mapChip[k + (l * xNanmasu)] + j + i * xItimasu,
				     lpBMPPixel + 3 * (j + i * iWidth + k * xItimasu + (yNanmasu-1 - l) * yItimasu * iWidth), 3);
				break;

				case 2:
				CopyMemory(enePix[k + (l * xNanmasu)] + j + i * xItimasu,
				     lpBMPPixel + 3 * (j + i * iWidth + k * xItimasu + (yNanmasu-1 - l) * yItimasu * iWidth), 3);
				break;

				default: break;

				}
				}}}}
		/*MessageBox(NULL,"1�}�X�ǂݍ��݊�","�����[",MB_OK);*/

		/* �t�@�C���ǂݍ��݃o�b�t�@��� */
		HeapFree(GetProcessHeap(), 0, lpBMP);

	//	MessageBox(NULL,"BMP�ǂݍ��݊�",BMPName[FNo],MB_OK);
	}
/*1�I��*/


	/*2*/

		fh = CreateFile("suuji.bmp", GENERIC_READ, 0, NULL,
		        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		/**/
		xItimasu = 8; yItimasu = 8; xNanmasu = 10;
		/**/

		if (fh == INVALID_HANDLE_VALUE) {

			MessageBox(NULL, "bmp��������܂���B", "�G���[", MB_OK);

			return 0;
		}

		/* �t�@�C���T�C�Y�擾 */
		iFileSize = GetFileSize(fh, NULL);

		/* �t�@�C���ǂݍ��݃o�b�t�@�m�� */
		lpBMP = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, iFileSize);

		/* �t�@�C���ǂݍ��� */
		ReadFile(fh, lpBMP, iFileSize, &dwRead, NULL);

		/* �t�@�C������� */
		CloseHandle(fh);

		/* BMP����BITMAPINFO�擾 */
		lpbiBMPInfo = (LPBITMAPINFOHEADER)
		              (lpBMP + sizeof(BITMAPFILEHEADER));

		/* �擪����s�N�Z����܂ł̃I�t�Z�b�g�擾 */
		dwOffset = *(LPDWORD)(lpBMP + 10);

		/* BMP���s�N�Z����̐擪�A�h���X�v�Z */
		lpBMPPixel = lpBMP + dwOffset;

		/* �r�b�g�}�b�v�̑傫���擾 */
		iWidth = lpbiBMPInfo->biWidth;
		iHeight = lpbiBMPInfo->biHeight;

		/* DIB�p�o�b�t�@���m�� */
		lpDIB8 = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		         sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * xNanmasu));

		/* DIB�p�|�C���^���z */

		lpbiInfo8 = (LPBITMAPINFO)lpDIB8;

		for(i = 0; i < xNanmasu ; i++){

			NUM[i] = (LPDWORD)(lpDIB8 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));}

		/* BITMAPINFO�ݒ� */

		lpbiInfo8->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiInfo8->bmiHeader.biWidth = xItimasu;
		lpbiInfo8->bmiHeader.biHeight = yItimasu;
		lpbiInfo8->bmiHeader.biPlanes = 1;
		lpbiInfo8->bmiHeader.biBitCount = 32;
		lpbiInfo8->bmiHeader.biCompression = BI_RGB;

	

		/* BMP���̃s�N�Z�����32�r�b�g�����ăR�s�[ */

	for (k = 0;k < xNanmasu; k++){////�}�X���}�X

		for (i = 0;i < yItimasu; i++){////�c�s�N�Z��

			for (j = 0;j < xItimasu; j++){////���s�N�Z��

				CopyMemory(NUM[k] + j + i * xItimasu,
				     lpBMPPixel + 3 * (j + i * iWidth + k * xItimasu), 3);
				}}}

	/* �t�@�C���ǂݍ��݃o�b�t�@��� */
	HeapFree(GetProcessHeap(), 0, lpBMP);
/*2�I��*/

/*3*/	
	/* �t�@�C���I�[�v�� */

	for(BNo=0; BNo<1; BNo++){

		fh = CreateFile(BMPMenu[BNo], GENERIC_READ, 0, NULL,
		        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (fh == INVALID_HANDLE_VALUE) {

			MessageBox(NULL, "bmp��������܂���B", "�G���[", MB_OK);

			return 0;
		}

		/* �t�@�C���T�C�Y�擾 */
		iFileSize = GetFileSize(fh, NULL);

		/* �t�@�C���ǂݍ��݃o�b�t�@�m�� */
		lpBMP = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, iFileSize);

		/* �t�@�C���ǂݍ��� */
		ReadFile(fh, lpBMP, iFileSize, &dwRead, NULL);

		/* �t�@�C������� */
		CloseHandle(fh);

		/* BMP����BITMAPINFO�擾 */
		lpbiBMPInfo = (LPBITMAPINFOHEADER)
		              (lpBMP + sizeof(BITMAPFILEHEADER));

		/* �擪����s�N�Z����܂ł̃I�t�Z�b�g�擾 */
		dwOffset = *(LPDWORD)(lpBMP + 10);

		/* BMP���s�N�Z����̐擪�A�h���X�v�Z */
		lpBMPPixel = lpBMP + dwOffset;

		/* �r�b�g�}�b�v�̑傫���擾 */
		iWidth = lpbiBMPInfo->biWidth;
		iHeight = lpbiBMPInfo->biHeight;

		/*64*16��؂�}�X*/
		xNanmasu = iWidth / 64; xItimasu = iWidth / xNanmasu;
		yNanmasu = iHeight / 16; yItimasu = iHeight / yNanmasu;
		/************/
		/* DIB�p�o�b�t�@���m�� */
		lpDIBMenu = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		         sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * xNanmasu * yNanmasu));

		/* DIB�p�|�C���^���z */
		lpbiInfoMenu[BNo] = (LPBITMAPINFO)lpDIBMenu;

		for(i = 0; i < xNanmasu * yNanmasu; i++){
			switch(BNo){
			case 0:
			dMenu[i] = (LPDWORD)(lpDIBMenu + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));

			case 1:
			//disp[i] = (LPDWORD)(lpDIBMenu + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			break;

		/*	case 1:
			mapChip[i] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			break;*/

			default: break;
			}
		}
		/* BITMAPINFO�ݒ� */

		lpbiInfoMenu[BNo]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiInfoMenu[BNo]->bmiHeader.biWidth = xItimasu;
		lpbiInfoMenu[BNo]->bmiHeader.biHeight = yItimasu;
		lpbiInfoMenu[BNo]->bmiHeader.biPlanes = 1;
		lpbiInfoMenu[BNo]->bmiHeader.biBitCount = 32;
		lpbiInfoMenu[BNo]->bmiHeader.biCompression = BI_RGB;

		/* BMP���̃s�N�Z�����Menu�r�b�g�����ăR�s�[ */
for (l = 0 ; l < yNanmasu ; l++){////y�}�X���}�X

	for (k = 0;k < xNanmasu; k++){////x�}�X���}�X

		for (i = 0;i < yItimasu; i++){////�c�s�N�Z��

			for (j = 0;j < xItimasu; j++){////���s�N�Z��

				switch(BNo){
				case 0:
				CopyMemory(dMenu[k + (l * xNanmasu)] + j + i * xItimasu,
				     lpBMPPixel + 3 * (j + i * iWidth + k * xItimasu + (yNanmasu-1 - l) * yItimasu * iWidth), 3);
				break;

				case 1:
			/*	CopyMemory(disp[k + (l * xNanmasu)] + j + i * xItimasu,
				     lpBMPPixel + 3 * (j + i * iWidth + k * xItimasu + (yNanmasu-1 - l) * yItimasu * iWidth), 3);*/
				break;

				default: break;

				}
				}}}}
		/*MessageBox(NULL,"1�}�X�ǂݍ��݊�","�����[",MB_OK);*/

		/* �t�@�C���ǂݍ��݃o�b�t�@��� */
		HeapFree(GetProcessHeap(), 0, lpBMP);

	//	MessageBox(NULL,"BMP�ǂݍ��݊�",BMPMenu[BNo],MB_OK);
	}
/*3�I��*/

/*4 32*32�ǉ����K�{FNo���v���X*/
	for(FNo; FNo<4; FNo++){
		getBMPAddress();
				/*16��؂�}�X*/
		xNanmasu = iWidth / 16; xItimasu = iWidth / xNanmasu;
		yNanmasu = iHeight / 16; yItimasu = iHeight / yNanmasu;
		/************/
		/* DIB�p�o�b�t�@���m�� */
		lpDIB16 = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		         sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * xNanmasu * yNanmasu));

		/* DIB�p�|�C���^���z */
		lpbiInfo[FNo] = (LPBITMAPINFO)lpDIB16;

		for(i = 0; i < xNanmasu * yNanmasu; i++){
			switch(FNo){
			case 3:/*�������ǉ����K�{*/
			chr16[i] = (LPDWORD)(lpDIB16 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			break;

		/*	case 1:
			mapChip[i] = (LPDWORD)(lpDIB16 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			break;*/

			default: break;
			}
		}
		/* BITMAPINFO�ݒ� */

		lpbiInfo[FNo]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiInfo[FNo]->bmiHeader.biWidth = xItimasu;
		lpbiInfo[FNo]->bmiHeader.biHeight = yItimasu;
		lpbiInfo[FNo]->bmiHeader.biPlanes = 1;
		lpbiInfo[FNo]->bmiHeader.biBitCount = 32;
		lpbiInfo[FNo]->bmiHeader.biCompression = BI_RGB;

		/* BMP���̃s�N�Z�����32�r�b�g�����ăR�s�[ */
for (l = 0 ; l < yNanmasu ; l++){////y�}�X���}�X

	for (k = 0;k < xNanmasu; k++){////x�}�X���}�X

		for (i = 0;i < yItimasu; i++){////�c�s�N�Z��

			for (j = 0;j < xItimasu; j++){////���s�N�Z��
		//MessageBox(NULL,"�o�b�t�@���������Ƃ���","�����[",MB_OK);
				switch(FNo){
				case 3:/*�������ǉ����K�{�����I������낢��E�E*/
				CopyMemory(chr16[k + (l * xNanmasu)] + j + i * xItimasu,
				     lpBMPPixel + 3 * (j + i * iWidth + k * xItimasu + (yNanmasu-1 - l) * yItimasu * iWidth), 3);
				break;

			/*	case 1:
				CopyMemory(mapChip[k + (l * xNanmasu)] + j + i * xItimasu,
				     lpBMPPixel + 3 * (j + i * iWidth + k * xItimasu + (yNanmasu-1 - l) * yItimasu * iWidth), 3);
				break;*/

				default: break;

				}
				}}}}
		/*MessageBox(NULL,"1�}�X�ǂݍ��݊�","�����[",MB_OK);*/

		/* �t�@�C���ǂݍ��݃o�b�t�@��� */
		HeapFree(GetProcessHeap(), 0, lpBMP);

	//	MessageBox(NULL,"BMP�ǂݍ��݊�",BMPName[FNo],MB_OK);
	}
/*4�I��*/
		/* �t�@�C���ǂݍ��݃o�b�t�@��� */
		HeapFree(GetProcessHeap(), 0, lpBMP);

	return 1;

}

/*�t�@�C���ǂݍ��݋���*/
void getBMPAddress(void){
	
		fh = CreateFile(/*BMP32[BNo]*/BMPName[FNo], GENERIC_READ, 0, NULL,
		        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


		if (fh == INVALID_HANDLE_VALUE) {

			MessageBox(NULL, "bmp��������܂���B", "", MB_OK);

			return ;
		}

		/* �t�@�C���T�C�Y�擾 */
		iFileSize = GetFileSize(fh, NULL);

		/* �t�@�C���ǂݍ��݃o�b�t�@�m�� */
		lpBMP = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, iFileSize);

		/* �t�@�C���ǂݍ��� */
		ReadFile(fh, lpBMP, iFileSize, &dwRead, NULL);

		/* �t�@�C������� */
		CloseHandle(fh);

		/* BMP����BITMAPINFO�擾 */
		lpbiBMPInfo = (LPBITMAPINFOHEADER)
		              (lpBMP + sizeof(BITMAPFILEHEADER));

		/* �擪����s�N�Z����܂ł̃I�t�Z�b�g�擾 */
		dwOffset = *(LPDWORD)(lpBMP + 10);

		/* BMP���s�N�Z����̐擪�A�h���X�v�Z */
		lpBMPPixel = lpBMP + dwOffset;

		/* �r�b�g�}�b�v�̑傫���擾 */
		iWidth = lpbiBMPInfo->biWidth;
		iHeight = lpbiBMPInfo->biHeight;
}

/*�Ǎ��ݏI���*/