//efect
BYTE e_sort[256];//efect表示順（）
WORD e_sousu = 0;//画面効果表示総数

struct efect_data{
//	BYTE atari[6];//[0,1]=X [2,3]=Y [4,5]=Z ﾀﾃ ﾖｺ ﾀｶｻ のあたり判定範囲。tigau?*全て/2の値で
	BYTE ataritype;//あたるタイプ 0～254:設定範囲 255:あたらない 

	int accel_x;//加速
	int accel_y;//
	int accel_z;//
	int XpSyo;//マップ少数点以下
	int YpSyo;
	int ZpSyo;
	WORD Xpos;
	WORD Ypos;
	WORD Zpos;
	WORD XCpos;
	WORD YCpos;
	BYTE asimoto;//着地

	BYTE size;
	
	BYTE dareno;//キャラNo
	BYTE daretype;//キャラタイプ//0:だれでもない 1:一般キャラ//表示微調整用？
	BOOL tenmetu;
	BOOL through;//通り抜け
	BOOL gra;//G
	
	BYTE num;//先頭アド
	BYTE a_loop; //アニメループ
	BYTE a_mati;//ここまで待つ
	BYTE a_cnt;//アニメカウント
	BYTE aNamN;
	BYTE muki;//1:left 2:up 4:right 8:down
	
//	BYTE jump_m;//jamp power
//	WORD jump_t;//jump time
	WORD levit;//浮遊
	
	
//	int huttobi_x;//ふっとび
//	int huttobi_y;
//	int huttobi_z;

} efect[256];

/*struct eata{
	BYTE xl;
	BYTE xr;
	BYTE yu;
	BYTE yd;
	BYTE zt;
	BYTE zb;
}e_atari[16];

void e_atari_syoki(void){
	e_atari[0].xl = 0;
	e_atari[0].xr = 0;
	e_atari[0].yu = 0;
	e_atari[0].yd = 0;
	e_atari[0].zt = 0;
	e_atari[0].zb = 0;

	e_atari[1].xl = 0;//hoko-huri
	e_atari[1].xr = 32;
	e_atari[1].yu = 8;
	e_atari[1].yd = 31;
	e_atari[1].zt = 20;
	e_atari[1].zb = 0;
}*/

void efect_syoki(BYTE ii){
/*	efect[ii].atari[0] = 9;//higasi
	efect[ii].atari[1] = 23;//nisi
	efect[ii].atari[2] = 25;//kita
	efect[ii].atari[3] = 31;//minami
	efect[ii].atari[4] = 20;//atama
	efect[ii].atari[5] = 0;//asi
*/	
	efect[ii].ataritype = 15;//atari

	efect[ii].accel_x = 0;
	efect[ii].accel_y = 0;
	efect[ii].accel_z = 0;
	efect[ii].XpSyo = 0;//マップ少数点以下
	efect[ii].YpSyo = 0;
	efect[ii].ZpSyo = 0;

	efect[ii].Xpos = 0;
	efect[ii].Ypos = 65535;
	efect[ii].Zpos = 0;
	
	efect[ii].XCpos = 0;
	efect[ii].YCpos = 0;

	efect[ii].asimoto = 8;
	efect[ii].size = 16;
	
	efect[ii].dareno = 0;//キャラNo
	efect[ii].daretype = 0;//キャラタイプ
	efect[ii].tenmetu = FALSE;
	efect[ii].through = FALSE;
	efect[ii].gra = FALSE;

	efect[ii].num = 0;//先頭アド 
	efect[ii].a_loop = FALSE; //アニメループ
	efect[ii].a_mati = 0;
	efect[ii].a_cnt = 0;//アニメカウント
	efect[ii].aNamN = 0;
	efect[ii].muki = 1;


	efect[ii].levit = 0;
	e_sort[ii] = ii;

}

//hurimawasi__
//void efectset(BYTE num, BYTE stype, BYTE loop, BYTE mati, BYTE muki, WORD x, WORD y, WORD z)//効果発生
//void efect_01(WORD x, WORD y, WORD z, int ax, int ay, int az){
	
//}
/*
void ef_stoneshot(BYTE vdnum, BYTE num, BYTE muki, BYTE dareno){
	WORD x, y;
	
		muki = chara[dareno].muki;
//		muki = weapon[dareno].kakudo;
//		x = weapon[dareno].Xpos;
		x = chara[dareno].Xpos;
		if(muki == 1){
			if(x < 45){x = 0;}
//			else{x -= 32;}
//			efect[vdnum].accel_x = -2250;
			efect[vdnum].accel_x = -weapon[dareno].m_spd;
		}
		else if(muki == 4){
			if(x > 65495){x = 65535;}
//			else{x += 32;}
			efect[vdnum].accel_x = weapon[dareno].m_spd;
		}
//		y = weapon[dareno].Ypos;
		y = chara[dareno].Ypos;
		if(muki == 2){
//			if(y < 45){y = 0;}
//			else{y -= 45;}
		}
		else if(muki == 8){
//			if(y > 65495){y = 65535;}
//			else{y += 45;}
		}
		
	z = weapon[dareno].Zpos;
	efect[vdnum].asimoto = 8;
		
	efect[vdnum].num = num;
	efect[vdnum].Xpos = x;
	efect[vdnum].Ypos = y;
	efect[vdnum].Zpos = z;
	efect[vdnum].XpSyo = 0;
	efect[vdnum].YpSyo = 0;
	efect[vdnum].ZpSyo = 0;
	efect[vdnum].tenmetu = 0;
	efect[vdnum].a_loop = loop;
	efect[vdnum].a_mati = mati;
	efect[vdnum].aNamN = 0;
	efect[vdnum].muki = muki;
	
	efect[vdnum].size = stype;
	e_sousu++;
}*/

