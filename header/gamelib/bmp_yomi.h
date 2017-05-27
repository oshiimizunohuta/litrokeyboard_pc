HANDLE fh;
DOUBLE iFileSize;
LPBITMAPINFOHEADER lpbiBMPInfo;
LPBYTE lpBMP,lpBMPPixel;
DWORD dwRead, dwOffset;

struct BMPFiles{
	char *BMPName;

	LPBYTE lpDIB;
	
	LPBITMAPINFO lpbiInfo;
	
	int iWidth, iHeight, iLength;
	
	BYTE masuType; //0:32*32 1:16:16 2:8*8 3:64*16
	BYTE xItimasu, yItimasu, xNanmasu, yNanmasu;
}BMPF[64];

	
void getBMPAddress(int FNo);
void getorgBMPAddress(int FNo);
	char *dirname = "NAMABMPFILES/";//�f�B���N�g���l�[��
	char box_ms[64];


	/*BMP�o�^�[BMP�ǉ����K�{*/
//	char *BMPName[5] = {"tespo.bmp", "enemy.bmp", "mapchip.bmp","weapon.bmp","chara16.bmp"},
//		 *BMP24[1] = {"chara24.bmp"},/**BMP16[1] = {"chara16.bmp"},*/
//		 *BMP8="suuji.bmp", *BMPMenu[2] = {"dmenu.bmp" , "dbdisp.bmp"};

//	char *IconName = "icon.ico";
	
//	LPDWORD chrPix[128], bmapChip[128], cmapChip[16][64], enePix[256], wePix[128], chr16[128], efePix16[256], efePix32[128], NUM[128], dMenu[64], dDisp[64], ICON32[16];//���ӁI���E�A���B�G�𑝂₵����z��v�f�����₷�B
	LPDWORD BMPixad[65535];
	/*********/

	/*���L�pDIB�錾�[BMP�ǉ����K�{*/
	LPBYTE lpDIB32 = NULL/*32*32*/;
	LPDWORD lpPixel32;
	/***********/

int k,l, FNo = 0, BNo=0, FNo8 = 0, xItimasu=32, yItimasu=32, xNanmasu=4, yNanmasu=4;
BYTE aMax = 0x0004;


//�L�����N�^�[�Ǎ���/
//�ǉ�
int BMPYomi32(void){	
	BYTE readMfiles;//ikutuyomu?
	WORD i;
	WORD j;

	bnamebtype_set();//BMP�J�X�^���Z�b�g
	for(i = 0; i < readbfiles; i++){
		BMPF[i].BMPName = Bcustom[i].BMName; BMPF[i].masuType = Bcustom[i].masutype;
	}
/*	HANDLE fh;


	/* �t�@�C���I�[�v�� BMP�ǉ����K�{FNo*/

	for(FNo=0; FNo < readbfiles; FNo++){

		getBMPAddress(FNo);

		/*32��؂�}�X*/
//		xNanmasu = iWidth / 32; xItimasu = iWidth / xNanmasu;
//		yNanmasu = iHeight / 32; yItimasu = iHeight / yNanmasu;
		
		switch(BMPF[FNo].masuType){
			case 0: xNanmasu = BMPF[FNo].iWidth / 32; xItimasu = BMPF[FNo].iWidth / xNanmasu;
					yNanmasu = BMPF[FNo].iHeight / 32; yItimasu = BMPF[FNo].iHeight / yNanmasu;break;
					
			case 1: xNanmasu = BMPF[FNo].iWidth / 16; xItimasu = BMPF[FNo].iWidth / xNanmasu;
					yNanmasu = BMPF[FNo].iHeight / 16; yItimasu = BMPF[FNo].iHeight / yNanmasu;break;
	
			case 2: xNanmasu = BMPF[FNo].iWidth / 8; xItimasu = BMPF[FNo].iWidth / xNanmasu;
					yNanmasu = BMPF[FNo].iHeight / 8; yItimasu = BMPF[FNo].iHeight / yNanmasu;break;
	
			case 3: xNanmasu = BMPF[FNo].iWidth / 12; xItimasu = BMPF[FNo].iWidth / xNanmasu;
					yNanmasu = BMPF[FNo].iHeight / 12; yItimasu = BMPF[FNo].iHeight / yNanmasu;break;

			case 4: xNanmasu = BMPF[FNo].iWidth / 64; xItimasu = BMPF[FNo].iWidth / xNanmasu;
					yNanmasu = BMPF[FNo].iHeight / 64; yItimasu = BMPF[FNo].iHeight / yNanmasu;break;
	
		}

		/************/
		/* DIB�p�o�b�t�@���m�� */
		lpDIB32 = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		         sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * xNanmasu * yNanmasu));

		// DIB�p�|�C���^���z �@BMP�ǉ����K�{/
		//�ǉ�
		BMPF[FNo].lpbiInfo = (LPBITMAPINFO)lpDIB32;

		for(i = 0; i < xNanmasu * yNanmasu; i++){
			if(FNo){//FileNo1�ȏ�
				BMPixad[i + endfilead[FNo - 1]] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			}
			else{//{0�̂Ƃ�
				BMPixad[i] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			}
		}
		if(FNo){//FileNo1�ȏ�
			endfilead[FNo] = i + endfilead[FNo - 1];//File��؂�
		}
		else{
			endfilead[FNo] = i;//File��؂�
		}
		/* BITMAPINFO�ݒ� */

		BMPF[FNo].lpbiInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		BMPF[FNo].lpbiInfo->bmiHeader.biWidth = xItimasu;
		BMPF[FNo].lpbiInfo->bmiHeader.biHeight = yItimasu;
		BMPF[FNo].lpbiInfo->bmiHeader.biPlanes = 1;
		BMPF[FNo].lpbiInfo->bmiHeader.biBitCount = 32;
		BMPF[FNo].lpbiInfo->bmiHeader.biCompression = BI_RGB;

		/* BMP���̃s�N�Z�����32�r�b�g�����ăR�s�[ BMP�ǉ����K�{*/
		for (l = 0 ; l < yNanmasu ; l++){////y�}�X���}�X

			for (k = 0;k < xNanmasu; k++){////x�}�X���}�X

				for (i = 0;i < yItimasu; i++){////�c�s�N�Z��

					for (j = 0;j < xItimasu; j++){////���s�N�Z��
						if(FNo){
							CopyMemory(BMPixad[k + (l * xNanmasu) + endfilead[FNo - 1]] + j + (i * xItimasu),
							     lpBMPPixel + 3 * (j + i * BMPF[FNo].iWidth + k * xItimasu + (yNanmasu-1 - l) * yItimasu * BMPF[FNo].iWidth), 3);
						}
						else{
							CopyMemory(BMPixad[k + (l * xNanmasu)] + j + (i * xItimasu),
							     lpBMPPixel + 3 * (j + i * BMPF[FNo].iWidth + k * xItimasu + (yNanmasu-1 - l) * yItimasu * BMPF[FNo].iWidth), 3);
						}
					}
				}
			}
		}
		/*MessageBox(NULL,"1�}�X�ǂݍ��݊�","�����[",MB_OK);*/

		/* �t�@�C���ǂݍ��݃o�b�t�@��� */
		HeapFree(GetProcessHeap(), 0, lpBMP);

	//	MessageBox(NULL,"BMP�ǂݍ��݊�",BMPName[FNo],MB_OK);
	}
