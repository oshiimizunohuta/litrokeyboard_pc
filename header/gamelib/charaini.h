///キャラクタデータ////
struct playdata{//セーブデータとか
	BYTE time[4];
	BYTE savecount;
	BYTE level;
	BYTE exp[2];//fight//eat
	BYTE HP[2];
	BYTE MP[2];//now,max
	WORD SP[3];//breath,now,max
	BYTE stre;//力
	BYTE conc;//集中力
	BYTE pass;//受身
	BYTE spir;//精神力
	BYTE quic;//敏捷
	BYTE equip[5];//装備
	BOOL event_sw[256];
	BOOL takara_sw[128];
} pdata;

////////関数検索できず

struct cha_data{
//	BYTE atari[6];//[0,1]=X [2,3]=Y [4,5]=Z ﾀﾃ ﾖｺ ﾀｶｻ のあたり判定範囲。tigau?*全て/2の値で
	BOOL active;//存在中のキャラ

	int accel_x;//加速
	int accel_y;//
	int accel_z;//
	int XpSyo;//マップ少数点以下
	int YpSyo;
	int ZpSyo;
	WORD Xpos;//マップ位置
	WORD Ypos;
	WORD Zpos;
	int XCpos;//ｶﾒﾗ位置
	int YCpos;
	int ZCpos;
	int huttobi_x;//ふっとび
	int huttobi_y;
	int huttobi_z;
	BYTE asimoto;//着地
	BYTE ataritype;//あたるタイプ 0～254:設定範囲 255:あたらない 

	

////アドレス固定ここまで
	BYTE animepat;
	BYTE setanime;
	BYTE aNamS; //ｷｬﾗｸﾀﾋﾞﾂﾄﾏﾂﾌﾟ最初のアドレス
	BYTE aNamN; //ｷｬﾗｸﾀﾋﾞﾂﾄﾏﾂﾌﾟ今のアドレス//カウントし続ける
	BYTE aNamT; //animecounttime
	BYTE aTyp; //ｷｬﾗｸﾀｱｸｼｱﾝﾀｲﾌﾟ
	WORD aSpe; //ｱﾆﾒｽﾋﾟｰﾄﾞ。１が早い。０はストップ。
	BYTE a_loop; //アニメループ
//	BOOL a_loop; //アニメループ
	
	BYTE muki; //向き　西:1 北:2 (北西:3) 東:4 (北東:6) 南:8 (南西:9) (南東:12) 上:16 下:32
	
	BYTE PoE;//敵味方//0:ｽﾄｯﾌﾟ(NPC),1:通り抜け(見方ｷｬﾗ),2<<<<:大きいものは小さいものを弾く(敵,武器,魔法,ﾄﾗｯﾌﾟ)

	BOOL pas;//T×Tなら当たらない T×Fだと当たる
	BOOL ene;//T×Tなら当たらない
	
	BYTE size[3];//大きさｻｲｽﾞ　[0] = 0~256 [1] = よこ倍率 [2] = たて倍率
	
	BYTE HP[2];//ﾋｯﾄﾎﾟｲﾝﾂ:[0]=now, [1]=max, [2]=animation
	BYTE MP[2];//ﾏｼﾞｯｷﾎﾟｲﾝﾂ:[0]=now, [1]=max, [2]=animation
	WORD SP[2];//ｽﾀﾐﾅﾎﾟｲﾝﾂ:[0]=now, [1]=max, [2]=animation

	BYTE AP[2];//ｱﾀｯｸﾎﾟｲﾝﾂ:[0]=now, [1]=kihon,
	BYTE DP[2];//ﾃﾞｨﾌｪﾝｽﾎﾟｲﾝﾂ:[0]=now, [1]=kihon,

	BYTE equiphand[2];//手装備 0:武器 1:参照weapondataNo
	BYTE glip[17];//手の位置 0:左　1:左　2:上　3:上　4:右　5:右　6:下　7:下 16:Zpos

