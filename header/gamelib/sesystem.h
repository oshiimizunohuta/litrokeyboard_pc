//演奏
//HANDLE wvfh;//concert.hで宣言済み
//DWORD dwWriteSize;
//DWORD wvdatsizepos[2];//長さを再書き込み

//HANDLE nwvhFile;//namafileyou
//DWORD wvFileSize;
//DWORD Dw;

//WORD flushcnt[4] = {0, 0, 0, 0};
//BYTE flushkey[4] = {0, 0, 0, 0};


//読み込みデータ溜め
struct sebind{
	BYTE mode[8192];
	BYTE time[8192];
	BYTE page[8192];
	short value[8192];
} seb[2];

//再生時ここに格納
struct soundefectnote{
	BYTE mode[256];
	BYTE time[256];
	BYTE page[256];
	short value[256];
} senote[4];

//LOADCHECK用
struct soundefectnoton{
	BYTE mode[256];
	BYTE time[256];
	BYTE page[256];
	short value[256];
} senoton[4];

WORD maxsenote = 256;

WORD se_ad[128];//seb用アドレス
WORD se_adcnt = 0;//アドレスがどこまでいったか

WORD seprg[4] = {0, 0, 0, 0};
BYTE se_page[2] = {0, 0};//seprg[0]seprg[1]用, seprg[2]seprg[3]用
BYTE se_time[2] = {0, 0};//
BYTE start_sepoint[4] = {0, 0, 0, 0};
BYTE end_sepoint[4] = {0, 0, 0, 0};
BYTE del_sepoint[4] = {0, 0, 0, 0};

BYTE seselch = 0; //0:ch[0],ch[1]  1:ch[2],ch[3]
BOOL seplay[2] = {0, 0};
//同じだったら終了とする予定
//BYTE res_point[2] = {0, 0};//time, page restartpoint
//BYTE ret_point[2] = {240, 255};//time, page returnpoint



//BOOL splay = FALSE;
//BOOL rectrig = FALSE;
//BYTE recloop = 0;
//DWORD wvsize = 0;

//DWORD output_c = 0;
//BOOL output_f = FALSE;


	
void se_syoki(void){
	BYTE i;
	FillMemory(senote, sizeof(senote), 255);
//	FillMemory(seb, sizeof(seb), 0);
//	FillMemory(thisname, sizeof(thisname), 0);

	for(i = 0; i < 4; i++){
		senote[i].mode[255] = 255;
		senote[i].time[255] = 239;
		senote[i].page[255] = 20;
	}
	
}

BOOL search_senote(BYTE ch)
{
	WORD i, j;
	BYTE p, t;
//	BYTE ch;
	BOOL fit = FALSE;
//	for(ch = 0; ch < 4; ch++){
	if(!senote[ch].mode[0]){return fit;}
//	if(!score_page && !score_time){return fit;}
	if(ch < 2){p = se_page[0]; t = se_time[0];}
	else{p = se_page[1]; t = se_time[1];}

	for(i = 0; i < maxsenote; i++){//位置を検索
		if(senote[ch].page[i] == p){
			if(senote[ch].time[i] == t){
//					fit += 1 < ch;
				fit = TRUE;
				break;
			}
		}
		if(!senote[ch].mode[i]){
			break;
//				break;
		}
	}
//	}
	return fit;

}

//timeとpageから再生位置を検索
WORD research_senote(BYTE ch, BYTE page, BYTE time){
	WORD i;
	BOOL fit;

	for(i = 0; i < maxsenote; i++){//位置を検索
		fit = TRUE;
		if(!senote[ch].mode[i]){break;}

		if(page > senote[ch].page[i]){fit = FALSE;}
		else if((page == senote[ch].page[i]) && (time > senote[ch].time[i])){fit = FALSE;}
		//同じかwvnoteが過ぎた場合はfit=TRUEで抜ける
		if(fit){break;}
	}
	return i;
}

void setuneup(BYTE ch, WORD i)
{
	BYTE och = ch + 12;
	switch(senote[ch].mode[i]){
//		case 1: 
		case 2: wv_ch[och].vol = senote[ch].value[i]; break;
		case 3: wv_ch[och].t_att = senote[ch].value[i]; break;
		case 4: wv_ch[och].t_gin = senote[ch].value[i]; break;
		case 5: wv_ch[och].dch = senote[ch].value[i]; break;
		case 6: wv_ch[och].dly = senote[ch].value[i]; break;
		case 7: wv_ch[och].msec = senote[ch].value[i]; break;
		case 8: wv_ch[och].fs = senote[ch].value[i]; break;
		case 9: wv_ch[och].type = senote[ch].value[i]; break;
		case 10: wv_ch[och].pws = senote[ch].value[i]; break;
		case 11: wv_ch[och].rfs = senote[ch].value[i]; break;
//		case 9: ; break;//stop
		default: break;
	}
}

