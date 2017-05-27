////necohoko01 BMP SET

BYTE readbfiles = 12;
WORD endfilead[16];

struct BMSET{
	char *BMNAME;
	BYTE MASUTYPE;
	
}BMSET[16];

struct pixel{
	LPDWORD data[2048];
	WORD mych_ad;
	WORD ench_ad;
	WORD bmap_ad;
	WORD cmap_ad[16];
	WORD wepo_ad;

	WORD item_ad;
	WORD efe16_ad;
	WORD efe32_ad;
	WORD numb_ad;
	WORD font12_ad;
	WORD msface_ad;

	WORD menu_ad;
	
}pix;

struct Bcustom{
	char *BMName;
	BYTE masutype;
} Bcustom[16];
	

void bnamebtype_set(void){
	BYTE no = 0, typ = 0;
	Bcustom[no].BMName = "BMPFILES/tespo.bmp";
	Bcustom[no].masutype = typ;
	no++;
	Bcustom[no].BMName = "BMPFILES/enemy.bmp";
	Bcustom[no].masutype = typ;
	no++;
	Bcustom[no].BMName = "BMPFILES/weapon.bmp";
	Bcustom[no].masutype = typ;
	no++;
	Bcustom[no].BMName = "BMPFILES/sqm01.bmp";
	Bcustom[no].masutype = typ;
	no++;
	Bcustom[no].BMName = "BMPFILES/cum01.bmp";
	Bcustom[no].masutype = typ;
	no++;
	Bcustom[no].BMName = "BMPFILES/ojm01.bmp";
	Bcustom[no].masutype = typ;
	no++;
	Bcustom[no].BMName = "BMPFILES/efect32_1.bmp";
	Bcustom[no].masutype = typ;
	no++;
	
	typ++;//1

	Bcustom[no].BMName = "BMPFILES/efect16_1.bmp";
	Bcustom[no].masutype = typ;
	no++;
	Bcustom[no].BMName = "BMPFILES/chara16.bmp";
	Bcustom[no].masutype = typ;
	no++;

	typ++;//2

	Bcustom[no].BMName = "BMPFILES/suuji.bmp";
	Bcustom[no].masutype = typ;
	no++;//10

	typ++;//3

	Bcustom[no].BMName = "BMPFILES/font12p.bmp";
	Bcustom[no].masutype = typ;
	no++;//11

	typ++;//4
	Bcustom[no].BMName = "BMPFILES/face_t.bmp";
	Bcustom[no].masutype = typ;

//	Bcustom[9].BMName = "BMPFILES/tespo.bmp";
//	Bcustom[9].masutype = 0;
//	Bcustom[10].BMName = "BMPFILES/tespo.bmp";
//	Bcustom[10].masutype = 0;
//	Bcustom[11].BMName = "BMPFILES/tespo.bmp";
//	Bcustom[11].masutype = 0;
//	"BMPFILES/dmenu.bmp"; BMPF[i].masuType = 3; break;
//	"BMPFILES/ddisp.bmp";  BMPF[i].masuType = 3; break;
}

void bmadnameset(WORD FNo){
	switch(FNo){
		case 0:	pix.mych_ad = 0;break;
		case 1: pix.ench_ad = endfilead[FNo - 1];break;
		case 2: pix.wepo_ad = endfilead[FNo - 1];break;//ここがエンド？
		case 3: pix.bmap_ad = endfilead[FNo - 1];break;
		case 4: pix.cmap_ad[0] = endfilead[FNo - 1];break;//ここがエンド？
		case 5: pix.cmap_ad[1] = endfilead[FNo - 1];break;
		case 6: pix.efe32_ad = endfilead[FNo - 1];break;

		case 7: pix.efe16_ad = endfilead[FNo - 1];break;
		case 8: pix.item_ad = endfilead[FNo - 1];break;

		case 9: pix.numb_ad = endfilead[FNo - 1];break;
		case 10: pix.font12_ad = endfilead[FNo - 1];break;
		case 11: pix.msface_ad = endfilead[FNo - 1];break;
//			case 10:pix.cmap_ad[0] = endfilead[FNo - 1];break;
//			case 11:pix.cmap_ad[1] = endfilead[FNo - 1];break;
		default: break;
	}

}
	
	
