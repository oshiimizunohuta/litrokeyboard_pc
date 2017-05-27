//世界とかドウクツ・町のエリア設定

BYTE zoneno;
BYTE zonename[24];

struct areamapcels{
//	BYTE areafild[3];//{xMas, yMas, zMas};//区ごとの広さ
//	BYTE arange; //そのエリアの広さ
	BYTE mapchs[8]; //8重マップ
	BYTE event[8]; //イベントタイプ
	BYTE ebo[16][4];//enemybox
} area[8][8][8];//

BYTE zonebgm = 0;//BGM
BYTE zonebgg = 0;//背景

BYTE nowarea[4] = {0, 0, 0, 0};//場所 地名,階層,北番,西番

struct s_message{
	BYTE w[256];//ページ*ワード//255文字まで可能？
	BYTE waittype;
	BYTE who[16];
//	WORD change_f[16];
	BYTE change_f[16];//lineごとのface管理
	BYTE change_b[16];//lineごとにBGM変更
//	BYTE color_type[4];//カラー
//	BYTE color_point[4][2];//カラーはじめる箇所終わる箇所
} msgdata[256];//64*8pageまで

BYTE startmessage = 0;
BYTE messagecnt = 0;//文字とページ用;
BYTE blmessagecnt = 0;//送り用;
BYTE linecnt = 0;//比較用ライン;
BYTE messagewait = 1;//メッセージウェイト
BYTE messagewtcnt = 0; //メッセージウェイトカウント
BYTE mescolorcnt = 0; //メッセージカラーカウント
BYTE msgscrcnt = 0;//メッセージスクロールカウント
BYTE msgslide = 0;
BYTE facepoint = 0;


void msgsyoki(void){
	BYTE i, j;
	for(i = 0; i < 64; i++){
		for(j = 0; j < 255; j++){
			msgdata[i].w[j] = 0;
		}
		msgdata[i].w[j] = 0;
		msgdata[i].waittype = 255;//nostop
		for(j = 0; j < 16; j++){
			msgdata[i].who[j] = 0;
			msgdata[i].change_f[j] = 0;
		}
	}
}

void areaload(BYTE X, BYTE Y, BYTE Z)//(8chmap)エリア読み込み
{
	BYTE ch, i;
	BYTE MAPNO;
	for(ch = 0; ch < 8; ch++){
		MAPNO = area[X][Y][Z].mapchs[ch];//loadmapNo
//		if(MAPNO){
	//		mapload(ch, MAPNO, 1);
			maploadZ(ch, MAPNO, 1);
//		}
	}

	delall_ene();//delete all
	
	for(i = 0; i < 16; i++){//eneboxset
		if(area[nowarea[1]][nowarea[2]][nowarea[3]].ebo[i][0]){//LEVELあり
			set_enemybox(i, 0, 0);
//			stockebo_01(i, 0);
		}
	}

}

