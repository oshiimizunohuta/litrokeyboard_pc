void set_efect(BYTE num, BYTE stype, BYTE loop, BYTE mati, BYTE dareno, BYTE daretype, BOOL tenmetu);//Œø‰Ê”­¶


struct weapon_data{
	char name;
	BYTE ID;
	
	WORD Xpos,Ypos,Zpos;
	WORD XCpos,YCpos;//pixelƒ|ƒWƒVƒ‡ƒ“
	
	BYTE muki; //0:¶Œü‚« 1:‰EŒü‚«
	BYTE num;//æ“ªƒAƒh

	BYTE glip[16];
//‡”Ô•Ï‚¦‚È‚¢//
	BYTE kakudo; //0:‚O“x 1:‚R‚O“x 2:‚S‚T“x 3:‚U‚O“x
	BOOL back; //ƒLƒƒƒ‰‚ÌŒã‚ë‚©H
////////////////
	
	BOOL hantei; //0:“–‚½‚ç‚È‚¢ 1:“–‚½‚é
	BYTE PoE;//“G‚©Œ©•û‚©

	BYTE daretype;
	BYTE atari[6];//bukiatariB glip‚©‚ç‚¢‚­‚Â—£‚ê‚Ä‚é‚©ˆÊ’u  á|„ª„ª„ªG„ª
	BYTE agi[8];//0:U‚è‚©‚Ô‚è@1:UŒ‚”­¶’†@2:—]•ªUŒ‚”­¶@3:–³ŒøU‚èØ‚è,255‚ª“ü‚ê‚Î‚¨‚í‚è
	BYTE reach;//ƒŠ[ƒ`
	BYTE swing[3][16];//‹OÕƒGƒtƒFƒNƒg

	WORD fillup;//[“U‘Ò‚¿ŠÔ
	BYTE fillcnt[32];//[“UƒJƒEƒ“ƒg
	BYTE bulletch;//˜AË”
	BYTE bullettyp;//’e‚Ìí—Ş//‚OFÎ
	BYTE dareno;
	BOOL trig;//‰Ÿ‚µ•ú‚µ
	
} weapon[255];

struct bullet_data{
	BYTE loop;
	BYTE a_spd;
	BYTE num;
	BYTE size;
	BYTE shottyp;//d—Í‚Æ‚©‰Á‘¬Œ¸‘¬////0:“™‰Á‘¬ 1:d—Í‚ÆŒ¸‘¬
	BYTE ataritype;
	BOOL gra;
	WORD accel_x;
	WORD accel_y;
	WORD accel_z;
}bullet[16];

///////////////
struct weaponset{
	BYTE Cglip[2];
	BYTE kakudo;
	BOOL back;
	BOOL fix;//ƒOƒŠƒbƒv1‚±‚Ü‚Ì‚İ
} rane[32][8], eneself;//rane=©ƒLƒƒƒ‰ƒpƒ^[ƒ“