	BYTE hand_item[2];//手に持ってるアイテム
	
	WORD speed[3];//速さ:[0]=maxaccel, [1]=accelcount, [2]=ﾌﾞﾚｰｷ
	BYTE cent_p[3]; //center

	BYTE jump_m;//jamp power
	WORD jump_t;//jump time
	BYTE jump_c;//jump time count
	
	BYTE a_mati;//次のアクションの待ち時間
	BYTE a_cnt;//アニメカウント
	BOOL action[16]; //大まかな行動分類
	BYTE actionb; //記憶
	BOOL anyact;
//	0 mati, 1 move, 2 break, 3 jump, 4 fall, 5 attack, 6 magical, 7 dam, 8 dead, 9 climb 16 emote
	
//	BYTE HIT[64][3];//64匹までの当たり関係西:1 北:2 東:4 南:8 上:16 下:32のあたり判定
					//[0]:敵味方 [1]:武器 [2]:アイテム


	BYTE AItype;//0:無視、1<<<<:移動とか攻撃とか勝手にする
	BYTE eboid;//どのBOX出生？//255なし(主人呼応toka)//boxは自身の
	
	
	BYTE Acycle[16];//0:huttobi 1:attack_false 7:dam 15die
	BOOL hold;
	BOOL bisible;

} chara[64];

struct ataridata{
	BYTE xl;
	BYTE xr;
	BYTE yu;
	BYTE yd;
	BYTE zt;
	BYTE zb;
}atari[16];

BYTE a_sousu = 0;//キャラクタ総数
BYTE c_sort[64];//キャラクタ表示順

BYTE cent32_x = 16;
BYTE cent32_y = 29;
BYTE cent32_z = 24;

BYTE cent16_x = 8;
BYTE cent16_y = 8;
BYTE cent16_z = 12;


void atari_syoki(void){
	BYTE i;
	i = 0;
	atari[i].xl = 0;//ataranai
	atari[i].xr = 0;
	atari[i].yu = 0;
	atari[i].yd = 0;
	atari[i].zt = 0;
	atari[i].zb = 0;
//	atari[i].zb = 32;
	
	i++;//1
	atari[i].xl = 9;//rane
	atari[i].xr = 23;
	atari[i].yu = 25;
	atari[i].yd = 31;
	atari[i].zt = 20;
	atari[i].zb = 0;
//	atari[i].zb = 32;
	
	i++;//2
	atari[i].xl = 0;//hoko-huri
	atari[i].xr = 32;
	atari[i].yu = 8;
	atari[i].yd = 31;
	atari[i].zt = 20;
	atari[i].zb = 0;

	i++;//3
	i++;//4
	i++;//5
	i++;//6
	i++;//7
	i++;//8
	atari[i].xl = 9;//rane
	atari[i].xr = 23;
	atari[i].yu = 25;
	atari[i].yd = 31;
	atari[i].zt = 31;
	atari[i].zb = 0;
	
	i++;//9
	i++;//10
	i++;//11
	i++;//12
	i++;//13
	i++;//14

	i++;//15
	atari[i].xl = 8;//1dot
	atari[i].xr = 8;
	atari[i].yu = 8;
	atari[i].yd = 8;
	atari[i].zt = 8;
	atari[i].zb = 8;

	i++;//16
	atari[i].xl = 4;//1dot
	atari[i].xr = 11;
	atari[i].yu = 4;
	atari[i].yd = 11;
	atari[i].zt = 11;
	atari[i].zb = 4;

	i++;//17
	i++;//18
	i++;//19
	i++;//20
	i++;//21
	i++;//22
	i++;//23
	i++;//24
	i++;//25
	i++;//26
	i++;//27
	i++;//28
	i++;//29
	i++;//30

	i++;//31
	atari[i].xl = 16;//1dot
	atari[i].xr = 16;
	atari[i].yu = 16;
	atari[i].yd = 16;
	atari[i].zt = 16;
	atari[i].zb = 16;

	i++;//32
	atari[i].xl = 16;//1dot
	atari[i].xr = 16;
	atari[i].yu = 16;
	atari[i].yd = 16;
	atari[i].zt = 16;
	atari[i].zb = 16;
/*	i++;//33
	i++;//34
	i++;//35
	i++;//36
	i++;//37
	i++;//38
	i++;//39
	i++;//40
	i++;//41
	i++;//42
	i++;//43
	i++;//44
	i++;//45
	i++;//46
	i++;//47
	i++;//48
	i++;//49
	i++;//50
	i++;//51
	i++;//52
	i++;//53
	i++;//54
	i++;//55
	i++;//56
	i++;//57
	i++;//58
	i++;//59
	i++;//60
	i++;//61
	i++;//62

	i++;//63*/
	atari[63].xl = 0;//32CUBE
	atari[63].xr = 32;
	atari[63].yu = 0;
	atari[63].yd = 32;
	atari[63].zt = 32;
	atari[63].zb = 0;
}

