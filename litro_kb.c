#include <windows.h>
#include <math.h>
#include "header/gamelib/kansu.h"

#include "header/gamelib/pk01bset.h"


#include "header/gamelib/bmp_yomi.h"
#include "header/gamelib/input_key23.h"

#include "header/gamelib/game_sys.h"
#include "header/gamelib/mapini.h"
#include "header/gamelib/wldini.h"
#include "header/gamelib/charaini.h"
#include "header/gamelib/enebox.h"
#include "header/gamelib/wave_f.h"
#include "header/gamelib/concert.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

ATOM InitApp(HINSTANCE);
BOOL InitInstance(HINSTANCE, int);

HWND hWnd, hwMessage, hwStatus;

	
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


//ファイルクリエイト？
HANDLE cfile;

BYTE dSize = 1;

WORD Dwidth = 384;//bitmap領域 
WORD Dheight = 400;
DWORD MAXPIX = 153600;
DWORD MAXPIX_msg = 115200;
DWORD MAXPIX_sta = 92160;
BYTE Mwidth = 14;//横表示マス
BYTE Mheight = 10;//盾表示マス
WORD Xdsize;//Mwidth * 32
WORD Ydsize;//

WORD STATOP = 192;
WORD STAwidth = 384, STAheight = 144;//ステータスさいず
WORD MESTOP = 336;
WORD MESwidth = 384, MESheight = 64;//メッセージさいず

WORD cub128 = 16384, cub256 = 65536;

////////sys
BYTE sikakumukou = 0;//cubemap点滅
BYTE rPASS = 86;
BYTE mPASS = 86;
BOOL mapscr = FALSE;
BOOL Zmove = FALSE;
BOOL protlump = FALSE;
BYTE freecnt[8];
char *menuname;//メニュー名称
DWORD menucolor;//メニュー色
BOOL menuact = FALSE;//メニュー操作中
BYTE selectmenu = 0;

BOOL numact = FALSE;
BYTE selectnum = 0;
BYTE selectnumc = 0;

BOOL typeact = FALSE;
BYTE selecttype = 0;
BYTE selecttpage = 0;
BYTE message_sel = 0;

BOOL batu = FALSE;//Xアイコン
BOOL BLOCK2D = FALSE;//2Dmap用
WORD ARwidth = 224;
WORD ARheight = 288;
WORD TWwidth = 352;
WORD TWheight = 192;
WORD PWwidth = 352;
WORD PWheight = 96;

BYTE menuw_x = 0x0; //メニューウィンドウ位置
BYTE menuw_y = 0x0;
BYTE menumasu_x = 0x0;//メニュー時マスコット一
BYTE menumasu_y = 0x0;
BYTE modemasu_x = 0x0;//mode用位置
BYTE modemasu_y = 0x0;

//0:mapスクロール１ 1:エリアブロック移動
//3:testwave
//7:soundsave;
//8:soundload;

BOOL wactive = TRUE;//windowがアクティブです。

BYTE mode_b = 0;//mode 記憶
BYTE value[256];//0: scroll 1:putebo 2: 7:FILE  8:ch 11:putebo_select

BYTE anime[8];//animeﾊﾟﾀｰﾝ
BYTE SKIP = 0;
BYTE timestep = 6;//time値スキップ

////org value
DWORD lcolor[4] = {0xa0ffa0, 0x40a040, 0x004020, 0x102010};
BYTE manudly[4] = {0, 0, 0, 0};//マニュアルディレイ
BYTE onlvl[8] = {3, 3, 3, 3,  3, 3, 3, 3};//オクターブ位置
//BYTE onkan[8] = {1, 1, 1, 1,  1, 1, 1, 1};//記憶用音階 wave.hへ移動
WORD note_no[8] = {0, 0, 0, 0, 0, 0, 0, 0};//現在NOTE位置
short rngnote = 0;//コピー範囲

BYTE washanime = 0;//消去時アニメカウント
BOOL byebye = FALSE;
BYTE byec = 0;
//saisin color 0x004020,
BOOL visible = FALSE;

//color 0:0xa0ffa0, 1:0x408040, 2: 0x40a040, 3: 0x204020, 3:0x102010
//color 0:0xa0ffa0, 1:0x408040, 2: 0x204020, 3:0x102010
struct copynote{
	BYTE mode[1024];
	BYTE time[1024];
	BYTE page[1024];
	short value[1024];
} cpnote[4];

WORD copyn[3] = {0, 0, 0};//コピーはじめ、コピー終わり ch
BYTE copyat[3] = {0, 0, 0};//コピー先:time , page, ch

char *mocode[16] = {"A","B","C","D","E","F","G","H","I","J"};

///
void makestatewin(BYTE stype);
void makemessagewin(BYTE stype);
void wavetest_disp(BYTE m);
void keyb_draw(void);
void score_draw(void);
void draw(void);
void exit_game(void);
void keychk(void);
void chkchk(void);

int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrevInst,
                    LPSTR lpsCmdLine, int nCmdShow){
	MSG  msg;
	BOOL bRet;
	

	if(!InitApp(hCurInst)){
		return FALSE;
	}
	if(!InitInstance(hCurInst, nCmdShow)){
		return FALSE;
	}
		
	bRet = 1;
	// メインループ
	while ((1)) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){//わかんないけど必須
			
			if(!GetMessage(&msg, NULL, 0, 0)){
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		else{
			if(wactive == TRUE){
				if(drawF){draw();}
				fcnt2();
				mati2();
				inkey();
				wvpnt();
				chkchk();

				InvalidateRect(hWnd, NULL, FALSE);
//				if(fast == FALSE){Sleep(10);}
			}

			else if(wactive == FALSE){//バックのときCPU負荷0
				if(splay && visible){
					fcnt2();
					mati2();
					wvpnt();
					sound_draw();
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else{
					while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0){
						if(bRet == -1){
							break;
						}
						else{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
			
						if(wactive == TRUE){break;}
					}
				}

			}
		}
	}
	return (int)msg.wParam;
}

ATOM InitApp(HINSTANCE hInst){
	WNDCLASS wc;
	//hInst = hInstance;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInst;
	wc.hIcon         = //LoadIcon(hInst, "ICON8");
						(HICON)LoadImage(hInst, "ICON8", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

	wc.hCursor       = LoadCursor(NULL, IDC_NO);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "mapWindow";

	return(RegisterClass(&wc));
		
}

InitInstance(HINSTANCE hInst, int nCmdShow){
	hWnd = CreateWindow("mapWindow","Litro_Keyboard ver1.200",//addfunc_changeSpecific_bugfix
	           WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT,
//	           WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
	          392, 434, NULL, NULL, hInst, NULL);
	if(!hWnd){
		return FALSE;
	}
	
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HDC hdc, hdcMem;

	LPRGBQUAD lpRGB;

	static DWORD dwTime;
	
	int i, j;
	int len;

	HDC hdcWin;
	PAINTSTRUCT ps;
	switch (msg) {

	case WM_CREATE:

//	if(BMPYomi32() == 0){//メイキング用
	if(orgBMPYomi32() == 0){//公式
		DestroyWindow(hWnd);
	}
//メイキングオリジナルグラフィックデータファイル
//		make_bmp(0, 8, 8);
//		make_bmp(1, 16, 8);
//		make_bmp(2, 10, 25);

	
	
	//WAVEFORMAT
	plsWf.wFormatTag=WAVE_FORMAT_PCM;
	plsWf.nChannels=1;	//モノラル
	plsWf.wBitsPerSample=16;	//量子化ビット数 振幅の型はWORD
	plsWf.nBlockAlign=plsWf.nChannels * plsWf.wBitsPerSample/8;//2BYTEで
	plsWf.nSamplesPerSec=SRATE;	//標本化周波数
	plsWf.nAvgBytesPerSec=plsWf.nSamplesPerSec * plsWf.nBlockAlign;

	waveOutOpen(&hplsWaveOut, WAVE_MAPPER, &plsWf, (DWORD)hWnd, 0, CALLBACK_WINDOW);
	lpplsWave[0]=(short *)calloc(plsWf.nAvgBytesPerSec, 2);
	lpplsWave[1]=(short *)calloc(plsWf.nAvgBytesPerSec, 2);
	lpplsWave[2]=(short *)calloc(plsWf.nAvgBytesPerSec, 2);

	plsWhdr[0].lpData=(LPSTR)lpplsWave[0];
	plsWhdr[1].lpData=(LPSTR)lpplsWave[1];
	plsWhdr[2].lpData=(LPSTR)lpplsWave[2];
	plsWhdr[0].dwBufferLength=wpf[selwvbuff]*2;//plsWf.nAvgBytesPerSec / 10;
	plsWhdr[1].dwBufferLength=wpf[selwvbuff]*2;//plsWf.nAvgBytesPerSec / 10;
	plsWhdr[2].dwBufferLength=wpf[selwvbuff]*2;//plsWf.nAvgBytesPerSec / 10;
	plsWhdr[0].dwFlags=WHDR_BEGINLOOP | WHDR_ENDLOOP;
	plsWhdr[1].dwFlags=WHDR_BEGINLOOP | WHDR_ENDLOOP;
	plsWhdr[2].dwFlags=WHDR_BEGINLOOP | WHDR_ENDLOOP;
	plsWhdr[0].dwLoops=1;
	plsWhdr[1].dwLoops=1;
	plsWhdr[2].dwLoops=1;
	
	waveOutPrepareHeader(hplsWaveOut, &plsWhdr[0], sizeof(WAVEHDR));
	waveOutPrepareHeader(hplsWaveOut, &plsWhdr[1], sizeof(WAVEHDR));
	waveOutPrepareHeader(hplsWaveOut, &plsWhdr[2], sizeof(WAVEHDR));


//		ウィンドウのデバイスコンテキストをゲット

		hdc = GetDC(hWnd);

		//MAP用
//		hmapBMP = CreateCompatibleBitmap(hdc, Dwidth + 32, Dheight + 32);
		hmapBMP = CreateCompatibleBitmap(hdc, Dwidth - 32, Dheight - 32);

		hdcmapBMP = CreateCompatibleDC(hdc);

		hmapOldBMP = SelectObject(hdcmapBMP, hmapBMP);
		
		//りったい
		hCmapBMP = CreateCompatibleBitmap(hdc, Dwidth +16, Dheight +16);

		hdcCmapBMP = CreateCompatibleDC(hdc);

		hCmapOldBMP = SelectObject(hdcCmapBMP, hCmapBMP);
		
		//ステータス用
		hstaBMP = CreateCompatibleBitmap(hdc, STAwidth, STAheight);

		hdcstaBMP = CreateCompatibleDC(hdc);

		hstaOldBMP = SelectObject(hdcstaBMP, hstaBMP);
		
		ZeroMemory(&staBInfo, sizeof(BITMAPINFO));

//		DIBSectionBITMAPINFO設定だ
		staBInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		staBInfo.bmiHeader.biWidth = STAwidth;
		staBInfo.bmiHeader.biHeight = -(STAheight);
		staBInfo.bmiHeader.biPlanes = 1;
		staBInfo.bmiHeader.biBitCount = 32;
		staBInfo.bmiHeader.biCompression = BI_RGB;

//		バックバッファDIBSection作成
		hstaBMP = CreateDIBSection(hdc, &staBInfo, DIB_RGB_COLORS, (LPVOID)(&lpstaBPixel), NULL, 0);

		hdcstaBMP = CreateCompatibleDC(hdc);

		hstaOldBMP = SelectObject(hdcstaBMP, hstaBMP);

//		makestatewin(3);//ステータスウィンドウBMPセット
		wavetest_disp(3);
		//メッセージ用
		hmesBMP = CreateCompatibleBitmap(hdc, MESwidth, MESheight);
		hmesBMP = CreateCompatibleBitmap(hdc, MESwidth, Dheight -32);

		hdcmesBMP = CreateCompatibleDC(hdc);

		hmesOldBMP = SelectObject(hdcmesBMP, hmesBMP);
//		バックバッファ用BITMAPINFOをクリア
		ZeroMemory(&mesBInfo, sizeof(BITMAPINFO));

//		DIBSectionBITMAPINFO設定だ
		mesBInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		mesBInfo.bmiHeader.biWidth = MESwidth;
		mesBInfo.bmiHeader.biHeight = -(Dheight -32);
		mesBInfo.bmiHeader.biPlanes = 1;
		mesBInfo.bmiHeader.biBitCount = 32;
		mesBInfo.bmiHeader.biCompression = BI_RGB;

//		バックバッファDIBSection作成
		hmesBMP = CreateDIBSection(hdc, &mesBInfo, DIB_RGB_COLORS, (LPVOID)(&lpmesBPixel), NULL, 0);

		hdcmesBMP = CreateCompatibleDC(hdc);

		hmesOldBMP = SelectObject(hdcmesBMP, hmesBMP);

		makemessagewin(0);

		//バックバッファ用BITMAPINFOをクリア
		ZeroMemory(&biBBInfo, sizeof(BITMAPINFO));

		//DIBSectionBITMAPINFO設定だ
		biBBInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		biBBInfo.bmiHeader.biWidth = Dwidth;
		biBBInfo.bmiHeader.biHeight = -Dheight;
		biBBInfo.bmiHeader.biPlanes = 1;
		biBBInfo.bmiHeader.biBitCount = 32;
		biBBInfo.bmiHeader.biCompression = BI_RGB;

		//バックバッファDIBSection作成
		hBBBMP = CreateDIBSection(hdc, &biBBInfo, DIB_RGB_COLORS, (LPVOID)(&lpBBPixel), NULL, 0);

		hdcBB = CreateCompatibleDC(hdc);

		hOldBBBMP = SelectObject(hdcBB, hBBBMP);

		ReleaseDC(hWnd, hdc);

/**終了***/

//		wldsyoki();
//		map_syoki();
//		msgsyoki();
//		messageload(1);
//		chasyoki2007();

	
//	icon_syoki();

/*マップポジション初期化*/

/*完結*/

//		SetTimer(hwnd, 1, 100, NULL);


		mode = 3;
		note_syoki();
		keyb_draw();
		score_draw();
		masterlisence();
		//前後の名前のみロード
		snameload((selectnumc + selectnum + 255 - 1) % 255, 0);
		snameload((selectnumc + selectnum) % 255, 1);
		snameload((selectnumc + selectnum + 1) % 255, 2);
//		createmasterkey();
		return 0;


	case MM_WOM_OPEN:
//		waveOutReset((HWAVEOUT)wParam);
//		waveOutWrite((HWAVEOUT)wParam,(LPWAVEHDR)lParam,sizeof(WAVEHDR));
		waveOutWrite((HWAVEOUT)wParam,&plsWhdr[(now_buff + 1) % 2],sizeof(WAVEHDR));
		waveOutWrite((HWAVEOUT)wParam,&plsWhdr[now_buff],sizeof(WAVEHDR));
//		waveOutWrite((HWAVEOUT)wParam,&plsWhdr[1],sizeof(WAVEHDR));
//		now_buff = (now_buff + 1) % 2;
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		
		BitBlt(hdc, 0,0, 640, 480, hdcBB, 0, 0, SRCCOPY);/*メイン画面*/

		EndPaint(hWnd, &ps);
		return 0;
				
	case WM_ACTIVATE:
		switch (wParam){
			case WA_INACTIVE: wactive = FALSE; break;
			//	ExitProcess();break;
			case WA_ACTIVE: wactive = TRUE; visible = FALSE; break;
			case WA_CLICKACTIVE: wactive = TRUE;break;
		}

	return 0;


	case WM_CLOSE:
	
		exit_game();
		break;
//	case WM_DESTROY: /* ウインドウ破棄時 */
//	return 0;

//	exit_game();

	}return DefWindowProc(hWnd, msg, wParam, lParam);
//	return 0;

}

void setpix_sta(DWORD disp, DWORD bcolor, BYTE multi)
{
	BYTE i, j;
	for(i = 0; i < multi; i++){
		if((disp + (i * STAwidth)) > MAXPIX_sta){break;}//大きすぎる値は無視
		for(j = 0; j < multi; j++){
			lpstaBPixel[disp + j + (i * STAwidth)] = bcolor;//一段目？
		}
	}

}


void setpix_msg(DWORD disp, DWORD bcolor, BYTE multi)
{
	BYTE i, j;
	for(i = 0; i < multi; i++){
		if((disp + (i * MESwidth)) > MAXPIX_msg){break;}//大きすぎる値は無視
		for(j = 0; j < multi; j++){
			lpmesBPixel[disp + j + (i * MESwidth)] = bcolor;//一段目？
		}
	}
}

void setpix(DWORD disp, DWORD bmpix, BYTE multi)
{
	BYTE i, j;
	DWORD dispP;//表示

	for(i = 0; i < multi; i++){
		if((disp + (i * Dwidth)) > MAXPIX){break;}//大きすぎる値は無視
		for(j = 0; j < multi; j++){
			lpBBPixel[disp + j + (i * Dwidth)] = bmpix;//一段目？
		}
	}
}

void icon_draw(LPDWORD lppix, BYTE dib, WORD x, WORD y, BYTE size, BYTE multi)
{
	BYTE i, j;
	DWORD disps;//表示

	disps = x + (y * Dwidth);

	switch(dib){
		case 0://tyoku
		for(i = 0; i < size; i++){
			for(j = 0; j < size; j++){
				if(*lppix && (x + j < Dwidth)){
					setpix(disps, *lppix, multi);
				}
				disps++;
				lppix++;
			}
			disps += (Dwidth - j);
			lppix -= 32;
		}break;

		case 1://sta
		for(i = 0; i < size; i++){
			for(j = 0; j < size; j++){
				if(*lppix && (x + j < Dwidth)){
					setpix_sta(disps, *lppix, multi);
				}
				disps++;
				lppix++;
			}
			disps += (Dwidth - j);
			lppix -= 32;
		}break;

		case 2://mess
		for(i = 0; i < size; i++){
			for(j = 0; j < size; j++){
				if(*lppix && (x + j < Dwidth)){
					setpix_msg(disps, *lppix, multi);
				}
				disps++;
				lppix++;
			}
			disps += (Dwidth - j);
			lppix -= 32;
		}break;
	}
}

////数字を一桁ずつ表示msg領域////070204
DWORD jussin_sta(DWORD kazu, BYTE keta, WORD XX, WORD YY, DWORD collor1, DWORD collor2)
{
	//表示する数字と桁をもってきま。
	BYTE i, j, k, nanketa;//d_kazu ha 256=0だけど気にしない
	BOOL minus = FALSE;
	DWORD dispP;
	DWORD d_kazu;//桁あふれてた060723
	if(keta < 6 && kazu > 65535){kazu *= -1; minus = TRUE;}
	for(nanketa = 0; nanketa < keta; nanketa++){
		d_kazu = kazu / pow(10, nanketa);//keta桁を１桁目に持っていく
		i = d_kazu % 10;

		for(j = 0; j < 8; j++){
			for(k = 0; k < 8; k++){
				if(XX + k >= Dwidth){break;}
				dispP = XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (7 - j + YY));
				if(*(pix.data[pix.numb_ad + i] +  k + (j * 8)) == 0x00ffff00){
					if(minus){lpstaBPixel[dispP] = 0xeeeeee;}
					else{lpstaBPixel[dispP] = collor1;}
				}
				if(*(pix.data[pix.numb_ad + i] + k + (j * 8)) == 0x00808000){
					if(minus){lpstaBPixel[dispP] = 0xeeeeee;}
					else{lpstaBPixel[dispP] = collor2;}
				}
			}
		}
	}
	return 0;
}

////数字を一桁ずつ表示msg領域////070204
DWORD jussin_msg(DWORD kazu, BYTE keta, WORD XX, WORD YY, DWORD collor1, DWORD collor2)
{
	//表示する数字と桁をもってきま。
	BYTE i, j, k, nanketa;//d_kazu ha 256=0だけど気にしない
	BOOL minus = FALSE;
	DWORD dispP;
	DWORD d_kazu;//桁あふれてた060723
	if(keta < 6 && kazu > 65535){kazu *= -1; minus = TRUE;}
	for(nanketa = 0; nanketa < keta; nanketa++){
		d_kazu = kazu / pow(10, nanketa);//keta桁を１桁目に持っていく
		i = d_kazu % 10;

		for(j = 0; j < 8; j++){
			for(k = 0; k < 8; k++){
				if(XX + k >= Dwidth){break;}
				dispP = XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (7 - j + YY));
				if(*(pix.data[pix.numb_ad + i] +  k + (j * 8)) == 0x00ffff00){
					if(minus){lpmesBPixel[dispP] = 0xeeeeee;}
					else{lpmesBPixel[dispP] = collor1;}
				}
				if(*(pix.data[pix.numb_ad + i] + k + (j * 8)) == 0x00808000){
					if(minus){lpmesBPixel[dispP] = 0xeeeeee;}
					else{lpmesBPixel[dispP] = collor2;}
				}
			}
		}
	}
	return 0;
}