/*1�I��*/

	for(FNo = 0; FNo < readbfiles; FNo++){
		for(j = 0; j < endfilead[FNo] - endfilead[FNo - 1]; j++){
			if(FNo){
				pix.data[j + endfilead[FNo - 1]] = BMPixad[j + endfilead[FNo - 1]];
			}
			else{
				pix.data[j] = BMPixad[j];
			}
		bmadnameset(FNo);
	}
//			wsprintf(box_ms,"%d %d %d %d %d",pix.mych_ad, pix.ench_ad, pix.efe16_ad, pix.numb_ad, pix.cmap_ad[1]);
//			MessageBox(NULL,box_ms,BMSET[FNo].BMNAME,MB_OK);

	}
	return 1;

}

/*�t�@�C���ǂݍ��݋���*/
void getBMPAddress(int FNo){
	LPBYTE FILENAME;
	char box_ms[64];
	
	FILENAME = BMPF[FNo].BMPName;
	
//	(*FILENAME) = (*dirname);
//	MoveMemory(FILENAME, FILENAME + (sizeof(dirname) * 2), sizeof(dirname) * 2);
//	CopyMemory(FILENAME, dirname, sizeof(dirname) * 2);

	
//	sizeof(dirname) + ;
	
		
	
		fh = CreateFile(/*BMP32[BNo]BMPF[FNo].BMPName*/FILENAME, GENERIC_READ, 0, NULL,
		        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


		if (fh == INVALID_HANDLE_VALUE) {
			wsprintf(box_ms,"%s", FILENAME);
			MessageBox(NULL, "bmp��������܂���B", box_ms, MB_OK);
			PostQuitMessage(0);
			return;
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
		BMPF[FNo].iWidth = lpbiBMPInfo->biWidth;
		BMPF[FNo].iHeight = lpbiBMPInfo->biHeight;
		
}

int orgBMPYomi32(void){	
	BYTE readMfiles;//ikutuyomu?
//	BYTE i;
	WORD i;
	WORD j;
	orgbnamebtype_set();//BMP�J�X�^���Z�b�g
	for(i = 0; i < readbfiles; i++){
		BMPF[i].BMPName = Bcustom[i].BMName;
		BMPF[i].masuType = Bcustom[i].masutype;
	}
//	 �t�@�C���I�[�v�� BMP�ǉ����K�{FNo

	for(FNo=0; FNo < readbfiles; FNo++){
		getorgBMPAddress(FNo);
	}

	for(FNo = 0; FNo < readbfiles; FNo++){
		for(j = 0; j < endfilead[FNo] - endfilead[FNo - 1]; j++){
			if(FNo){
				pix.data[j + endfilead[FNo - 1]] = BMPixad[j + endfilead[FNo - 1]];
			}
			else{
				pix.data[j] = BMPixad[j];
			}
		bmadnameset(FNo);
	}

//			wsprintf(box_ms,"%d %d %d %d %d",pix.mych_ad, pix.ench_ad, pix.efe16_ad, pix.numb_ad, pix.cmap_ad[1]);
//			MessageBox(NULL,box_ms,BMSET[FNo].BMNAME,MB_OK);

	}

	return 1;

}

void getorgBMPAddress(int FNo){
	LPBYTE FILENAME;

	HANDLE hFile;
	OPENFILENAME ofn;
	DWORD D, i;
	DWORD FileSize;
	DWORD colpal[256];
	BYTE xblock, yblock, mas, paldata, cnt, c;
	char box_ms[64];
	
	FILENAME = BMPF[FNo].BMPName;
	
	hFile = CreateFile(FILENAME, GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


	if(hFile == INVALID_HANDLE_VALUE) {
		wsprintf(box_ms,"%s", FILENAME);
		MessageBox(NULL, "bmp��������܂���B", box_ms, MB_OK);
		PostQuitMessage(0);
		return;
	}

		FillMemory(colpal, sizeof(colpal), 0);//syokika

		ReadFile(hFile, &xblock, 1, &D, NULL);
		ReadFile(hFile, &yblock, 1, &D, NULL);
		ReadFile(hFile, &mas, 1, &D, NULL);
		ReadFile(hFile, colpal, 256 * 4, &D, NULL);

//		�t�@�C���ǂݍ��݃o�b�t�@�m��

		//x���}�X�̐�*y���}�X�̐�*x��1�}�X������̃h�b�g��*y��1�}�X������̃h�b�g��*DWORD
		FileSize = (xblock * yblock * mas * mas * 4);

//		�t�@�C���ǂݍ��݃o�b�t�@�m�� 
		lpBMP = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, FileSize);

//		// �r�b�g�}�b�v�̑傫���擾 
		BMPF[FNo].iWidth = xblock * mas;
		BMPF[FNo].iHeight = yblock * mas;
		
		lpDIB32 = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		         sizeof(BITMAPINFO) + (xblock * yblock * 4 * mas * mas));

		BMPF[FNo].lpbiInfo = (LPBITMAPINFO)lpDIB32;
		
		for(i = 0; i < (DWORD)(xblock * yblock); i++){
			if(FNo){//FileNo1�ȏ�
				BMPixad[i + endfilead[FNo - 1]] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (mas * mas * 4 * i));
			}
			else{//{0�̂Ƃ�
				BMPixad[i] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (mas * mas * 4 * i));
			}
		}

		if(FNo){//FileNo1�ȏ�
			endfilead[FNo] = i + endfilead[FNo - 1];//File��؂�
		}
		else{
			endfilead[FNo] = i;//File��؂�
		}
