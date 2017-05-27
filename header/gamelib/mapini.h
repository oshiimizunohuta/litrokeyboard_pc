struct env_m{
	int gravit;//ﾏｽの重力
	BYTE height[32][32];
	BYTE htype;
	BOOL touka;

} mapType[8][64];

BYTE p_gra = 0x10;

struct env_g{
	BYTE gnd[32][32];
	
} g_height[64];

struct mapmemo{//透過用のマップを記憶
	BYTE chip_no;//記憶チップ
	WORD Xpos;//記憶ポジション
	WORD Ypos;//記憶ポジション
	WORD Zpos;//記憶ポジション
} mapmem[128];

struct mapwrite{//置いたマップを記憶
	BOOL arimasuka;//立体マップチップ置いた？
	WORD Zpos;//記憶チップ
} mapw[10][192];//MAPno:10, XYcamera

struct MAPDATA{
	WORD Xmas;
	WORD Ymas;
	
	WORD Xpos;
	WORD Ypos;
	WORD Zpos;
	
	WORD XCpos;//０～３１？
	WORD YCpos;//ﾃﾞﾊﾞｺﾝ用
	WORD ZCpos;
	
	BYTE hirosa[2];
//	BYTE yuka_y;
	
	BYTE Put[65536];
	BYTE Info[1024];
	
	BYTE No;
	BYTE palette;
	char *name;

} BGMAP;

BYTE LINEMAP[2097152];

BYTE mapdraw = 0;


struct MAP128{
	BYTE Xmas;
	BYTE Ymas;
	BYTE Zmas;
	
	WORD Xpos;//ベースマップに対してのXポジション
	WORD Ypos;
	WORD Zpos;

	WORD XCpos;
	WORD YCpos;
	WORD ZCpos;

	BYTE Put[128][16384];//128*128*128
	BOOL saikakiko[10][192];
	BOOL saikakikom[192];
	BOOL through[192];//マップ透過描画予約位置

//	BYTE Info[1024];
	BYTE hirosa[3]; //x, y, z
	
	BYTE palette;//chipの種類
	
	BYTE No;
} MAP128[16];

//BOOL saikakikom[192];

BYTE m_sort[128];//map表示順
BYTE m_sousu = 0;//透過マップ表示総数

BYTE backXmas;//変化利のとき
BYTE backYmas;

BOOL mapch[8] = {1, 0, 0, 0, 0, 0, 0, 0};//MAPチャンネル　8765 4321
BYTE nowch = 0;//現在編集中
BYTE PASS = 86;


void maplinecopy(BYTE type)//直線データで並べる
{
	DWORD t;
	BYTE i, j, k;

	t = 0;
//	maxdata = MAP128[nowch].hirosa[0] * MAP128[nowch].hirosa[1] * MAP128[nowch].hirosa[2];

	if(type == 0){//squermap
		for(j = 0; j < BGMAP.hirosa[1]; j++){
			for(k = 0; k < BGMAP.hirosa[0]; k++){
					LINEMAP[t++] = BGMAP.Put[k + (j * BGMAP.hirosa[0])];
			}
		}
	}
	else if(type == 1){//cubemap
		for(i = 0; i < MAP128[nowch].hirosa[2]; i++){//直線データに入れる
			for(j = 0; j < MAP128[nowch].hirosa[1]; j++){
				for(k = 0; k < MAP128[nowch].hirosa[0]; k++){
					LINEMAP[t++] = MAP128[nowch].Put[i][k + (j * MAP128[nowch].hirosa[0])];
				}
			}
		}
	}
}

void mapreversecopy(BYTE ch, BYTE type)//直線データをもどす
{
	DWORD t;
	BYTE i, j, k;

	t = 0;
	if(type == 0){//squermap
		for(j = 0; j < BGMAP.hirosa[1]; j++){
			for(k = 0; k < BGMAP.hirosa[0]; k++){
					BGMAP.Put[k + (j * BGMAP.hirosa[0])] = LINEMAP[t++];
			}
		}
	}
	else if(type == 1){//cubemap
		for(i = 0; i < MAP128[ch].hirosa[2]; i++){//直線データに入れる
			for(j = 0; j < MAP128[ch].hirosa[1]; j++){
				for(k = 0; k < MAP128[ch].hirosa[0]; k++){
					MAP128[ch].Put[i][k + (j * MAP128[ch].hirosa[0])] = LINEMAP[t++];
				}
			}
		}
	}
}

