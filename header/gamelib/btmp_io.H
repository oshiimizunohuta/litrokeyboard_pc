LPBITMAPINFOHEADER adr_BInfoH;
LPBYTE adr_BMP,adr_BPixel;
LPDWORD PixData[1028];

struct BMPDATA{
	BITMAPINFO pdinfo;

	HBITMAP pdhBMP;
	HDC pdhdc;
	LPDWORD lppd;
}BMPPP;

/*BMP登録ーBMP追加時必須*/
char *BFName[3] = {"BMP.DAT"};
/*********/

/*共有用DIB宣言ーBMP追加時必須*/
LPBITMAPINFO adr_BInfo;
LPDWORD adr_Pixel;
/***********/

/*キャラクター読込み*/

void bmp_regist(void){
int i,j,k,l, FNo, xItimasu=32, yItimasu=32, xNanmasu=4, yNanmasu=4;;
		getBMPAddress();

		/*32区切りマス*/
		xNanmasu = iWidth / 32; xItimasu = iWidth / xNanmasu;
		yNanmasu = iHeight / 32; yItimasu = iHeight / yNanmasu;
		/************/
		/* DIB用バッファを確保 */
		lpDIB32 = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		         sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * xNanmasu * yNanmasu));

		/* DIB用ポインタ分配 　BMP追加時必須*/
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
		/* BITMAPINFO設定 */

		lpbiInfo[FNo]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiInfo[FNo]->bmiHeader.biWidth = xItimasu;
		lpbiInfo[FNo]->bmiHeader.biHeight = yItimasu;
		lpbiInfo[FNo]->bmiHeader.biPlanes = 1;
		lpbiInfo[FNo]->bmiHeader.biBitCount = 32;
		lpbiInfo[FNo]->bmiHeader.biCompression = BI_RGB;

		/* BMP内のピクセル列を32ビット化してコピー BMP追加時必須*/