void zoneload(BYTE L, BYTE debug)//locate, gamemode, 
{
////////////////name_entry//////////////

	char no[2] = {'0', '0'};
	char type[4];
	char names[16];
	char *name;
	
	HANDLE hFile;
	DWORD D = 0;
	OPENFILENAME ofn;
	DWORD FileSize;
	
	BYTE zerocount;
	BYTE i, j, k, num, val = 0;
	BYTE ch0, ret;//0が何回？
	strcpy(names, "ZONEDATA/");

	for(i = 0; i < 2; i++){//2桁読む
		switch(i){//各桁ごとの数字
			case 0:	val = (L) / 10;break;//locate 2桁目
			case 1:	val = (L) % 10;break;//locate 1桁目
			default: break;
		}
		
		switch(val){//数値から文字データへ
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

	for(i = 0; i < 2; i++){//"MAPDATA/" + "XXX"
		names[9 + i] = no[i];//番号
	}

	strcpy(type, ".zdt");//zoneデータ
	for(; i < 6; i++){//4文字
		names[9 + i] = type[i - 2];//iは2から始まる
	}
	
	names[9 + i] = NULL;//end [15]
	name = &(names[0]);

	hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FileSize = GetFileSize(hFile, NULL);
	
	if(FileSize + 1 == 0){
		MessageBox(NULL,"読み込み失敗。AREADAEAがありません。", name,MB_OK);
		CloseHandle(hFile);
		return;
	}//読み失敗

	ReadFile(hFile, &zoneno, 1, &D, NULL);//ZONE No
	ReadFile(hFile, zonename, 24, &D, NULL);//ZONE Name
	ReadFile(hFile, &zonebgm, 1, &D, NULL);//ZONE No
	ReadFile(hFile, &zonebgg, 1, &D, NULL);//ZONE Name
	
	zerocount = 0;
	for(i = 0; i < 8; i++){
	for(j = 0; j < 8; j++){
	for(k = 0; k < 8; k++){
		if(!zerocount){
			ReadFile(hFile, area[k][j][i].mapchs, 2, &D, NULL);//連続2BYTE取り込み
			zerocount = area[k][j][i].mapchs[1];//2番目には0を繰り返す数がはいっている。1番目にデータがあるときはクリアされるので安心
		}
		if(zerocount){
			zerocount--;
		}
		if(area[k][j][i].mapchs[0]){//データあり
			ReadFile(hFile, area[k][j][i].mapchs + 2, 6, &D, NULL);//残りの6バイト取り込み
			ReadFile(hFile, area[k][j][i].event, 8, &D, NULL);
			for(num = 0; num < 16; num++){
				ReadFile(hFile, area[k][j][i].ebo[num], 4, &D, NULL);//敵データ
			}
			zerocount = 0;//
			if(debug == 1){//MAPEDITOR用
				nowarea[1] = k;
				nowarea[2] = j;
				nowarea[3] = i;
			}
		}
		else{//データなし
//			if(!zerocount){
//				zerocount = area[k][j][i].mapchs[1];//2番目には0を繰り返す数がはいっている
//			}
			for(num = 0; num < 8; num++){
				area[k][j][i].mapchs[num] = 0;//このエリアはすべて0
				area[k][j][i].event[num] = 0;
				area[k][j][i].ebo[num][0] = 0;
				area[k][j][i].ebo[num][1] = 0;
				area[k][j][i].ebo[num][2] = 0;
				area[k][j][i].ebo[num][3] = 0;
				area[k][j][i].ebo[num + 8][0] = 0;
				area[k][j][i].ebo[num + 8][1] = 0;
				area[k][j][i].ebo[num + 8][2] = 0;
				area[k][j][i].ebo[num + 8][3] = 0;
			}
		}
	}
	}
	}

	CloseHandle(hFile);
		
	D = 0;
//	MessageBox(NULL,"読み込み失敗。MAPDATAがありません。", zonename,MB_OK);

	areaload(nowarea[1], nowarea[2], nowarea[3]);
//	MessageBox(NULL,"ZONEdata読み込み完了だ",name ,MB_OK);
}

void zonesave(BYTE L)
{
////////////////name_entry//////////////
	HANDLE hFile;
	DWORD D;
	OPENFILENAME ofn;
	DWORD FileSize;
	
	BYTE i, j, k, num, val = 0;
	BYTE zerocount;
	char no[2] = {'0', '0'};
	char type[4];
	char names[16];
	char *name;
	strcpy(names, "ZONEDATA/");

	for(i = 0; i < 2; i++){//2桁読む
		switch(i){//各桁ごとの数字
			case 0:	val = (L) / 10;break;//locate 2桁目
			case 1:	val = (L) % 10;break;//locate 1桁目
			default: break;
		}
		
		switch(val){//数値から文字データへ
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

	for(i = 0; i < 2; i++){//"MAPDATA/" + "XXX"
		names[9 + i] = no[i];//番号
	}

	strcpy(type, ".zdt");//zoneデータ
	for(; i < 6; i++){//4文字
		names[9 + i] = type[i - 2];//iは2から始まる
	}
	
	names[9 + i] = NULL;//end [15]
	name = &(names[0]);

	hFile = CreateFile(name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	
	WriteFile(hFile, &L, 1, &D, NULL);//ZONE No
	WriteFile(hFile, zonename, 24, &D, NULL);//ZONE Name
	WriteFile(hFile, &zonebgm, 1, &D, NULL);//ZONE BGMu
	WriteFile(hFile, &zonebgg, 1, &D, NULL);//ZONE BGGr
	
	zerocount = 0;
	for(i = 0; i < 8; i++){
	for(j = 0; j < 8; j++){
	for(k = 0; k < 8; k++){
		if(area[k][j][i].mapchs[0]){
			if(zerocount){
				WriteFile(hFile, &zerocount, 1, &D, NULL);//いくつあったか？
			}
			zerocount = 0;//

			WriteFile(hFile, area[k][j][i].mapchs, 8, &D, NULL);//チャンネル:8BYTE
			WriteFile(hFile, area[k][j][i].event, 8, &D, NULL);//イベント:8BYTE
			
			for(num = 0; num < 16; num++){
				WriteFile(hFile, area[k][j][i].ebo[num], 4, &D, NULL);//敵データ
			}
		}
		else{
			if(!zerocount){
				WriteFile(hFile, &area[k][j][i].mapchs[0], 1, &D, NULL);
			}
			
			zerocount++;//空白データカウント
			if(zerocount == 255){
				MessageBox(NULL,"255ERROR確認?", name,MB_OK);
				WriteFile(hFile, &zerocount, 1, &D, NULL);//いくつあったか？
				zerocount = 0;
			}
			else if((i == 7) && (j == 7) && (k == 7)){
				MessageBox(NULL,"end処理確認", name,MB_OK);
				WriteFile(hFile, &zerocount, 1, &D, NULL);//いくつあったか？
				zerocount = 0;
			}

		}
	}
	}
	}
	
			
	CloseHandle(hFile);

	MessageBox(NULL,"ZONEdata書き出し完了だ", name,MB_OK);
}

void messageload(BYTE L){
////////////////name_entry//////////////
	HANDLE hFile;
	DWORD D;
	OPENFILENAME ofn;
	DWORD FileSize;
	
	BYTE zerocount;
	WORD i, j, k, val = 0;
	BYTE num;
	BYTE ch0, ret;//0が何回？
	BYTE kiokumsg[3] = {0, 0, 0};//違うデータ入れとく
	BYTE msgdatano;
	WORD recnt = 0;
	
	char no[2] = {'0', '0'};
	char type[4];
	char names[16];
	char *name;
	strcpy(names, "MSGDATA/");

	for(i = 0; i < 2; i++){//2桁読む
		switch(i){//各桁ごとの数字
			case 0:	val = (L) / 10;break;//locate 2桁目
			case 1:	val = (L) % 10;break;//locate 1桁目
			default: break;
		}
		
		switch(val){//数値から文字データへ
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

	for(i = 0; i < 2; i++){//"MAPDATA/" + "XXX"
		names[8 + i] = no[i];//番号
	}

	strcpy(type, ".msg");//zoneデータ
	for(; i < 6; i++){//4文字
		names[8 + i] = type[i - 2];//iは2から始まる
	}
	
	names[8 + i] = NULL;//end [15]
	name = &(names[0]);

	hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FileSize = GetFileSize(hFile, NULL);
	
	if(FileSize + 1 == 0){
		MessageBox(NULL,"読み込み失敗。MSGDAEAがありません。", name,MB_OK);
		CloseHandle(hFile);
		return;
	}//読み失敗

	ReadFile(hFile, &msgdatano, 1, &D, NULL);//msgdatano

	for(i = 0; i < 256; i++){
//		kiokumsg[0] = 255 - kiokumsg[1];//違うデータを入れとく
		kiokumsg[0] = 0;//違うデータを入れとく
		kiokumsg[1] = 1;//違うデータを入れとく
		for(j = 0; j < 256; j++){
			if(kiokumsg[0] != kiokumsg[1]){//前回のコピーが残ってるか、初期のばやい
				ReadFile(hFile, &kiokumsg[0], 1, &D, NULL);//kioku1
			}
			ReadFile(hFile, &kiokumsg[1], 1, &D, NULL);//kioku2
			if(kiokumsg[0] == kiokumsg[1]){//比較
				ReadFile(hFile, &recnt, 1, &D, NULL);//繰り返しカウント
				msgdata[i].w[j] = kiokumsg[0];
//				while(recnt){
				for(; recnt > 0; recnt--){
					j++;
					msgdata[i].w[j] = kiokumsg[0];
					if(j == 255){break;}
					if(recnt == 1){j--;}
				}
//				msgdata[i].w[j] = kiokumsg[0];//不足分
//				j--;//これも
				kiokumsg[0] = 255 - kiokumsg[1];//違うデータを入れとく
			}
			else{//違う場合
				msgdata[i].w[j] = kiokumsg[0];
				kiokumsg[0] = kiokumsg[1];
			}
		}
		kiokumsg[0] = 0;
		kiokumsg[1] = 0;
		
		for(j = 0; j < 16; j++){
			ReadFile(hFile, &kiokumsg[0], 1, &D, NULL);//kao
			if(kiokumsg[0]){
				msgdata[i].who[j] = kiokumsg[0];
			}
			else{//0だったら
//				ReadFile(hFile, &kiokumsg[1], j + 1, &D, NULL);//kao
				ReadFile(hFile, msgdata[i].change_f, j + 1, &D, NULL);//kao
				j++;
//				kiokumsg[1] = j;
//				j = 0;
				while(j != 16){
//				while(0){
					msgdata[i].who[j] = kiokumsg[0];
					msgdata[i].change_f[j] = kiokumsg[0];
					j++;
					kiokumsg[1]--;
					if(!kiokumsg[1]){break;}
				}
				break;
			}
		}
		ReadFile(hFile, &msgdata[i].waittype, 1, &D, NULL);//waittype
		ReadFile(hFile, msgdata[i].change_b, 16, &D, NULL);//waittype
	}
	ReadFile(hFile, &val, 2, &D, NULL);//dataNO

	CloseHandle(hFile);

}

void messagesave(BYTE L){

	char no[3] = {'0', '0', '0'};
	char type[4];
	char names[15];
	char back[4];
	char *name;

	HANDLE hFile;
	OPENFILENAME ofn;
	DWORD D;
	DWORD FileSize;

	WORD i, j, k, val = 0;
	BYTE zerocount;

////////////////name_entry//////////////
	strcpy(names, "MSGDATA/");
	strcpy(back, "bak/");

	for(i = 0; i < 2; i++){
		switch(i){
			case 0:	val = (L) / 10;break;
			case 1:	val = (L) % 10;break;
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

	
	for(i =0; i < 2; i++){
		names[8 + i] = no[i];
	}
	strcpy(type, ".msg");

	for(; i < 6; i++){
		names[8 + i] = type[i - 2];
	}
	
	name = &(names[0]);
////////////////name_entry//////////////

	val = L;
	hFile = CreateFile(name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//	FileSize = GetFileSize(hFile, NULL);

	WriteFile(hFile, &val, 1, &D, NULL);//dataNO
	for(i = 0; i < 256; i++){
		zerocount = 0;
		for(j = 0; j < 256; j++){
//			if(msgdata[i].w[j] == 255){//endmsg
//				break;
//			}
			if(msgdata[i].w[j] != msgdata[i].w[j - zerocount]){//同データ終わり
				if(zerocount > 1){//あったら
					WriteFile(hFile, &zerocount, 1, &D, NULL);//いくつあったか？
				}
				WriteFile(hFile, &msgdata[i].w[j], 1, &D, NULL);//違っていたデータいれ
				zerocount = 1;//0では食い違う
			}
			else{//同じデータ
				if(zerocount < 2){
					WriteFile(hFile, &msgdata[i].w[j], 1, &D, NULL);//もう一回１BYTE書き((A, A) * zerocount)
				}
				zerocount++;//
			}
//			WriteFile(hFile, &msgdata[i].w[j], 1, &D, NULL);//１BYTE書き
			
		}
		if(zerocount > 1){//あったら
			zerocount--;
			WriteFile(hFile, &zerocount, 1, &D, NULL);//いくつあったか？
		}
		if(zerocount == 0){//あったら0だけ
			zerocount = 255;
			WriteFile(hFile, &zerocount, 1, &D, NULL);//255？
			zerocount = 0;
		}
//		zerocount = 255;
//		WriteFile(hFile, &zerocount, 1, &D, NULL);//endfile

		for(j = 0; j < 16; j++){
//			if(msgdata[i].who[j]){
				WriteFile(hFile, &msgdata[i].who[j], 1, &D, NULL);//face
//				zerocount = 1;
//			}
//			else{
			if(!msgdata[i].who[j]){
//				WriteFile(hFile, &msgdata[i].who[j], 1, &D, NULL);//face=00
				j++;
				break;//おわり
			}
		}
//		val = j;//回数を記憶
		if(j){
//			WriteFile(hFile, &msgdata[i].change_f, j * 2, &D, NULL);//face
			WriteFile(hFile, msgdata[i].change_f, j, &D, NULL);//face
		}
		WriteFile(hFile, &msgdata[i].waittype, 1, &D, NULL);//waittype
		WriteFile(hFile, msgdata[i].change_b, 16, &D, NULL);//waittype
	}
	val = 256 - L + msgdata[0].w[L];//逆数にdataNo分先の文字
	WriteFile(hFile, &val, 2, &D, NULL);//dataNO//*WORD
	CloseHandle(hFile);
	MessageBox(NULL,"書き込み。", name,MB_OK);

}


void wldsyoki(void){
	BYTE i, j, k;
	BYTE mch;
	
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			for(k = 0; k < 8; k++){
//				area[k][j][i].arange = 0;///??
//				area[k][j][i].areafild[0] = 0;
//				area[k][j][i].areafild[1] = 0;
//				area[k][j][i].areafild[2] = 0;

				for(mch = 0; mch < 8; mch++){
					area[k][j][i].mapchs[mch] = 0;//ch
					area[k][j][i].event[mch] = 0;
					area[k][j][i].ebo[0][mch] = 0;//LV
					area[k][j][i].ebo[1][mch] = 0;//X
					area[k][j][i].ebo[2][mch] = 0;//Y
					area[k][j][i].ebo[3][mch] = 0;//Z
					area[k][j][i].ebo[0][mch + 8] = 0;
					area[k][j][i].ebo[1][mch + 8] = 0;
					area[k][j][i].ebo[2][mch + 8] = 0;
					area[k][j][i].ebo[3][mch + 8] = 0;
				}
			}
		}
	}
	for(i = 0; i < 32; i++){
		zonename[i] = 0;
	}
}

void TREASURE_001(void)
{
	
}
//[0]初期宝、[1]施設とか、[2]動的なやつ、[3][4][5][6][7]
void event_set(void)
{
	BYTE i;
	switch(area[nowarea[1]][nowarea[2]][nowarea[3]].event[0]){
		case 1: TREASURE_001();break;
		default: break;
	}

}