void buki_syoki(void)
{
	BYTE i;
	////////•Šíƒf[ƒ^‰Šú////////
	weapon[0].ID = 1;
	weapon[0].agi[0] = 20;//40;
	weapon[0].agi[1] = 80;//60;
	weapon[0].agi[2] = 80;//60;
	weapon[0].agi[3] = 20;
	weapon[0].agi[4] = 255;

	weapon[0].muki = 3;
	weapon[0].kakudo = 0;
	weapon[0].num = 0;
	
	//atari ˆÊ’u
	weapon[0].atari[0] = 25;//nisi
	weapon[0].atari[1] = 13;//higasi
	weapon[0].atari[2] = 3;//kita
	weapon[0].atari[3] = 3;//minami
	weapon[0].atari[4] = 1;//ue
	weapon[0].atari[5] = 1;//sita

	weapon[0].PoE = 3;//
	weapon[1].PoE = 2;//
	weapon[2].PoE = 2;//

	//•Šíæ‚Áè
	for(i = 0; i < 16; i++){
		switch(i){
			//¶Œü‚«			//Xpos							//Ypos
			case 0: weapon[0].glip[i] = 25; break;	case 8: weapon[0].glip[i] = 26; break;
			//ãŒü‚«
			case 1: weapon[0].glip[i] = 6; break;	case 9: weapon[0].glip[i] = 25; break;
			//‰EŒü‚«
			case 2: weapon[0].glip[i] = 6; break;	case 10: weapon[0].glip[i] = 5; break;
			//‰ºŒü‚«
			case 3: weapon[0].glip[i] = 26; break;	case 11: weapon[0].glip[i] = 6; break;

			case 4: weapon[0].glip[i] = 0; break;	case 12: weapon[0].glip[i] = 0; break;

			case 5: weapon[0].glip[i] = 0; break;	case 13: weapon[0].glip[i] = 0; break;

			case 6: weapon[0].glip[i] = 0; break;	case 14: weapon[0].glip[i] = 0; break;

			case 7: weapon[0].glip[i] = 0; break;	case 15: weapon[0].glip[i] = 0; break;
			default: break;
		}
	}
	
}
void glip_syoki(void)
{
	BYTE act = 0;//wait
	rane[act][0].Cglip[0] = 17;//¶Œü‚«wait
	rane[act][0].Cglip[1] = 26;
	rane[act][0].kakudo = 15;
	rane[act][0].back = TRUE;
	rane[act][0].fix = TRUE;
	
	rane[act][1].Cglip[0] = 24;//ãŒü‚«wait
	rane[act][1].Cglip[1] = 24;
	rane[act][1].kakudo = 5;
	rane[act][1].back = FALSE;
	rane[act][1].fix = TRUE;
	
	rane[act][3].Cglip[0] = 14;//‰EŒü‚«wait
	rane[act][3].Cglip[1] = 26;
	rane[act][3].kakudo = 9;
	rane[act][3].back = FALSE;
	rane[act][3].fix = TRUE;

	rane[act][7].Cglip[0] = 8;//‰ºŒü‚«wait
	rane[act][7].Cglip[1] = 24;
	rane[act][7].kakudo = 13;
	rane[act][7].back = TRUE;
	rane[act][7].fix = TRUE;
	act++;//1
	act++;//2
	act++;//3

	act++;//a = 4 walk no0
	rane[act][0].Cglip[0] = 18;//¶
	rane[act][0].Cglip[1] = 21;
	rane[act][0].kakudo = 3;
	rane[act][0].back = TRUE;
	rane[act][0].fix = FALSE;
	
	rane[act][1].Cglip[0] = 24;//ã
	rane[act][1].Cglip[1] = 24;
	rane[act][1].kakudo = 6;
	rane[act][1].back = FALSE;
	rane[act][1].fix = FALSE;
	
	rane[act][3].Cglip[0] = 23;//‰E
	rane[act][3].Cglip[1] = 21;
	rane[act][3].kakudo = 5;
	rane[act][3].back = FALSE;
	rane[act][3].fix = FALSE;

	rane[act][7].Cglip[0] = 9;//‰º
	rane[act][7].Cglip[1] = 22;
	rane[act][7].kakudo = 15;
	rane[act][7].back = TRUE;
	rane[act][7].fix = FALSE;

	act++;//a = 5 walk no1
	rane[act][0].Cglip[0] = 11;//¶
	rane[act][0].Cglip[1] = 24;
	rane[act][0].kakudo = 1;
	rane[act][0].back = TRUE;
	rane[act][0].fix = FALSE;
	
	rane[act][1].Cglip[0] = 24;//ã
	rane[act][1].Cglip[1] = 23;
	rane[act][1].kakudo = 5;
	rane[act][1].back = FALSE;
	rane[act][1].fix = FALSE;
	
	rane[act][3].Cglip[0] = 21;//‰E
	rane[act][3].Cglip[1] = 24;
	rane[act][3].kakudo = 7;
	rane[act][3].back = FALSE;
	rane[act][3].fix = FALSE;

	rane[act][7].Cglip[0] = 9;//‰º
	rane[act][7].Cglip[1] = 25;
	rane[act][7].kakudo = 13;
	rane[act][7].back = TRUE;
	rane[act][7].fix = FALSE;

	act++;//a = 6 walk no2
	rane[act][0].Cglip[0] = 9;//¶
	rane[act][0].Cglip[1] = 19;
	rane[act][0].kakudo = 5;
	rane[act][0].back = TRUE;
	rane[act][0].fix = FALSE;
	
	rane[act][1].Cglip[0] = 20;//ã
	rane[act][1].Cglip[1] = 20;
	rane[act][1].kakudo = 2;
	rane[act][1].back = TRUE;
	rane[act][1].fix = FALSE;
	
	rane[act][3].Cglip[0] = 12;//‰E
	rane[act][3].Cglip[1] = 24;
	rane[act][3].kakudo = 9;
	rane[act][3].back = FALSE;
	rane[act][3].fix = FALSE;

	rane[act][7].Cglip[0] = 14;//‰º
	rane[act][7].Cglip[1] = 25;
	rane[act][7].kakudo = 6;
	rane[act][7].back = FALSE;
	rane[act][7].fix = FALSE;

	act++;// a = 7 walk no3
	rane[act][0].Cglip[0] = 14;//¶
	rane[act][0].Cglip[1] = 26;
	rane[act][0].kakudo = 0;
	rane[act][0].back = TRUE;
	rane[act][0].fix = FALSE;
	
	rane[act][1].Cglip[0] = 22;//ã
	rane[act][1].Cglip[1] = 23;
	rane[act][1].kakudo = 3;
	rane[act][1].back = TRUE;
	rane[act][1].fix = FALSE;
	
	rane[act][3].Cglip[0] = 17;//‰E
	rane[act][3].Cglip[1] = 26;
	rane[act][3].kakudo = 8;
	rane[act][3].back = FALSE;
	rane[act][3].fix = FALSE;

	rane[act][7].Cglip[0] = 11;//‰º
	rane[act][7].Cglip[1] = 24;
	rane[act][7].kakudo = 5;
	rane[act][7].back = FALSE;
	rane[act][7].fix = FALSE;

	act++;//8
	act++;//9
	act++;//10
	act++;//11

	act++;//a = 12 jump
	rane[act][0].Cglip[0] = 20;//¶
	rane[act][0].Cglip[1] = 21;
	rane[act][0].kakudo = 6;
	rane[act][0].back = TRUE;
	rane[act][0].fix = TRUE;
	
	rane[act][1].Cglip[0] = 24;//ã
	rane[act][1].Cglip[1] = 22;
	rane[act][1].kakudo = 3;
	rane[act][1].back = FALSE;
	rane[act][1].fix = TRUE;
	
	rane[act][3].Cglip[0] = 11;//‰E
	rane[act][3].Cglip[1] = 21;
	rane[act][3].kakudo = 2;
	rane[act][3].back = FALSE;
	rane[act][3].fix = TRUE;

	rane[act][7].Cglip[0] = 7;//‰º
	rane[act][7].Cglip[1] = 21;
	rane[act][7].kakudo = 5;
	rane[act][7].back = TRUE;
	rane[act][7].fix = TRUE;

	act++;//13
	act++;//14
	act++;//15

	act++;//a = 16 FALL
	rane[act][0].Cglip[0] = 14;//¶
	rane[act][0].Cglip[1] = 24;
	rane[act][0].kakudo = 14;
	rane[act][0].back = FALSE;
	rane[act][0].fix = TRUE;
	
	rane[act][1].Cglip[0] = 18;//ã
	rane[act][1].Cglip[1] = 22;
	rane[act][1].kakudo = 6;
	rane[act][1].back = TRUE;
	rane[act][1].fix = TRUE;
	
	rane[act][3].Cglip[0] = 17;//‰E
	rane[act][3].Cglip[1] = 24;
	rane[act][3].kakudo = 1;
	rane[act][3].back = FALSE;
	rane[act][3].fix = TRUE;

	rane[act][7].Cglip[0] = 13;//‰º
	rane[act][7].Cglip[1] = 24;
	rane[act][7].kakudo = 2;
	rane[act][7].back = FALSE;
	rane[act][7].fix = TRUE;

	act++;//a = 8 hurimawasi no1-1
	rane[act][0].Cglip[0] = 20;//¶
	rane[act][0].Cglip[1] = 21;
	rane[act][0].kakudo = 6;
	rane[act][0].back = TRUE;
	rane[act][0].fix = FALSE;
	
	rane[act][1].Cglip[0] = 24;//ã
	rane[act][1].Cglip[1] = 22;
	rane[act][1].kakudo = 3;
	rane[act][1].back = FALSE;
	rane[act][1].fix = FALSE;
	
	rane[act][3].Cglip[0] = 11;//‰E
	rane[act][3].Cglip[1] = 21;
	rane[act][3].kakudo = 2;
	rane[act][3].back = FALSE;
	rane[act][3].fix = FALSE;

	rane[act][7].Cglip[0] = 7;//‰º
	rane[act][7].Cglip[1] = 21;
	rane[act][7].kakudo = 5;
	rane[act][7].back = TRUE;
	rane[act][7].fix = FALSE;

	act++;
	rane[act][0].Cglip[0] = 20;//¶
	rane[act][0].Cglip[1] = 21;
	rane[act][0].kakudo = 6;
	rane[act][0].back = TRUE;
	rane[act][0].fix = FALSE;
	
	rane[act][1].Cglip[0] = 24;//ã
	rane[act][1].Cglip[1] = 22;
	rane[act][1].kakudo = 3;
	rane[act][1].back = FALSE;
	rane[act][1].fix = FALSE;
	
	rane[act][3].Cglip[0] = 11;//‰E
	rane[act][3].Cglip[1] = 21;
	rane[act][3].kakudo = 2;
	rane[act][3].back = FALSE;
	rane[act][3].fix = FALSE;

	rane[act][7].Cglip[0] = 7;//‰º
	rane[act][7].Cglip[1] = 21;
	rane[act][7].kakudo = 5;
	rane[act][7].back = TRUE;
	rane[act][7].fix = FALSE;

/*	rane[4].Cglip[0] = 8;//¶ãŒü‚«wait
	rane[4].Cglip[1] = 24;
	rane[4].kakudo = 13;
	rane[4].back = TRUE;

	rane[5].Cglip[0] = 8;//‰EãŒü‚«wait
	rane[5].Cglip[1] = 24;
	rane[5].kakudo = 13;
	rane[5].back = TRUE;
*/

//‚OEwait(0:¶Œü‚«A1:ãŒü‚«A3:‰EŒü‚«A7:‰ºŒü‚«)
//‚P`‚SEƒAƒ^ƒbƒN‚P`‚S

//‚V¶‰º‚«‚—
//‚W`‚P‚Tmove

//‚P‚U`‚Q‚OƒAƒ^ƒbƒN‚P`‚S¶
//‚Q‚P`‚Q‚TƒAƒ^ƒbƒN‚P`‚Sã
//‚Q‚U`‚R‚OƒAƒ^ƒbƒN‚P`‚S‰E
//‚R‚P`‚R‚TƒAƒ^ƒbƒN‚P`‚S‰º
//‚R‚U`‚S‚OƒAƒ^ƒbƒN‚P`‚S¶ã
//‚S‚P`‚S‚TƒAƒ^ƒbƒN‚P`‚S‰Eã
//‚S‚U`‚T‚OƒAƒ^ƒbƒN‚P`‚S‰E‰º
//‚T‚P`‚T‚TƒAƒ^ƒbƒN‚P`‚S¶‰º
//‚T‚U`‚U‚O



	rane[32][0].Cglip[0] = 0;//‰Šú‰»—p
	rane[32][0].Cglip[1] = 0;
	rane[32][0].kakudo = 0;
	rane[32][0].back = FALSE;


}