for (l = 0 ; l < yNanmasu ; l++){////yマス何マス

	for (k = 0;k < xNanmasu; k++){////xマス何マス

		for (i = 0;i < yItimasu; i++){////縦ピクセル

			for (j = 0;j < xItimasu; j++){////横ピクセル

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
		/*MessageBox(NULL,"1マス読み込み完","ムラー",MB_OK);*/

		/* ファイル読み込みバッファ解放 */
		HeapFree(GetProcessHeap(), 0, lpBMP);

	//	MessageBox(NULL,"BMP読み込み完",BMPName[FNo],MB_OK);
	}
/*1終了*/

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
		/* BITMAPINFO設定 */

		lpbiInfo[FNo]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiInfo[FNo]->bmiHeader.biWidth = xItimasu;
		lpbiInfo[FNo]->bmiHeader.biHeight = yItimasu;
		lpbiInfo[FNo]->bmiHeader.biPlanes = 1;
		lpbiInfo[FNo]->bmiHeader.biBitCount = 32;
		lpbiInfo[FNo]->bmiHeader.biCompression = BI_RGB;

		/* BMP内のピクセル列を32ビット化してコピー BMP追加時必須*/
for (l = 0 ; l < yNanmasu ; l++){////yマス何マス

	for (k = 0;k < xNanmasu; k++){////xマス何マス

		for (i = 0;i < yItimasu; i++){////縦ピクセル

			for (j = 0;j < xItimasu; j++){////横ピクセル

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
		/*MessageBox(NULL,"1マス読み込み完","ムラー",MB_OK);*/

		/* ファイル読み込みバッファ解放 */
		HeapFree(GetProcessHeap(), 0, lpBMP);

	//	MessageBox(NULL,"BMP読み込み完",BMPName[FNo],MB_OK);
	}
/*1終了*/


	/*2*/

		fh = CreateFile("suuji.bmp", GENERIC_READ, 0, NULL,
		        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		/**/
		xItimasu = 8; yItimasu = 8; xNanmasu = 10;
		/**/

		if (fh == INVALID_HANDLE_VALUE) {

			MessageBox(NULL, "bmpが見つかりません。", "エラー", MB_OK);

			return 0;
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
		iWidth = lpbiBMPInfo->biWidth;
		iHeight = lpbiBMPInfo->biHeight;

		/* DIB用バッファを確保 */
		lpDIB8 = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		         sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * xNanmasu));

		/* DIB用ポインタ分配 */

		lpbiInfo8 = (LPBITMAPINFO)lpDIB8;

		for(i = 0; i < xNanmasu ; i++){

			NUM[i] = (LPDWORD)(lpDIB8 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));}

		/* BITMAPINFO設定 */

		lpbiInfo8->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiInfo8->bmiHeader.biWidth = xItimasu;
		lpbiInfo8->bmiHeader.biHeight = yItimasu;
		lpbiInfo8->bmiHeader.biPlanes = 1;
		lpbiInfo8->bmiHeader.biBitCount = 32;
		lpbiInfo8->bmiHeader.biCompression = BI_RGB;

	

		/* BMP内のピクセル列を32ビット化してコピー */

	for (k = 0;k < xNanmasu; k++){////マス何マス

		for (i = 0;i < yItimasu; i++){////縦ピクセル

			for (j = 0;j < xItimasu; j++){////横ピクセル

				CopyMemory(NUM[k] + j + i * xItimasu,
				     lpBMPPixel + 3 * (j + i * iWidth + k * xItimasu), 3);
				}}}

	/* ファイル読み込みバッファ解放 */
	HeapFree(GetProcessHeap(), 0, lpBMP);
/*2終了*/

/*3*/	
	/* ファイルオープン */

	for(BNo=0; BNo<1; BNo++){

		fh = CreateFile(BMPMenu[BNo], GENERIC_READ, 0, NULL,
		        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (fh == INVALID_HANDLE_VALUE) {

			MessageBox(NULL, "bmpが見つかりません。", "エラー", MB_OK);

			return 0;
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
		iWidth = lpbiBMPInfo->biWidth;
		iHeight = lpbiBMPInfo->biHeight;

		/*64*16区切りマス*/
		xNanmasu = iWidth / 64; xItimasu = iWidth / xNanmasu;
		yNanmasu = iHeight / 16; yItimasu = iHeight / yNanmasu;
		/************/
		/* DIB用バッファを確保 */
		lpDIBMenu = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		         sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * xNanmasu * yNanmasu));

		/* DIB用ポインタ分配 */
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
		/* BITMAPINFO設定 */

		lpbiInfoMenu[BNo]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiInfoMenu[BNo]->bmiHeader.biWidth = xItimasu;
		lpbiInfoMenu[BNo]->bmiHeader.biHeight = yItimasu;
		lpbiInfoMenu[BNo]->bmiHeader.biPlanes = 1;
		lpbiInfoMenu[BNo]->bmiHeader.biBitCount = 32;
		lpbiInfoMenu[BNo]->bmiHeader.biCompression = BI_RGB;

		/* BMP内のピクセル列をMenuビット化してコピー */
for (l = 0 ; l < yNanmasu ; l++){////yマス何マス

	for (k = 0;k < xNanmasu; k++){////xマス何マス

		for (i = 0;i < yItimasu; i++){////縦ピクセル

			for (j = 0;j < xItimasu; j++){////横ピクセル

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
		/*MessageBox(NULL,"1マス読み込み完","ムラー",MB_OK);*/

		/* ファイル読み込みバッファ解放 */
		HeapFree(GetProcessHeap(), 0, lpBMP);

	//	MessageBox(NULL,"BMP読み込み完",BMPMenu[BNo],MB_OK);
	}
/*3終了*/

/*4 32*32追加時必須FNoをプラス*/
	for(FNo; FNo<4; FNo++){
		getBMPAddress();
				/*16区切りマス*/
		xNanmasu = iWidth / 16; xItimasu = iWidth / xNanmasu;
		yNanmasu = iHeight / 16; yItimasu = iHeight / yNanmasu;
		/************/
		/* DIB用バッファを確保 */
		lpDIB16 = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		         sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * xNanmasu * yNanmasu));

		/* DIB用ポインタ分配 */
		lpbiInfo[FNo] = (LPBITMAPINFO)lpDIB16;

		for(i = 0; i < xNanmasu * yNanmasu; i++){
			switch(FNo){
			case 3:/*ここも追加時必須*/
			chr16[i] = (LPDWORD)(lpDIB16 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			break;

		/*	case 1:
			mapChip[i] = (LPDWORD)(lpDIB16 + sizeof(BITMAPINFO) + (xItimasu * yItimasu * 4 * i));
			break;*/

			default: break;
			}
		}
		/* BITMAPINFO設定 */

		lpbiInfo[FNo]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbiInfo[FNo]->bmiHeader.biWidth = xItimasu;
		lpbiInfo[FNo]->bmiHeader.biHeight = yItimasu;
		lpbiInfo[FNo]->bmiHeader.biPlanes = 1;
		lpbiInfo[FNo]->bmiHeader.biBitCount = 32;
		lpbiInfo[FNo]->bmiHeader.biCompression = BI_RGB;

		/* BMP内のピクセル列を32ビット化してコピー */
for (l = 0 ; l < yNanmasu ; l++){////yマス何マス

	for (k = 0;k < xNanmasu; k++){////xマス何マス

		for (i = 0;i < yItimasu; i++){////縦ピクセル

			for (j = 0;j < xItimasu; j++){////横ピクセル
		//MessageBox(NULL,"バッファが正しくとれんよ","ムラー",MB_OK);
				switch(FNo){
				case 3:/*ここも追加時必須だぜ！効率わろいや・・*/
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
		/*MessageBox(NULL,"1マス読み込み完","ムラー",MB_OK);*/

		/* ファイル読み込みバッファ解放 */
		HeapFree(GetProcessHeap(), 0, lpBMP);

	//	MessageBox(NULL,"BMP読み込み完",BMPName[FNo],MB_OK);
	}
/*4終了*/
		/* ファイル読み込みバッファ解放 */
		HeapFree(GetProcessHeap(), 0, lpBMP);

	return 1;

}

/*ファイル読み込み共通*/
void getBMPAddress(void){
	
		fh = CreateFile(/*BMP32[BNo]*/BMPName[FNo], GENERIC_READ, 0, NULL,
		        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


		if (fh == INVALID_HANDLE_VALUE) {

			MessageBox(NULL, "bmpが見つかりません。", "", MB_OK);

			return ;
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
		iWidth = lpbiBMPInfo->biWidth;
		iHeight = lpbiBMPInfo->biHeight;
}

/*読込み終わり*/
