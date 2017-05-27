/*ゲーム全般変数*/

static WORD CposX = 0, CposY = 0, CposZ = 0, MposX = 0, MposY = 0, MposZ = 0x80, mapX = 0, mapY = 0, mapZ = 0; /*画面表示用キャラ位置変数*/
WORD UniPosX[64] = {0x0000}, UniPosY[64] = {0x0000}, UniPosZ[64] = {0x0000};/*ゲーム中のキャラ位置変数*/
WORD pix_page = 0;
BYTE a_sousu = 3;
BYTE p_gra = 0x10;
BYTE false_time = 0;
BYTE list_ofs = 0;//menu listずらし表示

LPDWORD lpBBPixel;//！重要！

BYTE fwork = 0;

//debug用？
BYTE scroll = 0x00, MNo = 0x0000, menu[2] = {0x00, 0x00};
BYTE dSize = 0x0001;
BOOL c_on = FALSE;
BOOL dsamp = FALSE;
BYTE sno = 0;

DWORD testP[16];
////////


BYTE jNum = 0x01, jMax = 0x01;

BYTE d_kazu;//数字表示用変数
BOOL maski = TRUE;//マスクしますか。
int paintDo = 0;
BOOL osippa = FALSE;



float Cc = 0;//マップスクロールスピード

BYTE c_sort[128];


/*メインポジション*/
WORD XmasW, XmasE, YmasN, YmasS, Zmas;
BYTE XposW, XposE, YposN, YposS;

/*サブポジション*/
WORD subXmas, subYmas, subYmasN, subYmasS, subZmasH, subZmasL;
WORD sub_m = 0, sub_h = 0;


BYTE subXpos, subYpos, subYposN, subYposS;
BOOL subOK = FALSE;
/***************/

BYTE aNam[64]={0x0000,0,0,0};


/*struct keys{
	BOOL left;
	BOOL up;
	BOOL right;
	BOOL down;
	BOOL enter;
	BOOL num1;
	BOOL num2;
	BOOL num3;
	
	BYTE osi;
	BOOL tame;
	
	} keyState = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};
*/
	
struct ACTIONTYPE{
	BYTE le_w,	up_w,	ri_w,	do_w,//wait
	le_m,	up_m,	ri_m,	do_m,//move
	le_a,	up_a,	ri_a,	do_a,//dash?
	jump_m, damage;//etc
	

}atyp = {0,1,2,3,	4,5,6,7,	8,9,10,11,	18,13};//move と attackは4の間が開いていること。アタック関数で使用中

struct MAPDATA{
	WORD Xmas;
	WORD Ymas;
	
	WORD Xpos;
	WORD Ypos;
	WORD Zpos;
	
	WORD XCpos;
	WORD YCpos;
	WORD ZCpos;
	
	BYTE yuka_x;
	BYTE yuka_y;
	BYTE hirosa[2];
	
	BYTE Put[65535];
//	BYTE Info[1024];
	
	BOOL saikakiko[90][10];
	
	BYTE No;
} MAP;

struct MAP256{
	BYTE Xmas;
	BYTE Ymas;
	
	WORD Xpos;
	WORD Ypos;
	WORD Zpos;

	BYTE Put[256*256];
//	BYTE Info[1024];
	
	BYTE No;
} MAP256[4];

struct MAP128{
	BYTE Xmas;
	BYTE Ymas;
	BYTE Zmas;
	
	WORD Xpos;//ベースマップに対してのXポジション
	WORD Ypos;
	WORD Zpos;

//	BYTE Put[65536][128];
	BYTE Put[128][16384];//128*128*128
//	BYTE Info[1024];
	BYTE hirosa[3]; //x, y, z
	
	BYTE No;
} MAP128[8];

struct env_m{
	int gravit;
	BYTE height[32][32];
	BOOL touka;

} mapType[256];

struct env_a{
	int spring_n;/*now*/
	int spring_m;/*max*/
	int spring_c;/*carry*/
	int shoot;
	
	int huttobi_x;//ふっとび
	int huttobi_y;
	int huttobi_z;
	
	int accel_x;//加速
	int accel_y;//
	int accel_z;//
	
	BYTE asimoto;
	
	BYTE Acycle[5];//0:huttobi 1:attack_false
	} act[128];


struct cha_data{
	WORD Xpos;//マップ位置
	WORD Ypos;
	WORD Zpos;
	
	int XpSyo;//マップ少数点以下
	int YpSyo;
	int ZpSyo;
	
	
	WORD XCpos;//ｶﾒﾗ位置
	WORD YCpos;
	WORD ZCpos;