void enemy_syoki(BYTE ii){
	BYTE i, j;

	chara[ii].ataritype = 31;
	chara[ii].Xpos = 65535;
	chara[ii].Ypos = 65535;
	chara[ii].Zpos = 65535;
	chara[ii].cent_p[0] = 16;//x
	chara[ii].cent_p[1] = 29;//y
	chara[ii].cent_p[2] = 24;//z= *2ssu
	chara[ii].HP[0] = 3;//現在
	chara[ii].HP[1] = 25;//MAX
//	chara[ii].HP[2] = 5;//ｱﾆﾒｰｼｮﾝ
	chara[ii].MP[0] = 0;//現在
	chara[ii].MP[1] = 0;//MAX
//	chara[ii].MP[2] = 0;//ｱﾆﾒｰｼｮﾝ
	chara[ii].SP[0] = 0;//ブレス
	chara[ii].SP[1] = 0;//現在
	chara[ii].SP[2] = 0;//MAX
	chara[ii].equiphand[0] = 0;
	chara[ii].equiphand[1] = 0;
	chara[ii].jump_m = 64;
	chara[ii].jump_t = 20;
	chara[ii].jump_c = 0x0;
	chara[ii].hold = FALSE;//トマレ
	chara[ii].speed[0] = 0x100; //max
	chara[ii].speed[1] = 0x100; //count
	chara[ii].speed[2] = 0x020; //bleeki
	chara[ii].PoE = 2;//chara あたり勝ち負け判定
//	chara[ii].size[0] = chara[ii].atari[1] - chara[ii].atari[0];
//	chara[ii].size[1] = chara[ii].atari[3] - chara[ii].atari[2];
//	chara[ii].size[2] = 32;
	chara[ii].asimoto = 0;//着地してるのか
	chara[ii].aTyp = 0x00;
	chara[ii].aNamS = 0;//ｱﾆﾒｽﾀｰﾄ
	chara[ii].aSpe = 0x20;//	ｱﾆﾒｽﾋﾟｰﾄﾞ
	chara[ii].a_cnt = 0;
	chara[ii].a_loop = TRUE;
	chara[ii].a_mati = 0;
	chara[ii].action[4] = TRUE;//まち
	for(j = 0; j < 16; j++){
		chara[ii].Acycle[j] = 0;
		chara[ii].action[j] = FALSE; //大まかな行動分類
	}
	chara[j].AItype = 0;
	chara[j].eboid = 255;
}