////数字を一桁ずつ表示////060730
DWORD jussin(DWORD kazu, BYTE keta, WORD XX, WORD YY, DWORD collor1, DWORD collor2)
{
	//表示する数字と桁をもってきま。
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
				dispP = XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (7 - j + YY));
				if(dispP > MAXPIX){break;}
				if(XX + k >= Dwidth){break;}
				if(*(pix.data[pix.numb_ad + i] + k + (j * 8)) == 0x00ffff00){
					if(minus){lpBBPixel[dispP] = 0xeeeeee;}
					else{lpBBPixel[dispP] = collor1;}
				}
				if(*(pix.data[pix.numb_ad + i] + k + (j * 8)) == 0x00808000){
					if(minus){lpBBPixel[dispP] = 0xeeeeee;}
					else{lpBBPixel[dispP] = collor2;}
				}
			}
		}
	}
	return 0;
}

void moji3(short x, short y, BYTE code, char *gecha, DWORD color)
{
	short i, j, k;

	BYTE a_ofset = 65;//aのchara code:0065
	BYTE iconofst = 16;//アイコン管理
	char w[32];//文字置き場
	
	LPDWORD mendoi;
	DWORD mendisp;
	BYTE mul = 1;
	BYTE masutype = 8;
	BOOL hant[2] = {0, 0};//xy 反転
	
	k = 0;
	if(y < 0){hant[1] = 1; y = -y - 1;}//負は反転
	if(x < 0){hant[0] = 1; x = -x - 1;}
	
	while(gecha[k] != NULL){//文字列エンドはﾇﾙﾙｰ
		switch(gecha[k]){
			case 32:w[k] = 64; break;//ｓｕｐｅ－ｓｕ
			case 47:w[k] = 31 + a_ofset; break;//slash

			case 48:
			case 49:
			case 50:
			case 51:
			case 52:
			case 53:
			case 54:
			case 55:
			case 56:
			case 57:w[k] = gecha[k] + 1; break;//数
			
			case 58:w[k] = 30 + a_ofset; break;//colon
			case 63:w[k] = 28; break;//hatena

			default :w[k] = (char)gecha[k]; break;//文字列から１字ずつ入
		}
		if(k == 31){break;}
		k++;
	}
	
	for(; k > 0; k--){
		mendisp = ((y + (hant[1] * masutype)) * Dwidth) + (mul * ((k - 1) * masutype)) + x + (hant[0] * masutype);
		mendoi = pix.data[pix.numb_ad + w[k - 1] + iconofst - a_ofset] + masutype * (masutype - 1);//←読み込みＹ予約

		for(i = 0; i < 8; i++){
			for(j = 0; j < 8; j++){
				if(*mendoi == 0x00ffff00){
					*mendoi = color;

					if(*mendoi != 0x00ffffff){
						switch(code){
							case 0:	setpix(mendisp, *mendoi, mul); break;
							case 1:	setpix_sta(mendisp, *mendoi, mul); break;
							case 2:	setpix_msg(mendisp, *mendoi, mul); break;
						}
					}
					
					*mendoi = 0x00ffff00;
				}
				mendoi++;//次のXpix
				if(hant[0]){mendisp -= mul;}//次のXひょうじ位置
				else{mendisp += mul;}
			}
			if(hant[0]){j *= -1;}
			if(hant[1]){mendisp -= (mul * (Dwidth + j));} //640:次の段表示
			else{mendisp += mul * (Dwidth - j);} //640:次の段表示
			mendoi -= masutype * 2;
		}
	}
	gecha = "/";
	w[0] = (char)gecha[0];
//	jussin(w[0], 5 , 50, 120, 0);//十進下マス表示
//	jussin(w[1], 5 , 50, 130, 0);//十進下マス表示
//	jussin(w[2], 5 , 50, 140, 0);//十進下マス表示
//	jussin(w[3], 5 , 50, 150, 0);//十進下マス表示
//	jussin(w[4], 5 , 50, 160, 0);//十進下マス表示
//	jussin(w[8], 5 , 50, 140, 0);//十進下マス表示

}

void makestatewin(BYTE stype)//ステータスウィンドウ
{
	LPDWORD lpPixel;
	DWORD lpB;//算出キャラpixel格納
	WORD iconH, iconW;
	BYTE i, j, counticon;
	BYTE beef = 0;
	
	BYTE stwinW = 14 - 1, stwinH = 18 - 1;//サイズ16のマスでの大きさ
	BYTE sikii[8] = {0, 0, 0, 0, 0, 0, 0, 0};//敷居専用
	BOOL ao = TRUE;//青地あり
	BOOL kuro = FALSE;//黒塗りあり
	
	i = 0;
	if(stype == 0){
		stwinW = 24 - 1, stwinH = 11 - 1;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;

		sikii[i] = 202; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0;
		kuro = FALSE;
		ao = TRUE;
	}
	else if(stype == 21){
		stwinW =  24 - 1, stwinH = 10 - 1;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;

		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0;
		ao = TRUE;
	}
	else if(stype == 210){
		stwinW =  22 - 1, stwinH = 6 - 1;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;

		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0;
		ao = TRUE;
	}
	else if(stype == 3){
		stwinW =  24 - 1, stwinH = 10 - 1;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;

		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0;
		kuro = TRUE;
		ao = TRUE;
	}
	i = 0;
	
	for(i = 1; i < stwinW; i++){//上バー
		StretchDIBits(hdcstaBMP, i * 16, 0, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 10], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
	}
	//右上
		StretchDIBits(hdcstaBMP, stwinW * 16, 0, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 11], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
	
	for(i = 1; i < stwinH; i++){//右バー
		StretchDIBits(hdcstaBMP, stwinW * 16, i * 16, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 12], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
	}

	//右下
		StretchDIBits(hdcstaBMP, stwinW * 16, stwinH * 16, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 13], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);

	for(i = 1; i < stwinW; i++){//したバー
		StretchDIBits(hdcstaBMP, 16 * (stwinW - i), stwinH * 16, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 14], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
	}
	//左下
		StretchDIBits(hdcstaBMP, 0, stwinH * 16, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 15], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);

	for(i = 1; i < stwinH; i++){//左バー
		StretchDIBits(hdcstaBMP, 0, 16 * (stwinH - i), 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 16], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
	}
	//左上
		StretchDIBits(hdcstaBMP, 0, 0, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 17], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
		 
	if(ao){
		for(i = 1; i < stwinW; i++){
			for(j = 1; j < stwinH; j++){//青地
				StretchDIBits(hdcstaBMP, 16 * i, 16 * j, 16, 16,
 				 0, 0, 16, 16, pix.data[pix.keyb_ad + 9], BMPF[beef].lpbiInfo,
				 DIB_RGB_COLORS, SRCCOPY);
			}
		}
	}