	BYTE aNam; //ｷｬﾗｸﾀﾋﾞﾂﾄﾏﾂﾌﾟ何番目？
	BYTE aTyp; //ｷｬﾗｸﾀｱｸﾃｨｵﾝﾀｲﾌﾟ
	BYTE aSpe; //ｱﾆﾒｽﾋﾟｰﾄﾞ
	
	BYTE muki; //向き　西:1 北:2 東:4 南:8 上:16 下:32
	
	BYTE PoE;//敵味方//0:ｽﾄｯﾌﾟ(NPC),1:通り抜け(見方ｷｬﾗ),2<<<<:大きいものは小さいものを弾く(敵,武器,魔法,ﾄﾗｯﾌﾟ)
	LPDWORD pxData;//ｷｬﾗｸﾀ画像データ先頭アド
	BYTE size[3];//大きさｻｲｽﾞ　[0] = 0~256 [1] = よこ倍率 [2] = たて倍率
	
	WORD HP[3];//ﾋｯﾄﾎﾟｲﾝﾂ:[0]=now, [1]=max, [2]=animation
	
	BYTE equips[5];//装備 0:武器
	BYTE glip[17];//手の位置 0:左　1:左　2:上　3:上　4:右　5:右　6:下　7:下 16:Zpos

	
	WORD speed[3];//速さ:[0]=maxaccel, [1]=accelcount, [2]=ﾌﾞﾚｰｷ
	BYTE atari[7];//[0,1]=X [2,3]=Y [4,5]=Z ﾀﾃ ﾖｺ ﾀｶｻ のあたり判定範囲。tigau?*全て/2の値で
	BYTE cent_p[3]; //center

	BYTE jump;
	
	
	BYTE a_mati;//次のアクションの待ち時間
	BYTE action; //大まかな行動分類
	
	BYTE HIT[64][2];//64匹までの当たり関係西:1 北:2 東:4 南:8 上:16 下:32のあたり判定


	BOOL active;//存在中のキャラ
	BYTE AItype;//0:無視、1<<<<:移動とか攻撃とか勝手にする
} chara[64];

struct weapon_data{
	char name;
	BYTE ID;
	
	WORD Xpos,Ypos,Zpos;
	WORD XCpos,YCpos;//pixelポジション
	
	BYTE glip[16];
	BYTE muki; //0:左向き 1:右向き
	BYTE pxData;//先頭アド
	BYTE kakudo; //0:０度 1:３０度 2:４５度 3:６０度
	BOOL back; //キャラの後ろか？
	
	BOOL hantei; //0:当たらない 1:当たる
	BYTE PoE;//敵か見方か

	BYTE atari[6];//bukiatari。 glipからいくつ離れてるか位置  ≪|━━━G━
//	BYTE cent_p[3];//atari用中心
	BYTE agi[8];//0:振りかぶり　1:攻撃発生中　2:余分攻撃発生　3:無効振り切り,255が入ればおわり
} weapon[64];

/*順番整列*/
void junhyouji(void){
	BYTE i = 0, j = 0, kaku = 0, count = 0;////i = 表示する順番、j = loop用カウント、kaku = 最小を保存、count = 0になったら抜け
		for(i = 0; i < a_sousu; i++){
			for(j = (i + 1); j < a_sousu; j++){
				if(chara[c_sort[j]].Ypos < chara[c_sort[i]].Ypos){
					kaku = c_sort[j];	//最前にあるものを選別
					count = j;			//何番目に入れ替えたか
				}
			}
			if(count){
				c_sort[count] = c_sort[i];	//最小の場所に入れる
				c_sort[i] = kaku;			//最小を順序に入れる
				count = 0;
			}
		}
}
	