void cuedata_cha(BYTE i, BYTE j)
{
	CopyMemory(&chara[i].active, &chara[j].active, sizeof(chara[0]));
	   //先、元、いくつ
}
void cuedata_c(BYTE i, BYTE j){//i << j
	BYTE c;
/*	chara[i].atari[0] = chara[j].atari[0];//higasi
	chara[i].atari[1] = chara[j].atari[1];//nisi
	chara[i].atari[2] = chara[j].atari[2];//kita
	chara[i].atari[3] = chara[j].atari[3];//minami
	chara[i].atari[4] = chara[j].atari[4];//atama
	chara[i].atari[5] = chara[j].atari[5];//asi
*/	chara[i].ataritype = chara[j].ataritype;
	chara[i].Xpos = chara[j].Xpos;
	chara[i].Ypos = chara[j].Ypos;
	chara[i].Zpos = chara[j].Zpos;
	chara[i].cent_p[0] = chara[j].cent_p[0];//x
	chara[i].cent_p[1] = chara[j].cent_p[1];//y
	chara[i].cent_p[2] = chara[j].cent_p[2];//z= *2ssu
	chara[i].HP[0] = chara[j].HP[0];//現在
	chara[i].HP[1] = chara[j].HP[1];//MAX
//	chara[i].HP[2] = 5;//ｱﾆﾒｰｼｮﾝ
	chara[i].MP[0] = chara[j].MP[0];//現在
	chara[i].MP[1] = chara[j].MP[1];//MAX
//	chara[i].MP[2] = 0;//ｱﾆﾒｰｼｮﾝ
	chara[i].SP[0] = chara[j].SP[0];//ブレス
	chara[i].SP[1] = chara[j].SP[1];//現在
	chara[i].SP[2] = chara[j].SP[2];//MAX
	chara[i].equiphand[0] = chara[j].equiphand[0];
	chara[i].equiphand[1] = chara[j].equiphand[1];
	chara[i].jump_m = chara[j].jump_m;
	chara[i].jump_t = chara[j].jump_t;
	chara[i].jump_c = chara[j].jump_c;
	chara[i].hold = chara[j].hold;//トマレ
	chara[i].speed[0] = chara[j].speed[0]; //max
	chara[i].speed[1] = chara[j].speed[1]; //count
	chara[i].speed[2] = chara[j].speed[2]; //bleeki
	chara[i].PoE = 2;//chara あたり勝ち負け判定
	chara[i].size[0] = chara[j].size[0];
	chara[i].size[1] = chara[j].size[1];
	chara[i].size[2] = chara[j].size[2];
	chara[i].asimoto = chara[j].asimoto;//着地してるのか
	chara[i].aTyp = chara[j].aTyp;
	chara[i].aNamS = chara[j].aNamS;//ｱﾆﾒｽﾀｰﾄ
	chara[i].aNamN = chara[j].aNamN;//ｱﾆﾒｽﾀｰﾄ
	chara[i].aNamT = chara[j].aNamT;//ｱﾆﾒｽﾀｰﾄ
	chara[i].aSpe = chara[j].aSpe;//	ｱﾆﾒｽﾋﾟｰﾄﾞ
	chara[i].a_cnt = chara[j].a_cnt;
	chara[i].a_loop = chara[j].a_loop;
	chara[i].a_mati = chara[j].a_mati;
	chara[i].actionb = chara[j].actionb;//まち
	for(c = 0; c < 16; c++){
		chara[i].Acycle[c] = chara[j].Acycle[c];
		chara[i].action[c] = chara[j].action[c];
	}

	chara[i].huttobi_x = chara[j].huttobi_x;//ふっとび
	chara[i].huttobi_y = chara[j].huttobi_y;//ふっとび
	chara[i].huttobi_y = chara[j].huttobi_z;//ふっとび

	chara[i].accel_x = chara[j].accel_x;//か
	chara[i].accel_y = chara[j].accel_y;//そ
	chara[i].accel_z = chara[j].accel_z;//く

	chara[i].AItype =  chara[j].AItype;//ai_box
	chara[i].eboid = chara[j].eboid;
//	int huttobi_y;
//	int huttobi_z;
	
//	int accel_x;//加速
//	int accel_y;//
//	int accel_z;//

}


