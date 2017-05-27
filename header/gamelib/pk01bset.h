////necohoko01 BMP SET

BYTE readbfiles = 3;
WORD endfilead[3];

struct BMSET{
	char *BMNAME;
	BYTE MASUTYPE;
	
}BMSET[16];

struct pixel{
	LPDWORD data[2048];
//	WORD mych_ad;

//	WORD item_ad;
//	WORD efe16_ad;
//	WORD efe32_ad;
	WORD numb_ad;
	WORD font12_ad;
	WORD keyb_ad;
	
}pix;

struct Bcustom{
	char *BMName;
	BYTE masutype;
} Bcustom[16];
	

void bnamebtype_set(void){
	BYTE no = 0, typ = 0;

	typ++;//1

	Bcustom[no].BMName = "NAMABMP/keybset.bmp";
	Bcustom[no].masutype = typ;
	no++;

	typ++;//2

	Bcustom[no].BMName = "NAMABMP/suuji.bmp";
	Bcustom[no].masutype = typ;
	no++;//10

	typ++;//3

	Bcustom[no].BMName = "NAMABMP/font12p.bmp";
	Bcustom[no].masutype = typ;
	no++;//11

	typ++;//4
}

void bmadnameset(WORD FNo){
	switch(FNo){
//		case 0:	pix.mych_ad = 0;break;
///		case 1: pix.efe32_ad = endfilead[FNo - 1];break;

//		case 2: pix.efe16_ad = endfilead[FNo - 1];break;
//		case 0: pix.item_ad = 0; break;
		case 0: pix.keyb_ad = 0;break;

		case 1: pix.numb_ad = endfilead[FNo - 1];break;
		case 2: pix.font12_ad = endfilead[FNo - 1];break;
//			case 10:pix.cmap_ad[0] = endfilead[FNo - 1];break;
//			case 11:pix.cmap_ad[1] = endfilead[FNo - 1];break;
		default: break;
	}

}

void orgbnamebtype_set(void){
	BYTE no = 0, typ = 0;

	typ++;//1

	Bcustom[no].BMName = "BMPFILES/000.gdt";
	Bcustom[no].masutype = typ;
	no++;

	typ++;//2

	Bcustom[no].BMName = "BMPFILES/001.gdt";
	Bcustom[no].masutype = typ;
	no++;//10

	typ++;//3

	Bcustom[no].BMName = "BMPFILES/002.gdt";
	Bcustom[no].masutype = typ;
	no++;//11

	typ++;//4
}


WORD fit_col(DWORD color, LPDWORD pal)//フィットしたカラーパレットを返す
{
	WORD cad = 0;
	
	while(cad < 256){
		if(*(pal + cad) == color){return cad;}
		else{cad++;}
	}
	return cad;
}

void dump_bmp(LPDWORD palad, LPDWORD pixad, BYTE xblock, BYTE yblock, BYTE mas, BYTE FNo, char *op)
{
	HANDLE hFile;
	OPENFILENAME ofn;
	DWORD D, i;
	DWORD FileSize;

	BYTE fitcol,backfcol, cnt, memcol, k, val = 0;
	char type[4];
	char no[3] = {'0', '0', '0'};
	char names[16];
	char *name;
	strcpy(names, "BMPFILES/");


	for(i = 0; i < 3; i++){
		switch(i){
			case 0:	val = (FNo) / 100;break;//100
			case 1:	val = (FNo % 100) / 10;break;//10
			case 2:	val = (FNo) % 10;break;//1
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
		names[9 + i] = no[i];
	}

	strcpy(type, op);//background mugic data

	for(; i < 7; i++){//4moji
		names[9 + i] = type[i - 3];
	}
	names[9 + i] = NULL;
	
	name = &(names[0]);

	
	val = 0;
	hFile = CreateFile(name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hFile, &xblock, 1, &D, NULL);//block数
	WriteFile(hFile, &yblock, 1, &D, NULL);//block数
//	WriteFile(hFile, block, 1, &D, NULL);//block数
	WriteFile(hFile, &mas, 1, &D, NULL);//xyマス
	WriteFile(hFile, palad, 256 * 4, &D, NULL);//パレット
	
	
	memcol = 0;
	backfcol = 0;
	cnt = 0;
	//圧縮
	for(i = 0; i < (DWORD)(xblock * yblock * mas * mas); i++){
		fitcol = fit_col(*(pixad + i), palad);//fitしたパレット
		memcol = fitcol;
		if(!i){
			WriteFile(hFile, &fitcol, 1, &D, NULL);
			backfcol = fitcol;
		}//最初

		if(255 < fitcol){//フィットしなかったら
			MessageBox(NULL,"失敗。", name, MB_OK);
			return;
		}
		else{
			if(backfcol == fitcol){//同じ色だったら
				if(cnt == 255){
					WriteFile(hFile, &cnt, 1, &D, NULL);//dataNO
					cnt = 0;
					backfcol = fitcol;
				}
				else{
					cnt++;
				}
			}
			else{
				WriteFile(hFile, &cnt, 1, &D, NULL);//dataNO
				cnt = 0;
				backfcol = fitcol;
			}
			if(!cnt){WriteFile(hFile, &fitcol, 1, &D, NULL); cnt++;}//dataNO
		}
	}
	WriteFile(hFile, &cnt, 1, &D, NULL);//最後
	CloseHandle(hFile);

}

void make_bmp(WORD FNo, BYTE xblock, BYTE yblock)
{
	
	WORD adad, fitcol, colad = 0, ofad;
	DWORD colpal[256], i;//256色まで
	WORD val = 0;
	DWORD mas;

	FillMemory(colpal, sizeof(colpal), 0);

	switch(FNo){
		case 0: adad = pix.keyb_ad;break;
		case 1: adad = pix.numb_ad; break;
		case 2: adad = pix.font12_ad;break;
		default: return;
	}
	
	switch(Bcustom[FNo].masutype){
		case 0: mas = 32; break;
		case 1: mas = 16; break;
		case 2: mas = 8; break;
		case 3: mas = 12; break;
		case 4: mas = 64; break;
	}

	//カラーパレット作成
	colpal[0] = 0;
	fitcol = 0;
	for(ofad = 0; ofad < xblock * yblock; ofad++){
		for(i = 0; i < mas * mas; i++){
			if(colpal[colad] != (*pix.data[adad + ofad] + i)){//今のパレットと違う
				fitcol = fit_col(*(pix.data[adad + ofad] + i), &colpal[0]);//fitしたパレット

				if(255 < fitcol){//フィットしなかったら
					colad++;
					colpal[colad] = *(pix.data[adad + ofad] + i);
					if(colad > 255){return;}
				}
			}
		}
	}
	
	dump_bmp(colpal, pix.data[adad], xblock, yblock, mas, FNo, ".gdt");
}
	

