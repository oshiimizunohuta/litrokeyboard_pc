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
	char *dirname = "NAMABMPFILES/";//ディレクトリネーム
	char box_ms[64];


	/*BMP登録ーBMP追加時必須*/
//	char *BMPName[5] = {"tespo.bmp", "enemy.bmp", "mapchip.bmp","weapon.bmp","chara16.bmp"},
//		 *BMP24[1] = {"chara24.bmp"},/**BMP16[1] = {"chara16.bmp"},*/
//		 *BMP8="suuji.bmp", *BMPMenu[2] = {"dmenu.bmp" , "dbdisp.bmp"};

//	char *IconName = "icon.ico";
	
//	LPDWORD chrPix[128], bmapChip[128], cmapChip[16][64], enePix[256], wePix[128], chr16[128], efePix16[256], efePix32[128], NUM[128], dMenu[64], dDisp[64], ICON32[16];//注意！限界アリ。絵を増やしたら配列要素も増やす。
	LPDWORD BMPixad[65535];
	/*********/

	/*共有用DIB宣言ーBMP追加時必須*/
	LPBYTE lpDIB32 = NULL/*32*32*/;
	LPDWORD lpPixel32;
	/***********/

int k,l, FNo = 0, BNo=0, FNo8 = 0, xItimasu=32, yItimasu=32, xNanmasu=4, yNanmasu=4;
BYTE aMax = 0x0004;


//キャラクター読込み/
//追加
int BMPYomi32(void){	
	BYTE readMfiles;//ikutuyomu?
	WORD i;
	WORD j;

	bnamebtype_set();//BMPカスタムセット
	for(i = 0; i < readbfiles; i++){
		BMPF[i].BMPName = Bcustom[i].BMName; BMPF[i].masuType = Bcustom[i].masutype;
	}
/*	HANDLE fh;


	/* ファイルオープン BMP追加時必須FNo*/

	for(FNo=0; FNo < readbfiles; FNo++){

		getBMPAddress(FNo);

		/*32区切りマス*/
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
		/* DIB用バッファを確保 */
		lpDIB32 = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		         sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * xNanmasu * yNanmasu));

		// DIB用ポインタ分配 　BMP追加時必須/
		//追加
		BMPF[FNo].lpbiInfo = (LPBITMAPINFO)lpDIB32;

		for(i = 0; i < xNanmasu * yNanmasu; i++){
			if(FNo){//FileNo1以上
				BMPixad[i + endfilead[FNo - 1]] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			}
			else{//{0のとき
				BMPixad[i] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			}
		}
		if(FNo){//FileNo1以上
			endfilead[FNo] = i + endfilead[FNo - 1];//File区切り
		}
		else{
			endfilead[FNo] = i;//File区切り
		}
		/* BITMAPINFO設定 */

		BMPF[FNo].lpbiInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		BMPF[FNo].lpbiInfo->bmiHeader.biWidth = xItimasu;
		BMPF[FNo].lpbiInfo->bmiHeader.biHeight = yItimasu;
		BMPF[FNo].lpbiInfo->bmiHeader.biPlanes = 1;
		BMPF[FNo].lpbiInfo->bmiHeader.biBitCount = 32;
		BMPF[FNo].lpbiInfo->bmiHeader.biCompression = BI_RGB;

		/* BMP内のピクセル列を32ビット化してコピー BMP追加時必須*/
		for (l = 0 ; l < yNanmasu ; l++){////yマス何マス

			for (k = 0;k < xNanmasu; k++){////xマス何マス

				for (i = 0;i < yItimasu; i++){////縦ピクセル

					for (j = 0;j < xItimasu; j++){////横ピクセル
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
		/*MessageBox(NULL,"1マス読み込み完","ムラー",MB_OK);*/

		/* ファイル読み込みバッファ解放 */
		HeapFree(GetProcessHeap(), 0, lpBMP);

	//	MessageBox(NULL,"BMP読み込み完",BMPName[FNo],MB_OK);
	}
/*1終了*/

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

/*ファイル読み込み共通*/
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
			MessageBox(NULL, "bmpが見つかりません。", box_ms, MB_OK);
			PostQuitMessage(0);
			return;
		}

		/* ファイルサイズ取得 */
		iFileSize = GetFileSize(fh, NULL);

		/* ファイル読み込みバッファ確保 */
		lpBMP = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, iFileSize);

		/* ファイル読み込み */
		ReadFile(fh, lpBMP, iFileSize, &dwRead, NULL);

		/* ファイルを閉じる */
		CloseHandle(fh);

		/* BMP内のBITMAPINFO取得 */
		lpbiBMPInfo = (LPBITMAPINFOHEADER)
		              (lpBMP + sizeof(BITMAPFILEHEADER));

		/* 先頭からピクセル列までのオフセット取得 */
		dwOffset = *(LPDWORD)(lpBMP + 10);

		/* BMP内ピクセル列の先頭アドレス計算 */
		lpBMPPixel = lpBMP + dwOffset;

		/* ビットマップの大きさ取得 */
		BMPF[FNo].iWidth = lpbiBMPInfo->biWidth;
		BMPF[FNo].iHeight = lpbiBMPInfo->biHeight;
		
}

int orgBMPYomi32(void){	
	BYTE readMfiles;//ikutuyomu?
//	BYTE i;
	WORD i;
	WORD j;
	orgbnamebtype_set();//BMPカスタムセット
	for(i = 0; i < readbfiles; i++){
		BMPF[i].BMPName = Bcustom[i].BMName;
		BMPF[i].masuType = Bcustom[i].masutype;
	}
//	 ファイルオープン BMP追加時必須FNo

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
		MessageBox(NULL, "bmpが見つかりません。", box_ms, MB_OK);
		PostQuitMessage(0);
		return;
	}

		FillMemory(colpal, sizeof(colpal), 0);//syokika

		ReadFile(hFile, &xblock, 1, &D, NULL);
		ReadFile(hFile, &yblock, 1, &D, NULL);
		ReadFile(hFile, &mas, 1, &D, NULL);
		ReadFile(hFile, colpal, 256 * 4, &D, NULL);

//		ファイル読み込みバッファ確保

		//x軸マスの数*y軸マスの数*x軸1マスあたりのドット数*y軸1マスあたりのドット数*DWORD
		FileSize = (xblock * yblock * mas * mas * 4);

//		ファイル読み込みバッファ確保 
		lpBMP = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, FileSize);

//		// ビットマップの大きさ取得 
		BMPF[FNo].iWidth = xblock * mas;
		BMPF[FNo].iHeight = yblock * mas;
		
		lpDIB32 = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		         sizeof(BITMAPINFO) + (xblock * yblock * 4 * mas * mas));

		BMPF[FNo].lpbiInfo = (LPBITMAPINFO)lpDIB32;
		
		for(i = 0; i < (DWORD)(xblock * yblock); i++){
			if(FNo){//FileNo1以上
				BMPixad[i + endfilead[FNo - 1]] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (mas * mas * 4 * i));
			}
			else{//{0のとき
				BMPixad[i] = (LPDWORD)(lpDIB32 + sizeof(BITMAPINFO) + (mas * mas * 4 * i));
			}
		}

		if(FNo){//FileNo1以上
			endfilead[FNo] = i + endfilead[FNo - 1];//File区切り
		}
		else{
			endfilead[FNo] = i;//File区切り
		}
//		BITMAPINFO設定 

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

//読込み終わり

////////////////
////////////////	