void set_efect_s(BYTE btype, BYTE dareno){//shot用エフェクト
	BYTE vdnum, muki;//空きデータいくつ？
	WORD x, y, z;
	for(vdnum = 0; vdnum < 255; vdnum++){
		if(efect[vdnum].num == 0){break;}
	}
	if(vdnum == 255){vdnum = 0;return;}
	efect[vdnum].dareno = dareno;
//	efect[vdnum].daretype = daretype;
	efect[vdnum].daretype = 0;
	
	muki = chara[dareno].muki;
	if(muki == 1){
		efect[vdnum].accel_x = chara[dareno].accel_x - bullet[btype].accel_x;
		efect[vdnum].accel_y = -bullet[btype].accel_y;
		x = chara[dareno].Xpos;
	}
//	else if(muki == 2){
//		efect[vdnum].accel_x = -bullet[btype].m_spd;
//	}
	else if(muki == 4){
		efect[vdnum].accel_x = (bullet[btype].accel_x + chara[dareno].accel_x);
		efect[vdnum].accel_y = -bullet[btype].accel_y;
		x = chara[dareno].Xpos;
	}
//	else if(muki == 8){
//		efect[vdnum].accel_x = -bullet[btype].m_spd;
//	}
	z = weapon[dareno].Zpos;
	
	if(bullet[btype].shottyp == 0){
		efect[vdnum].asimoto = 8;
	}
	else if(bullet[btype].shottyp == 1){
		efect[vdnum].levit = 1;
		efect[vdnum].asimoto = 0;
		efect[vdnum].through = FALSE;
	}
	y = chara[dareno].Ypos;
	z = weapon[dareno].Zpos;
	
	if(bullet[btype].size == 16){//サイズ変更による位置調整
		x += 8;
		y += 8;
		z += 16;
	}
	
	efect[vdnum].num = bullet[btype].num;
	efect[vdnum].Xpos = x;
	efect[vdnum].Ypos = y;
	efect[vdnum].Zpos = z;
	efect[vdnum].XpSyo = 0;
	efect[vdnum].YpSyo = 0;
	efect[vdnum].ZpSyo = 0;
	efect[vdnum].tenmetu = 0;
	efect[vdnum].a_loop = bullet[btype].loop;
	efect[vdnum].gra = bullet[btype].gra;
	efect[vdnum].a_mati = bullet[btype].a_spd;
	efect[vdnum].aNamN = 0;
	efect[vdnum].muki = muki;
	efect[vdnum].ataritype = bullet[btype].ataritype;
	
	efect[vdnum].size = bullet[btype].size;
	e_sousu++;
}

