/*********メインシステム*********/
static BYTE mTime=0, aCount=0x40;

int stabi = 1;//antei count
DWORD cpupow = 0;//cpu power
DWORD dwTime;

/*フレーム表示*/
//WORD Fcount[10] = {6,6,6,6,6, 6,6,6,6,6};//10回計って
WORD Lcount = 0; //ゲームサイクルカウント
DWORD Famari = 0, Fwari = 10000, Fsyoki = 1;//syokiti 
DWORD Fhenka = 0, Fold = 0;
DWORD P[2] = {0, 0}, I[2] = {0, 0}, D[2] = {1, 0},  Ka = 0;//1:新しい 2:古い
BOOL mo;

//int oldclk, startclk;
WORD mSec = 1000;
WORD showFlame = 0;
WORD vFlame = 60; //ビデオﾌﾚｰﾑ60
BOOL getFlame = FALSE;
BYTE tgFlame = 1;
BYTE slp = 10;//Sleep time
//BYTE FCi = 0;//for 用カウント
//BYTE Fwait = 1;
/**************/

/*******rndm*******/
BYTE rp = 0;//rndm呼び出し回数
BYTE redro = 0;//再描回数
BYTE failp = 0;
/******************/

////randam///基本は value = rndm(Lcount, redro, 1[桁数], true(+符号), max)
int rndm(BYTE times, BYTE option, BOOL hu, WORD m_val){
	long value = 0;//99億tukau
	int int_val;
	WORD bb_val;
	BYTE c, i;
	
	bb_val = m_val + 1;
	failp = 0;
	value = times;
	
	while(bb_val > m_val){
		bb_val = 0;
		for(i = 0; i < option - (failp/2); i++){
//			c = value % 10;//hitoketame
			c = ((redro + 1) + /*(i / 10)*/ + (failp + 1) + (value / 10) * (rp + 1) + (i % 10)) % 10;

			switch(c){
				case 0:value = 1680749523;break;//////////////////////////
				case 1:value = 3529678401;break;////////////ら////////////
				case 2:value = 2417305869;break;////////////　////////////
				case 3:value = 9154282736;break;////////////ん////////////
				case 4:value =  792831654;break;////////////　////////////
				case 5:value = 6835094217;break;////////////だ////////////
				case 6:value = 4203956178;break;////////////　////////////
				case 7:value = 8376120945;break;////////////　////////////
				case 8:value = 7941563082;break;////////////む////////////
				case 9:value = 5268417390;break;//////////////////////////
				default:break;
			}

			value = value / pow(10, (failp + times) % 10);//redraw桁を１桁目に持っていく
			bb_val += (value % 10) * (pow(10, i));//hitoketame
		
			rp++;if(rp > 249){rp = 0;}
		}
		failp++;if(failp > option){failp = 0;}
		
		if(bb_val < m_val){break;}
	}

	if(hu == TRUE){return(bb_val);}//ﾌﾟﾗｽﾉﾐ

	else if(hu == FALSE){////マイナスアリ
		int_val	= bb_val;
		if((times + rp) % 2 == 1){
			int_val	*= -1;
		}
		return(int_val);
	
	}

}

////円16等分三角関数
BYTE HEXcircle(BYTE kakudo/*角度16等分*/, BYTE vecter, BOOL Ymode){
	BYTE ans,/* ansY,*/ l_kakudo,/*0=0_1=90_2=180_3=270*/ s_kakudo/*0=0, 1=30, 2=45, 3=60, 4=90, 5=120, 6=135, 7=150, ....*/;
	
	l_kakudo = kakudo / 4;
	s_kakudo = kakudo % 8;

	if(Ymode == FALSE){
		switch(s_kakudo){
			case 0: ans = vecter; break;
			case 1: ans = vecter * 0.92/*7*/; break;
			case 2: ans = vecter * 0.71/*1*/; break;
			case 3: ans = vecter * 0.38/*2*/; break;
			case 4: ans = 0; break;
			case 5: ans = vecter * 0.38/*2*/; break;
			case 6: ans = vecter * 0.71/*1*/; break;
			case 7: ans = vecter * 0.92/*7*/; break;
		}
	}
	if(Ymode == TRUE){
		switch(s_kakudo){
			case 0: ans = 0; break;
			case 1: ans = vecter * 0.38/*2*/; break;
			case 2: ans = vecter * 0.71/*1*/; break;
			case 3: ans = vecter * 0.92/*7*/; break;
			case 4: ans = vecter; break;
			case 5: ans = vecter * 0.92/*7*/; break;
			case 6: ans = vecter * 0.71/*1*/; break;
			case 7: ans = vecter * 0.38/*2*/; break;
		}
	}
	return(ans);
}

