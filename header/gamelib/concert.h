//演奏
HANDLE wvfh;
DWORD dwWriteSize;
DWORD wvdatsizepos[2];//長さを再書き込み

HANDLE nwvhFile;//namafileyou
DWORD wvFileSize;
DWORD Dw;

WORD flushcnt[4] = {0, 0, 0, 0};
BYTE flushkey[4] = {0, 0, 0, 0};

struct wavenote{
	BYTE mode[4048];
	BYTE time[4048];
	BYTE page[4048];
	short value[4048];
} wvnote[8];

WORD maxnote = 4048;

struct wavenoton{//LOADCHECK用
	BYTE mode[4048];
	BYTE time[4048];
	BYTE page[4048];
	short value[4048];
} wvnoton[8];


//mode 0:null
//mode 1:onkai
//mode 2:vol
//mode 3:att
//mode 4:frq
//mode 5:dch
//mode 6:dly
//mode 7:lng
//mode 8:type
//mode 128~:fm
//mode 255:end

BYTE filename[10][16];//10file, 32charactor
BYTE thisname[32];

/*struct eventnote{
	BYTE time[1024];
	BYTE page[1024];
	BYTE vol;
	short att;
	BYTE ch;
	BYTE dch; 
	BYTE dly;
	BYTE lng;
	BYTE type;
	BYTE fm;
}
*/

WORD prg[4] = {0, 0, 0, 0};
BYTE score_page = 0;
BYTE score_time = 0;
BYTE start_point[2] = {0, 0};
BYTE end_point[2] = {0, 0};
BYTE del_point[2] = {0, 0};

//同じだったら終了とする予定
BYTE res_point[2] = {0, 0};//time, page restartpoint
BYTE ret_point[2] = {240, 255};//time, page returnpoint

BYTE dly_a[4] = {0, 0, 0, 0};//ディレイ予約
WORD dly_s[4] = {0, 0, 0, 0};//ディレイ予約開始時間

BOOL splay = FALSE;
BOOL rectrig = FALSE;
BYTE recloop = 0;
BYTE spquick = 0;
DWORD wvsize = 0;

DWORD output_c = 0;
BOOL output_f = FALSE;

BOOL masterfile = FALSE;//userは観覧・編集・コピーできません
	
void note_syoki(void){
	BYTE i;
	FillMemory(wvnote, sizeof(wvnote), 0);
	FillMemory(filename, sizeof(filename), 0);
	FillMemory(thisname, sizeof(thisname), 0);

	for(i = 0; i < 1; i++){
		wvnote[i].mode[255] = 255;
		wvnote[i].time[255] = 239;
		wvnote[i].page[255] = 20;
	}
}

BOOL search_note(BYTE ch)
{
	WORD i, j;
//	BYTE ch;
	BOOL fit = FALSE;
//	for(ch = 0; ch < 4; ch++){
	if(!wvnote[ch].mode[0]){return fit;}
//	if(!score_page && !score_time){return fit;}

	for(i = 0; i < maxnote; i++){//位置を検索
		if(wvnote[ch].page[i] == score_page){
			if(wvnote[ch].time[i] == score_time){
//					fit += 1 < ch;
				fit = TRUE;
				break;
			}
		}
		if(!wvnote[ch].mode[i]){
			break;
//				break;
		}
	}
//	}
	return fit;

}

//timeとpageから再生位置を検索
WORD research_note(BYTE ch, BYTE page, BYTE time){
	WORD i;
	BOOL fit;

	for(i = 0; i < maxnote; i++){//位置を検索
		fit = TRUE;
		if(!wvnote[ch].mode[i]){break;}

		if(page > wvnote[ch].page[i]){fit = FALSE;}
		else if((page == wvnote[ch].page[i]) && (time > wvnote[ch].time[i])){fit = FALSE;}
		//同じかwvnoteが過ぎた場合はfit=TRUEで抜ける
		if(fit){break;}
	}
	return i;
}