void bullet_syoki(void){
	BYTE i = 0;
	bullet[i].loop = 2;
	bullet[i].a_spd = 10;
	bullet[i].accel_x = 1800;
	bullet[i].accel_z = 128;
	bullet[i].num = 4;
	bullet[i].size = 16;
	bullet[i].shottyp = 1;
	bullet[i].ataritype = 16;
	bullet[i].gra = TRUE;
//	i++;


}

void weapon_sling(BYTE wno){
	BYTE i;
	chara[0].equiphand[0] = 64;//64‚ÍƒXƒŠƒ“ƒO
	chara[0].equiphand[1] = wno;
	weapon[wno].bulletch = 3;
//	weapon[wno].fillup = 60;
	weapon[wno].fillup = 30;
	weapon[wno].bullettyp = 0;//
	
	for(i = 0; i < 3; i++){
		weapon[wno].fillcnt[i] = 0;
	}
	
}
/*
void set_swing(BYTE no, BYTE type){
	BYTE i;
	
	switch(type){
		case 0:
		weapon[no].swing[0][0] = 20;
		weapon[no].swing[1][1] = 10;
		weapon[no].swing[2][2] = 10;
		weapon[no].swing[0][0] = 22;
		weapon[no].swing[1][1] = 10;
		weapon[no].swing[2][2] = 10;
		weapon[no].swing[0][0] = 24;
		weapon[no].swing[0][0] = 26;
		weapon[no].swing[0][0] = 28;
		weapon[no].swing[0][0] = 28;
	}
}
*/
/*
void weapon_set(BYTE settype)
{
	switch(chara[0].aNamN){
		case 0: weapon[0].kakudo = 15; weapon[0].back = TRUE;
				chara[0].glip[0] = 9; chara[0].glip[1] = 22;
				break;

		case 1: weapon[0].kakudo = 13; weapon[0].back = TRUE;
				chara[0].glip[0] = 9; chara[0].glip[1] = 25;
				break;

		case 2: weapon[0].kakudo = 6; weapon[0].back = FALSE;
				chara[0].glip[0] = 14; chara[0].glip[1] = 25;
				break;

		case 3: weapon[0].kakudo = 5; weapon[0].back = FALSE;
				chara[0].glip[0] = 11; chara[0].glip[1] = 24;
				break;
	}
	
}
*/
void hurimawasi(BYTE chno)
{
	BYTE muki = chara[chno].muki;
	
//	if(chara[chno].action[5] == TRUE || chara[0].actionb == 5){
	switch(muki){
		case 1:switch(chara[chno].aNamN){//¶
//					case 0: chara[chno].glip[muki - 1] = 20;
//							chara[chno].glip[muki + 7] = 21;
//							weapon[chno].kakudo = 6;
//							weapon[chno].back = TRUE;
//							break;
					case 0: chara[chno].glip[0] = 20;
							chara[chno].glip[1] = 21;
							weapon[chno].kakudo = 6;
							weapon[chno].back = TRUE;
							break;
							
					case 1: chara[chno].glip[0] = 13;
							chara[chno].glip[1] = 22;
							weapon[chno].kakudo = 4;
							weapon[chno].back = TRUE;
							break;
							
					case 2: chara[chno].glip[0] = 8;
							chara[chno].glip[1] = 26;
							weapon[chno].kakudo = 15;
							weapon[chno].back = TRUE;
							break;

					case 3: chara[chno].glip[0] = 12;
							chara[chno].glip[1] = 24;
							weapon[chno].kakudo = 7;
							weapon[chno].back = FALSE;
							break;
							
					default:break;
		}break;

		case 2:switch(chara[chno].aNamN){
					case 0: chara[chno].glip[0] = 24;
							chara[chno].glip[1] = 22;
							weapon[chno].kakudo = 3;
							weapon[chno].back = FALSE;
							break;
							
					case 1: chara[chno].glip[0] = 21;
							chara[chno].glip[1] = 19;
							weapon[chno].kakudo = 7;
							weapon[chno].back = TRUE;
							break;
							
					case 2: chara[chno].glip[0] = 13;
							chara[chno].glip[1] = 16;
							weapon[chno].kakudo = 3;
							weapon[chno].back = TRUE;
							break;
							
					case 3: chara[chno].glip[0] = 16;
							chara[chno].glip[1] = 24;
							weapon[chno].kakudo = 1;
							weapon[chno].back = TRUE;
							break;
							
					default:break;
		}break;

		case 4:switch(chara[chno].aNamN){
					case 0: chara[chno].glip[0] = 11;
							chara[chno].glip[1] = 21;
							weapon[chno].kakudo = 2;
							weapon[chno].back = FALSE;
							break;
							
					case 1: chara[chno].glip[0] = 18;
							chara[chno].glip[1] = 28;
							weapon[chno].kakudo = 14;
							weapon[chno].back = FALSE;
							break;
							
					case 2: chara[chno].glip[0] = 23;
							chara[chno].glip[1] = 26;
							weapon[chno].kakudo = 7;
							weapon[chno].back = FALSE;
							break;
							
					case 3: chara[chno].glip[0] = 19;
							chara[chno].glip[1] = 26;
							weapon[chno].kakudo = 1;
							weapon[chno].back = TRUE;
							break;
							
					default:break;
		}break;

		case 8:switch(chara[chno].aNamN){
					case 0: chara[chno].glip[0] = 7;
							chara[chno].glip[1] = 21;
							weapon[chno].kakudo = 5;
							weapon[chno].back = TRUE;
							break;
							
					case 1: chara[chno].glip[0] = 16;
							chara[chno].glip[1] = 28;
							weapon[chno].kakudo = 15;
							weapon[chno].back = TRUE;
							break;
							
					case 2: chara[chno].glip[0] = 16;
							chara[chno].glip[1] = 27;
							weapon[chno].kakudo = 10;
							weapon[chno].back = FALSE;
							break;
							
					case 3: chara[chno].glip[0] = 12;
							chara[chno].glip[1] = 25;
							weapon[chno].kakudo = 7;
							weapon[chno].back = FALSE;
							break;
							
					default:break;
		}break;
		
		default:break;
		
	}
	weapon[chno].reach = 28;
	
//	weapon[chno].Zpos = chara[0].Zpos;
	if(chara[chno].aNamN == 1){
//		efectset(1, 16, weapon[chno].Xpos, weapon[chno].Ypos, weapon[chno].Zpos);
		set_efect(1, 16, 1, 4, chno, 1, 0);
	}
	if(chara[chno].aNamN == 2){
		set_efect(32, 32, 0, 8, chno, 2, 0);
//		efectset(8, 32, 0, 4, weapon[chno].Xpos - 48, weapon[chno].Ypos + 8, weapon[chno].Zpos + 8);
	}
	if(chara[chno].aNamN == 3){
		set_efect(33, 32, 0, 8, chno, 2, 0);
//		efectset(8, 32, 0, 4, weapon[chno].Xpos - 48, weapon[chno].Ypos + 8, weapon[chno].Zpos + 8);
	}
//	}
}
/*
void bukiposition(void){
	BYTE i, j;
	BYTE l_kakudo, s_kakudo;
	
	for(i = 0; i < a_sousu; i++){
		for(j = 0; j < 1; j++)//‚Æ‚è‚ ‚¦‚¸•Ší‚Ì‚İ
		if(chara[i].equips != 0){
			l_kakudo = weapon[i].kakudo / 4;//0 ~ 3‚Ì‚Í‚¸//•ŠíŒü‚«
//			s_kakudo = weapon[i].kakudo % 4;//Œü‚«‚Ì’†‚ÌŠp“x

			weapon[i].Xpos = chara[i].Xpos + chara[i].glip[chara[i].muki - 1] - weapon[i].glip[l_kakudo];//buki x²•\¦ˆÊ’u
			weapon[i].Ypos = chara[i].Ypos + chara[i].glip[chara[i].muki + 7] - weapon[i].glip[l_kakudo + 8];//buki y²•\¦ˆÊ’u
		}
	}
}
*/