////数字を一桁ずつ表示////
DWORD jussin(DWORD kazu, BYTE keta, WORD XX, WORD YY, BYTE zure){//表示する数字と桁をもってきますた。
	BYTE i, j, k, nanketa;//d_kazu ha 256=0だけど気にしない
	
	DWORD d_kazu;//桁あふれてた060723
	for(nanketa = 0; nanketa < keta; nanketa++){
		d_kazu = kazu / pow(10, nanketa);//keta桁を１桁目に持っていく
			i = d_kazu % 10;

		for(j = 0; j < 8; j++){
			for(k = 0; k < 8; k++){
				if(*(NUM[i] + k + (j * 8)) != 0x00ffffff){
					lpBBPixel[/**/XX + k/**/ - (nanketa * 8) + /**/(320 * (8 - j + YY + (nanketa * zure)))/**/] = *(NUM[i] + k + (j * 8));
				}
			}
		}
	}
	return(kazu);
}

void anime_count(void){
	BYTE p;
	/*タイマーな処理*/
	for(p = 0; p < a_sousu; p++){
		if(!(mTime % (aCount - chara[p].aSpe)) && chara[p].aSpe){
			//if(!(p - 1)){mTime=0;}
		/*aNam0100 aMax0100の時 aNam0000にするつまりNANDアルね。*/
			aNam[p]++;
			aNam[p] = (aNam[p] &~ aMax);
			
		}
	}

	mTime++;
}

//////////////////60フレームズレ検知（精度：貧弱）////////////////////
void flame_cnt(void){
	if(Lcount > 59){
		mSec = GetTickCount() - dwTime;
		dwTime = GetTickCount();
		
		if(showFlame > 58 && showFlame < 62 ){//ずれてない場合
			if(stabi > 0){stabi++;}
			if(stabi > 5){ cpupow = Fwari; stabi = 0;}//基本負荷値を記憶。
		}
		
		else if(stabi > 0){stabi = 1; cpupow = Fwari;}
		//if(stabi < 5){Fwari;}


		if((mSec * 1 > 1016) || (mSec * 1 < 984)){//ずれてる場合
			showFlame =vFlame - (((mSec * 1) - 1000.1) / (1000 / vFlame));//60 + ｛(何mSずれてるか) / (1ﾌﾚｰﾑ何mSか)｝←何ﾌﾚｰﾑずれてるかになる

			if(mSec * 1 > 1320 && Fwari > 5000){Fwari -= 5000; mSec = 1000;}//>75 or 45>
			if(mSec * 1 < 680){Fwari += 5000; mSec = 1000;}

			if(mSec * 1 > 1160 &&  Fwari > 2500){Fwari -= 2500; mSec = 1000;}//>70 or 50>
			if(mSec * 1 < 840){Fwari += 2500; mSec = 1000;}

			if(mSec * 1 > 1080 &&  Fwari > 1250){Fwari -= 1250; mSec = 1000;}//>65 or 55>
			if(mSec * 1 < 920){Fwari += 1250; mSec = 1000;}
		
			if(mSec * 1 > 1016 &&  Fwari > 512){Fwari -= 512;}//>61 or 59>
			if(mSec * 1 < 984){Fwari += 512;}	//flame安定却下。
			
		}

		Lcount = 0;
	}
	
	Lcount++;
}