//		BITMAPINFO�ݒ� 

		BMPF[FNo].lpbiInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		BMPF[FNo].lpbiInfo->bmiHeader.biWidth = mas;
		BMPF[FNo].lpbiInfo->bmiHeader.biHeight = mas;
		BMPF[FNo].lpbiInfo->bmiHeader.biPlanes = 1;
		BMPF[FNo].lpbiInfo->bmiHeader.biBitCount = 32;
		BMPF[FNo].lpbiInfo->bmiHeader.biCompression = BI_RGB;

		cnt = 0;
		i = 0;
		paldata = 0;
		if(FNo){
//			for(i = 0; i < xblock * yblock * mas * mas; i++){
//				ReadFile(hFile, &paldata, 1, &D, NULL);//
//				CopyMemory(BMPixad[endfilead[FNo - 1]] + i, &colpal[paldata], 4);
//			}
			while(i < (DWORD)(xblock * yblock * mas * mas)){
				ReadFile(hFile, &paldata, 1, &D, NULL);
				ReadFile(hFile, &cnt, 1, &D, NULL);
				for(c = 0; c < cnt; c++){
					CopyMemory(BMPixad[endfilead[FNo - 1]] + i, &colpal[paldata], 4);
					i++;
					if(i == (DWORD)(xblock * yblock * mas * mas)){break;}
				}
			}
		}
		else{
			while(i < (DWORD)(xblock * yblock * mas * mas)){
				ReadFile(hFile, &paldata, 1, &D, NULL);
				ReadFile(hFile, &cnt, 1, &D, NULL);
				for(c = 0; c < cnt; c++){
					CopyMemory(BMPixad[0] + i, &colpal[paldata], 4);
					i++;
//					if(i == 512){return;}
					if(i == (DWORD)(xblock * yblock * mas * mas)){break;}
				}
			}
		}

		CloseHandle(hFile);

		HeapFree(GetProcessHeap(), 0, lpBMP);
}

//�Ǎ��ݏI���

////////////////
////////////////	