void tuneup(BYTE ch, WORD i)
{
	switch(wvnote[ch].mode[i]){
//		case 1: 
		case 2: wv_ch[ch].vol = wvnote[ch].value[i]; break;
		case 3: wv_ch[ch].t_att = wvnote[ch].value[i]; break;
		case 4: wv_ch[ch].t_gin = wvnote[ch].value[i]; break;
		case 5: wv_ch[ch].dch = wvnote[ch].value[i]; break;
		case 6: wv_ch[ch].dly = wvnote[ch].value[i]; break;
		case 7: wv_ch[ch].msec = wvnote[ch].value[i]; break;
		case 8: wv_ch[ch].fs = wvnote[ch].value[i]; break;
		case 9: wv_ch[ch].type = wvnote[ch].value[i]; break;
		case 10: wv_ch[ch].pws = wvnote[ch].value[i]; break;
		case 11: wv_ch[ch].rfs = wvnote[ch].value[i]; break;
//		case 9: ; break;//stop
		default: break;
	}
}

void sound_draw(void)
{
	WORD i, no, dno;
	BYTE ch;
	BYTE wat;
	BYTE wap;
	BOOL th;
	BOOL tu;
	BOOL putdly;

	for(ch = 0; ch < 4; ch++){
		th = TRUE;
		tu = FALSE;
		putdly = FALSE;
		wat = wvnote[ch].time[prg[ch]];//次のデータ、発生タイム
		wap = wvnote[ch].page[prg[ch]];//（ディレイがある場合遅めに設定されることになる）
		
		if(wvnote[ch].mode[prg[ch]] > 1){tu = TRUE;}//tuneだよ
		else if(!wvnote[ch].mode[prg[ch]]){th = FALSE; tu = FALSE;}//NODATA

		if(wv_ch[ch].dly){//ディレイ値加算
			dno = research_note(ch, score_page, score_time);

			wap += (wat + wv_ch[ch].dly) / 240;
			wat = (wat + wv_ch[ch].dly) % 240;
			if(score_time != wat){th = FALSE; tu = FALSE;}//まだはじまらない
			if(score_page != wap){th = FALSE; tu = FALSE;}//まだはじまらない

			if(score_time == wvnote[ch].time[dno]){
				if(score_page == wvnote[ch].page[dno]){
					if(wvnote[ch].mode[dno] > 1){
						putdly = TRUE;//ディレイのみ取り出し用
					}
				}
			}
				
			
//			if(th && tu){
//				no = research_note(ch, wap, wat);
//				prg[ch] = no;
//			}
		}
		else{
			if(score_time != wat){th = FALSE; tu = FALSE;}//まだはじまらない
			if(score_page != wap){th = FALSE; tu = FALSE;}//まだはじまらない
		}

		
		
		
		if(putdly){
			no = dno;//dno はディレイがあったときのバックアップ
					while(putdly == TRUE){//ディレイあるときしかこないはず
						if(wvnote[ch].mode[no] == 6){//ディレイきた
							if(wvnote[ch].value[no] < wv_ch[ch].dly){
								tu = TRUE;
								th = TRUE;
								prg[ch] = dno;//そのときのTUNE群の先頭位置
								break;
							}
						}
						no++;
						if(wvnote[ch].mode[dno] < 2){tu = FALSE; th = TRUE;}
						if(wvnote[ch].time[no] != wvnote[ch].time[no - 1]){putdly = FALSE;}
						if(wvnote[ch].page[no] != wvnote[ch].page[no - 1]){putdly = FALSE;}
					}
		}

		if(th && tu){//tune変更
			putdly = FALSE;//dlyセットしたか
//			if(wat == score_time && wap == score_page){
	
				dno = prg[ch];
				if(wv_ch[ch].dly == 0){//0なら
//				if(wv_ch[ch],dly < wvnote[ch].value[dno]){//0なら
					while(tu == TRUE && th == TRUE){
						tuneup(ch, dno);

						dno++;
						if(wvnote[ch].mode[dno] < 2){tu = FALSE;}
						if(wat != wvnote[ch].time[dno]){th = FALSE;}
						if(wap != wvnote[ch].page[dno]){th = FALSE;}
						
					}
					if(wv_ch[ch].dly > 0){//中でディレイがセットされたら
						tu = FALSE;
						th = FALSE;
//						wv_ch[ch].dly = 255;
//						dno = research_note(ch, score_page - , score_time);
					}
					if(putdly){th = FALSE;}
					prg[ch] = dno;
					
				}
				else{
					while(tu == TRUE && th == TRUE){
						tuneup(ch, dno);
						if(wv_ch[ch].dly > 0){//中でディレイがセットされたら
							putdly = TRUE;
						}

						dno++;
						if(wvnote[ch].mode[dno] < 2){tu = FALSE;}
						if(wvnote[ch].time[dno] != wvnote[ch].time[dno - 1]){th = FALSE;}
						if(wvnote[ch].page[dno] != wvnote[ch].page[dno - 1]){th = FALSE;}
					}
//					if(putdly){
//						dno = research_note(ch, score_page, score_time);
//					}
					prg[ch] = dno;
					
					
				}
//			}
		}
		
		if(th && !tu){//notedata inputwave
			flushcnt[ch] = wv_ch[ch].msec;
			flushkey[ch] = wvnote[ch].value[prg[ch]] % 12;
			wv_ch[ch].freq = onkai[wvnote[ch].value[prg[ch]]];
			wv_ch[ch].nonkai = onkai[wvnote[ch].value[prg[ch]]];
			if(wvch_sel(ch, wvbase_set(ch))){
				wv_loop(ch);
			}
	
			prg[ch]++;
		}
	}

	if(ret_point[1] == score_page){//本終了地点で終わった
		if(ret_point[0] == score_time){
			score_page = res_point[1];
			score_time = res_point[0];
			for(ch = 0; ch < 4; ch++){
				prg[ch] = research_note(ch, score_page, score_time);
			}
			if(recloop){recloop--;}
			return;
		}
	}
	if(end_point[0] || end_point[1]){//仮ポイントで終わった
		if(end_point[1] == score_page){
			if(end_point[0] == score_time){
				score_page = start_point[1];
				score_time = start_point[0];
				for(ch = 0; ch < 4; ch++){
					prg[ch] = research_note(ch, score_page, score_time);
				}
				if(recloop){recloop--;}
				return;
			}
		}
	}
	
	if(score_time == 239){
		score_page++;
		
	}
	score_time = (score_time + 1) % 240;

}

	
void soundsave(BYTE L){

	char no[3] = {'0', '0', '0'};
	char type[4];
	char names[16];
	char back[4];
	char *name;

	HANDLE hFile;
	OPENFILENAME ofn;
	DWORD D;
	DWORD FileSize;

	WORD i, j, k, val = 0;
	BYTE notecount, ch;

////////////////name_entry//////////////
	strcpy(names, "BGMDATA/");
	strcpy(back, "bak/");

	for(i = 0; i < 3; i++){
		switch(i){
			case 0:	val = (L) / 100;break;//100
			case 1:	val = (L % 100) / 10;break;//10
			case 2:	val = (L) % 10;break;//1
			default: break;
		}
		switch(val){
			case 0: no[i] = '0';break;
			case 1: no[i] = '1';break;
			case 2: no[i] = '2';break;
			case 3: no[i] = '3';break;
			case 4: no[i] = '4';break;
			case 5: no[i] = '5';break;
			case 6: no[i] = '6';break;
			case 7: no[i] = '7';break;
			case 8: no[i] = '8';break;
			case 9: no[i] = '9';break;
			default: no[i] = 'X'; break;
		}
	}

	
	for(i =0; i < 3; i++){
		names[8 + i] = no[i];
	}
	strcpy(type, ".bmd");//background mugic data

	for(; i < 7; i++){
		names[8 + i] = type[i - 3];
	}
	names[8 + i] = NULL;
	
	name = &(names[0]);
////////////////name_entry//////////////

	val = 0;
	hFile = CreateFile(name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//	FileSize = GetFileSize(hFile, NULL);

//	WriteFile(hFile, &val, 1, &D, NULL);//dataNO
	WriteFile(hFile, &mastercode, 2, &D, NULL);//dataNO
	WriteFile(hFile, thisname, 32, &D, NULL);//dataNO
	WriteFile(hFile, res_point, 2, &D, NULL);//dataNO
	WriteFile(hFile, ret_point, 2, &D, NULL);//dataNO
	for(ch = 0; ch < 4; ch++){
		for(i = 0; i < maxnote; i++){
			WriteFile(hFile, &wvnote[ch].mode[i], 1, &D, NULL);//modeから4BYTE一気に読む
			WriteFile(hFile, &wvnote[ch].value[i], 1, &D, NULL);//modeから4BYTE一気に読む
			WriteFile(hFile, &wvnote[ch].time[i], 1, &D, NULL);//modeから4BYTE一気に読む
			WriteFile(hFile, &wvnote[ch].page[i], 1, &D, NULL);//modeから4BYTE一気に読む
			val = (val + wvnote[ch].value[i]) % 256;//VALUE加算
			if(!wvnote[ch].mode[i]){break;}
		}
	}
	val = (val + thisname[0] + thisname[1] + mastercode) % 256;//VALUE加算
	WriteFile(hFile, &val, 1, &D, NULL);//alldata % 256
	CloseHandle(hFile);
	MessageBox(NULL,"書き込み。", name,MB_OK);
}

void loadsuccess(void){
	WORD i;
	BYTE ch;
	BOOL def;
	
	for(ch = 0; ch < 4; ch++){
		def = FALSE;
		for(i = 0; i < maxnote; i++){
			if(def){
				wvnote[ch].mode[i] = 0;
				wvnote[ch].value[i] = 0;
				wvnote[ch].time[i] = 0;
				wvnote[ch].page[i] = 0;
			}
			else{
				wvnote[ch].mode[i] = wvnoton[ch].mode[i];
				wvnote[ch].value[i] = wvnoton[ch].value[i];
				wvnote[ch].time[i] = wvnoton[ch].time[i];
				wvnote[ch].page[i] = wvnoton[ch].page[i];
			}
			if(!wvnoton[ch].mode[i]){def = TRUE;}
			wv_ch[ch].on = TRUE;
		}
	}
}

void loadbuffdel(void){
	WORD i;
	BYTE ch;
	
	for(ch = 0; ch < 4; ch++){
		for(i = 0; i < maxnote; i++){
			wvnoton[ch].mode[i] = 0;
			wvnoton[ch].value[i] = 0;
			wvnoton[ch].time[i] = 0;
			wvnoton[ch].page[i] = 0;
		}
	}
}

void soundload(BYTE L, BYTE op){

	char no[3] = {'0', '0', '0'};
	char type[4];
	char names[16];
	char back[4];
	char *name;

	HANDLE hFile;
	OPENFILENAME ofn;
	DWORD D;
	DWORD FileSize;

	WORD i, j, k, val = 0, rval = 0;
	BYTE notecount, ch;
	WORD mas = 0;
	BOOL mess = FALSE;

////////////////name_entry//////////////
	strcpy(names, "BGMDATA/");
	strcpy(back, "bak/");

	for(i = 0; i < 3; i++){
		switch(i){
			case 0:	val = (L) / 100;break;//100
			case 1:	val = (L % 100) / 10;break;//10
			case 2:	val = (L) % 10;break;//1
			default: break;
		}
		switch(val){
			case 0: no[i] = '0';break;
			case 1: no[i] = '1';break;
			case 2: no[i] = '2';break;
			case 3: no[i] = '3';break;
			case 4: no[i] = '4';break;
			case 5: no[i] = '5';break;
			case 6: no[i] = '6';break;
			case 7: no[i] = '7';break;
			case 8: no[i] = '8';break;
			case 9: no[i] = '9';break;
			default: no[i] = 'X'; break;
		}
	}

	
	for(i =0; i < 3; i++){
		names[8 + i] = no[i];
	}
	strcpy(type, ".bmd");//background mugic data

	for(; i < 7; i++){
		names[8 + i] = type[i - 3];
	}
	names[8 + i] = NULL;
	
	name = &(names[0]);
////////////////name_entry//////////////

	val = 0;
	hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FileSize = GetFileSize(hFile, NULL);

	if(FileSize + 1 == 0){
		MessageBox(NULL,"読み込み失敗。データがありません。", name,MB_OK);
		CloseHandle(hFile);
		return;
	}//読み失敗

//	ReadFile(hFile, &val, 1, &D, NULL);//dataNO
	ReadFile(hFile, &mas, 2, &D, NULL);//dataNO
	ReadFile(hFile, thisname, 32, &D, NULL);//dataNO
	ReadFile(hFile, res_point, 2, &D, NULL);//dataNO
	ReadFile(hFile, ret_point, 2, &D, NULL);//dataNO
	if(!op){//edit mode
		if((mas != mastercode) && !mastermode){//userがmasterのファイルをひらいた、逆にmasterがuserファイルを開ける
//			MessageBox(NULL,"このファイルは開けません。", name,MB_OK);
//			CloseHandle(hFile);
//			return;
			masterfile = TRUE;
		}
		else{
			masterfile = FALSE;
		}
	}
	else if(op == 1){
		if((mas != mastercode)){//userがmasterのファイルをひらいた、逆にmasterがuserファイルを開ける
			MessageBox(NULL,"このファイルは開けません。", name,MB_OK);
			CloseHandle(hFile);
			return;
		}
	}
	
	for(ch = 0; ch < 4; ch++){
		for(i = 0; i < maxnote; i++){
			ReadFile(hFile, &wvnoton[ch].mode[i], 1, &D, NULL);
			ReadFile(hFile, &wvnoton[ch].value[i], 1, &D, NULL);
			ReadFile(hFile, &wvnoton[ch].time[i], 1, &D, NULL);
			ReadFile(hFile, &wvnoton[ch].page[i], 1, &D, NULL);
			val = (val + wvnoton[ch].value[i]) % 256;
			if(!wvnoton[ch].mode[i]){break;}
//			if(wvnoton[ch].time[i] == 0x32 && wvnoton[ch].page[i] == 0x04){break;}
//			if(wvnoton[ch].time[i] == 0x34 && wvnoton[ch].page[i] == 0x04){break;}
		}
		
	}
	val = (val + thisname[0] + thisname[1] + mastercode) % 256;//VALUE加算
	ReadFile(hFile, &rval, 1, &D, NULL);//dataNO//*WORD
/*	if(rval == val || mastermode){
		loadsuccess();
		if(!op){
			MessageBox(NULL,"読み込み。", name,MB_OK);
		}
	}
	else if(!mastermode){
		MessageBox(NULL,"データが壊れています。", name,MB_OK);
	}*/
	if(rval == val || rval == (val + 8678) % 256 || mastermode){
		loadsuccess();
		if(!op && !masterfile){
			MessageBox(NULL,"読み込み。", name,MB_OK);
		}
		else if(!op  && masterfile){
			MessageBox(NULL,"このファイルは再生のみ可能です。", name,MB_OK);
		}
	}
	else{
		MessageBox(NULL,"データが壊れています。", name,MB_OK);
	}
	loadbuffdel();
	
	start_point[0] = 0;
	start_point[1] = 0;
	end_point[0] = 0;
	end_point[1] = 0;
	
	CloseHandle(hFile);

}

void snameload(WORD L, BYTE nameb)//名前だけ
{

	char no[3] = {'0', '0', '0'};
	char type[4];
	char names[16];
	char back[4];
	char *name;

	HANDLE hFile;
	OPENFILENAME ofn;
	DWORD D;
	DWORD FileSize;

	WORD i, j, k, val, rval;
	BYTE notecount, ch;
	WORD mas = 0;

////////////////name_entry//////////////
	strcpy(names, "BGMDATA/");
	strcpy(back, "bak/");

	L++;
	for(i = 0; i < 3; i++){
		switch(i){
			case 0:	val = (L) / 100;break;//100
			case 1:	val = (L % 100) / 10;break;//10
			case 2:	val = (L) % 10;break;//1
			default: break;
		}
		switch(val){
			case 0: no[i] = '0';break;
			case 1: no[i] = '1';break;
			case 2: no[i] = '2';break;
			case 3: no[i] = '3';break;
			case 4: no[i] = '4';break;
			case 5: no[i] = '5';break;
			case 6: no[i] = '6';break;
			case 7: no[i] = '7';break;
			case 8: no[i] = '8';break;
			case 9: no[i] = '9';break;
			default: no[i] = 'X'; break;
		}
	}

	
	for(i =0; i < 3; i++){
		names[8 + i] = no[i];
	}
	strcpy(type, ".bmd");//background mugic data

	for(; i < 7; i++){
		names[8 + i] = type[i - 3];
	}
	names[8 + i] = NULL;
	
	name = &(names[0]);
////////////////name_entry//////////////

	val = 0;
	hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FileSize = GetFileSize(hFile, NULL);

	if(FileSize + 1 == 0){
//		MessageBox(NULL,"読み込み失敗。データがありません。", name,MB_OK);
		i = 0;
		filename[nameb][i++] = 14 + 180;
		filename[nameb][i++] = 15 + 180;
		filename[nameb][i++] = 0;
		filename[nameb][i++] = 14 + 180;
		filename[nameb][i++] = 1 + 180;
		filename[nameb][i++] = 13 + 180;
		filename[nameb][i++] = 5 + 180;
		for(; i < 16; i++){	
			filename[nameb][i] = 0;
		}
		CloseHandle(hFile);
		return;
	}//読み失敗

//	ReadFile(hFile, &val, 1, &D, NULL);//dataNO
	ReadFile(hFile, &mas, 2, &D, NULL);//dataNO
	ReadFile(hFile, filename[nameb], 16, &D, NULL);//dataNO

	CloseHandle(hFile);
	
}


void set_bgmdata(BYTE no, BYTE op)//2番目、最初から
{
	BYTE ch;
	soundload(no, 1);
	score_page = 0;
	score_time = 0;
	for(ch = 0; ch < 4; ch++){
		prg[ch] = research_note(ch, score_page, score_time);
	}

}

void wavefmt_out(void)
{
//	DWORD Dw;
	short val;
	DWORD i;
	

	for(i = 0; i < (wvFileSize / 2) / 60; i++){
//	for(i = 0; i < 10000; i++){
		if(!output_f){
			output_c = 0;
			CloseHandle(wvfh);
			CloseHandle(nwvhFile);
			mode = 3;
			return;
		}
		
		ReadFile(nwvhFile, &val, 2, &Dw, NULL);//
		WriteFile(wvfh, &val, 2, &dwWriteSize,NULL);
		output_c++;
		if((wvFileSize / 2) == output_c){
			output_f = FALSE;
			break;
		}
	}
}

void wavefmt_chk(void)
{
	WORD rpf = wpf[selwvbuff] / 6;//6フレームあたりのbyte(2BYTE)count
	WORD lpf = rpf * ((Lcount + 1) % 6);//現在のバッファ位置
	BOOL nextbuff = FALSE;//次のバッファにします
	BYTE b;
	WORD i;

	if(!rectrig){return;}
//	WriteFile(wvfh,plsWhdr[nowbuff]->lpData,plsWhdr[0]->dwBufferLength,&dwWriteSize,NULL);
	if((Lcount + 1) % 6 == 0){nextbuff = TRUE;}
	if((Lcount + 1) % 6 == 5){rpf = wpf[selwvbuff] - (rpf * 5);}//余り分を補正
	b = (now_buff + nextbuff) % 2;


	WriteFile(wvfh, &lpplsWave[b][lpf], rpf * 2, &dwWriteSize, NULL);
}

void wavefmt_save(BYTE fmt)
{
	WORD rpf = wpf[selwvbuff] / 6;
//	HANDLE nwvhFile;//グローバルへ
//	OPENFILENAME ofn;
//	DWORD D;//グローばるへ
	DWORD i;

    DWORD size=(plsWhdr->dwBufferLength + 44)-8;
	
	if(fmt == 0){
		wvfh = CreateFile("BGMDATA/NAMADATA", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		rectrig = TRUE;//録音
		wvsize = 0;
		score_page = start_point[1]; score_time = start_point[0];
//		prg[0] = research_note(0, score_page, score_time);
//		prg[1] = research_note(1, score_page, score_time);
//		prg[2] = research_note(2, score_page, score_time);
//		prg[3] = research_note(3, score_page, score_time);
		prg[0] = 0; prg[1] = 0; prg[2] = 0; prg[3] = 0;
		splay = TRUE;
	}
	else if(fmt == 1){
		nwvhFile = CreateFile("BGMDATA/NAMADATA", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		wvFileSize = GetFileSize(nwvhFile, NULL);
		
		wvfh = CreateFile("WAVEFILE.wav", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		
		wvFileSize += 44 - 8;
		WriteFile(wvfh,"RIFF",4,&dwWriteSize,NULL);//1-4
		WriteFile(wvfh,&wvFileSize,4,&dwWriteSize,NULL);//filesize?//5-8
		WriteFile(wvfh,"WAVE",4,&dwWriteSize,NULL);//9-12
		WriteFile(wvfh,"fmt ",4,&dwWriteSize,NULL);//13-16
		size = 16;
		WriteFile(wvfh,&size,4,&dwWriteSize,NULL);//16bi//17-20

		size = 1;
		WriteFile(wvfh,&plsWf.wFormatTag,2,&dwWriteSize,NULL);//wvfmtex//21-22
		WriteFile(wvfh,&plsWf.nChannels,2,&dwWriteSize,NULL);//wvfmtex//23-24
		size = 22050;
		WriteFile(wvfh,&plsWf.nSamplesPerSec,4,&dwWriteSize,NULL);//wvfmtex//25-28
		WriteFile(wvfh,&size,4,&dwWriteSize,NULL);//wvfmtex//29-32
		size = 2;
		WriteFile(wvfh,&plsWf.nBlockAlign,2,&dwWriteSize,NULL);//wvfmtex//33-34
		WriteFile(wvfh,&plsWf.wBitsPerSample,2,&dwWriteSize,NULL);//wvfmtex//35-36

		WriteFile(wvfh,"data",4,&dwWriteSize,NULL);//37-40
		wvFileSize -= 126 - 8;
		WriteFile(wvfh,&wvFileSize,4,&dwWriteSize,NULL);//41-44

		output_f = TRUE;
		mode = 10;
//		for(i = 0; i < (FileSize / 2); i++){
//			ReadFile(nwvhFile, &val, 2, &Dw, NULL);//
//			WriteFile(wvfh, &val, 2, &dwWriteSize,NULL);
//		}
	}

}

void wavefmt_close(void)//ENDSTOP
{
	CloseHandle(wvfh);

	rectrig = FALSE;
	splay = FALSE;

	wavefmt_save(1);
	
}