//	if(stype==21){return;}
	for(i = 0; i < 4; i++){//横敷居
		if(!sikii[i]){break;}
		for(j = 1; j < stwinW; j++){
			StretchDIBits(hdcstaBMP, 16 * j, sikii[i], 16, 16,
			 0, 0, 16, 16, pix.data[pix.keyb_ad + 10], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
		}

		StretchDIBits(hdcstaBMP, 0, sikii[i], 16, 16,
		 0, 0, 16, 16, pix.data[pix.keyb_ad + 18], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);

		StretchDIBits(hdcstaBMP, 16 * stwinW, sikii[i], 16, 16,
		 0, 0, 16, 16, pix.data[pix.keyb_ad + 19], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);

	}
	
	for(i = 4; i < 8; i++){//縦敷居
		if(!sikii[i]){break;}

		for(j = 1; j < stwinH - 1; j++){
			StretchDIBits(hdcstaBMP, sikii[i], 16 * j, 16, 16,
			 0, 0, 16, 16, pix.data[pix.keyb_ad + 16], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
		}
		StretchDIBits(hdcstaBMP, sikii[i], 0, 16, 16,
		 0, 0, 16, 16, pix.data[pix.keyb_ad + 20], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);

	}

	if(stype == 0 || stype == 3){
		for(counticon = 0; counticon < 4; counticon++){
			iconW = (40 * (counticon % 8)) + 48;
			iconH = 8;
			for(i = 0; i < 16; i++){//scrollicon
				for(j = 0; j < 16; j++){
					lpB = j + iconW + ((i + iconH) * STAwidth);
					lpPixel = pix.data[counticon + pix.keyb_ad] + j + ((15 - i) * 16);
					if(*lpPixel){
						setpix_sta(lpB, *lpPixel, 2);
					}
				}
			}
		}
	}

	if(kuro == FALSE){
		return;
	}
	for(i = 13; i < stwinW; i++){//kuro
		for(j = 2; j < stwinH - 1; j++){
			StretchDIBits(hdcstaBMP, 16 * i, (16 * j), 16, 16,
 			 0, 0, 16, 16, pix.data[pix.keyb_ad + 8], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
		}
	}
}

void makemessagewin(BYTE stype)//メッセージウィンドウ
{
	BYTE i, j, ch, counticon;
	BYTE mswinW = 40 - 1, mswinH = 11 - 1;//サイズ16のマスでの大きさ
	LPDWORD lpPixel;
	DWORD lpB;//算出キャラpixel格納
	WORD sikii[8] = {0, 0, 0, 0, 0, 0, 0, 0};//敷居専用
	BYTE beef = 0;
//	BYTE icon16p[16] = {40, 0, 45, 46, 0, 0, 0, 34,
//						0, 1, 2, 3, 4, 5, 6, 7};
//	BYTE icon16t[16] = {8, 1, 8, 8, 0, 0, 0, 8, 
//						8, 8, 8, 8, 8, 8, 8, 8};//
	WORD iconH, iconW;
	
	i = 0;
	if(stype == 0 || stype == 3){
		mswinW = 40 - 1, mswinH = 11 - 1;
		sikii[i] = 144; i++;
//		sikii[i] = 128; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;

		sikii[i] = 320; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0;
	}
	else if(stype == 7 || stype == 8){
		mswinW =  24 - 1, mswinH = 4 - 1;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0; i++;
		sikii[i] = 0;
	}

	i = 0;
	
	for(i = 1; i < mswinW; i++){
		StretchDIBits(hdcmesBMP, i * 16, 0, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 10], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
	}

		StretchDIBits(hdcmesBMP, mswinW * 16, 0, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 11], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
	
	for(i = 1; i < mswinH; i++){
		StretchDIBits(hdcmesBMP, mswinW * 16, i * 16, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 12], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
	}

		StretchDIBits(hdcmesBMP, mswinW * 16, mswinH * 16, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 13], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);

	for(i = 1; i < mswinW; i++){
		StretchDIBits(hdcmesBMP, 16 * (mswinW - i), mswinH * 16, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 14], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
	}

		StretchDIBits(hdcmesBMP, 0, mswinH * 16, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 15], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);

	for(i = 1; i < mswinH; i++){
		StretchDIBits(hdcmesBMP, 0, 16 * (mswinH - i), 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 16], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
	}

		StretchDIBits(hdcmesBMP, 0, 0, 16, 16,
 		 0, 0, 16, 16, pix.data[pix.keyb_ad + 17], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
		 
	for(i = 1; i < mswinW; i++){
		for(j = 1; j < mswinH; j++){
			StretchDIBits(hdcmesBMP, 16 * i, 16 * j, 16, 16,
 			 0, 0, 16, 16, pix.data[pix.keyb_ad + 9], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
		}
	}
////////
	for(i = 0; i < 4; i++){//横敷居
		if(!sikii[i]){break;}
		for(j = 1; j < mswinW; j++){
			StretchDIBits(hdcmesBMP, 16 * j, sikii[i], 16, 16,
			 0, 0, 16, 16, pix.data[pix.keyb_ad + 10], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
		}
		StretchDIBits(hdcmesBMP, 16 * mswinW, sikii[i], 16, 16,
		 0, 0, 16, 16, pix.data[pix.keyb_ad + 19], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);

		StretchDIBits(hdcmesBMP, 0, sikii[i], 16, 16,
	 	0, 0, 16, 16, pix.data[pix.keyb_ad + 18], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
	}
	i = 4;
	for(; i < 8; i++){//縦敷居
		if(!sikii[i]){break;}

		for(j = 1; j < mswinH - 1; j++){
			StretchDIBits(hdcmesBMP, sikii[i], 16 * j, 16, 16,
			 0, 0, 16, 16, pix.data[pix.keyb_ad + 16], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
		}
		StretchDIBits(hdcmesBMP, sikii[i], 0, 16, 16,
		 0, 0, 16, 16, pix.data[pix.keyb_ad + 20], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);

		StretchDIBits(hdcmesBMP, sikii[i], sikii[i % 4], 16, 16,
		 0, 0, 16, 16, pix.data[pix.keyb_ad + 21], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
	}

	if(stype == 7 || stype == 8){return;}
	if(stype == 3){
		for(counticon = 0; counticon < 4; counticon++){
			iconW = (48 * (counticon % 8)) + 432;
			iconH = 8;
			for(i = 0; i < 16; i++){//scrollicon
				for(j = 0; j < 16; j++){
					lpB = j + iconW + ((i + iconH) * MESwidth);
					lpPixel = pix.data[counticon + pix.keyb_ad] + j + ((15 - i) * 16);
					if(*lpPixel){
						setpix_msg(lpB, *lpPixel, 2);
					}
				}
			}
		}
	}
}

void draw_cursol(BYTE c_dir, BYTE piad, LPWORD xyinfo, LPBYTE maxsel, BYTE sel, BOOL selc)
{
	DWORD lbm;
	LPDWORD lpx;
	

	BYTE i, j;
	WORD of_x, of_y, step_x, step_y;
	BYTE max_x, max_y;
	of_x = *xyinfo;
	of_y = *(xyinfo + 1);
	step_x = *(xyinfo + 2);
	step_y = *(xyinfo + 3);
	max_x = *maxsel;
	max_y = *(maxsel + 1);
	
	
	for(i = 0; i < 16; i++){//menu
		for(j = 0; j < 16; j++){
			lbm = j + of_x + (((i * 1) + of_y) * MESwidth);
			if(selc){//縦並び
				lbm += ((sel / max_x) * step_x);
				lbm += (((sel % max_x) % max_y) * step_y * MESwidth);
			}
			else{
				lbm += ((sel % max_x) * step_x);
				lbm += (((sel / max_x) % max_y) * step_y * MESwidth);
			}
			if(c_dir == 1){//カーソル向き
				lpx = pix.data[piad + pix.keyb_ad] + i + (j * 16);
			}
			else if(c_dir == 2){//カーソル向き
				lpx = pix.data[piad + pix.keyb_ad] + i + ((15 - j) * 16);
			}
			else if(c_dir == 4){
				lpx = pix.data[piad + pix.keyb_ad] + (15 - j) + ((15 - i) * 16);
			}
			else if(c_dir == 8){
				lpx = pix.data[piad + pix.keyb_ad] + 15 - i + ((15 - j) * 16);
			}
			if(*lpx){
				setpix(lbm, *lpx, 1);
			}
		}
	}
}

void menucursol(void)
{
	BYTE i, j;
	BYTE ofs = 28, an = 0;

	WORD of[4] ={0, 0, 0, 0}; //of_x= 0, of_y = 0, step_x = 0, step_y = 0;

	BYTE maxsel[2] = {1, 1};
	BYTE mul = 1;
	BYTE cursol_dir = 1; //カーソルむき
	BOOL selcnt = FALSE; //F:横, T:縦
	BYTE sel = 0;//現在選択
	
	if(mode == 1 || mode == 2 || mode == 3 || mode == 9 || mode == 10){//wavetest用

		of[0] = 32; of[1] = STATOP + 8;
		of[2] = 40; of[3] = 12;
		maxsel[0] = 4; maxsel[1] = 4;
		mul = 1; selcnt = FALSE;
		cursol_dir = 4; sel = selch;
		draw_cursol(cursol_dir, ofs + an, of, maxsel, sel, selcnt);

		if(!menuact){an = (Lcount - 1) / 15;}
		of[0] = 32 + (selch * of[2]); of[1] = 24 + STATOP;
		of[2] = 72; of[3] = 11;
		maxsel[0] = 10; maxsel[1] = 10;
		mul = 1; selcnt = TRUE;
		cursol_dir = 4; sel = value[mode];
		draw_cursol(cursol_dir, ofs + an, of, maxsel, sel, selcnt);
	}
	
	if(menuact && !typeact){
		an = (Lcount - 1) / 15;
		of[0] = 204; of[1] = STATOP + 17;
		of[2] = 32; of[3] = 12;
		maxsel[0] = 9, maxsel[1] = 9;
		mul = 1;
		cursol_dir = 4;
		sel = selectmenu;
		selcnt = TRUE;
		draw_cursol(cursol_dir, ofs + an, of, maxsel, sel, selcnt);
		
	}

	if(typeact){
		an = 0;
		if(!menuact){an = (Lcount - 1) / 15;}
		of[0] = 8; of[1] = STATOP + 24;
		if(selecttype % 15 > 4){
			of[0] += 16;
		}
		if(selecttype % 15 > 9){
			of[0] += 16;
		}
		//入力
		of[2] = 16; of[3] = 24;
		maxsel[0] = 15; maxsel[1] = 6;
		sel = selecttype; selcnt = FALSE;
		draw_cursol(cursol_dir, ofs + an, of, maxsel, sel, selcnt);//まずは文字盤

		if(menuact){
			an = (Lcount - 1) / 15;
			of[0] = 10; of[1] = MESTOP + 46;
			of[2] = 80; of[3] = 16;
			maxsel[0] = 16; maxsel[1] = 3;
			cursol_dir = 4;
			sel = value[mode];
			draw_cursol(cursol_dir, ofs + an, of, maxsel, sel, selcnt);//メッセージデータ位置
		}
		else{
		//名前位置
		of[0] = 10; of[1] = 360;
		of[2] = 14; of[3] = 16;
		maxsel[0] = 16; maxsel[1] = 2;
		sel = message_sel;
		
		draw_cursol(cursol_dir, ofs + an, of, maxsel, sel, selcnt);//メッセージデータ位置
		}

	}
	if(numact){//num用
		an = 0;
		if(!typeact){an = (Lcount - 1) / 15;}

		of[0] = 302; of[1] = STATOP + 18;
		of[2] = 40; of[3] = 12;
		maxsel[0] = 8; maxsel[1] = 8;
		mul = 1; selcnt = TRUE;
		cursol_dir = 4; sel = selectnumc;
		draw_cursol(cursol_dir, ofs + an, of, maxsel, sel, selcnt);
	}
	
	if(!of[0] && !of[1]){return;}
}

void clmap(void)
{
	BYTE i, j;
	
	for(i = 0; i < Mheight; i++){
		for(j = 0; j < Mwidth; j++){
//			StretchDIBits(hdcCmapBMP, j * 32, i * 32, 32, 32,
//			 0, 0, 32, 32, pix.data[pix.cmap_ad[MAP128[0].palette] + 0], BMPF[1].lpbiInfo,
//			 DIB_RGB_COLORS, SRCCOPY);
		}
	}
}

BYTE set_msgdata_r(BYTE bank, WORD sel, BYTE waku)
{
	LPDWORD mendoi;
	DWORD color;
	DWORD col = 0x102010;//出力用
	DWORD mendisp;
	WORD i, j, x, y;
	BYTE mul = 1;
	BYTE masutype = 12;
	BYTE mojidata;
	
	//"r"//
	x = ((sel % 16) * 14) + 12;//基本color位置に注意。塗りつぶされる
	y = (((sel / 16) % 3) * 16) + 12;
	if(bank){
		x = ((sel % 16) * 14) + 86;//基本color位置に注意。塗りつぶされる
		y = ((bank - 1) * 16) + 12;
	}
//	y += (waku % 3) * 64;//どんな数でも3つのウィンドウ位置へ
//	mojidata = msgdata[bank].w[sel];//"r"
	if(!bank){
		mojidata = thisname[sel];
	}
	else{//"r"
		mojidata = filename[(bank + 256 - 1) % 256][sel];
		if(bank != 2){col = 0x40a040;}
	}

	mendisp = (y * STAwidth) + (x);
	if(mojidata == 255){//endfile
		masutype = 8;
		mendoi = pix.data[pix.numb_ad + 20] + masutype * (masutype - 1);//←読み込みＹ予約
	}
	else{
		mendoi = pix.data[pix.font12_ad + mojidata - 1] + masutype * (masutype - 1);//←読み込みＹ予約
	}
//	color = *(lpstaBPixel + 16 + (16 * STAwidth));//"r"
	color = *(lpmesBPixel + 11 + ((11) * STAwidth));

	if(mojidata == 0){
		mendoi = lpmesBPixel;
		for(i = 0; i < masutype; i++){
			for(j = 0; j < masutype; j++){
				if(*(mendoi + mendisp) != color){
					setpix_msg(mendisp, color, 1);
				}
//				setpix_sta(mendisp, color, 1);
				mendisp++; //640:次の段表示
			}
			mendisp += (MESwidth - j); //640:次の段表示"r"
		}
		return(mojidata);
	}
	else if(mojidata == 255){
		for(i = 0; i < masutype; i++){
			for(j = 0; j < masutype; j++){
				setpix_sta(mendisp, *mendoi, 2);
				mendoi++; //640:次の段表示
				mendisp += 2; //640:次の段表示
			}
			mendisp += (STAwidth - j) * 2; //640:次の段表示
		}
		return(mojidata);
	}
	for(i = 0; i < masutype; i++){
		for(j = 0; j < masutype; j++){
			if(*mendoi != color){
				setpix_msg(mendisp, color, 1);
			}
			if(*mendoi){
				if(waku < 3){
					setpix_msg(mendisp, col, 1);
				}
				else if(waku < 6){
					setpix_msg(mendisp, 0x999922, 1);
				}
				else if(waku < 9){
					setpix_msg(mendisp, 0xffaaaa, 1);
				}
			}
			mendoi++;//次のXpix
			mendisp++; //640:次の段表示
		}
		mendisp += (STAwidth - j); //640:次の段表示
		mendoi -= masutype * 2;
	}
	return(mojidata);
}


void wordblast_p(WORD m, BYTE type)//ﾌﾟﾛﾄﾀｲﾌﾟ
{

	BYTE wordcnt = messagecnt % 48;//48文字
	BYTE pagecnt = messagecnt / 48;//8ぺ－じ
	BOOL halfpg = FALSE;
	WORD wa = msgdata[m].w[pagecnt];

	LPDWORD mendoi;
	DWORD col = 0x204020;//カラー
	DWORD mendisp;
	BYTE mul = 1;
	BYTE masutype = 12;
	BYTE iconofst = 0;//アイコン管理
	BYTE maxtype = 249;

//	BYTE c = 0;//現在
	BYTE i, j;
	WORD x, y;
	
	if(m > maxtype){return;}

	if(type == 5){
		wa = m;
		x = ((wordcnt % 16) * 14) + 10;
		y = (((wordcnt / 16) * mul) * 9) + 6;
//		y = 6;
	}
	else if(type == 1){
		x = ((wordcnt % 16) * 10) + 10;
		y = (((wordcnt / 16) * mul) * 9) + 6;
	}
	else if(type == 4){//ダイレクト文字表示
		wa = m;
		x = ((m % 15) * 16) + 10;
		if((m % 15) > 4){
			x += 16;
		}
		if((m % 15) > 9){
			x += 16;
		}
		y = ((m / 15) * 24);
		y = y % (24 * 6);
		y += 12;
	}

	mendisp = (y * MESwidth) + (mul * x);
	mendoi = pix.data[pix.font12_ad + wa] + masutype * (masutype - 1);//←読み込みＹ予約


	for(i = 0; i < 12; i++){
		for(j = 0; j < 12; j++){
			if(*mendoi){
				if(type % 3 == 0){
					setpix(mendisp, col, mul);
				}
				else if(type % 3 == 1){
					setpix_sta(mendisp, col, mul);
				}
				else{
					setpix_msg(mendisp, col, mul);
				}
			}
			mendoi++;//次のXpix
			mendisp += mul;//次のXひょうじ位置
		}
		mendisp += mul * (MESwidth - j); //640:次の段表示
		mendoi -= masutype * 2;
	}
}


void fontboard(BYTE page)
{
	BYTE i;
	makestatewin(21);
	for(i = 0; i < 90; i++){
		wordblast_p(i + (page * 90), 4);
	}
}


void bbset(void){
	BitBlt(hdcBB, 0, 0, Dwidth, Dheight - STAheight - MESheight, hdcCmapBMP, 0, 0, SRCCOPY);
	BitBlt(hdcBB, 0, STATOP, STAwidth, STAheight, hdcstaBMP, 0, 0, SRCCOPY);
	BitBlt(hdcBB, 0, MESTOP, MESwidth, MESheight, hdcmesBMP, 0, 0, SRCCOPY);
	
}

void tuneb_draw(void)
{
	WORD x, y;
	DWORD color;
	
	BitBlt(hdcBB, 0, STATOP, STAwidth, STAheight, hdcstaBMP, 0, 0, SRCCOPY);
//	moji3(x, y, 0, menuname, menucolor);

}

void keyb_draw(void)
{
	BYTE i, j, p;
	BYTE bsize = 32;
	BYTE beef = 0;
//	char *mojji[15] = {"Q", "W", "3", "E", "4",
//						"R", "T", "6", "Y", "7", 
//						"U", "8", "I", "O", "P"};
	WORD x, y;
	DWORD disps;
	LPDWORD ndo;
	char *mojji[23] = {"Q", "W", "E", "R", "T", "Y",
						"U", "I", "O", "P",
						"2", "3", " ", "5", "6", "7", " ",  "9", "0",
						"A", "Z", "K", "M"};

	for(j = 0; j < 12; j++){
		if(j == 0 || j == 11){//鍵盤切り替えスイッチ
			StretchDIBits(hdcmesBMP, j * bsize, 0, bsize, bsize,
			 0, 0, bsize / 2, bsize / 2, pix.data[pix.keyb_ad + 26], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
			StretchDIBits(hdcmesBMP, j * bsize, bsize, bsize, bsize,
			 0, 0, bsize / 2, bsize / 2, pix.data[pix.keyb_ad + 34], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
		}
		else{//普通の白鍵盤
			StretchDIBits(hdcmesBMP, j * bsize, 0, bsize, bsize,
			 0, 0, bsize / 2, bsize / 2, pix.data[pix.keyb_ad + 24], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
			StretchDIBits(hdcmesBMP, j * bsize, bsize, bsize, bsize,
			 0, 0, bsize / 2, bsize / 2, pix.data[pix.keyb_ad + 32], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
		}
	}
	for(p = 0; p < 11; p++){
		x = (p * bsize) + 24;
		y = 0;
		disps = x + (y * MESwidth);
		ndo = pix.data[pix.keyb_ad + 25] + (16 * 15);
		for(i = 0; i < 16; i++){//黒鍵
			if(p == 0 || p == 3 || p == 7 || p == 10){break;}
			for(j = 0; j < 16; j++){
				if(*ndo){
					setpix_msg(disps, *ndo, 2);
				}
				disps += 2;
				ndo++;
			}
			disps += (MESwidth * 2) - 32;
			ndo -= 32;
		}
	}
	
	for(i = 0; i < 10; i++){
		moji3((bsize * i) + 48, bsize + 4, 2, mojji[i], 0x204020);
	}
	for(i = 0; i < 9; i++){
		moji3((bsize * i) + 62, 8, 2, mojji[i + 10], 0x80ff80);
	}
	for(i = 0; i < 2; i++){
		moji3(8 + (i * 8), 6 + (i * 32), 2, mojji[i + 19], 0x204020);
		moji3(360 + (i * 8), 6 + (i * 32), 2, mojji[i + 21], 0x204020);
	}
	BitBlt(hdcBB, 0, MESTOP, MESwidth, MESheight, hdcmesBMP, 0, 0, SRCCOPY);
	
}

void wv_info(void)
{
	WORD x,y;
	BYTE step_y = 11;
	BYTE step_x = 40;
	BYTE i;
	DWORD chcolor1[4] = {0x204020, 0x204020, 0x204020, 0x204020};
	DWORD chcolor2[4] = {0x102010, 0x102010, 0x102010, 0x102010};
//	char *kcode[12] = {"C","C","D","D","E","F","F","G","G","A","A","B"};


//	y = 220;
	x = 48;
	
//	jussin(now_buff, 1, x + 10, y - 160, 0xdddddd, 0x666666);
//	jussin(rebuff, 1, x + 20, y - 160, 0xdddddd, 0x666666);
//	jussin(wv_ch[0].length, 5, x + 80, y - 160, 0xccdddd, 0x666666);
//	jussin(wv_ch[0].loops, 6, x + 80, y - 170, 0xccdddd, 0x666666);
//	jussin(oscpos, 8, x + 80, y - 40, 0xcccccc, 0x666666);
	
	for(i = 0; i < 4; i++){
		y = STATOP + 28;
		if(!wv_ch[i].on){chcolor1[i] = 0x40a040; chcolor2[i] = 0x40a040;}

		jussin(wv_ch[i].vol, 2, x, y, chcolor1[i], chcolor2[i]);
		y += step_y;

		//ATT
		if((wv_ch[i].t_att) % 16 > 9){//att	基本16進1桁目
			moji3(x + 16, y, 0, mocode[(wv_ch[i].t_att % 16) - 10], chcolor2[i]);
		}
		else{
			jussin(wv_ch[i].t_att % 16, 1, x + 16, y, chcolor1[i], chcolor2[i]);//dlx
		}
		if(wv_ch[i].t_att < 64){
			moji3(x, y, 0, "N", chcolor2[i]);
			jussin(wv_ch[i].t_att / 16, 1, x + 8, y, chcolor1[i], chcolor2[i]);//dlx
		}
		else{
			moji3(x + 8, y, 0, mocode[((wv_ch[i].t_att - 64) / 48)], chcolor2[i]);
			jussin((((wv_ch[i].t_att - 64) / 16) % 3) + 1, 1, x, y, chcolor1[i], chcolor2[i]);
		}
		y += step_y;

		//GAIN
		if((wv_ch[i].t_gin) % 16 > 9){//gin	基本16進
			moji3(x + 16, y, 0, mocode[(wv_ch[i].t_gin % 16) - 10], chcolor2[i]);
		}
		else{
			jussin(wv_ch[i].t_gin % 16, 1, x + 16, y, chcolor1[i], chcolor2[i]);//dlx
		}
		if(wv_ch[i].t_gin < 64){
			moji3(x, y, 0, "N", chcolor2[i]);
			jussin(wv_ch[i].t_gin / 16, 1, x + 8, y, chcolor1[i], chcolor2[i]);//dlx
		}
		else{
			moji3(x + 8, y, 0, mocode[(wv_ch[i].t_gin - 64) / 48], chcolor2[i]);
			jussin((((wv_ch[i].t_gin - 64) / 16) % 3) + 1, 1, x, y, chcolor1[i], chcolor2[i]);
		}
		y += step_y;

		//DCH
		if(wv_ch[i].dch){
			moji3(x, y, 0, mocode[1 - (((wv_ch[i].dch - 1) / 32) % 2)], chcolor2[i]);
			jussin(((wv_ch[i].dch - 1) / 16) % 2, 1, x + 8, y, chcolor1[i], chcolor2[i]);

			if((wv_ch[i].dch - 1) % 16 > 9){
				moji3(x + 16, y, 0, mocode[((wv_ch[i].dch - 1) % 16) - 10], chcolor2[i]);
			}
			else{
				jussin((wv_ch[i].dch - 1) % 16, 1, x + 16, y, chcolor1[i], chcolor2[i]);//dlx
			}
		}
		else{
			jussin(0, 3, x, y, chcolor1[i], chcolor2[i]);
		}

		y += step_y;

		jussin(wv_ch[i].dly, 3, x, y, chcolor1[i], chcolor2[i]);
		y += step_y;

		jussin(wv_ch[i].msec, 3, x, y, chcolor1[i], chcolor2[i]);
		y += step_y;

		jussin(wv_ch[i].type, 2, x, y, chcolor1[i], chcolor2[i]);
		y += step_y;

		//FS
		if(wv_ch[i].fs > 128){
			jussin((((wv_ch[i].fs - 129) / 4) % 4) + 1, 1, x, y, chcolor1[i], chcolor2[i]);
			moji3(x + 8, y, 0, mocode[(wv_ch[i].fs - 129) / 16], chcolor2[i]);
			jussin(((wv_ch[i].fs - 1) % 4) + 1, 1, x + 16, y, chcolor1[i], chcolor2[i]);
		}
		else if(wv_ch[i].fs < 128){
			jussin(((131 - (wv_ch[i].fs / 4)) % 4) + 1, 1, x, y, chcolor1[i], chcolor2[i]);
			moji3(-x - 8, -y, 0, mocode[((127 - wv_ch[i].fs) / 16)], chcolor2[i]);
			jussin(4 - (wv_ch[i].fs % 4), 1, x + 16, y, chcolor1[i], chcolor2[i]);
		}
		else{
			jussin(0, 3, x, y, chcolor1[i], chcolor2[i]);
		}
		y += step_y;

		//PWS
		if(wv_ch[i].pws > 128){
			jussin((((wv_ch[i].pws - 129) / 4) % 4) + 1, 1, x, y, chcolor1[i], chcolor2[i]);
			moji3(x + 8, y, 0, mocode[(wv_ch[i].pws - 129) / 16], chcolor2[i]);
			jussin(((wv_ch[i].pws - 1) % 4) + 1, 1, x + 16, y, chcolor1[i], chcolor2[i]);
		}
		else if(wv_ch[i].pws < 128){
			jussin(((131 - (wv_ch[i].pws / 4)) % 4) + 1, 1, x, y, chcolor1[i], chcolor2[i]);
			moji3(-x - 8, -y, 0, mocode[((127 - wv_ch[i].pws) / 16)], chcolor2[i]);
			jussin(4 - (wv_ch[i].pws % 4), 1, x + 16, y, chcolor1[i], chcolor2[i]);
		}
		else{
			jussin(0, 3, x, y, chcolor1[i], chcolor2[i]);
		}
		y += step_y;

		//RFS
		if(wv_ch[i].rfs > 128){
			jussin((((wv_ch[i].rfs - 129) / 4) % 4) + 1, 1, x, y, chcolor1[i], chcolor2[i]);
			moji3(x + 8, y, 0, mocode[(wv_ch[i].rfs - 129) / 16], chcolor2[i]);
			jussin(((wv_ch[i].rfs - 1) % 4) + 1, 1, x + 16, y, chcolor1[i], chcolor2[i]);
		}
		else if(wv_ch[i].rfs < 128){
			jussin(((131 - (wv_ch[i].rfs / 4)) % 4) + 1, 1, x, y, chcolor1[i], chcolor2[i]);
			moji3(-x - 8, -y, 0, mocode[((127 - wv_ch[i].rfs) / 16)], chcolor2[i]);
			jussin(4 - (wv_ch[i].rfs % 4), 1, x + 16, y, chcolor1[i], chcolor2[i]);
		}
		else{
			jussin(0, 3, x, y, chcolor1[i], chcolor2[i]);
		}
		x += step_x;

	}

//	y += step_y;
//	jussin(onlvl[selch], 1, x - step_x, y, chcolor1[0], chcolor2[0]);

//	chcolor1[0] = 0x00007f;
//	chcolor1[0] += 0x000080 * ((testkey / 12) % 2);//0, 1
//	chcolor1[0] += 0x007900 * ((testkey / 24) % 2);//0, 1
//	chcolor1[0] += 0x790000 * ((testkey / 48) % 2);//0, 1

//	chcolor1[1] = 0xff2222;
//	chcolor1[2] = 0xffbbbb;
//	chcolor1[3] = 0xffffbb;
//	y += step_y + 4;
//	x = 432;
//	step_x = (testkey % 12) * 10;
//	if(testkey % 12 == 1 || testkey % 12 == 3 || testkey % 12 == 6 || testkey % 12 == 8 || testkey % 12 == 10){
//		step_y = 4;
//	}
//	else{step_y = 0;}
//	moji3(x + step_x, y - step_y, 0, kcode[testkey % 12], chcolor1[0]);
	
}

void wavetest_disp(BYTE m)//WAVE情報表示
{
	DWORD col = 0x102010;
	WORD x,y;
	BYTE step_y = 11;
	x = 0;
	y = 28;

	makestatewin(m);

	moji3(x, y, 1, " VLM ", col);
	y += step_y;
	moji3(x, y, 1, " ATT ", col);
	y += step_y;
	moji3(x, y, 1, " GIN ", col);
	y += step_y;
	moji3(x, y, 1, " DCH ", col);
	y += step_y;
	moji3(x, y, 1, " DLY ", col);
	y += step_y;
	moji3(x, y, 1, " LNG ", col);
	y += step_y;
	moji3(x, y, 1, " TYP ", col);
	y += step_y;
	moji3(x, y, 1, " FS ", col);
	y += step_y;
	moji3(x, y, 1, " PWS ", col);
	y += step_y;
	moji3(x, y, 1, " RFS ", col);

	
}

void note_info(void)
{
//	WORD x = 20, y = 130 + STATOP, is, ie, i, sta, end;
	WORD x = 20, y, is, ie, i, sta, end;
	BYTE seltime, selpage;
	BYTE infopage;
	BOOL ap;
	DWORD col = 0x102010;
	
	if(rngnote < 0){//選択範囲マイナス
		if(note_no[selch] + rngnote < 0){sta = 0;}//行きすぎストップ
		else{sta = note_no[selch] + rngnote;}
		end = note_no[selch];
	}
	else{
		sta = note_no[selch];
		end = note_no[selch] + rngnote;
	}

	seltime = wvnote[selch].time[note_no[selch]];
	selpage = wvnote[selch].page[note_no[selch]];
	if(!seltime && !selpage && !wvnote[selch].mode[note_no[selch]]){//データ何も無しは抜け
		return;
	}
	is = note_no[selch];
	ie = note_no[selch];
	if(is){
//		while((wvnote[selch].time[is - 1] == seltime) && (wvnote[selch].page[is] == selpage)){
		while((wvnote[selch].time[is - 1] == seltime) && (wvnote[selch].page[is - 1] == selpage)){
			if(!is){break;}
			is--;//同じ時間なら表示範囲を増やす
		}
	}
	if(ie < maxnote){
//		while((wvnote[selch].time[ie] == seltime) && (wvnote[selch].page[ie] == selpage)){
		while((wvnote[selch].time[ie + 1] == seltime) && (wvnote[selch].page[ie + 1] == selpage)){
			if(ie == maxnote){break;}
			if(!wvnote[selch].mode[ie + 1]){break;}
			ie++;
		}
	}
	x = 300;
	y = 20 + STATOP;
	moji3(x, y, 0, " TIME ", col);
 	jussin(wvnote[selch].time[note_no[selch]], 3, x + 50, y, col, col);

//	y +=10;
	if(wvnote[selch].mode[note_no[selch]] > 7){infopage = 1;}
	else{infopage = 0;}

	ap = TRUE;

	for(i = is; i < ie + 1; i++){
		x = 300;
		mo = wvnote[selch].mode[i];//２役変数:mo
		if(!infopage){
			if(mo > 7){
				ap = FALSE;
			}//セレクトがLNGまで
			else{
				ap = TRUE;
			}
		}
		else if(infopage){
			if(mo < 8 && mo != 1){
				ap = FALSE;
			}//セレクトがLNGまで
			else{
				ap = TRUE;
			}
		}

		if(((end < i || sta > i) || (Lcount % 4 < 2)) && ap){
			switch(mo){
				case 1: moji3(x, y + (mo * 10), 0, " NOTE ", col); break;
				case 2: moji3(x, y + (mo * 10), 0, " VLM ", col); break;
				case 3: moji3(x, y + (mo * 10), 0, " ATT ", col); break;
				case 4: moji3(x, y + (mo * 10), 0, " GIN ", col); break;
				case 5: moji3(x, y + (mo * 10), 0, " DCH ", col); break;
				case 6: moji3(x, y + (mo * 10), 0, " DLY ", col); break;
				case 7: moji3(x, y + (mo * 10), 0, " LNG ", col); break;//ここから2ページ目
				case 8: mo = 9; moji3(x, y + ((mo - 6) * 10), 0, " FS ", col); break;
				case 9: mo = 8; moji3(x, y + ((mo - 6) * 10), 0, " TYP ", col); break;
				case 10: moji3(x, y + ((mo - 6) * 10), 0, " PWS ", col); break;
				case 11: moji3(x, y + ((mo - 6) * 10), 0, " RFS ", col); break;
				default: moji3(x, y + (1 * 10), 0, "NODATA", col); break;
			}
			x += 50;
//			y += (mo * 10);
			if(mo == 3 || mo == 4){
				if((wvnote[selch].value[i]) % 16 > 9){//att	基本16進1桁目
					moji3(x + 16, y + (mo * 10), 0, mocode[(wvnote[selch].value[i] % 16) - 10], col);
				}
				else{
					jussin(wvnote[selch].value[i] % 16, 1, x + 16, y + (mo * 10), col, col);//dlx
				}
				if(wvnote[selch].value[i] < 64){
					moji3(x, y + (mo * 10), 0, "N", col);
					jussin(wvnote[selch].value[i] / 16, 1, x + 8, y + (mo * 10), col, col);//dlx
				}
				else{
					moji3(x + 8, y + (mo * 10), 0, mocode[((wvnote[selch].value[i] - 64) / 48)], col);
					jussin((((wvnote[selch].value[i] - 64) / 16) % 3) + 1, 1, x, y + (mo * 10), col, col);
				}
			}
			else if(mo == 5){
				if(wvnote[selch].value[i]){
					moji3(x, y + (mo * 10), 0, mocode[1 - ((wvnote[selch].value[i] / 32) % 2)], col);
					jussin((wvnote[selch].value[i] / 16) % 2, 1, x + 8, y + (mo * 10), col, col);
		
					if((wvnote[selch].value[i]) % 16 > 9){
						moji3(x + 16,  y + (mo * 10), 0, mocode[(wvnote[selch].value[i] % 16) - 10], col);
					}
					else{
						jussin(wvnote[selch].value[i] % 16, 1, x + 16,  y + (mo * 10), col, col);//dlx
					}
				}
				else{
					jussin(0, 3, x,  y + (mo * 10), col, col);
				}
			}
			
			else if(mo == 9 || mo == 10 || mo == 11){//fs
				if(wvnote[selch].value[i] > 128){
					jussin((((wvnote[selch].value[i] - 129) / 4) % 4) + 1, 1, x, y + ((mo - 6) * 10), col, col);
					moji3(x + 8, y + ((mo - 6) * 10), 0, mocode[(wvnote[selch].value[i] - 129) / 16], col);
					jussin(((wvnote[selch].value[i] - 1) % 4) + 1, 1, x + 16, y + ((mo - 6) * 10), col, col);
				}
				else if(wvnote[selch].value[i] < 128){
					jussin(((131 - (wvnote[selch].value[i] / 4)) % 4) + 1, 1, x, y + ((mo - 6) * 10), col, col);
					moji3(-x - 8, -(y + ((mo - 6) * 10)), 0, mocode[((127 - wvnote[selch].value[i]) / 16)], col);
					jussin(4 - (wvnote[selch].value[i] % 4), 1, x + 16, y + ((mo - 6) * 10), col, col);
				}
				else{
					jussin(0, 3, x, y + ((mo - 6) * 10), col, col);
				}

			}
			else{
				if(mo > 7){
					jussin(wvnote[selch].value[i], 3, x, y + ((mo - 6) * 10), col, col);
				}
				else{
					jussin(wvnote[selch].value[i], 3, x, y + (mo * 10), col, col);
				}
			}
//			y += 10;
		}
	}
}
/*
BOOL search_note(BYTE ch)
{
	WORD i, j;
//	BYTE ch;
	BOOL fit = FALSE;
//	for(ch = 0; ch < 4; ch++){
		for(i = 0; i < maxnote; i++){//位置を検索
			if(wvnote[ch].page[i] == score_page){
				if(wvnote[ch].time[i] == score_time){
//					fit += 1 < ch;
					fit = TRUE;
					break;
				}
			}
			if(!wvnote[ch].mode){
				break;
//				break;
			}
		}
//	}
	return fit;

}
*/
void head_draw(void)//再生位置表示
{
	BYTE i, j, ch, con;
	WORD x, y, stime;
	DWORD disps, col = 0x102010;
	LPDWORD lpd;
	BYTE mojico[24] = {4, 45, 41, 21, 11, 30, 12, 5, 83,//おわりにしますか？
					171, 4, 45, 41, 85, 184, 198, 182, 46,//おわり・ＥＳＣ＿
					 120, 150, 85, 29, 5, 173,};//マダ・ほか
	BYTE bye[8] = {181, 234, 214, 85, 181, 234, 214, 81};
//	BYTE credi[22] = {191, 218, 229, 227, 224,  179, 190, 214, 234, 211,  224, 210, 227, 213,//Litro Keyboard
//					 179, 179, 239, 217, 210, 222, 225, 214};// champe
	BYTE credi[19] = {242, 240, 240, 247,  207, 240, 248,//2007,08
					 179, 179, 239, 217, 210, 222, 225, 214,  179,  179,  179, 179};// champe
	
	BYTE minim[19] = {213, 224, 232, 223,  179,  220, 214, 234,//down key
					  179,  238,  179,  222, 218, 223, 218, 222, 218, 228, 214};// : minimise
	
	y = 10 + 28;

	if(output_f || mode > 253){
		x = 0;
	}
	else{
		x = score_time * 1.34;
	}
	lpd = pix.data[pix.keyb_ad + 56] + (16 * 15);
	icon_draw(lpd, 0, x, y, 16, 1);//再生キャラ表示

	if(washanime){
		if((washanime % 20) < 10){
			lpd = pix.data[pix.keyb_ad + 57] + (16 * 15);
			icon_draw(lpd, 0, x + 16, y, 16, 1);//再生キャラ表示
		}
		washanime--;
	}
	
	if(!output_f && mode < 254){//time表示
		if(rngnote < 0 && (Lcount / 30)){
			stime = ((wvnote[selch].page[note_no[selch]] - wvnote[selch].page[note_no[selch] - 1]) * 240) + (wvnote[selch].time[note_no[selch]] - wvnote[selch].time[note_no[selch] - 1]);
			jussin(stime, 4, x + 16, y + 8, 0x408040, 0x204020);
			moji3(x + 48, y + 8, 0, "STEP", 0x204020);
		}
		else if(rngnote > 0 && (Lcount / 30)){
			stime = ((wvnote[selch].page[note_no[selch] + 1] - wvnote[selch].page[note_no[selch]]) * 240) + (wvnote[selch].time[note_no[selch] + 1] - wvnote[selch].time[note_no[selch]]);
			jussin(stime, 4, x + 16, y + 8, 0x408040, 0x204020);
			moji3(x + 48, y + 8, 0, "STEP", 0x204020);
		}
		else{
			jussin(score_time, 3, x + 16, y + 8, 0x408040, 0x204020);
			moji3(x + 40, y + 8, 0, "TIME", 0x204020);
		}
	}
	
	if(mode == 255 && !byebye){
		for(con = 0; con < 24; con++){
			lpd = pix.data[pix.font12_ad + mojico[con]] + (12 * 11);
			disps = 18 + x + (con * 13) + (y * Dwidth);
			for(i = 0; i < 12; i++){
				for(j = 0; j < 12; j++){
					if(*lpd){
						setpix(disps, col, 1);
					}
					disps++;
					lpd++;
				}
				disps += Dwidth - j;
				lpd -= j * 2;
			}
		}
	}
	else if(mode == 255 && byebye){
		for(con = 0; con < 8; con++){
			lpd = pix.data[pix.font12_ad + bye[con]] + (12 * 11);
			disps = 18 + x + (con * 13) + (y * Dwidth);
			for(i = 0; i < 12; i++){
				for(j = 0; j < 12; j++){
					if(*lpd){
						setpix(disps, col, 1);
					}
					disps++;
					lpd++;
				}
				disps += Dwidth - j;
				lpd -= j * 2;
			}
		}
	}
	else if(mode == 254){
		for(con = 0; con < 19; con++){
			if(Lcount / 30){
				lpd = pix.data[pix.font12_ad + credi[con]] + (12 * 11);
			}
			else{
				lpd = pix.data[pix.font12_ad + minim[con]] + (12 * 11);
			}
			disps = 18 + x + (con * 13) + (y * Dwidth);
			for(i = 0; i < 12; i++){
				for(j = 0; j < 12; j++){
					if(*lpd){
						setpix(disps, col, 1);
					}
					disps++;
					lpd++;
				}
				disps += Dwidth - j;
				lpd -= j * 2;
			}
		}
	}
	
	x = Dwidth - 40;

	jussin(score_page, 3, x, 16, 0x408040, 0x204000);

//	ch = (Lcount - 1) / 15;
//	ch = (ch + 1) * search_note(ch);

	x -= 8;
	for(i = 0; i < 4; i++){
		ch = search_note(i);
		if(ch && ((Lcount - 1) / 6 != i)){
			jussin(i + 1, 1, x + (i * 8), y - 8, 0x408040, 0x204020);
		}
	}
//	}


}

void resp_draw(void)
{
	WORD x, y;
	if(score_page == res_point[1]){
		x = (res_point[0] * 1.34) + 12;
		y = 38;
		icon_draw(pix.data[pix.keyb_ad + 58] + (16 * 15), 0, x, y, 16, 1);
	}
	if(score_page == ret_point[1]){
		x = (ret_point[0] * 1.34) + 12;
		y = 38;
		icon_draw(pix.data[pix.keyb_ad + 59] + (16 * 15), 0, x, y, 16, 1);
	}
	if(score_page == start_point[1]){
		if(start_point[0] || start_point[1]){
			x = (start_point[0] * 1.34) + 12;
			y = 38;
			icon_draw(pix.data[pix.keyb_ad + 22] + (16 * 15), 0, x, y, 16, 1);
		}
	}
	if(score_page == end_point[1]){
		if(end_point[0] || end_point[1]){
			x = (end_point[0] * 1.34) + 12;
			y = 38;
			icon_draw(pix.data[pix.keyb_ad + 23] + (16 * 15), 0, x, y, 16, 1);
		}
	}
}

void note_draw(WORD da, BOOL tem, BYTE ch)
{
	BYTE i, j;
	WORD x, y;
	DWORD disps;
	LPDWORD lpd;
	BYTE elese_y = 255;
	BYTE ret = 0;
	BYTE iconp;
	BYTE chp = 0;
	BOOL onka;//tune系だったら
	BOOL han = FALSE;//半音
	BOOL hi;//高音
	BOOL lo = FALSE;//低音

	iconp = 36 + ch;
	
	if(da < 65535){
		x = (wvnote[ch].time[da] * 1.34) + 8;
		if(tem && (selch == ch)){
			elese_y = (Lcount + wvnote[ch].time[da]) % 16;
		}
		if(wvnote[ch].mode[da] > 1){//tunedata
			onka = FALSE;
			y = ((wvnote[ch].mode[da] - 2) * 4) + 7;
			lpd = pix.data[pix.keyb_ad + iconp] + (16 * 15);
			hi = FALSE;
			lo = FALSE;
		}
		else{//音階データ
			for(i = 0; i < 5; i++){
				if(wvnote[ch].value[da] % 12 < hanon[i]){//半音分いくつ高いか
					break;
				}
			}
			if(i){
				han = hanon[i - 1] == (wvnote[ch].value[da] % 12);//v1111
			}
			chp = (wvnote[ch].value[da] % 60) / 12;//オクターブ値

			if(wvnote[ch].value[da] > 59){hi = TRUE;}//高いときは
			else{hi = FALSE;}

			chp = (wvnote[ch].value[da] % 60) - (chp * 5) - i;
			onka = TRUE;
			y = STATOP - 8 - (chp * 4);//補正 v1111
			lpd = pix.data[pix.keyb_ad + iconp] + (16 * 15);
		}
	}
	else{//現在地
		x = (score_time * 1.34) + 8;
		for(i = 0; i < 5; i++){
			if(onkan[ch] % 12 < hanon[i]){//半音分いくつ高いか
				 break;
			}
		}
		if(i){
			han = hanon[i - 1] == (onkan[ch]  % 12);//v1111
		}
//		chp = onkan[ch] / 12;
		chp = (onkan[ch] % 60) / 12;//オクターブ値

		if(wvnote[ch].value[da] > 59){hi = TRUE;}//高いときは
		else{hi = FALSE;}

//		chp = onkan[ch] - (chp * 5) - i;
		chp = (onkan[ch] % 60) - (chp * 5) - i;
		y = STATOP - 8 - (chp * 4);
		onka = TRUE;
		if(tem){
			elese_y = (Lcount + score_time) % 16;
		}
		lpd = pix.data[pix.keyb_ad + iconp] + (16 * 15);
	}
//	if(han){return;}

//	lpd = pix.data[pix.keyb_ad + iconp] + (16 * 15);
	disps = x + ((y + (han * 15)) * Dwidth);

	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			if(elese_y == j){ret = 8;}

			if(*lpd && !ret){
				if(hi){//反転処理
//					if(*lpd == lcolor[1] || *lpd == lcolor[2]){
					if(*lpd == lcolor[3]){
						setpix(disps, lcolor[0], 1);
					}
					else if(*lpd == lcolor[2]){
						setpix(disps, lcolor[1], 1);
					}
					else{
						setpix(disps, lcolor[3], 1);
					}
				}
				else if(lo){//反転処理
//					if(*lpd == lcolor[1] || *lpd == lcolor[2]){
					if(*lpd == lcolor[0]){
						setpix(disps, lcolor[2], 1);
					}
					else{
						setpix(disps, lcolor[3], 1);
					}
				}
				else{
					if(onka){
						setpix(disps, *lpd, 1);
					}
					else if(*lpd == lcolor[3]){
						setpix(disps, *lpd, 1);
					}
				}
			}
			if(ret){ret--;}
			if(han){//90度回転 v1111
				disps -= Dwidth;
			}
			else{
				disps++;
			}
			lpd++;
		}
		if(!onka){//上下反転表示
			disps -= (Dwidth + j);
		}
		else if(han){//90ド回転 v1111
//		if(han){//90ド回転 v1111
			disps += 1 + (Dwidth * j);
		}
		else{//通常表示
			disps += (Dwidth - j);
		}
		lpd -= 32;
	}
}


void scoredata(void)
{
	WORD i, sta, end;
	BYTE ch, nowp;
	BOOL temjin;//tenmetu
	
	BYTE select[4] = {0, 0, 0, 0};//順番
	BYTE chp = 0;
	BYTE nct;
	
	select[chp] = 1;
	chp += wv_ch[0].on;
		
	select[chp] = 2;
	chp += wv_ch[1].on;
	
	select[chp] = 3;
	chp += wv_ch[2].on;
	
	select[chp] = 4;
	chp += wv_ch[3].on;
	
	if(chp == 0){return;}

	nct = (Lcount - 1) / (60 / chp);
	if(!select[nct]){return;}

	ch = select[nct] - 1;

	if(wv_ch[ch].on){
		if(rngnote < 0){
			if(note_no[ch] + rngnote < 0){sta = 0;}
			else{sta = note_no[ch] + rngnote;}
			end = note_no[ch];
		}
		else{
			sta = note_no[ch];
			end = note_no[ch] + rngnote;
		}
		
		if(mode == 1){nowp = score_page;}
		else {nowp = score_page;}
		i = 0;
		while(i < maxnote){
			if(!wv_ch[ch].on){break;}
			if(wvnote[ch].page[i] == nowp){break;}
			i++;
		}
		
		for(; i < maxnote; i++){
			temjin = FALSE;
			if(!wvnote[ch].mode[i]){break;}
			if(wvnote[ch].mode[i] == 255){break;}
			if(wvnote[ch].page[i] != nowp){break;}
			if((i == note_no[ch]) || (i >= sta && i <= end)){temjin = TRUE;}
			if(wvnote[ch].mode[i] > 0){
				note_draw(i, temjin, ch);
			}
		}
	}
}

void score_draw(void)
{
	BYTE i, j;
	BYTE bsize = 16;
	BYTE iconn = 40;
	BYTE beef = 0;
//	BYTE scsc[12] = {1, 3, 5, 7, 1, 5, 9, 1, 3, 5, 11, 15};
	BYTE scsc[12] = {3, 5, 7, 1, 5, 9, 1, 3, 5, 11, 1, 15};
	
	
	iconn = 9;
	StretchDIBits(hdcCmapBMP, 0, 0, bsize, bsize,
	 0, 0, bsize, bsize, pix.data[pix.keyb_ad + iconn], BMPF[beef].lpbiInfo,
	 DIB_RGB_COLORS, SRCCOPY);
	StretchDIBits(hdcCmapBMP, 0, 16, bsize, bsize,
	 0, 0, bsize, bsize, pix.data[pix.keyb_ad + iconn], BMPF[beef].lpbiInfo,
	 DIB_RGB_COLORS, SRCCOPY);
	iconn = 40;
	
	for(j = 2; j < 3; j++){//hidari sankaku?
		iconn += 14;
		StretchDIBits(hdcCmapBMP, 0, (j * bsize), bsize, bsize * 1.5,
		 0, 0, bsize, bsize, pix.data[pix.keyb_ad + iconn], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
		iconn = 40;
	}
	for(j = 3; j < 12; j++){//hidari sankaku?
		iconn += 15;
		StretchDIBits(hdcCmapBMP, 0, (j * bsize) + 8, bsize, bsize,
		 0, 0, bsize, bsize, pix.data[pix.keyb_ad + iconn], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
		iconn = 40;
	}
	
	for(j = 0; j < 10; j++){//メイン五線譜
		for(i = 1; i < 21; i++){
			if(((i - 1) % 5) == 4){iconn--;}
			iconn += scsc[j];
			StretchDIBits(hdcCmapBMP, i * bsize, (11 - j) * bsize + 12, bsize, bsize,
			 0, 0, bsize, bsize, pix.data[pix.keyb_ad + iconn], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
			iconn = 40;
		}
	}

	for(j = 0; j < 3; j++){//tune五線譜
		for(i = 1; i < 21; i++){
			if(((i - 1) % 5) == 4){iconn--;}
			iconn += 1;
			if(j == 2){
				StretchDIBits(hdcCmapBMP, i * bsize, (j) * bsize, bsize, bsize - 8,
				 0, 0, bsize, bsize - 8, pix.data[pix.keyb_ad + iconn], BMPF[beef].lpbiInfo,
				 DIB_RGB_COLORS, SRCCOPY);
			}
			else{
				StretchDIBits(hdcCmapBMP, i * bsize, (j) * bsize, bsize, bsize,
				 0, 0, bsize, bsize, pix.data[pix.keyb_ad + iconn], BMPF[beef].lpbiInfo,
				 DIB_RGB_COLORS, SRCCOPY);
			}
			iconn = 40;
		}
	}
	
	
	for(i = 1; i < 21; i++){//上空白
		iconn += 14;
		for(j = 2; j < 3; j++){//上空白
			StretchDIBits(hdcCmapBMP, i * bsize, j * 16, bsize, bsize,
			 0, 8, bsize, bsize, pix.data[pix.keyb_ad + iconn], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
		}
/*
		StretchDIBits(hdcCmapBMP, i * bsize, j * 16, bsize, bsize / 2,
		 0, 0, bsize, bsize, pix.data[pix.keyb_ad + iconn], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
*/		iconn = 40;
	}
	
	for(i = 21; i < 24; i++){//page
		iconn += 12 + i - 21;
		StretchDIBits(hdcCmapBMP, i * bsize, 0, bsize, bsize,
		 0, 0, bsize, bsize, pix.data[pix.keyb_ad + iconn], BMPF[beef].lpbiInfo,
		 DIB_RGB_COLORS, SRCCOPY);
		iconn = 40;
	}
	for(j = 1; j < 12; j++){//右空白
		for(i = 21; i < 24; i++){
			iconn += 14;
			StretchDIBits(hdcCmapBMP, i * bsize, j * bsize, bsize, bsize,
			 0, 0, bsize, bsize, pix.data[pix.keyb_ad + iconn], BMPF[beef].lpbiInfo,
			 DIB_RGB_COLORS, SRCCOPY);
			iconn = 40;
		}
	}
	
	BitBlt(hdcBB, 0, 0, Dwidth, Dheight, hdcCmapBMP, 0, 0, SRCCOPY);
	
}

void osc_draw(void)
{
	WORD divi = 80;
	WORD dispsize_x = 160;
	WORD dispsize_y = 120;
	WORD i, j;
	WORD prot_x, prot_y, back_y;
	DWORD disp;
	DWORD pcolor = 0x80ff80;
	DWORD pcolor2 = 0x408040;
	
	BOOL thisbuff = now_buff;
	prot_x = 208;
	prot_y = STATOP + 80;
	back_y = STATOP + 80;


	oscpos = (((Lcount) % 6)) % 6;
	oscpos = (wpf[selwvbuff] / 6) * oscpos;


	for(i = 0; i < 4; i++){
//		divi = divi * (wv_ch[i].on + 1);//マルチCH用縮小
		divi = divi + ((wv_ch[i].on + 1) * divi * 0.25);//マルチCH用縮小
		divi = (divi / 10) * volume_m;
		if(Zmove){
			divi = (dispsize_y / 10) * volume_m;
		}
	}
//	divi = 100 - (divi * 0.25)
	
	
	disp = prot_x + ((prot_y - 60) * Dwidth);
	for(j = 1; j < 6; j++){
		for(i = 0; i < 16; i++){
			setpix(disp + i + (j * 20 * Dwidth), pcolor2, 1);
		}
	}
	for(i = 0; i < dispsize_x; i++){
		if((short)(i + (oscpos)) > wpf[thisbuff] * 2){
//			return;
		}

		if(Zmove){
//			if(i == wv_ch[selch].length){break;}
			if(!wv_ch[selch].length){break;}
			prot_y -= WAVEDATA[selch][i % wv_ch[selch].length] / divi;
//			prot_y -= WAVEDATA[16][i % wv_ch[selch].length] / 240;
		}
		else if((short)(oscpos + i) < wpf[selwvbuff]){
			prot_y -= lpplsWave[thisbuff][i + oscpos] / divi;
		}


		if(prot_y != 220){timebyte = oscpos + i;}

		disp = prot_x + (prot_y * Dwidth);

		if(prot_y != back_y){//縦に連続
			if(prot_y < back_y){
				back_y = back_y - prot_y;
				for(j = 0; j < back_y; j++){
	
					if(!i || (j + prot_y < STATOP + 20) || (j + prot_y > STATOP + 140)){break;}
					setpix(disp + (j * Dwidth), pcolor, 1);
				}
			}
			else if(prot_y > back_y){
				back_y = prot_y - back_y;
				for(j = 0; j < back_y; j++){
					if(!i || (prot_y - j < STATOP + 20) || (prot_y - j > STATOP + 140)){break;}
					setpix(disp - (j * Dwidth), pcolor, 1);
				}
			}
		}
		if((prot_y > STATOP + 20) && (prot_y < STATOP + 140)){
			setpix(disp, pcolor, 1);
		}
		prot_x++;
		back_y = prot_y;
		prot_y = STATOP + 80;
	}
}

void testnote(void)
{
	BYTE ch;
	for(ch = 0; ch < 4; ch++){
		if(wv_ch[ch].on && !wvnote[selch].mode[note_no[selch]]){
			if((wvnote[ch].time[note_no[ch]] == 0) && (wvnote[ch].page[note_no[ch]] == 0)){
//				note_draw(onkan, score_time, 1, selch);
				note_draw(65535, 1, selch);
				return;
			}
		}
	}
}



void octab_draw(void)
{
	LPDWORD lpb = lpBBPixel;
	DWORD disp;
	BYTE ch, dib = 0, size = 16, multi = 1, iconno = 0, i, j;
	WORD x, y;
	BOOL hi;
	
	for(ch = 0; ch < 4; ch++){
		if(onlvl[ch] > 4){
			hi = TRUE;
		}
		else{
			hi = FALSE;
		}
		iconno = ch;
		x = 336 + (ch * 12);
		if(hi){
			y = STATOP - 16 - ((onlvl[ch] % 5) * 28);
		}
		else{
			y = STATOP - 16 - (onlvl[ch] * 28);
		}
		icon_draw(pix.data[pix.keyb_ad + iconno] + (16 * 15), dib, x, y, size, multi);

		if(hi){
			disp = x + (y * Dwidth);
			for(i = 0; i < 16; i++){
				for(j = 0; j < 16; j++){
					if(lpb[disp] == lcolor[3]){
						setpix(disp, lcolor[0], 1);
					}
					disp++;
				}
				disp += (Dwidth - j);
			}
		}
	}
	
}

void keyflush_draw(void)
{
	
	DWORD col[4] = {0xa0ffa0, 0x40a040, 0x004020, 0x102010};
	DWORD disp;
	LPDWORD lpb = lpBBPixel;
	WORD x, y, skips = 0;
	BYTE i, j, ch, key, colsel, cgcol;
	BOOL keytop[17] = {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1,
						1, 0, 1, 0, 1};//17lvl

//	for(ch = 0; ch < 4; ch++){
	ch = selch;
		if(!wv_ch[ch].on){return;}
		key = flushkey[ch];
		if(key > 4){skips += 16;}
		if(key > 11){skips += 16;}//hanuke

		colsel = ((flushcnt[ch] / 8) + keytop[key]) % 4;
		if(flushcnt[ch]){//カウントあれば
			flushcnt[ch]--;
			
			if(keytop[key]){cgcol = 0;}
			else{cgcol = 2;}//判定用カラー
			
			x = (key * 16) + skips + 32;
			y = 338;
			//上半分
			for(i = 0; i < 32; i++){
				for(j = 0; j < 28; j++){
					disp = x + j + ((y + i) * Dwidth);
					if(lpb[disp] == col[cgcol]){
						setpix(disp, col[colsel], 1);
					}
				}
			}
			//下半分
			if(keytop[key]){
				for(i = 32; i < 46; i++){
					for(j = 0; j < 32; j++){
						disp = x + j + ((y + i) * Dwidth);
						if(lpb[disp] == col[cgcol]){
							setpix(disp, col[colsel], 1);
						}
					}
				}
			}
		}
//	}
}

void edmenu_draw(void)
{
	WORD y = 10;
	WORD x = 220;
	WORD step = 12;
	DWORD col = 0x102010;
	DWORD col2 = 0x408040;
	
	if(!numact){
		moji3(x + 20, y, 1, "TIMESTEP", col);
		jussin(timestep, 3, x + 100, y + STATOP, 1, col);
	}
	y += step;

	if(!masterfile){
		moji3(x, y, 1, "PLAY", col);
		y += step;
		moji3(x, y, 1, "CATCH", col);
		y += step;
		moji3(x, y, 1, "TUNESET", col);
		y += step;
		moji3(x, y, 1, "START END", col);
		y += step;
		moji3(x, y, 1, "WAVETEST", col);
		y += step;
		moji3(x, y, 1, "SAVEDATA", col);
		y += step;
		moji3(x, y, 1, "LOADDATA", col);
		y += step;
		moji3(x, y, 1, "WAVEFORMAT", col);
		y += step;
		moji3(x, y, 1, "MINIMISE", col);
	}
	else{
		moji3(x, y, 1, "PLAY", col);
		y += step;
		moji3(x, y, 1, "CATCH", col2);
		y += step;
		moji3(x, y, 1, "TUNESET", col2);
		y += step;
		moji3(x, y, 1, "START END", col2);
		y += step;
		moji3(x, y, 1, "WAVETEST", col2);
		y += step;
		moji3(x, y, 1, "SAVEDATA", col2);
		y += step;
		moji3(x, y, 1, "LOADDATA", col);
		y += step;
		moji3(x, y, 1, "WAVEFORMAT", col2);
	}
}

void modedisp(BYTE m)
{
	WORD x = 220;
	
	if(typeact){
		x = 300;
	}
	else{
		moji3(x, STATOP + 130, 0, "MODE:", 0x204020);

		x += 50;
	}
	switch(m){
		case 0:	moji3(x, STATOP + 130, 0, "OVERWRITE", 0x204020); break;
		case 1:	moji3(x, STATOP + 130, 0, "COPYNOTE", 0x204020); break;
		case 2:	moji3(x, STATOP + 130, 0, "TUNESET", 0x204020); break;
		case 3:	moji3(x, STATOP + 130, 0, "WAVETEST", 0x204020); break;
		case 4:	moji3(x, STATOP + 130, 0, "PLAYSOUND", 0x204020); break;
		case 7:	moji3(x, STATOP + 130, 0, "SAVEDATA", 0x204020); break;
		case 8:	moji3(x, STATOP + 130, 0, "LOADDATA", 0x204020); break;
		case 9:	moji3(x - 10, STATOP + 130, 0, "WAVEFORMAT", 0x204020);
				jussin(recloop - 1, 2,  x + 80, STATOP + 130, 0x204000, 0x102010); break;
		case 10:moji3(x, STATOP + 130, 0, ":::WAIT:::", 0x204020); break;
		case 255:moji3(x, STATOP + 130, 0, "  EXIT ? ", 0x204020); break;
	}
}

void filenum_draw(void)
{
	WORD y = STATOP + 10;
	WORD x = 330;
	WORD step = 12;
	DWORD col = 0x102010;
	BYTE i;

	moji3(x - 10, y, 0, "FILE NO", col);
	y += step;
	for(i = 0; i < 8; i++){
		jussin(((selectnum + i) % 255) + 1, 3, x, y, col, col);
		y += step;
	}
}
void filename_draw(BYTE mo)
{
	BYTE i, fnb;
	if(mo == 0){
		for(i = 0; i < 32; i++){
			messagecnt = i;
			set_msgdata_r(0, i, 2);
	//		wordblast_p(thisname[i], 5);
		}
	}
	else if(mo == 1){
		icon_draw(pix.data[pix.keyb_ad + 60] + (16 * 15), 0, 20, 360, 16, 1);
		icon_draw(pix.data[pix.keyb_ad + 61] + (16 * 15), 0, 36, 360, 16, 1);
		icon_draw(pix.data[pix.keyb_ad + 62] + (16 * 15), 0, 52, 360, 16, 1);
		icon_draw(pix.data[pix.keyb_ad + 63] + (16 * 15), 0, 68, 360, 16, 1);
		
		for(fnb = 0; fnb < 3; fnb++){
			for(i = 0; i < 16; i++){
				messagecnt = i;
				set_msgdata_r(fnb + 1, i, 2);
		//		wordblast_p(thisname[i], 5);
			}
		}
	}
}

void filecom_draw(void)
{
	WORD x = 30, y = MESTOP + 48;
	BYTE step = 80;

	moji3(x, y, 0, "CANSEL", 0x204020);
	x += step;
	moji3(x, y, 0, "EXIT  ", 0x204020);
	x += step;
	moji3(x, y, 0, "SAVE  ", 0x204020);
	x += step;
		
	
	if(value[mode] == 3){
		moji3(x, y, 0, "USER", 0x204020);
	}
}

void mastermode_draw(void)
{
	if(mastermode){
		icon_draw(pix.data[pix.keyb_ad + 63] + (16 * 15), 0, 8, STATOP + 8, 16, 1);
	}
}

void filerest_draw(void)
{
	DWORD disp, col = 0x102010;
	WORD x, y, hiritu, i, j, k;
	
	BYTE step_x = 5, step_y = 10;
	
	hiritu = (output_c * 80) / (wvFileSize / 2);
//	hiritu = (wvFileSize / 2) / (output_c * 60);
	for(i = 0; i < 60; i++){
		x = (i * step_x) + 18;
//		y = ((i % 2) * step_x) + 8;
		y = 8 + 34;
		
		for(j = 0; j < step_x; j++){
			if(i > hiritu){break;}
			
			disp = x + j + ((y + ((i % 2) * step_y)) * Dwidth);
			setpix(disp, col, 1);//x

			disp = x + ((y + j) * Dwidth);
			setpix(disp, col, 1);//y
			disp = x + ((y + j + (step_y / 2)) * Dwidth);
			setpix(disp, col, 1);//y
		}
	}
	
	if(hiritu > 60){
		icon_draw(pix.data[pix.keyb_ad + 33] + (16 * 15), 0, step_x * 60 + 18, 4 + 34, 16, 1);
	}
}

void typeact_d(void)
{
	menucursol();
	scoredata();
	filenum_draw();
	filename_draw(0);
	if(menuact){filecom_draw();}
}

void numact_d(void)
{
	wv_info();
	menucursol();
	scoredata();
	edmenu_draw();
//		note_info(1);
	filenum_draw();
	filename_draw(1);
}

void menuact_d(void)
{
	wv_info();
	menucursol();
	scoredata();
	edmenu_draw();
	note_info();
}

void lit_d(void){
	if(mode == 10){
		filerest_draw();
	}
	modedisp(mode);
	head_draw();
}

void posline_draw(void)//tatesen v1111,
{
	BYTE i, cosel = 0;
	WORD x, y;
	DWORD disdis = 0;
	if(output_f || mode > 253){
		return;
	}
	else{
		x = (score_time * 1.34) + 15;
		y = ((Lcount + 1) * 6);
	}
	

	for(i = 0; i < 80; i++){
		
		setpix(disdis, lcolor[cosel], 1);
		y = (STATOP + y + 1) % (STATOP);
		disdis = x + (y * Dwidth);
		cosel = ((y + x) / 4) % 4;
	}
}

void draw(void)
{
	BYTE i;
	short data;
	
//	score_draw();
//	tuneb_draw();
//	keyb_draw();
	bbset();

//	jussin(CLUSTOP, 4, 0, 90, 0xffff00, 0xdddd99);
	if(mode == 10 || mode > 253){
		lit_d();
		return;
	}
	if(typeact){
		typeact_d();
	}
	else if(numact){
		if(!masterfile){
			numact_d();
		}
		else{
			menucursol();
			edmenu_draw();
			filenum_draw();
			filename_draw(1);
		}
	}
	else if(menuact){
		if(!masterfile){
			menuact_d();
		}
		else{
			menucursol();
			edmenu_draw();
		}
	}
	else{
		menucursol();
		if(!masterfile){
			scoredata();
			wv_info();
		}
		osc_draw();
	}
	
	mastermode_draw();
	
	resp_draw();
	octab_draw();
	testnote();
	head_draw();
	modedisp(mode);
	keyflush_draw();
	posline_draw();

/*	data = 0;
	for(i = 0; i < wv_ch[selch].length; i++){
		if(WAVEDATA[selch][i] > data){
			data = WAVEDATA[selch][i];
		}
//		else 
//if(WAVEDATA[ch][i] < data){
//			break;
//		}
	}
*/


//	(atn / 48) + 2
	 
//	jussin(wvnote[0].mode[prg[0]], 4, 50, 100, 0xffff00, 0xdddd99);
/*	jussin(wv_ch[17].length, 4, 100, 90, 0xffff00, 0xdddd99);
	jussin(((wv_ch[selch].t_att - 64) / 48) + 2, 5, 0, 100, 0xffff00, 0xdddd99);
	jussin(wv_ch[selch].maxg, 5, 0, 110, 0xffff00, 0xdddd99);
/*	if(wv_ch[0].length){
	jussin(masterfile, 8, 0, 140 - (wv_ch[0].length / 2) - ((wv_ch[0].pasloop) % (wv_ch[0].length / 2)), 0xffff00, 0xdddd99);
	jussin((SRATE / (wv_ch[0].length - 4)) / 16, 5, 0, 50, 0xffff00, 0xdddd99);
	}*/
//	jussin(chara[0].Zpos / 64, 5, 0, 60, 0xffff00, 0xdddd99);

/*	moji3(10, 424, 0, "MODE", 0xfffffe);
	jussin(mode, 3, 50, 424, 0xfffff5, 0xdddd99);

	moji3(0, 0, 0, "FPS", 0xfffffe);
	jussin(showFlame, 3, 0, 10, 0xffff00, 0xdddd99);

	moji3(30, 0, 0, "SLP", 0xaaaaff);
	jussin(showslp, 5, 30, 10, 0xffff00, 0xdddd99);

	moji3(80, 0, 0, "DMY", 0xaaffdd);
	jussin(showdmy, 10, 80, 10, 0xffff00, 0xdddd99);
*/
}

void restart_set(BYTE val)
{
	if(!val){
		res_point[0] = score_time;//B Button
		res_point[1] = score_page;
	}
	else if(val == 1){
		ret_point[0] = score_time;//A Button
		ret_point[1] = score_page;
	}
}


void set_freq(BYTE ch, BYTE key_sel)//key押し
{
//	BYTE ch;
//	for(ch = 0; ch < 4; ch++){
//		if(key_sel + (onlvl * 18) < 96){
		if(key_sel < 96){
//			wv_ch[ch].freq = onkai[key_sel + (onlvl * 12)];
			wv_ch[ch].freq = onkai[key_sel];
			wv_ch[ch].nonkai = onkai[key_sel];
		}
		else{
			wv_ch[ch].freq = onkai[95];
			wv_ch[ch].nonkai = onkai[95];
		}
//	}
}

BYTE instune(BYTE ch, BYTE mo, WORD ii)//TUNEDATA再挿入位置決定
{
//modeが
	BYTE i = 0;
	BYTE j = 0;
	BYTE hikakumo = 0;
	while((wvnote[ch].page[ii] == wvnote[ch].page[ii + j + 1]) && (wvnote[ch].time[ii] == wvnote[ch].time[ii + j + 1])){
		if(!wvnote[ch].mode[ii + j + 1]){break;}
		j++;
	}
//	j = 0;
	while(i < 12){
		
		if(ii - i + j + 1){//-の場合はやらない
			hikakumo = wvnote[ch].mode[ii - i + j];
			if(hikakumo == 1){hikakumo = 254;}
			if(mo == hikakumo){//modeが同じなら
				i++;
				return i;
			}
			else if((mo > hikakumo) && (1 < hikakumo)){//noteDATAは後ろへもってく
				return i;
			}
			else if((wvnote[ch].time[ii - i + j] != wvnote[ch].time[ii + j]) || (wvnote[ch].page[ii - i + j] != wvnote[ch].page[ii + j])){//違うタイミング
//				i--;
				return i;
			}
//			else if(2 > wvnote[ch].mode[ii - i + j]){return i - j;}
			i++;
		}
		else{return i - j;}//初期データとみなす
	}
}

WORD insnote(BYTE ch, WORD no, BYTE mo, short val)
{
	WORD i, j;
	BYTE mond, sp, st;
	short va;
	
	mond = mo;
	sp = score_page;
	st = score_time;
//	sp = score_page;
//	st = score_time;
	va = val;
	
	i = 0;
//	no は終端からはじまる
	for(i = no; i > 0; i--){//位置を検索
		if(sp > wvnote[ch].page[i - 1]){
			break;//後ろのページ
		}
		else if(sp == wvnote[ch].page[i - 1]){
			if(st > wvnote[ch].time[i - 1]){
				break;//同じページでも、タイムが遅い
			}
			else if(st == wvnote[ch].time[i - 1]){
				if(mond > 1){//tune系だったら
					if(i){
						j = instune(ch, mo, i - 1);//tune整列
						i -= j;
					}
					if(mond == wvnote[ch].mode[i]){
						note_no[ch]--;//同じmodeだったらすり替え
						return i;
					}
					break;
				}
				else{
					if(wvnote[ch].mode[i - 1] > 1){//tuneは消さない
						break;
					}
					else{
						note_no[ch]--; i--;return i;//同じタイミング
					}
				}
			}
			else if(st < wvnote[ch].time[i - 1]){
				//タイムが早い＝次検索
			}
		}
		else if(sp < wvnote[ch].page[i - 1]){
			//前のページ＝次検索
		}
	}
	
//	i = search_note(ch, sp, st, mond);

	j = i;//1 2 3 4 _x_ 5 6 : j = 4

	for(i = no; i > j; i--){//検索該当箇所まで
		wvnote[ch].mode[i] = wvnote[ch].mode[i - 1];
		wvnote[ch].page[i] = wvnote[ch].page[i - 1];
		wvnote[ch].time[i] = wvnote[ch].time[i - 1];
		wvnote[ch].value[i] = wvnote[ch].value[i - 1];
	}
	
	return (i);//
}

void delnote(void)
{
	WORD i, j, sta, end;
	BYTE ch = selch;
	
//	for(ch = 0; ch < 4; ch++){
//		if(wv_ch[ch].on){
			if(rngnote < 0){
				sta = note_no[ch] + rngnote;
				end = note_no[ch] + 1;
				note_no[ch] = sta;
//				sta = note_no[ch] + rngnote - 1;
//				end = note_no[ch];
			}
			else if(rngnote >= 0){
				sta = note_no[ch];
				end = note_no[ch] + rngnote + 1;
			}

			for(i = sta; i < maxnote; i++){//選択位置から位置から
				if(i + end - sta < maxnote){
					for(j = 0; j < end - sta; j++){//範囲分del（シフト）回数
						wvnote[ch].mode[i + j] = wvnote[ch].mode[i + j + (end - sta)];
						wvnote[ch].page[i + j] = wvnote[ch].page[i + j + (end - sta)];
						wvnote[ch].time[i + j] = wvnote[ch].time[i + j + (end - sta)];
						wvnote[ch].value[i + j] = wvnote[ch].value[i + j + (end - sta)];
					}
				}
				else{//残り少ない
					for(j = 0; j < end - sta; j++){//範囲分del回数
						wvnote[ch].mode[i + j] = 0;
						wvnote[ch].page[i + j] = 0;
						wvnote[ch].time[i + j] = 0;
						wvnote[ch].value[i + j] = 0;
					}
					washanime = 59;
					break;
				}
			}
//		}
//	}
	rngnote = 0;
}



void set_note(BYTE ch, BYTE mod, short val)
{
	WORD no;
//	WORD no = 0;

//	for(ch = 0; ch < 4; ch++){
		if(wv_ch[ch].on){
			no = note_no[ch];
			while(no < maxnote){
//				if((wvnote[ch].page[no] == 0) && (wvnote[ch].time[no] == 0)){
				if(wvnote[ch].mode[no] == 0){
					break;
				}
				no++;
			}
			no = insnote(ch, no, mod, val);
			
			wvnote[ch].mode[no] = mod;
			wvnote[ch].page[no] = score_page;
			wvnote[ch].time[no] = score_time;
			wvnote[ch].value[no] = val;
			note_no[ch]++;
		}
//	}
}

void set_tune(void){

	switch(value[mode]){
		case 0: set_note(selch, 2, wv_ch[selch].vol); break;
		case 1: set_note(selch, 3, wv_ch[selch].t_att); break;
		case 2: set_note(selch, 4, wv_ch[selch].t_gin); break;
		case 3: set_note(selch, 5, wv_ch[selch].dch); break;
		case 4: set_note(selch, 6, wv_ch[selch].dly); break;
		case 5: set_note(selch, 7, wv_ch[selch].msec); break;
		case 6: set_note(selch, 9, wv_ch[selch].type); break;
		case 7: set_note(selch, 8, wv_ch[selch].fs); break;
		case 8: set_note(selch, 10, wv_ch[selch].pws); break;
		case 9: set_note(selch, 11, wv_ch[selch].rfs); break;
	}

}
void scoremove(BYTE val)//time移動
{
	BYTE d;
	
	if(val < 17){//timestep操作 v1111
//		d = 60;
		d = timestep;
	}
	else{
		d = 1;
	}
	if((val & 0x1) == 0x1){
		if(score_time - d < 0){
			if(score_page){
				score_time += 240 - d;
				score_page--;
			}
		}
		else{
			score_time = score_time - d;//60*4で次ページ
		}
	}
	else{
		if(score_time + d > 239){
			if(score_page < 255){
				score_page++;
				score_time = (score_time + d) % 240;//60*4で次ページ
			}
			else{
				score_time = 239;
			}
		}
		else{
			score_time = (score_time + d) % 240;//60*4で次ページ
		}
	}
}

void copynote(void)
{
	WORD i;
	WORD timep;
	BYTE ch;
//	for(ch = 0; ch < 4; ch++){
//		if(wv_ch[ch].on){
			if(mode != 1){//modeまえ処理
				if(rngnote < 0){
					copyn[0] = note_no[selch] + rngnote;
					copyn[1] = note_no[selch];
					copyn[2] = selch;
				}
				else if(rngnote >= 0){
					copyn[0] = note_no[selch];
					copyn[1] = note_no[selch] + rngnote;
					copyn[2] = selch;
				}
				if(!wvnote[selch].mode[copyn[1]]){return;}
				
				copyat[0] = score_time;//copysaki
				copyat[1] = score_page;
				score_time = wvnote[selch].time[copyn[0]];
				score_page = wvnote[selch].page[copyn[0]];
//				copyat = wvnote[selch].time[copyn[0]] + (wvnote[selch].page[copyn[0]] * 240);
				mode = 1;
				
				for(i = 0; i <= copyn[1] - copyn[0]; i++){
					cpnote[selch].time[i] = wvnote[selch].time[i + copyn[0]];
					cpnote[selch].page[i] = wvnote[selch].page[i + copyn[0]];
					cpnote[selch].mode[i] = wvnote[selch].mode[i + copyn[0]];
					cpnote[selch].value[i] = wvnote[selch].value[i + copyn[0]];
				}
			}
			else if(mode == 1){//コピ
				if(cpnote[selch].mode[0] == 0){return;}
//				if((wvnote[selch].time[copyn[0]] == score_time) && (wvnote[selch].page[copyn[0]] == score_page) && (selch == copyn[2])){
				if((cpnote[selch].time[0] == score_time) && (cpnote[selch].page[0] == score_page) && (selch == copyn[2])){
//					return;
				}//同じ箇所
				for(i = 0; i <= copyn[1] - copyn[0]; i++){
//				for(i = copyn[0]; i < copyn[1] + 1; i++){
//					set_note(selch, wvnote[copyn[2]].mode[i], wvnote[copyn[2]].value[i]);
					set_note(selch, cpnote[copyn[2]].mode[i], cpnote[copyn[2]].value[i]);
					if(i == copyn[1] - copyn[0]){break;}
					timep = (cpnote[copyn[2]].page[i + 1] - cpnote[copyn[2]].page[i]);
					timep = (timep * 240) + cpnote[copyn[2]].time[i + 1] - cpnote[copyn[2]].time[i];
					score_page += (score_time + timep) / 240;
					score_time = (score_time + timep) % 240;
				}
			}
//		}
//	}
}

void notesel(BYTE val)//catch移動
{
	BOOL seton = FALSE;
	BYTE ch = selch;
	short vo;
//	for(ch = 0; ch < 4; ch++){
		if(wv_ch[ch].on){
			if(val == 1){
				if(note_no[ch]){note_no[ch]--;}
			}
			else if(val == 4){
				if(note_no[ch] < maxnote){
					if(wvnote[ch].mode[note_no[ch]]){
						note_no[ch]++;
					}
				}
			}
			else if(val == 17){
				if(note_no[ch]){
					if(wvnote[ch].mode[note_no[ch]]){
						if(note_no[ch] + rngnote < maxnote){
							rngnote++;
							note_no[ch]--;
						}
					}
				}
			}
			else if(val == 20){
				if(note_no[ch] < maxnote){
					if(wvnote[ch].mode[note_no[ch] + 1]){
						if(note_no[ch] + rngnote + 1 > 0){
							rngnote--;
							note_no[ch]++;
						}
					}
				}
			}
			

			if(wvnote[ch].mode[note_no[ch]] > 1){//tuneだよ
				tuneup(ch, note_no[ch]);
			}
			else if(wvnote[ch].mode[note_no[ch]] == 1){
				flushcnt[ch] = wv_ch[ch].msec;
				flushkey[ch] = wvnote[ch].value[note_no[ch]] % 12;
				wv_ch[ch].freq = onkai[wvnote[ch].value[note_no[ch]]];
				wv_ch[ch].nonkai = onkai[wvnote[ch].value[note_no[ch]]];
	
				vo = wvbase_set(ch);
				seton = wvch_sel(ch, vo);
				if(seton){
					wv_loop(ch);
				}
			}//note

//			sound_draw();
		}
//	}
}

void jumpcursol(void)
{
	if(mode == 1){
//		score_time = (copyat + wvnote[selch].time[note_no[selch]]) % 240;
//		score_page = ((copyat / 240) + wvnote[selch].page[note_no[selch]]) + (((copyat % 240) + wvnote[selch].time[note_no[selch]]) / 240);
		score_time = wvnote[selch].time[note_no[selch]];
		score_page = wvnote[selch].page[note_no[selch]];
	}
	else{
		score_time = wvnote[selch].time[note_no[selch]];
		score_page = wvnote[selch].page[note_no[selch]];
	}
}

void tune_wave(BYTE val)
{
	if(masterfile){return;}
	if(val == 1){
		switch(value[mode]){
			case 0:	if(wv_ch[selch].vol){wv_ch[selch].vol--;} break;//volume
//			case 1:	if(wv_ch[selch].t_att > 0){wv_ch[selch].t_att--;} break;//att
			case 1:	wv_ch[selch].t_att = (255 + wv_ch[selch].t_att) % 256; break;//att
			case 2:	wv_ch[selch].t_gin = (255 + wv_ch[selch].t_gin) % 256; break;//att
			case 3:	if(wv_ch[selch].dch){wv_ch[selch].dch--;} break;//deley
			case 4:	if(wv_ch[selch].dly){wv_ch[selch].dly--;}else{wv_ch[selch].dly = 255;} break;//deley
//			case 5:	if(wvset_msec > 10){wvset_msec -= 10;} break;
			case 5:	if(wv_ch[selch].msec > 1){wv_ch[selch].msec -= 1;} break;
			case 6:	if(wv_ch[selch].type){wv_ch[selch].type--;} break;//x_axis_position
			case 7:	if(wv_ch[selch].fs > 0){wv_ch[selch].fs -= 1;} break;//att
			case 8:	if(wv_ch[selch].pws > 0){wv_ch[selch].pws -= 1;} break;//x_axis_position
			case 9:	if(wv_ch[selch].rfs > 0){wv_ch[selch].rfs -= 1;} break;//att
			default: break;
		}
	}
	if(val == 4){
		switch(value[mode]){
			case 0:	if(wv_ch[selch].vol < 16){wv_ch[selch].vol++;} break;
			case 1:	wv_ch[selch].t_att = (wv_ch[selch].t_att + 1) % 256; break;
			case 2:	wv_ch[selch].t_gin = (wv_ch[selch].t_gin + 1) % 256; break;
			case 3:	if(wv_ch[selch].dch < 65535){wv_ch[selch].dch++;} break;
			case 4:	if(wv_ch[selch].dly < 11025){wv_ch[selch].dly++;}else{wv_ch[selch].dly = 0;} break;
			case 5:	if(wv_ch[selch].msec + 1 < 255){wv_ch[selch].msec += 1;} break;
			case 6:	if(wv_ch[selch].type < 19){wv_ch[selch].type++;} break;
			case 7:	if(wv_ch[selch].fs < 255){wv_ch[selch].fs += 1;} break;
			case 8:	if(wv_ch[selch].pws < 255){wv_ch[selch].pws += 1;} break;
			case 9:	if(wv_ch[selch].rfs < 255){wv_ch[selch].rfs += 1;} break;
			default: break;
		}
	}
}

void move_mode3(BYTE val)//or mode2
{
	if(!osimati || osimati == 1){
		if(Zmove){
			switch(val){
				case 1:	if(selch){selch--;} break;
				case 2:	wv_ch[selch].on = TRUE; break;
				case 4:	if(selch < 3){selch++;} break;
				case 8:	wv_ch[selch].on = FALSE; break;
				default: break;
			}
		}
		else if(wv_ch[selch].on){
			switch(val){
				case 1:	tune_wave(val); break;
				case 2:	value[mode] = (9 + value[mode]) % 10; break;
				case 4:	tune_wave(val); break;
				case 8:	value[mode] = (value[mode] + 1) % 10; break;
				default: break;
			}
		}
	}
}

void kettei(void){
	if(CLUSTOP){return;}
//	note_no[ch]++;
	switch(mode){
		case 2: set_tune(); menuact = TRUE; mode = 3;  wavetest_disp(0);break;
		default: break;
	}
	CLUSTOP = 1;

}

void cansel(void){
	if(CLUSTOP){return;}
	if(mode == 2){mode = 3; menuact = TRUE;  wavetest_disp(0);}//tuneset nuke v1111

//	note_no[ch]--;
	CLUSTOP = 1;
}

void trig(void)
{
//	if(CLUSTOP){return;}
/*	switch(mode){
		case 0: Zmove = TRUE; break;
		case 1: Zmove = TRUE; break;
		case 2: Zmove = TRUE; break;
		case 3: Zmove = TRUE; break;
		case 7: Zmove = TRUE; break;
		case 8: Zmove = TRUE; break;
		default: break;
	}
*///	CLUSTOP = 1;
	Zmove = TRUE;
	
}

void trig_free(void){
/*	switch(mode){
		case 0: Zmove = FALSE; break;
		case 1: Zmove = FALSE; break;//eboxセレクト
		case 2: Zmove = FALSE; break;//eboxセレクト
		case 3: Zmove = FALSE; break;
		case 7: Zmove = FALSE; break;
		case 8: Zmove = FALSE; break;
				
		default: break;
	}
*/	Zmove = FALSE;
}

void filecom_kettei(void)
{
	WORD memocode;
	if(CLUSTOP){return;}
	switch(value[mode]){
		case 0: menuact = FALSE; fontboard(0); break;//名前入力に戻る
		case 1: menuact = FALSE; typeact = FALSE; wavetest_disp(0); makemessagewin(7); break;//fileselまでもどる
		case 2: soundsave(((selectnum + selectnumc) % 255) + 1);
				mode = 3; numact = FALSE; typeact = FALSE; keyb_draw(); wavetest_disp(0);//実行後//メニューに戻る
				break;
		case 3: memocode = mastercode;
				mastercode = 0;
				soundsave(((selectnum + selectnumc) % 255) + 1);
				mastercode = memocode;
				mode = 3; numact = FALSE; typeact = FALSE; keyb_draw(); wavetest_disp(0);		default:break;
	}
	CLUSTOP = TRUE;

}

void filecom_move(BYTE val)
{
	if(!osimati || osimati == 1){
		if(Zmove){
		}
		else{
			switch(val){
				case 1:	if(value[mode] == 0){value[mode] = 3 + mastermode;} value[mode]--; break;
				case 2: break;
				case 4:	value[mode] = (value[mode] + 1) % (3 + mastermode); break;
				case 8: break;
				default: break;
			}
		}
	}
}

void typekettei(void)
{
	if(!osimati || osimati == 1){
		thisname[message_sel] = selecttype + 1;
	
		message_sel = (message_sel + 1) % 32;
	}
}

void typecansel(void)
{
	if(!osimati || osimati == 1){
	
		if(message_sel){message_sel -= 1;}
		else{message_sel = 31;}
		thisname[message_sel] = 0;
	}
}

void typemove(BYTE val)
{
	BYTE maxtype = 250;
	if(!osimati || osimati == 1){
		if(Zmove){
			switch(val){
				case 1:	message_sel = (message_sel + 32 - 1) % 32; break;
				case 2:	message_sel = (message_sel + 32 - 16) % 32; break;
				case 4:	message_sel = (message_sel + 1) % 32; break;//load
				case 8:	message_sel = (message_sel + 16) % 32; break;//cancel
				default: break;
			}
		}
		else{
			switch(val){
				case 1:	if(selecttype == 0){selecttype = maxtype;} selecttype --; break;
				case 2:	if(selecttype - 15 < 0){selecttype = maxtype;} selecttype -= 15; break;
				case 4:	selecttype = (selecttype + 1) % maxtype; break;//load
				case 8:	selecttype += 15; if(selecttype > maxtype - 1){selecttype -= maxtype - 1;} break;//cancel
				default: break;
			}
			if(selecttype > 179){
				fontboard(2);
			}
			else if(selecttype > 89){
				fontboard(1);
			}
			else if(selecttype < 90){
				fontboard(0);
			}
		}
	}
}

void numkettei(void)
{
	if(menuact){filecom_kettei(); return;}
	if(typeact){typekettei(); return;}
//	if(!osimati || osimati == 1){
	if(CLUSTOP){return;}
	switch(mode){
		case 7: if(!typeact){typeact = TRUE; menuact = FALSE; fontboard(0); makemessagewin(7);} else{soundsave(((selectnum + selectnumc) % 255) + 1);} break;
		case 8: soundload(((selectnum + selectnumc) % 255) + 1, 0); numact = FALSE; mode = 3; makemessagewin(0); keyb_draw();break;
		default: break;
	}
}

void numcansel(void){
	if(menuact){menuact = FALSE; fontboard(0); return;}
	if(typeact){typecansel(); return;}
	if(CLUSTOP){return;}
//	if(!osimati || osimati == 1){
	mode = 3; numact = FALSE; typeact = FALSE; menuact = TRUE; keyb_draw();
//	ositg = TRUE;
	CLUSTOP = 1;

}

void num_move(BYTE val)
{
	if(!osimati || osimati == 1){
		if(menuact){filecom_move(val); return;}
		if(typeact){typemove(val); return;}
		if(Zmove){
			switch(val){
				case 1:	 break;
				case 2:	selectnum = ((selectnum + 255 - 10) % 255); break;//FILENO 10STEP v1111
				case 4:	break;
				case 8:	selectnum = (selectnum % 255) + 10; break;
				default: break;
			}
		}
		else{
			switch(val){
				case 1:	 break;
				case 2:	if(selectnumc > 0){selectnumc--;}
						else{selectnum = ((selectnum + 255 - 1) % 255);}
						break;
				case 4:	break;
				case 8:	if(selectnumc < 7){selectnumc++;}
						else{selectnum = (selectnum % 255) + 1;} break;
				default: break;
			}
			//前後の名前のみロード
		}
		snameload((selectnumc + selectnum + 255 - 1) % 255, 0);
		snameload((selectnumc + selectnum) % 255, 1);
		snameload((selectnumc + selectnum + 1) % 255, 2);
		
	}
}

void menukettei(void)
{
	if(CLUSTOP){return;}
	switch(selectmenu){
//		case 0:break;
		case 0:	if(Zmove){end_point[1] = score_page; end_point[0] = score_time; break;}
//				if(splay){score_page = start_point[1]; score_time = start_point[0];}
				score_page = start_point[1]; score_time = start_point[0];
				prg[0] = research_note(0, score_page, score_time);
				prg[1] = research_note(1, score_page, score_time);
				prg[2] = research_note(2, score_page, score_time);
				prg[3] = research_note(3, score_page, score_time);
//				prg[0] = 0; prg[1] = 0; prg[2] = 0; prg[3] = 0;
				splay = TRUE; break;
		case 1: copynote(); rngnote = 0; break;//copymode
		case 2: mode = 2; menuact = FALSE; wavetest_disp(3); break;//tunes
		case 3: if(Zmove){end_point[1] = score_page; end_point[0] = score_time; break;}
				restart_set(1); break;//retun
		case 4: mode = 3; menuact = FALSE; wavetest_disp(3); break; //wavetest
		case 5: mode = 7; makemessagewin(7); numact = TRUE; menuact = FALSE; break; //save
		case 6: mode = 8; makemessagewin(8); numact = TRUE; menuact = FALSE; break; //load
		case 7: mode = 9; if(!splay){wavefmt_save(0); recloop++;
						score_page = start_point[1]; score_time = start_point[0];
						prg[0] = research_note(0, score_page, score_time);
						prg[1] = research_note(1, score_page, score_time);
						prg[2] = research_note(2, score_page, score_time);
						prg[3] = research_note(3, score_page, score_time);
				} else{recloop++;} break; //load
//		case 8: if(mode == 254){mode = 3;}else {mode = 254;} break; //load
		case 8: mode = 254; break; //load
	}
//	note_no[ch]++;
	CLUSTOP = 1;
}

void menucansel(void)
{
	if(CLUSTOP){return;}
	switch(selectmenu){
		case 0: if(Zmove){start_point[1] = score_page; start_point[0] = score_time; break;}
				if(splay){splay = FALSE;}
				else{score_page = 0; score_time = 0;}
				break;
		case 1: if(mode != 1){copynote();} delnote(); rngnote = 0; break;
		case 3: if(Zmove){start_point[1] = score_page; start_point[0] = score_time; break;}
				restart_set(0); break;//restart
		case 7: if((recloop == 1) && rectrig){recloop--; wavefmt_close();} else if(recloop > 1){recloop--;} break;//restart
	}
	CLUSTOP = 1;
}

void menuvol(BYTE value)
{
	switch(selectmenu){
		case 1: if(mode == 1){scoremove(value);} else{notesel(value); jumpcursol();}break;
		default: scoremove(value); break;
//		case 4: endpos(value); break;
//		case 5: recycle(value); break;
	}
}

void mcomon_move(BYTE value)//メニュー共通
{
	if(!osimati || osimati == 1){
		if(Zmove){
			switch(value){
				case 1: menuvol(value + 16); break;
//				case 2: if(selectmenu){mode = 3; rngnote = 0;}
				case 2: timestep = (timestep % 240) + 1; break;
				case 4: menuvol(value + 16); break;
//				case 8: if(selectmenu < 7){mode = 3; rngnote = 0;}
//						else{selectmenu = 0;}  break;
				case 8: timestep = ((timestep + 240 - 2) % 240) + 1; break;//1が最低値
				default:break;
			}
			return;
		}
		else{
			rngnote = 0;//上下で選択範囲解除
			switch(value){
				case 1: menuvol(value); break;
				case 2: if(masterfile){selectmenu = 0;} else if(selectmenu){selectmenu--;} break;
				case 4: menuvol(value); break;
				case 8: if(masterfile){selectmenu = 6;} else if(selectmenu < 8){selectmenu++;} break;
				default:break;
			}
		}
	}
}

void menu_move(BYTE value)
{
	switch(mode){
		case 7: num_move(value); break;
		case 8: num_move(value); break;
		
		default: mcomon_move(value); break;
	}
}

void menupush(void)
{
//	BYTE mod;
	if(CLUSTOP){return;}
	if(mode == 1){mode = 3; CLUSTOP = TRUE; return;}
	if(Zmove && menuact){
		mode = 1;
		rngnote = 0;
		CLUSTOP = TRUE;
		return;
	}

	
	menuact = (menuact + 1) % 2;
	if(mode == 7 || mode == 8){
		if(!typeact){
			mode = 3; keyb_draw(); numact = FALSE;
		}
		else{
			if(typeact){makemessagewin(0);}
		}
	}
	else if(mode == 2){mode = 3; wavetest_disp(0);}
	else if(menuact){wavetest_disp(0);}
	else{wavetest_disp(3); keyb_draw();}
	CLUSTOP = TRUE;
}

void move_left(void)
{
	switch(mode){
		case 1:move_mode3(1); nocursor[0] = FALSE; ositg = TRUE; break;
		case 2:move_mode3(1); nocursor[0] = FALSE; ositg = TRUE; break;
		case 3:move_mode3(1); nocursor[0] = FALSE; ositg = TRUE; break;
		default: break;
	}
	
}

void move_up(void){
	switch(mode){
		case 1:move_mode3(2); nocursor[1] = FALSE; ositg = TRUE; break;
		case 2:move_mode3(2); nocursor[1] = FALSE; ositg = TRUE; break;
		case 3:move_mode3(2); nocursor[1] = FALSE; ositg = TRUE; break;
		default: break;
	}
}

void move_right(void){
	switch(mode){
		case 1:move_mode3(4); nocursor[2] = FALSE; ositg = TRUE; break;
		case 2:move_mode3(4); nocursor[2] = FALSE; ositg = TRUE; break;
		case 3:move_mode3(4); nocursor[2] = FALSE; ositg = TRUE; break;
		default: break;
	}
}

void move_down(void){
	switch(mode){
		case 1:move_mode3(8); nocursor[3] = FALSE; ositg = TRUE; break;
		case 2:move_mode3(8); nocursor[3] = FALSE; ositg = TRUE; break;
		case 3:move_mode3(8); nocursor[3] = FALSE; ositg = TRUE; break;
		default: break;
	}
}

void prev_onk(void)
{
	if(CLUSTOP){return;}
	if(onlvl[selch]){
		onlvl[selch]--;
	}
	CLUSTOP = TRUE;
}

void next_onk(void)
{
	if(CLUSTOP){return;}
	if(onlvl[selch] < 8){
		onlvl[selch]++;
	}
	CLUSTOP = TRUE;
}

void hitfkey(BYTE f)
{
	if(CLUSTOP){return;}
	if(selch == f){
		if(!masterfile){
			wv_ch[f].on = (wv_ch[f].on + 1) % 2;
		}
	}
	else{
		selch = f;
		wv_ch[f].on = TRUE;
	}
	CLUSTOP = TRUE;

}

BOOL keybkeychk(void)
{
	BYTE i, yu;
	BYTE yubi;
//	BYTE onklst; //描画用音階
	BYTE skip_key[3] = {5, 13, 255};
	BYTE skipcnt = 0;
	LPBOOL kst = &keyState._q;
	BOOL ons = FALSE;
	BOOL changelvl = FALSE;
	
	
	if(masterfile){return 0;}
	yubi = 0;
	for(i = 0; i < 4; i++){//押しっぱなし確認？
		if(*(kst + musikey[i]) && musikey[i] < 255){
			ons = TRUE;
			changelvl = TRUE;
		}
	}
	for(i = 0; i < 19; i++){//１９のキーのうち
		if(skip_key[skipcnt] == i){kst++; i++; skipcnt++;}//とりあえず2連続は無い
		if(*kst){//どれかキー押してる
			if((musikey[0] != i) && (musikey[1] != i) && (musikey[2] != i) && (musikey[3] != i) && (musikey[4] != i)){
				for(yu = 0; yu < 4; yu++){
					set_freq(yu, i - skipcnt + (onlvl[yu] * 12));
					onkan[yu] = i - skipcnt + (onlvl[yu] * 12);
					flushcnt[yu] = 30;
					flushkey[yu] = (i - skipcnt) % 17;
				}
				CLUSTOP = FALSE;
				for(yubi = 0; yubi < 4; yubi++){
//					set_freq(yubi, i - skipcnt + (onlvl[yubi] * 12));
//					onkan = i - skipcnt + (onlvl[yubi] * 12);
					if(musikey[yubi] == 255){
						musikey[yubi] = i;//押したままのを記憶
						break;
					}
				}
			}
		}
		else{
			for(yu = 0; yu < 4; yu++){//5本の指どれかはなれる
				if(musikey[yu] == i){
					musikey[yu] = 255;
				}
			}
		}
		kst++;
	}
	
	for(i = 0; i < 4; i++){//オクターブチェンジ
		if(*kst){
			if(i % 2 == 0){
				next_onk();//a or k
			}
			else{
				prev_onk();//z or m
			}
			changelvl = TRUE;
		}
		kst++;
	}

	if(ons){//セットする
		if(!CLUSTOP && (osimati == 0)){
//			wv_test();

//					if(wvch_sel(selch, wvbase_set(selch))){
//						wv_loop(selch);
//					}
			for(i = 0; i < 4; i++){
				if(wv_ch[i].on){
					if(wvch_sel(i, wvbase_set(i))){
						if(wv_ch[i].dly){manudly[i] = wv_ch[i].dly + 1;}//再生関数とは別にディレイ値をセット
						else{wv_loop(i);}//無ければそのまま再生
					}
				}
			}
			if(wv_ch[selch].on){
				set_note(selch, 1, onkan[selch]);
			}
		}
		CLUSTOP = TRUE;
		for(yu = 0; yu < 4; yu++){//押しっぱなしは引き伸ばし
			if(!manudly[yu]){//ディレイ無ければ
				if(wv_ch[yu].on && wv_ch[yu].loops == 0){
					wv_ch[yu].loops = ((SRATE * 16 / wv_ch[yu].length) * wv_ch[selch].msec / 1000);
					flushcnt[yu] += 1;
				}
			}
		}
//		scoremove(4);
	}
	
	return changelvl;
}

void manudlyefect(void)
{
	BYTE ch;
	for(ch = 0; ch < 4; ch++){
		if(manudly[ch]){
			if(manudly[ch]){manudly[ch]--;}
			if(wv_ch[ch].on && !manudly[ch]){
				if(wvch_sel(ch, wvbase_set(ch))){
					wv_loop(ch);//無ければそのまま再生
				}
			}
		}
	}
}

BOOL fkeychk(void)//hotキー入力
{
	BOOL anykey = FALSE;
	
	if(keyState.f1 == TRUE){hitfkey(0); anykey = TRUE;}
	if(keyState.f1 == FALSE){}

	if(keyState.f2 == TRUE){hitfkey(1); anykey = TRUE;}
	if((keyState.f2 == FALSE)){}

	if(keyState.f3 == TRUE){hitfkey(2); anykey = TRUE;}
	if(keyState.f3 == FALSE){}

	if(keyState.f4 == TRUE){hitfkey(3); anykey = TRUE;}
	if(keyState.f4 == FALSE){}
	
	return anykey;
}

void numkeychk(void)//メニュー時キー入力
{	

	BOOL anykey = FALSE;
	
//		keyState.osi = 0;
//	BOOL anyact = FALSE;

//	chara[0].action[0] = 0;
	
	if(keyState.num3 == TRUE){numkettei(); anykey = TRUE; nocursor[6] = FALSE;}
	if(keyState.num3 == FALSE){nocursor[6] = TRUE; ositg = FALSE;}

	if(keyState.num2 == TRUE){numcansel(); anykey = TRUE; nocursor[5] = FALSE;}
	if((keyState.num2 == FALSE)){SKIP = FALSE; nocursor[5] = TRUE; ositg = FALSE;}

	if(keyState.num1 == TRUE){trig(); anykey = TRUE;}
	if(keyState.num1 == FALSE){trig_free();}

	if(keyState.enter == TRUE){menupush(); anykey = TRUE;}//menuch
	if(keyState.enter == FALSE){}

	if(keyState.left == TRUE){num_move(1); CLUSTOP = 0; ositg = TRUE; nocursor[0] = FALSE;}
	else{nocursor[0] = TRUE;}
	if(keyState.up == TRUE){num_move(2); CLUSTOP = 0; ositg = TRUE; nocursor[1] = FALSE;}
	else{nocursor[1] = TRUE;}
	if(keyState.right == TRUE){num_move(4); CLUSTOP = 0; ositg = TRUE; nocursor[2] = FALSE;}
	else{nocursor[2] = TRUE;}
	if(keyState.down == TRUE){num_move(8); CLUSTOP = 0; ositg = TRUE; nocursor[3] = FALSE;}
	else{nocursor[3] = TRUE;}
 //
	if(keyState.esc == TRUE){
		exit_game();
 		anykey = TRUE;
	}
//	else{}

//	if((keyState.num3 == FALSE) && (keyState.num2 == FALSE) && (keyState.enter == FALSE) && (keyState.num1 == FALSE) && (clst23 == FALSE)){CLUSTOP = 0; ositg = FALSE;}//3ボタン連打防止
	if(!anykey){CLUSTOP = 0; ositg = FALSE;}//3ボタン連打防止
}

void menukeychk(void)//メニュー時キー入力
{	

	BOOL clst23 = FALSE;
	BOOL anykey = FALSE;
	
	clst23 = keybkeychk();
	anykey = fkeychk();

//		keyState.osi = 0;
//	BOOL anyact = FALSE;

//	chara[0].action[0] = 0;
	
	if(keyState.num3 == TRUE){menukettei(); anykey = TRUE;}
	if(keyState.num3 == FALSE){nocursor[6] = TRUE; ositg = FALSE;}

	if(keyState.num2 == TRUE){menucansel(); anykey = TRUE;}
	if((keyState.num2 == FALSE)){SKIP = FALSE; nocursor[5] = TRUE; ositg = FALSE;}

	if(keyState.num1 == TRUE){trig(); anykey = TRUE;}
	if(keyState.num1 == FALSE){trig_free();}

	if(keyState.enter == TRUE){menupush(); anykey = TRUE;}//menuch
	if(keyState.enter == FALSE){}

	if(keyState.left == TRUE){menu_move(1); ositg = TRUE; nocursor[0] = FALSE;}//連打消し無し　v1111
	else{nocursor[0] = TRUE;}
	if(keyState.up == TRUE){menu_move(2); ositg = TRUE; nocursor[1] = FALSE;}
	else{nocursor[1] = TRUE;}
	if(keyState.right == TRUE){menu_move(4); ositg = TRUE; nocursor[2] = FALSE;}
	else{nocursor[2] = TRUE;}
	if(keyState.down == TRUE){menu_move(8); ositg = TRUE; nocursor[3] = FALSE;}
	else{nocursor[3] = TRUE;}
 //
	if(keyState.esc == TRUE){
		exit_game();
		anykey = TRUE;
	}
//	else{}

//	if((keyState.num3 == FALSE) && (keyState.num2 == FALSE) && (keyState.enter == FALSE) && (keyState.num1 == FALSE) && (clst23 == FALSE)){CLUSTOP = 0; ositg = FALSE;}//3ボタン連打防止
	if(!anykey && (clst23 == FALSE)){CLUSTOP = 0; ositg = FALSE;}//3ボタン連打防止
}

void keychk(void)//キー入力
{	
	BOOL anykey = FALSE;
	BOOL clst23 = FALSE;
	
	clst23 = keybkeychk();
	anykey = fkeychk();
	
	if(keyState.num3 == TRUE){kettei(); anykey = TRUE;}
	if(keyState.num3 == FALSE){nocursor[6] = TRUE; ositg = FALSE;}

	if(keyState.num2 == TRUE){cansel(); SKIP = TRUE; anykey = TRUE;}
	if((keyState.num2 == FALSE)){SKIP = FALSE; nocursor[5] = TRUE; ositg = FALSE;}

	if(keyState.num1 == TRUE){trig(); anykey = TRUE;}
	if(keyState.num1 == FALSE){trig_free();}

	if(keyState.enter == TRUE){menupush(); anykey = TRUE;}//menuch
	if(keyState.enter == FALSE){}

	if(keyState.left == TRUE){move_left(); CLUSTOP = 0;}
	else{nocursor[0] = TRUE;}
	if(keyState.up == TRUE){move_up(); CLUSTOP = 0;}
	else{nocursor[1] = TRUE;}
	if(keyState.right == TRUE){move_right(); CLUSTOP = 0;}
	else{nocursor[2] = TRUE;}
	if(keyState.down == TRUE){move_down(); CLUSTOP = 0;}
	else{nocursor[3] = TRUE;}
 
	if(keyState.esc == TRUE){
//		DeleteObject(hmapBMP);
//		DeleteObject(hCmapBMP);
		exit_game();
		anykey = TRUE;
	}
	else{}


//	if((keyState.num3 == FALSE) && (keyState.num2 == FALSE) && (keyState.enter == FALSE) && (keyState.num1 == FALSE) && (clst23 == FALSE)){CLUSTOP = 0; ositg = FALSE;}//3ボタン連打防止
	if(!anykey && (clst23 == FALSE)){CLUSTOP = 0; ositg = FALSE;}//3ボタン連打防止
}
void exitkeychk(void)//キー入力
{	
	BOOL anykey = FALSE;
	BOOL esckey = FALSE;
	
//	clst23 = keybkeychk();
//	anykey = fkeychk();
	
	if(byebye && !byec){
		exit_game();
	}
	if(byec){byec--; return;}

	if(keyState.num3 == TRUE){
		if(CLUSTOP){anykey = FALSE; esckey = TRUE;}
		else{anykey = TRUE;}
	}
	else if(keyState.num3 == FALSE){esckey = FALSE;}

	if(keyState.num2 == TRUE){anykey = TRUE;}
//	if((keyState.num2 == FALSE)){SKIP = FALSE; nocursor[5] = TRUE; ositg = FALSE;}

	if(keyState.num1 == TRUE){anykey = TRUE;}
//	if(keyState.num1 == FALSE){}

	if(keyState.enter == TRUE){anykey = TRUE;}//menuch
//	if(keyState.enter == FALSE){}

	if(keyState.left == TRUE){anykey = TRUE;}
	else{nocursor[0] = TRUE;}
	if(keyState.up == TRUE){anykey = TRUE;}
	else{nocursor[1] = TRUE;}
	if(keyState.right == TRUE){anykey = TRUE;}
	else{nocursor[2] = TRUE;}
	if(keyState.down == TRUE){
		if(mode == 254){
			wactive = FALSE;
			visible = TRUE;
			CloseWindow(hWnd);
	//		mode = 3; CLUSTOP = 1;
		}
		else{
			anykey = TRUE;
		}
	}
	else{nocursor[3] = TRUE;}

	
	if(keyState.esc == TRUE){
		if(mode == 255){
			if(!CLUSTOP){
				byebye = TRUE;
				byec = 30;
			}
			else{
				esckey = TRUE;
			}
		}
		else{
//				esckey = TRUE;
//			CLUSTOP = 1;
			
		}
//		anykey = TRUE;
	}
	else if(keyState.esc == FALSE){
		if(mode == 255){
			esckey = FALSE;
		}
	}
	
	if(anykey && !esckey && !CLUSTOP){
		CLUSTOP = 1;
		mode = 3;
		ositg = FALSE;
		numact = FALSE;
		typeact = FALSE;
		menuact = TRUE;
//		makemessagewin(0);
//		makestatewin(0);
		keyb_draw();
		wavetest_disp(0);
	}


	if(!esckey && !anykey){
		CLUSTOP = 0; ositg = FALSE;
	}
//	if(!anykey && (clst23 == FALSE)){CLUSTOP = 0; ositg = FALSE;}//3ボタン連打防止
}

void busy_keychk(void)
{
	if(keyState.esc == TRUE){
//		DeleteObject(hmapBMP);
//		DeleteObject(hCmapBMP);
		CLUSTOP = TRUE;
		if(output_f){
			output_f = FALSE;
		}
//		exit_game();
	}
	else{}
	
}

void key_wait(void)
{
	if(mode == 21 || typeact){
		if(nocursor[0] && nocursor[1]  && nocursor[2]  && nocursor[3] && nocursor[5] && nocursor[6]){
			osimati = 0;
		}
		else if(osimati){osimati--; if(osimati == 0){osimati = 1;}}
		else if(!osimati){osimati = 0x10;}
		return;
	}
	if(nocursor[0] && nocursor[1]  && nocursor[2]  && nocursor[3]){
			osimati = 0;
	}
	else if(osimati){osimati--; if(osimati == 0){osimati = 1;}}
	else if(!osimati){osimati = 0x10;}
}

void chkchk(void)
{
	BYTE i;
//	scrchk2();
//	for(i = 0; i < a_sousu; i++){
//		Cpos(c_sort[i]);
//	}
	if(mode == 10){
		if(Lcount % 2){
			wavefmt_out();
		}
		busy_keychk();
	}
	else if(mode > 253){
		exitkeychk();
	}
	else if(numact){
		numkeychk();
	}
	else if(menuact){
		menukeychk();
	}
	else{
		keychk();
	}

	key_wait();
	
	if(splay){
		sound_draw();
		wavefmt_chk();
		if(rectrig && !recloop){
			wavefmt_close();
		}
	}
	
	manudlyefect();
}

void exit_game(void)
{
	if(CLUSTOP){return;}

	if(rectrig){
		wavefmt_close();
 		CLUSTOP = 1;
		return;
	}

	if(mode != 255){
		mode = 255;
		CLUSTOP = TRUE;
		return;
	}
	
	waveOutReset(hplsWaveOut);
	waveOutUnprepareHeader(hplsWaveOut,&plsWhdr[0],sizeof(WAVEHDR));
	waveOutUnprepareHeader(hplsWaveOut,&plsWhdr[1],sizeof(WAVEHDR));
	waveOutUnprepareHeader(hplsWaveOut,&plsWhdr[2],sizeof(WAVEHDR));
	waveOutClose(hplsWaveOut);
	free(lpplsWave[0]);
	free(lpplsWave[1]);
	free(lpplsWave[2]);

	SelectObject(hdcmapBMP, hmapBMP);
	DeleteObject(hmapBMP);
	DeleteObject(hdcmapBMP);

	SelectObject(hdcCmapBMP, hCmapBMP);
	DeleteObject(hdcCmapBMP);
	DeleteObject(hCmapBMP);

	SelectObject(hdcstaBMP, hstaBMP);
	DeleteObject(hstaBMP);
	DeleteObject(hdcstaBMP);

	SelectObject(hdcmesBMP, hmesBMP);
	DeleteObject(hmesBMP);
	DeleteObject(hdcmesBMP);

	SelectObject(hdcBB, hBBBMP);
	DeleteObject(hdcBB);
	DeleteObject(hBBBMP);
	
	
	KillTimer(hWnd, 1);

	PostQuitMessage(0);

}