void set_weapon(BYTE num, BYTE dareno, BYTE daretype, BYTE part)//Œø‰Ê”­¶
{
	BYTE vdnum, muki;//‹ó‚«ƒf[ƒ^‚¢‚­‚ÂH
	WORD x, y, z;
	for(vdnum = 0; vdnum < 255; vdnum++){
		if(weapon[vdnum].num == 0){break;}
	}
	if(vdnum == 255){vdnum = 0;return;}
	
	weapon[vdnum].dareno = dareno;
	weapon[vdnum].daretype = daretype;
	if(part == 0){//è
		chara[dareno].equiphand[1] = vdnum;//ƒLƒƒƒ‰QÆAŠÖŒW•t‚¯
	}
/*	if(daretype == 0){//charadata
		x = chara[dareno].Xpos;
		y = chara[dareno].Ypos;
		z = chara[dareno].Zpos;
		if(chara[dareno].muki == 1){
		}
		else if(chara[dareno].muki == 4){
		}
		else if(chara[dareno].muki == 2){
		}
		else if(chara[dareno].muki == 8){
		}

//		weapon[vdnum].asimoto = 0;
//		muki = 1;
	}
	
	if(daretype == 1){//charadata
		x = chara[dareno].Xpos;
		y = chara[dareno].Ypos;
		z = chara[dareno].Zpos;
//		muki = chara[dareno].muki;
		muki = 1;
		weapon[vdnum].asimoto = 8;
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
		weapon[vdnum].ataritype = 1;
		z = chara[dareno].Zpos;
		weapon[vdnum].asimoto = 8;
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
		weapon[vdnum].asimoto = 8;

	}
	if(daretype == 32){//weapon-shot
		muki = chara[dareno].muki;
//		muki = weapon[dareno].kakudo;
//		x = weapon[dareno].Xpos;
		x = chara[dareno].Xpos;
		if(muki == 1){
			if(x < 45){x = 0;}
//			else{x -= 32;}
			weapon[vdnum].accel_x = -2250;
		}
		else if(muki == 4){
			if(x > 65495){x = 65535;}
//			else{x += 32;}
			weapon[vdnum].accel_x = 2250;
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
		weapon[vdnum].asimoto = 8;

	}
	
	if(stype == 16){//ƒTƒCƒY•ÏX‚É‚æ‚éˆÊ’u’²®
		x += 8;
		y += 8;
		z += 16;
	}
*/
	weapon[vdnum].num = num;
	switch(num){
		case 32: weapon_sling(vdnum); break;
	}
//	weapon[vdnum].Xpos = x;
//	weapon[vdnum].Ypos = y;
//	weapon[vdnum].Zpos = z;
//	weapon[vdnum].XpSyo = 0;
//	weapon[vdnum].YpSyo = 0;
//	weapon[vdnum].ZpSyo = 0;
//	weapon[vdnum].tenmetu = tenmetu;
//	weapon[vdnum].a_loop = loop;
//	weapon[vdnum].a_mati = mati;
//	weapon[vdnum].aNamN = 0;
//	weapon[vdnum].muki = muki;
	
//	weapon[vdnum].size = stype;
//	w_sousu++;
}