void set_efect(BYTE num, BYTE stype, BYTE loop, BYTE mati, BYTE dareno, BYTE daretype, BOOL tenmetu)//効果発生
{
	BYTE vdnum, muki;//空きデータいくつ？
	WORD x, y, z;
	for(vdnum = 0; vdnum < 255; vdnum++){
		if(efect[vdnum].num == 0){break;}
	}
	if(vdnum == 255){vdnum = 0;return;}
	
	efect[vdnum].dareno = dareno;
	efect[vdnum].daretype = daretype;
	if(daretype == 0){//charadata
		x = chara[dareno].Xpos;
		y = chara[dareno].Ypos;
		z = chara[dareno].Zpos;
		if(chara[dareno].muki == 1){
			efect[vdnum].accel_x = -300;
			efect[vdnum].accel_y = -600;
			efect[vdnum].accel_z = 800;
		}
		else if(chara[dareno].muki == 4){
			efect[vdnum].accel_x = 300;
			efect[vdnum].accel_y = 600;
			efect[vdnum].accel_z = 800;
		}
		else if(chara[dareno].muki == 2){
			efect[vdnum].accel_x = -600;
			efect[vdnum].accel_y = -300;
			efect[vdnum].accel_z = 800;
		}
		else if(chara[dareno].muki == 8){
			efect[vdnum].accel_x = 600;
			efect[vdnum].accel_y = 300;
			efect[vdnum].accel_z = 800;
		}

		efect[vdnum].asimoto = 0;
		muki = 1;
		efect[vdnum].ataritype = 0;
		efect[vdnum].levit = 1;
		efect[vdnum].gra = TRUE;
	}
	
	if(daretype == 1){//charadata
		x = chara[dareno].Xpos;
		y = chara[dareno].Ypos;
		z = chara[dareno].Zpos;
//		muki = chara[dareno].muki;
		muki = 1;
		efect[vdnum].ataritype = 0;
		efect[vdnum].asimoto = 8;
		efect[vdnum].dareno = dareno;
		efect[vdnum].daretype = daretype;
	}
	if(daretype == 2){//myweapon
		muki = chara[dareno].muki;
		if(muki == 1){
			x =chara[dareno].Xpos;
			if(x < weapon[dareno].reach){x = 0;}
			else{x -= weapon[dareno].reach;}
			y = weapon[dareno].Ypos;
		}
		else if(muki == 4){
			x =chara[dareno].Xpos;
			if(x > 65535 - weapon[dareno].reach){x = 65535;}
			else{x += weapon[dareno].reach;}
			y = weapon[dareno].Ypos;
		}
		if(muki == 2){
			x = weapon[dareno].Xpos;
			y = chara[dareno].Ypos + 16;
			if(y < weapon[dareno].reach){y = 0;}
			else{y -= weapon[dareno].reach;}
		}
		else if(muki == 8){
			x = weapon[dareno].Xpos;
			y = chara[dareno].Ypos;
			if(y > 65535 - weapon[dareno].reach){y = 65535;}
			else{y += weapon[dareno].reach;}
		}
		efect[vdnum].ataritype = 1;
		z = chara[dareno].Zpos;
		efect[vdnum].asimoto = 8;
	}
	if(daretype == 3){//weapon
		muki = chara[dareno].muki;
//		muki = weapon[dareno].kakudo;
		x = weapon[dareno].Xpos;
		if(muki == 1){
			if(x < 45){x = 0;}
			else{x -= 32;}
		}
		else if(muki == 4){
			if(x > 65495){x = 65535;}
			else{x += 32;}
		}
		y = weapon[dareno].Ypos;
		if(muki == 2){
//			if(y < 45){y = 0;}
//			else{y -= 45;}
		}
		else if(muki == 8){
//			if(y > 65495){y = 65535;}
//			else{y += 45;}
		}
		
		z = weapon[dareno].Zpos;
		efect[vdnum].asimoto = 8;
	}
	
	if(stype == 16){//サイズ変更による位置調整
		x += 8;
		y += 8;
		z += 16;
	}
////////
	efect[vdnum].num = num;
	
	efect[vdnum].Xpos = x;
	efect[vdnum].Ypos = y;
	efect[vdnum].Zpos = z;
	efect[vdnum].XpSyo = 0;
	efect[vdnum].YpSyo = 0;
	efect[vdnum].ZpSyo = 0;
	efect[vdnum].tenmetu = tenmetu;
	efect[vdnum].a_loop = loop;
	efect[vdnum].through = FALSE;
	efect[vdnum].levit = 0;
	efect[vdnum].a_mati = mati;
	efect[vdnum].aNamN = 0;
	efect[vdnum].muki = muki;
	
	efect[vdnum].size = stype;
	e_sousu++;
}

void cuedata_e(WORD i, WORD j){//i << j
	efect[i].ataritype = efect[j].ataritype;

	efect[i].accel_x = efect[j].accel_x;
	efect[i].accel_y = efect[j].accel_y;
	efect[i].accel_z = efect[j].accel_z;
	efect[i].XpSyo = efect[j].XpSyo;//マップ少数点以下
	efect[i].YpSyo = efect[j].YpSyo;
	efect[i].ZpSyo = efect[j].ZpSyo;

	efect[i].Xpos = efect[j].Xpos;
	efect[i].Ypos = efect[j].Ypos;
	efect[i].Zpos = efect[j].Zpos;
	
	efect[i].XCpos = efect[j].XCpos;
	efect[i].YCpos = efect[j].YCpos;
	
	efect[i].asimoto = efect[j].asimoto;
	efect[i].size = efect[j].size;
	
	efect[i].dareno = efect[j].dareno;//
	efect[i].daretype = efect[j].daretype;//
	efect[i].tenmetu = efect[j].tenmetu;
	efect[i].through = efect[j].through;
	efect[i].gra = efect[j].gra;
	
	efect[i].num = efect[j].num;//先頭アド 
	efect[i].a_loop = efect[j].a_loop; //アニメループ
	efect[i].a_mati = efect[j].a_mati;
	efect[i].a_cnt = efect[j].a_cnt;//アニメカウント
	efect[i].aNamN = efect[j].aNamN;
	efect[i].muki = efect[j].muki;

	efect[i].levit = efect[j].levit;
	
}
void delete_efect(BYTE ii)
{
	WORD i;
	efect[ii].num = 0;
	efect[ii].Ypos = 65535;
	for(i = ii; i < e_sousu; i++){
		cuedata_e(i, i + 1);
	}
	
	e_sousu--;
}