void se_draw(void)
{
	WORD i, no, dno;
	BYTE ch, och;
	BYTE seton;
	BYTE wat;
	BYTE wap;
	BYTE s_t, s_p;
	BOOL th;
	BOOL tu;
	BOOL endflg[4] = {0, 0, 0, 0};//4つが1立つとDATA再生終わり
	BOOL putdly;
	short vo;

//	for(ch = 2 - seselch + seselch; ch < 4 - (seselch + seselch); ch++){
	for(ch = 0; ch < 4; ch++){
		och = ch + 12;
		th = TRUE;
		tu = FALSE;
		putdly = FALSE;
		seton = 0;

		if(ch < 2){
			s_t = se_time[0];
			s_p = se_page[0];
		}
		else{
			s_t = se_time[1];
			s_p = se_page[1];
		}
		
		wat = senote[ch].time[seprg[ch]];
		wap = senote[ch].page[seprg[ch]];

		if(senote[ch].mode[seprg[ch]] > 1){tu = TRUE;}//tuneだよ
		else if(!senote[ch].mode[seprg[ch]]){th = FALSE; tu = FALSE;}//NODATA

		if(wv_ch[och].dly){//ディレイ値加算
			dno = research_note(ch, score_page, score_time);

			wap += (wat + wv_ch[och].dly) / 240;
			wat = (wat + wv_ch[och].dly) % 240;
			if(s_t != wat){th = FALSE; tu = FALSE;}//まだはじまらない
			if(s_p != wap){th = FALSE; tu = FALSE;}//まだはじまらない

			if(s_t == senote[ch].time[dno]){
				if(s_p == senote[ch].page[dno]){
					if(senote[ch].mode[dno] > 1){
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
			if(s_t != wat){th = FALSE; tu = FALSE;}//まだはじまらない
			if(s_p != wap){th = FALSE; tu = FALSE;}//まだはじまらない
		}

		if(putdly){
			no = dno;//dno はディレイがあったときのバックアップ
					while(putdly == TRUE){//ディレイあるときしかこないはず
						if(senote[ch].mode[no] == 6){//ディレイきた
							if(senote[ch].value[no] < wv_ch[och].dly){
								tu = TRUE;
								th = TRUE;
								seprg[ch] = dno;//そのときのTUNE群の先頭位置
								break;
							}
						}
						no++;
						if(senote[ch].mode[dno] < 2){tu = FALSE; th = TRUE;}
						if(senote[ch].time[no] != senote[ch].time[no - 1]){putdly = FALSE;}
						if(senote[ch].page[no] != senote[ch].page[no - 1]){putdly = FALSE;}
					}
		}

		if(th && tu){//tune変更
			putdly = FALSE;//dlyセットしたか
			dno = seprg[ch];
			if(wv_ch[och].dly == 0){//0なら
				while(tu == TRUE && th == TRUE){
					setuneup(ch, dno);

					dno++;
					if(senote[ch].mode[dno] < 2){tu = FALSE;}
					if(wat != senote[ch].time[dno]){th = FALSE;}
					if(wap != senote[ch].page[dno]){th = FALSE;}
					
				}
				if(wv_ch[och].dly > 0){//中でディレイがセットされたら
					tu = FALSE;
					th = FALSE;
				}
				if(putdly){th = FALSE;}
				seprg[ch] = dno;
				
			}
			else{
				while(tu == TRUE && th == TRUE){
					setuneup(ch, dno);
					if(wv_ch[och].dly > 0){//中でディレイがセットされたら
						putdly = TRUE;
					}

					dno++;
					if(senote[ch].mode[dno] < 2){tu = FALSE;}
					if(senote[ch].time[dno] != senote[ch].time[dno - 1]){th = FALSE;}
					if(senote[ch].page[dno] != senote[ch].page[dno - 1]){th = FALSE;}
				}
				seprg[ch] = dno;
			}
		}


		if(th && !tu){
//			flushcnt[ch] = wv_ch[ch].msec;
//			flushkey[ch] = wvnote[ch].value[prg[ch]] % 12;
			wv_ch[och].freq = onkai[senote[ch].value[seprg[ch]]];
			if(wvch_sel(och, wvbase_set(och))){
				wv_loop(och);
			}
	
//			vo = wvbase_set(ch);
//			seton = wvch_sel(ch, vo);
//			if(seton){
//				wv_loop(ch);
//			}
			seprg[ch]++;
		}
		if(senote[ch].mode[seprg[ch]] == 255){endflg[ch] = TRUE;}
	}

	if(endflg[0] && endflg[1]){
		seplay[0] = FALSE;
	}
	if(endflg[2] && endflg[3]){
		seplay[1] = FALSE;
	}
	
	if(se_time[0] == 239){
		se_page[0]++;
		
	}
	if(se_time[1] == 239){
		se_page[1]++;
		
	}
	se_time[0] = (se_time[0] + 1) % 240;
	se_time[1] = (se_time[1] + 1) % 240;

}


void se_drawold(void)
{
	WORD i, no, dno;
	BYTE ch, och;
	BYTE seton;
	BYTE wat = 0;
	BYTE wap = 0;
	BYTE s_t, s_p;
	BOOL th;
	BOOL tu;
	BOOL endflg[4] = {0, 0, 0, 0};//4つが1立つとDATA再生終わり
	BOOL putdly;
	short vo;

//	for(ch = 2 - seselch + seselch; ch < 4 - (seselch + seselch); ch++){
	for(ch = 0; ch < 4; ch++){
		och = ch + 12;
		th = TRUE;
		tu = FALSE;
		putdly = FALSE;
		seton = 0;
		
		wat = senote[ch].time[seprg[ch]];
		wap = senote[ch].page[seprg[ch]];

		if(senote[ch].mode[seprg[ch]] > 1){tu = TRUE;}//tuneだよ
		else if(!senote[ch].mode[seprg[ch]]){th = FALSE; tu = FALSE;}//NODATA

		if(wv_ch[ch].dly){//ディレイ値加算
			dno = research_note(ch, score_page, score_time);

			wap += (wat + wv_ch[och].dly) / 240;
			wat = (wat + wv_ch[och].dly) % 240;
			if(score_time != wat){th = FALSE; tu = FALSE;}//まだはじまらない
			if(score_page != wap){th = FALSE; tu = FALSE;}//まだはじまらない

			if(score_time == senote[ch].time[dno]){
				if(score_page == senote[ch].page[dno]){
					if(senote[ch].mode[dno] > 1){
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
		
/*		if(!tu){//tuneだったらWAIT無視
			i = 0;
			while(i < wv_ch[och].dly){
				no = research_senote(ch, wap, wat);
				if(senote[ch].mode[no] > 1){
					if((senote[ch].time[no] == wat) && (senote[ch].page[no] == wap)){
						seprg[ch] = no;
						wat = senote[ch].time[seprg[ch]];
						wap = senote[ch].page[seprg[ch]];
						return;
					}
				}
				i++;
				wap += (wat + 1) / 240;
				wat = (wat + 1) % 240;
			}

//			wap += (wat + wv_ch[ch].dly) / 240;
//			wat = (wat + wv_ch[ch].dly) % 240;
			
		}
*/
		if(ch < 2){
			s_t = se_time[0];
			s_p = se_page[0];
		}
		else{
			s_t = se_time[1];
			s_p = se_page[1];
		}
		if(s_t != wat){th = FALSE; tu = FALSE;}//まだはじまらない
		if(s_p != wap){th = FALSE; tu = FALSE;}//まだはじまらない

		if(th && tu){//変更
			while(tu == TRUE && th == TRUE){
				setuneup(ch, seprg[ch]);
//				wvch_sel(ch, wv_ch[ch].vol * (volume_m) * 32);
				seprg[ch]++;
				if(senote[ch].mode[seprg[ch]] < 2){tu = FALSE;}
//				if(score_time != wvnote[ch].time[prg[ch]]){tu = FALSE; th = FALSE;}
//				if(score_page != wvnote[ch].page[prg[ch]]){tu = FALSE; th = FALSE;}
				if(s_t != senote[ch].time[seprg[ch]]){th = FALSE;}
				if(s_p != senote[ch].page[seprg[ch]]){th = FALSE;}
			}
		}

		if(th && !tu){
//			flushcnt[ch] = wv_ch[ch].msec;
//			flushkey[ch] = wvnote[ch].value[prg[ch]] % 12;
			wv_ch[och].freq = onkai[senote[ch].value[seprg[ch]]];
			if(wvch_sel(och, wvbase_set(och))){
				wv_loop(och);
			}
	
//			vo = wvbase_set(ch);
//			seton = wvch_sel(ch, vo);
//			if(seton){
//				wv_loop(ch);
//			}
			seprg[ch]++;
		}
		if(senote[ch].mode[seprg[ch]] == 255){endflg[ch] = TRUE;}
	}

	if(endflg[0] && endflg[1]){
		seplay[0] = FALSE;
	}
	if(endflg[2] && endflg[3]){
		seplay[1] = FALSE;
	}
	
	if(se_time[0] == 239){
		se_page[0]++;
		
	}
	if(se_time[1] == 239){
		se_page[1]++;
		
	}
	se_time[0] = (se_time[0] + 1) % 240;
	se_time[1] = (se_time[1] + 1) % 240;

}


void seloadsuccess(void){
	WORD i, cntm[2];
	BYTE ch, och;
	BOOL def;
	
	if(!se_adcnt){//最初
		se_ad[0] = 0;//note
	}

	for(ch = 0; ch < 2; ch++){
		och = ch + 12;
		def = FALSE;
		for(i = 0; i < maxsenote; i++){
			if(def){//そのチャンネルは
				seb[ch].mode[i + se_ad[se_adcnt]] = 0;
				seb[ch].value[i + se_ad[se_adcnt]] = 0;
				seb[ch].time[i + se_ad[se_adcnt]] = 0;
				seb[ch].page[i + se_ad[se_adcnt]] = 0;
				break;
			}
			else{
				seb[ch].mode[i + se_ad[se_adcnt]] = senoton[ch].mode[i];
				seb[ch].value[i + se_ad[se_adcnt]] = senoton[ch].value[i];
				seb[ch].time[i + se_ad[se_adcnt]] = senoton[ch].time[i];
				seb[ch].page[i + se_ad[se_adcnt]] = senoton[ch].page[i];
				if(senoton[ch].mode[i] == 0){def = TRUE;}
			}
		}
		wv_ch[och].on = TRUE;
		cntm[ch] = i;
	}
	

	if(cntm[0] > cntm[1]){//大きい方のアドレスをセット
		i = cntm[0];
	}
	else{
		i = cntm[1];
	}
	
	se_adcnt++;
	se_ad[se_adcnt] = i + se_ad[se_adcnt - 1];
	
	if(se_ad[se_adcnt] > 8195){PostQuitMessage(0);}
}
/*
void seloadsuccess(void){
	WORD i;
	BYTE ch, och;
	BOOL def;
	
	for(ch = 0; ch < 4; ch++){
		och = ch + 12;
		def = FALSE;
		for(i = 0; i < maxsenote; i++){
			if(def){
				senote[ch].mode[i] = 0;
				senote[ch].value[i] = 0;
				senote[ch].time[i] = 0;
				senote[ch].page[i] = 0;
			}
			else{
				senote[ch].mode[i] = senoton[ch].mode[i];
				senote[ch].value[i] = senoton[ch].value[i];
				senote[ch].time[i] = senoton[ch].time[i];
				senote[ch].page[i] = senoton[ch].page[i];
			}
			if(!senoton[ch].mode[i]){def = TRUE;}
			wv_ch[och].on = TRUE;
		}
	}
}
*/
void set_sedata(BYTE no)
{
	WORD ad, i, soko;
	BYTE sech, ch, och;
	BOOL def;
	
	
	if(!no){return;}
	ad = se_ad[no - 1];
	
	soko = maxsenote;
	
	for(ch = 0; ch < 2; ch++){
		sech = ch + (seselch * 2);
		och = 12 + sech;//12～15chを使用
		def = FALSE;
		for(i = 0; i < se_ad[no] - ad; i++){
			if(def){
				senote[sech].mode[i] = 0;
				senote[sech].value[i] = 0;
				senote[sech].time[i] = 0;
				senote[sech].page[i] = 0;
//				if(!ch){break;}//最初はぬける
//				if(def &&(soko > i)){break;}
			}
			else{
				senote[sech].mode[i] = seb[ch].mode[ad + i];
				senote[sech].value[i] = seb[ch].value[ad + i];
				senote[sech].time[i] = seb[ch].time[ad + i];
				senote[sech].page[i] = seb[ch].page[ad + i];
			}
			
			if(!seb[ch].mode[ad + i]){def = TRUE;}
		}
		wv_ch[och].on = TRUE;
	}
	sech = seselch + seselch;
	
	seplay[seselch] = TRUE;
	se_time[seselch] = 0; se_page[seselch] = 0;
	seprg[sech] = 0; seprg[sech + 1] = 0;

//	seplay[1] = TRUE;
//	se_time[1] = 0; se_page[1] = 0;
//	seprg[2] = 0; seprg[3] = 0;
	
	seselch = (seselch + 1) % 2;//交互に入れる
}


void seloadbuffdel(void){
	WORD i;
	BYTE ch;
	
	for(ch = 0; ch < 2; ch++){
		for(i = 0; i < maxsenote; i++){
			senoton[ch].mode[i] = 0;
			senoton[ch].value[i] = 0;
			senoton[ch].time[i] = 0;
			senoton[ch].page[i] = 0;
		}
	}
}

void seload(BYTE L, BYTE op){

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
	
	BYTE dummy[32];
	BYTE dummy2[2];

////////////////name_entry//////////////
	strcpy(names, "SEDATA/");
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
		names[7 + i] = no[i];
	}
	strcpy(type, ".sed");//sound efect data

	for(; i < 7; i++){
		names[7 + i] = type[i - 3];
	}
	names[7 + i] = NULL;
	
	name = &(names[0]);
////////////////name_entry//////////////

	val = 0;
	hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FileSize = GetFileSize(hFile, NULL);

	if(FileSize == -1){
		MessageBox(NULL,"読み込み失敗。データがありません。", name,MB_OK);
		CloseHandle(hFile);
		return;
	}//読み失敗

//	ReadFile(hFile, &val, 1, &D, NULL);//dataNO
	ReadFile(hFile, &mas, 2, &D, NULL);//dataNO
	ReadFile(hFile, dummy, 32, &D, NULL);//dataNO
	ReadFile(hFile, dummy2, 2, &D, NULL);//dataNO
	ReadFile(hFile, dummy2, 2, &D, NULL);//dataNO
//	ReadFile(hFile, thisname, 32, &D, NULL);//dataNO
//	ReadFile(hFile, res_point, 2, &D, NULL);//dataNO
//	ReadFile(hFile, ret_point, 2, &D, NULL);//dataNO
	if(!op){//edit mode
		if((mas != mastercode) && !mastermode){//userがmasterのファイルをひらいた、逆にmasterがuserファイルを開ける
			MessageBox(NULL,"このファイルは開けません。", name,MB_OK);
			CloseHandle(hFile);
			return;
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
		for(i = 0; i < maxsenote; i++){
			ReadFile(hFile, &senoton[ch].mode[i], 1, &D, NULL);
			ReadFile(hFile, &senoton[ch].value[i], 1, &D, NULL);
			ReadFile(hFile, &senoton[ch].time[i], 1, &D, NULL);
			ReadFile(hFile, &senoton[ch].page[i], 1, &D, NULL);
			val = (val + senoton[ch].value[i]) % 256;
			if(!senoton[ch].mode[i]){break;}
//			if(wvnoton[ch].time[i] == 0x32 && wvnoton[ch].page[i] == 0x04){break;}
//			if(wvnoton[ch].time[i] == 0x34 && wvnoton[ch].page[i] == 0x04){break;}
		}
		
	}
	val = (val + dummy[0] + dummy[1] + mastercode) % 256;//VALUE加算
	ReadFile(hFile, &rval, 1, &D, NULL);//dataNO//*WORD
//	if(rval == val || 1){
	if(rval == val || mastermode){
		seloadsuccess();
		if(mess){
			MessageBox(NULL,"読み込み。", name,MB_OK);
		}
	}
	else if(!mastermode){
		MessageBox(NULL,"データが壊れています。", name,MB_OK);
	}
	seloadbuffdel();
	
	start_sepoint[0] = 0;
	start_sepoint[1] = 0;
	end_sepoint[0] = 0;
	end_sepoint[1] = 0;
	
	CloseHandle(hFile);

}

void senameload(WORD L, BYTE nameb)//名前だけ
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
	strcpy(names, "SEDATA/");
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
		names[7 + i] = no[i];
	}
	strcpy(type, ".sed");//background mugic data

	for(; i < 7; i++){
		names[7 + i] = type[i - 3];
	}
	names[7 + i] = NULL;
	
	name = &(names[0]);
////////////////name_entry//////////////

	val = 0;
	hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FileSize = GetFileSize(hFile, NULL);

	if(FileSize == -1){
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

/*
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
*/
/*
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
*/
/*
void wavefmt_close(void)//ENDSTOP
{
	CloseHandle(wvfh);

	rectrig = FALSE;
	splay = FALSE;

	wavefmt_save(1);
	
}
*/