int syokiset(void){
	BYTE h, i, j, k;
	menu[0] = 0x00;
//	keyState.osi = 0x00;
//	keyState.tame = FALSE;
	
	
	MAP.XCpos = 0x20;
	MAP.YCpos = 0x20;
	MAP.Xpos = 0x20;
	MAP.Ypos = 0x20;
	MAP.Xmas = 0x00;
	MAP.Ymas = 0x00;
	
	/*ｷｬﾗｼｮｷ*/
	for(i = 0; i < a_sousu; i++){
		chara[i].Xpos = 0x0128 + 0x0040*i*3;
		chara[i].Ypos = 0x0050 + 0x0040*i*2;
		chara[i].Zpos = 0x60;
	
		chara[i].jump = 0xa;
		
		chara[i].aTyp = 0x00;
		
		////atari800
		chara[i].atari[0] = 10;//higasi
		chara[i].atari[1] = 23;//nisi
		chara[i].atari[2] = 23;//kita
		chara[i].atari[3] = 31;//minami
		chara[i].atari[4] = 20;//atama
		chara[i].atari[5] = 0;//asi
		//////////////////
		
		////chara center//////
		chara[i].cent_p[0] = 16;//x
		chara[i].cent_p[1] = 24;//y
		chara[i].cent_p[2] = 24;//z= *2ssu
		
		chara[i].HP[0] = 65535;//現在
		chara[i].HP[1] = 65535;//MAX
		chara[i].HP[2] = 65535;//ｱﾆﾒｰｼｮﾝ
		
		chara[i].equips[0] = 1;
		
		act[i].spring_n = 0x00;
		act[i].spring_m = 0x20;
		act[i].spring_c = 0x00;
		
		for(j = 0; j < 5; j++){
			act[i].Acycle[j] = 0x00;
		}
		chara[i].speed[0] = 0x200; //max
		chara[1].speed[0] = 0x10; //max
		chara[2].speed[0] = 0x80; //max
		chara[i].speed[1] = 0x40; //count
		chara[0].speed[2] = 0x020; //bleeki
		chara[1].speed[2] = 0x004; //bleeki
		chara[2].speed[2] = 0x00b; //bleeki
		
		chara[0].PoE = 1;//chara あたり勝ち負け判定
		chara[1].PoE = 2;//chara あたり勝ち負け判定
		chara[2].PoE = 2;//chara あたり勝ち負け判定
		
		chara[i].size[0] = 32;
		chara[i].size[1] = 32;
		chara[i].size[2] = 32;
		
		act[i].asimoto = 0;//着地してるのか
	}
	
	//ｱｸﾃｨﾌﾞｷｬﾗ初期//
	for(i = 0; i < 64; i++){
		chara[i].active = FALSE;
		for(j = 0; j < 64; j++){
			chara[i].HIT[j][0] = 0x00;
			chara[i].HIT[j][1] = 0x00;
		}
	}
	chara[0].active = TRUE;
	chara[1].active = TRUE;
	chara[2].active = TRUE;
	/////////////////
	
	/*anime先頭*/
	for(i = 0; i < a_sousu; i++){
		switch(i){
			case 0: chara[i].aNam = 0x04;break;
			case 1:	chara[i].aNam = 0x02 + 0x0020;break;
			case 2:	chara[i].aNam = 0x04 + 0x0020;break;
		}
	}
	/*ｱﾆﾒｽﾋﾟｰﾄﾞ*/
	for(i = 0; i < a_sousu; i++){
		switch(i){
			case 0: chara[i].aSpe = 0x20;break;
			case 1: chara[i].aSpe = 0x20;break;
			case 2:	chara[i].aSpe = 0x3b;break;
		}
	}
	chara[1].AItype = 1;
	chara[2].AItype = 1;

//	for(i = 0; i < 24; i++){
//		for(j = 0; j < 18; j++){
//			MAP128[0].Put[j][i][0] = 0;
//		}
//	}

	
	/*順番初期*/
	for(i = 0; i < 128; i++){
		c_sort[i] = i;
	}
	for(i = 0; i < 31; i++){//ﾏｯﾌﾟﾁｯﾌﾟ要素
		mapType[i].touka = FALSE;
		
	}
	
	for(i = 0; i < 31; i++){//ﾏｯﾌﾟﾁｯﾌﾟ要素
		switch(i){
			case 0: mapType[i].gravit = p_gra;
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[j][k] = 0x00;
						}
					}break;

			case 1: mapType[i].gravit = p_gra;
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[j][k] = 0x10;
						}
					}break;

			case 4: mapType[i].gravit = p_gra;
					for(j = 0; j < 31; j++){//列・段中心へ
						for(k = j; k < 31 - j; k++){//0から始まる→1から始まる・・・/15から始まって16で終わる（最後kは1回だけ変動）
						mapType[i].height[k][j] = j + 16;//上段右に
						mapType[i].height[j][31 - k] = j + 16;// 左列上へ
						mapType[i].height[31 - j][k] = j + 16;//右列下へ
						mapType[i].height[31 - k][31 - j] = j + 16;//下段左へ
						}
					}break;

			case 8: mapType[i].gravit = p_gra;
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[j][k] = 63;
						}
					}break;

			case 9: mapType[i].gravit = p_gra;
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[j][k] = 63;
						}
					}break;

			case 17: mapType[i].gravit = p_gra;
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[k][j] = k;
						}
					}
					break;

			case 18: mapType[i].gravit = p_gra;
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[k][j] = k + 32;
						}
					}
					break;
					
			case 19: mapType[i].gravit = p_gra;//右くだり上半
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[k][j] = 63 - k;
						}
					}
					break;

			case 20: mapType[i].gravit = p_gra;//右くだり下判
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[k][j] = 31 - k;
						}
					}
					break;

			case 22: mapType[i].gravit = p_gra;
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[k][j] = k;
						}
					}
					break;

					
			case 23: mapType[i].gravit = p_gra;
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[k][j] = k + 32;
						}
					}
					break;

			case 24: mapType[i].gravit = p_gra;//kabe
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[k][j] = 63;
						}
					}

					break;
					
			case 25: mapType[i].gravit = p_gra;//右くだり上
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k ++){
							mapType[i].height[31 - k][j] = k + 32;
						}
					}
					break;

			case 26: mapType[i].gravit = p_gra;//右くだり下
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[31 - k][j] = k;
						}
					}
					break;

			case 27: mapType[i].gravit = p_gra;//のぼり切れ下
					mapType[i].touka = TRUE;
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[k][j] = k;
						}
					}
					break;

			case 28: mapType[i].gravit = p_gra;//のぼり切れ上
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[k][j] = k + 32;
						}
					}
					break;

			case 29: mapType[i].gravit = p_gra;//くだり切れ上
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[31 - k][j] = k + 32;
						}
					}
					break;

			case 30: mapType[i].gravit = p_gra;//くだり切れ下
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[31 - k][j] = k;
						}
					}
					break;

			default: mapType[i].gravit = p_gra; mapType[i].height[0][0] = 0x10;break;
		}
	}
	
	chara[0].a_mati = 0;
	chara[0].action = 0;//まち

	////////武器データ初期////////
	weapon[0].ID = 1;
	weapon[0].agi[0] = 20;//40;
	weapon[0].agi[1] = 80;//60;
	weapon[0].agi[2] = 20;//60;
	weapon[0].agi[3] = 20;
	weapon[0].agi[4] = 255;

	weapon[0].muki = 3;
	weapon[0].kakudo = 0;
	weapon[0].pxData = 0;
	
	//atari 位置
	weapon[0].atari[0] = 25;//nisi
	weapon[0].atari[1] = 13;//higasi
	weapon[0].atari[2] = 3;//kita
	weapon[0].atari[3] = 3;//minami
	weapon[0].atari[4] = 1;//ue
	weapon[0].atari[5] = 1;//sita

	//武器取っ手
	for(i = 0; i < 16; i++){
		switch(i){
			//左向き
			case 0: weapon[0].glip[i] = 25; break;/*x*/	case 8: weapon[0].glip[i] = 26; break;//y
			//上向き
			case 1: weapon[0].glip[i] = 6; break;/*x*/	case 9: weapon[0].glip[i] = 25; break;//y
			//右向き
			case 2: weapon[0].glip[i] = 6; break;/*x*/	case 10: weapon[0].glip[i] = 5; break;//y
			//下向き
			case 3: weapon[0].glip[i] = 26; break;/*x*/	case 11: weapon[0].glip[i] = 6; break;//y

			case 4: weapon[0].glip[i] = 0; break;/*x*/	case 12: weapon[0].glip[i] = 0; break;//y

			case 5: weapon[0].glip[i] = 0; break;/*x*/	case 13: weapon[0].glip[i] = 0; break;//y

			case 6: weapon[0].glip[i] = 0; break;/*x*/	case 14: weapon[0].glip[i] = 0; break;//y

			case 7: weapon[0].glip[i] = 0; break;/*x*/	case 15: weapon[0].glip[i] = 0; break;//y
			default: break;
		}
	}
	
	weapon[0].hantei = FALSE;//
	weapon[1].hantei = FALSE;//
	weapon[2].hantei = FALSE;//

	weapon[0].PoE = 3;//
	weapon[1].PoE = 2;//
	weapon[2].PoE = 2;//
	
	///プレイヤー初期////
	chara[0].equips[0] = 1;
	for(i = 0; i < 17; i++){
		switch(i){
			case 0: chara[0].glip[i] = 17; break;
			case 8: chara[0].glip[i] = 26; break;

			case 1: chara[0].glip[i] = 24; break;
			case 9: chara[0].glip[i] = 24; break;

			case 2: chara[0].glip[i] = 0; break;
			case 10: chara[0].glip[i] = 0; break;

			case 3: chara[0].glip[i] = 14; break;
			case 11: chara[0].glip[i] = 26; break;

			case 4: chara[0].glip[i] = 0; break;
			case 12: chara[0].glip[i] = 0; break;

			case 5: chara[0].glip[i] = 0; break;
			case 13: chara[0].glip[i] = 0; break;

			case 6: chara[0].glip[i] = 0; break;
			case 14: chara[0].glip[i] = 0; break;

			case 7: chara[0].glip[i] = 8; break;
			case 15: chara[0].glip[i] = 24; break;

			case 16: chara[0].glip[i] = 9; break;//Zpos
			default: break;
		}
	}
	/*******************/
}
















