void maploadZ(BYTE mapchanel, WORD mapNo, BOOL sub_m)//格納MAPch, mapno, sab or bg?指定 //ZIP!!
{
	char no[3] = {'0', '0', '0'};
	char type[4];
	char names[16];
	char *name;

	HANDLE hFile;
	OPENFILENAME ofn;
	DWORD D;
	DWORD FileSize;
	DWORD ij, maxdata;
	
	BYTE i, j, val = 0;
	BYTE retrycount = 0, backcount = 0;
	BYTE kiokumap[8] = {0, 0, 0, 0, 0, 0, 0, 0};

////////////////name_entry//////////////
	strcpy(names, "MAPDATA/");

	for(i = 0; i < 3; i++){//3桁読む		
		switch(i){//各桁ごとの数字
			case 0:	val = (mapNo) / 100;break;//mapは001から始まる
			case 1:	val = ((mapNo / 10) - (val * 10));break;
			case 2:	val = (mapNo) % 10;break;
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

	switch(sub_m){//メインかサブか
		case 0: strcpy(type, ".mmp"); break;
		case 1: strcpy(type, ".smp"); break;
		default: strcpy(type, ".err"); break;//最後がstrcpyだと余計な文字が消える。

	}
	
	for(i = 0; i < 3; i++){//"MAPDATA/" + "XXX"
		names[8 + i] = no[i];
	}

	for(; i < 7; i++){
		names[8 + i] = type[i - 3];
	}
	
	names[15] = NULL;//end
	name = &(names[0]);
	
////////////////name_entry//////////////
	
	hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FileSize = GetFileSize(hFile, NULL);
	
	if((long)FileSize == -1){
		MessageBox(NULL,"読み込み失敗。MAPDATAがありません。", name,MB_OK);
		CloseHandle(hFile);
	}//読み失敗

	else{//本読み込み

		ReadFile(hFile, &PASS, 1, &D, NULL);
		if(PASS != 86){
			MessageBox(NULL,"FILEPASSがちがいます。読めないよ。", name,MB_OK);
			CloseHandle(hFile);
			PASS = 86;
			return;
		}

		if(sub_m == 0){//main
			ReadFile(hFile, &BGMAP.palette, 1, &D, NULL);//&D=2まで
			ReadFile(hFile, &BGMAP.hirosa, 2, &D, NULL);//&D=2まで
			maxdata = BGMAP.hirosa[0] * BGMAP.hirosa[1];
		}
		else{
			ReadFile(hFile, &MAP128[mapchanel].palette, 1, &D, NULL);
			ReadFile(hFile, MAP128[mapchanel].hirosa, 3, &D, NULL);
			maxdata = MAP128[mapchanel].hirosa[0] * MAP128[mapchanel].hirosa[1] * MAP128[mapchanel].hirosa[2];
		}
			
		retrycount = 0;
		ij = 0;
		while(ij != maxdata){
			if(!retrycount){
				ReadFile(hFile, &LINEMAP[ij], 1, &D, NULL);//&D=0(読み込んだアドレス)
				if(LINEMAP[ij] > 127){
					backcount = LINEMAP[ij] - 127;
					ReadFile(hFile, &retrycount, 1, &D, NULL);//繰り返し回数
					retrycount--;
					for(j = 0; j < backcount; j++){//128から136
						kiokumap[j] = LINEMAP[ij - backcount + j];//コピーするパターンを記憶
					}
				}
				else{ij++;}
			}
			else if(retrycount){
				retrycount--;
				for(j = 0; j < backcount; j++){//128から136
					LINEMAP[ij] = kiokumap[j];//パターンをコピーする
					ij++;
					if(ij == maxdata){break;}
				}
			}
			if(ij == maxdata){break;}
		}
		if(sub_m == 0){//main
			mapreversecopy(mapchanel, 0);
//			mos = FALSE;
		}
		else{
			mapreversecopy(mapchanel, 1);
//			mos = TRUE;
			MAP128[mapchanel].Xpos = 0;
			MAP128[mapchanel].Ypos = (MAP128[mapchanel].hirosa[2]) * 32;
			MAP128[mapchanel].Zpos = 0;
		}

//		wsprintf(box_ms,"現在の安定度 ＝ %d, CPU Power = %7d %d", st, cp, FileSize);
//		MessageBox(NULL,"MAPdata読み込み完了だ", name,MB_OK);
		//読み成功
		CloseHandle(hFile);
	}
}


void pic_syoki(void){
	WORD i;
	for(i = 0; i < 64; i++){
		switch(i){
//			case 8:
//			case 9:
//			case 10:
//			case 21:
///			case 22:
//			case 27:
			case 28:mapType[0][i].touka = TRUE; break;
//			case 40:
//			case 41:
//			case 42:
//			case 43:
//			case 63: mapType[0][i].touka = TRUE; break;
			default:mapType[0][i].touka = FALSE; break;
		}
	}
}

void stmp_t_yama01(BYTE mapchannel)
{
	mapType[0][0].htype = 0;
	mapType[0][1].htype = 1;//床
	mapType[0][2].htype = 1;
	mapType[0][3].htype = 1;
	mapType[0][4].htype = 1;
	mapType[0][5].htype = 1;
	mapType[0][6].htype = 1;
	mapType[0][7].htype = 1;
		
	mapType[0][8].htype = 1;
	mapType[0][9].htype = 1;
	mapType[0][10].htype = 1;
	mapType[0][11].htype = 1;
	mapType[0][12].htype = 1;
	mapType[0][13].htype = 2;//ブロック
	mapType[0][14].htype = 2;
	mapType[0][15].htype = 2;
	
	mapType[0][16].htype = 1;
	mapType[0][17].htype = 1;
	mapType[0][18].htype = 1;
	mapType[0][19].htype = 1;
	mapType[0][20].htype = 1;
	mapType[0][21].htype = 2;//ブロック米
	mapType[0][22].htype = 2;
	mapType[0][23].htype = 2;

	mapType[0][24].htype = 1;//ブロック米
	mapType[0][25].htype = 1;
	mapType[0][26].htype = 1;
	mapType[0][27].htype = 2;//ブロック米
	mapType[0][28].htype = 2;
	mapType[0][29].htype = 2;
	mapType[0][30].htype = 2;
	mapType[0][31].htype = 2;

	mapType[0][32].htype = 2;//ブロック米
	mapType[0][33].htype = 2;
	mapType[0][34].htype = 2;
	mapType[0][35].htype = 2;
	mapType[0][36].htype = 0;
	mapType[0][37].htype = 0;
	mapType[0][38].htype = 0;
	mapType[0][39].htype = 0;

	mapType[0][40].htype = 0;////階段Ｖ４
	mapType[0][41].htype = 0;////階段Ｖ４
	mapType[0][42].htype = 0;////階段Ｖ４
	mapType[0][43].htype = 0;////階段Ｖ４
	mapType[0][44].htype = 2;////階段Ｖ４
//	mapType[0][44].htype = 7;////階段Ｖ４
//	mapType[0][45].htype = 2;////階段Ｖ４
	mapType[0][47].htype = 2;////階段Ｖ４
//	mapType[0][47].htype = 6;////階段Ｖ４

	mapType[0][48].htype = 3;//斜面"／"下半
	mapType[0][49].htype = 4;//斜面"／"上半
	mapType[0][50].htype = 5;//斜面"＼"上半
	mapType[0][51].htype = 6;//斜面"＼"下半
	mapType[0][52].htype = 9;//斜面"Λ"下半
	mapType[0][53].htype = 10;//斜面"Λ"上半
	mapType[0][54].htype = 1;//ダミー斜面
	mapType[0][55].htype = 2;////
	
	mapType[0][56].htype = 3;//斜面"／"下半
	mapType[0][57].htype = 4;//斜面"／"上半
	mapType[0][58].htype = 5;//斜面"＼"上半
	mapType[0][59].htype = 6;//斜面"＼"下半
	mapType[0][60].htype = 7;//斜面"Ｖ"下半
	mapType[0][61].htype = 8;//斜面"Ｖ"上半
	mapType[0][62].htype = 1;//ダミー斜面
	mapType[0][63].htype = 2;////

	mapType[0][0].gravit = p_gra;

	
}
	
	
void sethtype(void)
{
	mapType[0][0].htype = 0;
	mapType[0][1].htype = 1;
	mapType[0][2].htype = 1;
	mapType[0][3].htype = 1;
	mapType[0][4].htype = 1;
	mapType[0][5].htype = 1;
	mapType[0][6].htype = 1;
	mapType[0][7].htype = 1;
	
	mapType[0][8].htype = 2;
	mapType[0][9].htype = 2;
	mapType[0][10].htype = 2;
	mapType[0][11].htype = 2;
	mapType[0][12].htype = 2;
	mapType[0][13].htype = 2;
	mapType[0][14].htype = 2;
	mapType[0][15].htype = 2;
//	mapType[0][16].htype = 1;
//	mapType[0][17].htype = 3;//斜面"／"下半
//	mapType[0][18].htype = 4;//斜面"／"上半
//	mapType[0][19].htype = 5;//斜面"＼"上半
	mapType[0][20].htype = 2;
//	mapType[0][20].htype = 6;//斜面"＼"下半
	mapType[0][23].htype = 2;//ブロック米
//	mapType[0][24].htype = 2;//ブロック米
//	mapType[0][25].htype = 2;//ブロック米
	mapType[0][27].htype = 2;//ブロック米
	mapType[0][28].htype = 2;//ブロック米
	mapType[0][31].htype = 2;//ブロック米
//	mapType[0][32].htype = 40;//階段Ｖ１
//	mapType[0][33].htype = 41;////階段Ｖ２
//	mapType[0][34].htype = 42;////階段Ｖ３
	mapType[0][35].htype = 2;////階段Ｖ４
//	mapType[0][35].htype = 43;////階段Ｖ４
	mapType[0][36].htype = 2;////階段Ｖ４
//	mapType[0][36].htype = 44;////階段Ｖ４
//	mapType[0][37].htype = 45;////階段Ｖ４
//	mapType[0][38].htype = 45;////階段Ｖ４
	mapType[0][39].htype = 2;////階段Ｖ４
//	mapType[0][39].htype = 45;////階段Ｖ４
	mapType[0][40].htype = 0;////階段Ｖ４
	mapType[0][41].htype = 0;////階段Ｖ４
	mapType[0][42].htype = 5;////階段Ｖ４
	mapType[0][43].htype = 6;////階段Ｖ４
	mapType[0][44].htype = 2;////階段Ｖ４
//	mapType[0][44].htype = 7;////階段Ｖ４
//	mapType[0][45].htype = 2;////階段Ｖ４
	mapType[0][47].htype = 2;////階段Ｖ４
//	mapType[0][47].htype = 6;////階段Ｖ４
	mapType[0][48].htype = 3;////階段Ｖ４
	mapType[0][49].htype = 4;////階段Ｖ４
	mapType[0][50].htype = 5;////階段Ｖ４
	mapType[0][51].htype = 6;////階段Ｖ４
	mapType[0][52].htype = 2;////階段Ｖ４
	mapType[0][53].htype = 2;////階段Ｖ４
	mapType[0][54].htype = 2;////階段Ｖ４
	mapType[0][55].htype = 2;////階段Ｖ４
	mapType[0][56].htype = 3;////階段Ｖ４
	mapType[0][57].htype = 4;////階段Ｖ４
	mapType[0][58].htype = 5;////階段Ｖ４
	mapType[0][59].htype = 6;////階段Ｖ４
	mapType[0][63].htype = 2;////階段Ｖ４



//	mapType[0][87].htype = 2;////階段Ｖ４
	mapType[0][0].gravit = p_gra;

}

void gnd_syoki(void){
	BYTE i, j, k;
	for(i = 0; i < 64; i++){
		switch(i){
			case 0:	for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[j][k] = 0x00;
						}
					}break;//足場０タイプ
			
			case 1: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[j][k] = 1;
						}
					}break;//平面足場
					
			case 2: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[j][k] = 64;
						}
					}break;//ブロック米

			//斜面"／"下半
			case 3: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[k][j] = k;
						}
					}
					break;

			//斜面"／"上半
			case 4: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[k][j] = k + 32;
						}
					}
					break;
					
			//斜面"＼"上半
			case 5: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[k][j] = 63 - k;
						}
					}
					break;
			
			//斜面"＼"下半
			case 6: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[k][j] = 31 - k;
						}
					}
					break;

			//斜面"Ｖ"下半
			case 7: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[j][k] = k;
						}
					}
					break;
					
			//斜面"Ｖ"上半
			case 8: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[j][k] = k + 32;
						}
					}
					break;
					
			//斜面"Λ"下半
			case 9: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[j][k] = 32 - k;
						}
					}
					break;
					
			//斜面"Λ"上半
			case 10: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[j][k] = 63 - k;
						}
					}
					break;

			//斜面"┓"上半
			case 11: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[j][k] = 63 - k;
						}
					}
					break;

			case 36: for(j = 0; j < 12; j++){
						for(k = 0; k < 10; k++){
							g_height[i].gnd[j][k] = 8;
						}
					}
					 for( ;j < 22; j++){
						for(k = 0; k < 22; k++){
							g_height[i].gnd[j][k] = k + 8 - 12;
						}
					}
					 for( ;j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[j][k] = 16;
						}
					}
					break;//斜面" ,,／'' "

			case 37: for(j = 0; j < 12; j++){
						for(k = 0; k < 10; k++){
							g_height[i].gnd[j][k] = 16;
						}
					}
					 for( ;j < 22; j++){
						for(k = 0; k < 22; k++){
							g_height[i].gnd[j][k] = 16 - k + 12;
						}
					}
					 for( ;j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[j][k] = 8;
						}
					}
					break;//斜面" ''＼,, "

			case 38: for(j = 0; j < 12; j++){
						for(k = 0; k < 10; k++){
							g_height[i].gnd[k][j] = 16;
						}
					}
					 for( ;j < 22; j++){
						for(k = 0; k < 22; k++){
							g_height[i].gnd[k][j] = 16 - k + 12;
						}
					}
					 for( ;j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[k][j] = 8;
						}
					}
					break;//斜面" ＼,,/ "

			case 39: for(j = 0; j < 12; j++){
						for(k = 0; k < 10; k++){
							g_height[i].gnd[k][j] = 8;
						}
					}
					 for( ;j < 22; j++){
						for(k = 0; k < 22; k++){
							g_height[i].gnd[k][j] = 8 + k - 12;
						}
					}
					 for( ;j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[k][j] = 16;
						}
					}
					break;//斜面" ＼,,/ "

			case 40: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[k][31 - j] = 1 + ((j / 8) * 8);
						}
					}
					break;//斜面"Λ"上半

			case 41: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[k][31 - j] = 33 + ((j / 8) * 8);
						}
					}
					break;//斜面"Λ"上半

			case 42: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[k][31 - j] = 64;
						}
					}
					break;//斜面"Λ"上半

			case 43: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[k][j] = 1 + ((j / 8) * 8);
						}
					}
					break;//斜面"Λ"上半

			case 44: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[k][j] = 33 + ((j / 8) * 8);
						}
					}
					break;//斜面"Λ"上半


					
			case 63:for(j = 0; j < 31; j++){//列・段中心へ
						for(k = j; k < 31 - j; k++){//0から始まる→1から始まる・・・/15から始まって16で終わる（最後kは1回だけ変動）
						g_height[i].gnd[k][j] = j + 16;//上段右に
						g_height[i].gnd[j][31 - k] = j + 16;// 左列上へ
						g_height[i].gnd[31 - j][k] = j + 16;//右列下へ
						g_height[i].gnd[31 - k][31 - j] = j + 16;//下段左へ
						}
					}break;//ピラミッド型？

			default: for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							g_height[i].gnd[j][k] = 0x10;
						}
					}break;
		}

	}
//	sethtype();
	pic_syoki();
}