void fcnt2(void){
	WORD gp = 50, gi = 1, gd = 10;
	short Psa;
	

	if(Lcount >= vFlame){
		mSec = GetTickCount() - dwTime;
		dwTime = GetTickCount();//60週したときの時間
		if(showFlame > 59 && showFlame < 61 ){//ずれてない場合
			if(stabi > 0){stabi++;}
			if(stabi > 5){cpupow = Fwari; stabi = 0;}//基本負荷値を記憶。
		}
		else if(stabi > 0){stabi = 1; cpupow = Fwari;}//ずれたら安定値を１に（０は安定固定状態）、

		showFlame = (vFlame * 1000) / (mSec * 1);//やっと気づきました。2006年夏

		if(mSec * 1 < 1000){//60f/sがずれてる場合
			P[0] = 1000 - (mSec * 1);//比例(目的との差) 今のとこonetime P[0] = 700ぐらいに
			Fold = Fwari - Fold;//前回負荷変化量
			D[1] = P[0] - P[1];//一度で変化した60f間の時間 700 - 500 => 200
			if(D[1] < 1){ D[1] = 1;}

			Ka = (P[0] * 50);// + (gd * (D[0])) + (gi * I[0]);

			if((Fwari + Ka) > 0){Fwari += Ka;}
			else{Fwari = 1;}

			I[1] = P[1];
			P[1] = P[0];//前回の差に記憶
			mo = 0;
		
		}
		
		else if(mSec * 1 > 1000){
			P[0] = (mSec * 1) - 1000;//比例(目的との差) 今のとこonetime P[0] = 700ぐらいに
			if(Fold > Fwari){
				Fold = Fold - Fwari;//前回負荷変化量
			}
			else{Fold = 0;}
			
			D[1] = P[1] - P[0];//一度で変化した60f間の時間 700 - 500 => 200
			if(D[1] < 1){ D[1] = 1;}
			Ka = (P[0] * 50);
			if(Fwari - Ka > 0){Fwari = Fwari - Ka;}
			else{Fwari = 1;}
			P[1] = P[0];//前回の差に記憶
			mo = 1;
		}

		Lcount = 0;
	
//	jussin(showFlame, 6, 40, 81, 0);

//	if(mo == 0){
//		jussin(Ka, 6, 40, 81, 0);
//		jussin(Fold, 8, 65, 90, 0);
//		jussin(D[1], 10, 65, 99, 0);
//		jussin(P[0], 10, 65, 108, 0);
//	}
	
//	if(mo == 1){
//		jussin(Ka, 6, 40, 81, 3);
//		jussin(Fold, 8, 65, 90, 3);
//		jussin(D[1], 10, 65, 99, 3);
//		jussin(Fwari + Ka, 10, 65, 108, 3);
//	}
	
	Lcount++;

}
	
/*			Fhenka = Fwari - Fsyoki;//累積負荷変化量
			Fold = Fwari - Fold;//前回負荷変化量

			P[0] = 1000 - (mSec * 1);//比例(目的との差) 今のとこonetime P[0] = 700ぐらいに
			if(K && P[0] > 0){
				D[1] = P[0] - P[1];//一度で変化した60f間の時間 700 - 500 => 200
				if(D[1] < 0 ){
					D[1] = 1;
				}
//				if(D[0] == 1){
					D[0] = Fold / (D[1] + 0.01);//負荷変化でいくら時間が変化したか？ (4000 / 200) *500 
//				}
//				else{D[0] = 0;}
			}
			I[1] = P[1];
			P[1] = P[0];//前回の差に記憶

			showFlame = (vFlame * 1000) / (mSec * 1);//やっと気づきました。2006年夏
			K = (P[0] * D[0]);// + (gd * (D[0])) + (gi * I[0]);

			if((Fwari += K) > 0){Fwari += K;}
			else{Fwari = 1;}

*/	
}
	
/////////////////////メモリ書き直し負荷///////////////////////
void mati(void){
	for(Famari = 0; Famari < cpupow; Famari++){
		lpBBPixel[Famari / 100]=lpBBPixel[Famari / 100];
//WaitForInputIdle
	}
Famari = 0;
//	while(Famari > cpupow){
//		Famari++;
//	}
	Sleep(10);
//	WaitForInputIdle(Famari);
}

DWORD wait_set(BYTE st, DWORD cp){
	HANDLE hFile;
	DWORD D;
	BYTE mode;
	
	OPENFILENAME ofn;
	DWORD FileSize = 0;
	
	char box_ms[64];
	
	if(keyState.num1){mode = 0;}
	else if(keyState.num2){mode = 1;}
	else if(keyState.num3){mode = 2;}
		
	switch(mode){
		////安定時書きだし
		case 0: if(stabi == 0){hFile = CreateFile("huka.pre", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					WriteFile(hFile, &cpupow, (DWORD)sizeof(cpupow), &D, NULL);
					wsprintf(box_ms,"現在の安定度 ＝ %d, CPU Power = %7d", st, cp);
					MessageBox(NULL,"CpuPower書き出し完了",box_ms,MB_OK);
				}
				else{
					wsprintf(box_ms,"現在の安定度 ＝ %d, CPU Power = %7d", st, cp);
					MessageBox(NULL,"安定してませんよ",box_ms,MB_OK);
				}
				break;
				
		////読み込み
		case 1: 
				hFile = CreateFile("huka.pre", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				FileSize = GetFileSize(hFile, NULL);
				if(FileSize == -1){MessageBox(NULL,"読み込み失敗。huka.preがありません。","FileSizeが '-1' です",MB_OK);
					break;}//読み失敗

				stabi = 0;//すでに安定とす				
				ReadFile(hFile, &cp, FileSize, &D, NULL);
				
				wsprintf(box_ms,"現在の安定度 ＝ %d, CPU Power = %7d %d", st, cp, FileSize);
				MessageBox(NULL,"CpuPower読み込み完了",box_ms,MB_OK);
				//読み成功
				break;
				
		////リセット
		case 2: stabi = 1; 
				wsprintf(box_ms,"現在の安定度 ＝ %d, CPU Power = %7d", st, cp);
				MessageBox(NULL,"負荷測定リセット",box_ms,MB_OK);
				break;
		
		default: break;
	}
	
	CloseHandle(hFile);
	return cp;

}

void exit_game(void){
//	if (lpDIB32 != NULL) {

//		HeapFree(GetProcessHeap(), 0, lpDIB32);
//		HeapFree(GetProcessHeap(), 0, lpBuf);
//	}
//	KillTimer(hWnd, 1);

	
	
	PostQuitMessage(0);

}