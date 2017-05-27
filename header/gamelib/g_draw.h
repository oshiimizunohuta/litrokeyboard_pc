//ビデオ設定

//ビットマップコンテキスト、Old以前関連付けされたものは削除用
//StretchDIBits関数で扱う
	HDC hdcmapBMP;
	HBITMAP hmapBMP, hmapOldBMP;//マップスクロール用
	BITMAPINFO biBBInfo;

	HDC hdcCmapBMP;
	HBITMAP hCmapBMP, hCmapOldBMP;//立体マップスクロール用

	HBITMAP hOld;//いらない？

	HDC hdcstaBMP;
	HBITMAP hstaBMP, hstaOldBMP;//ステータスウィンドウ用
	BITMAPINFO staBInfo;
	LPDWORD lpstaBPixel;

	HDC hdcmesBMP;
	HBITMAP hmesBMP, hmesOldBMP;//メッセージウィンドウ用
	BITMAPINFO mesBInfo;

	LPDWORD lpmesBPixel;//！

//バックバッファ。ここに集めて表示する。
HBITMAP hBBBMP, hOldBBBMP;
HDC hdcBB;

LPDWORD lpBBPixel;//重要

BYTE dSize = 1;

WORD Dwidth = 640;//bitmap領域 
WORD Dheight = 480;
DWORD MAXPIX = 307200;
DWORD MAXPIX_msg = 115200;
DWORD MAXPIX_sta = 92160;
BYTE Mwidth = 14;//横表示マス
BYTE Mheight = 10;//盾表示マス
WORD Xdsize;//Mwidth * 32
WORD Ydsize;//

WORD STAwidth = 352, STAheight = 288;//ステータスさいず
WORD MESwidth = 640, MESheight = 180;//メッセージさいず

WORD cub128 = 16384, cub256 = 65536;


void setpixx(DWORD disp, DWORD bcolor, LPDWORD lpd, BYTE multi)
{
	BYTE i, j;
	DWORD dispP;//表示
//	WORD dwidth = Dwidth;

//	lpBBPixel[disp] = *bmpix;
	for(i = 0; i < multi; i++){
		if((disp + (i * Dwidth)) > MAXPIX){break;}//大きすぎる値は無視
		for(j = 0; j < multi; j++){
//			lpBBPixel[disp + j + (i * Dwidth)] = bcolor;//一段目？
			lpd[disp + j + (i * Dwidth)] = bcolor;//一段目？
		}
	}

}


////数字を一桁ずつ表示////060730
DWORD jussin(DWORD kazu, BYTE keta, WORD XX, WORD YY, DWORD collor1, DWORD collor2)
{//表示する数字と桁をもってきま。
	BYTE i, j, k, nanketa;//d_kazu ha 256=0だけど気にしない
	BOOL minus = FALSE;
	
	DWORD d_kazu;//桁あふれてた060723
	DWORD dispP;
	LPDWORD mendoi;
	
	if(keta < 6 && kazu > 65535){kazu *= -1; minus = TRUE;}
	for(nanketa = 0; nanketa < keta; nanketa++){
		d_kazu = kazu / pow(10, nanketa);//keta桁を１桁目に持っていく
		i = d_kazu % 10;
		for(j = 0; j < 8; j++){
			for(k = 0; k < 8; k++){
				dispP = XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (8 - j + YY));
				if(dispP > MAXPIX){break;}
//				if(*(NUM[i] + k + (j * 8)) == 0x00ffff00){
				if(*(pix.data[pix.numb_ad + i] + k + (j * 8)) == 0x00ffff00){
//					if(minus){lpBBPixel[XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (8 - j + YY))] = 0xeeeeee;}
					if(minus){lpBBPixel[dispP] = 0xeeeeee;}
					else{lpBBPixel[XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (8 - j + YY))] = collor1;}
				}
//				if(*(NUM[i] + k + (j * 8)) == 0x00808000){
				if(*(pix.data[pix.numb_ad + i] + k + (j * 8)) == 0x00808000){
					if(minus){lpBBPixel[XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (8 - j + YY))] = 0xeeeeee;}
					else{lpBBPixel[XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (8 - j + YY))] = collor2;}
				}
			}
		}
	}
	return 0;
}


void icon_draw(LPDWORD lppix, BYTE dib, int x, int y, BYTE size, BYTE reva, BYTE multi)
{
	BYTE i, j, hamofx, hamofy;
	DWORD disps;//表示
	LPDWORD lpsel;

	if(x < 0){hamofx = -x; x = 0;}
	else{hamofx = 0;}
	if(y < 0){hamofy = -y; y = 0;}
	else{hamofy = 0;}
	disps = x + (y * Dwidth);
	
	switch(dib){
		case 0: lpsel = lpBBPixel; break;
		case 1: lpsel = lpstaBPixel; break;
		case 2: lpsel = lpmesBPixel; break;
		default : lpsel = lpBBPixel; break;
	}
	switch(reva){
		case 0://普通
		lppix += (size * (size - 1 - hamofy)) + hamofx;
		for(i = hamofy; i < size; i++){
			if(y + i < Dheight){
				for(j = hamofx; j < size; j++){
					if(*lppix && (x + j < Dwidth)){
						setpixx(disps, *lppix, lpsel, multi);
					}
					disps++;
					lppix++;
				}
			}
			disps += (Dwidth - j + hamofx);
			lppix -= size + size - hamofx;
		}break;
		
		case 1://左右反転
		lppix += (size * size) - 1;
		for(i = 0; i < size; i++){
			for(j = 0; j < size; j++){
				if(*lppix && (x + j < Dwidth)){
					setpixx(disps, *lppix, lpsel, multi);
				}
				disps++;
				lppix--;
			}
			disps += (Dwidth - j);
//			lppix -= j + j;
		}break;
		
	}
}



