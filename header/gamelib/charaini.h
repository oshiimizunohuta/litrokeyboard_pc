///�L�����N�^�f�[�^////
struct playdata{//�Z�[�u�f�[�^�Ƃ�
	BYTE time[4];
	BYTE savecount;
	BYTE level;
	BYTE exp[2];//fight//eat
	BYTE HP[2];
	BYTE MP[2];//now,max
	WORD SP[3];//breath,now,max
	BYTE stre;//��
	BYTE conc;//�W����
	BYTE pass;//��g
	BYTE spir;//���_��
	BYTE quic;//�q��
	BYTE equip[5];//����
	BOOL event_sw[256];
	BOOL takara_sw[128];
} pdata;

////////�֐������ł���

struct cha_data{
//	BYTE atari[6];//[0,1]=X [2,3]=Y [4,5]=Z �� ֺ ��� �̂����蔻��͈́Btigau?*�S��/2�̒l��
	BOOL active;//���ݒ��̃L����

	int accel_x;//����
	int accel_y;//
	int accel_z;//
	int XpSyo;//�}�b�v�����_�ȉ�
	int YpSyo;
	int ZpSyo;
	WORD Xpos;//�}�b�v�ʒu
	WORD Ypos;
	WORD Zpos;
	int XCpos;//��׈ʒu
	int YCpos;
	int ZCpos;
	int huttobi_x;//�ӂ��Ƃ�
	int huttobi_y;
	int huttobi_z;
	BYTE asimoto;//���n
	BYTE ataritype;//������^�C�v 0�`254:�ݒ�͈� 255:������Ȃ� 

	

////�A�h���X�Œ肱���܂�
	BYTE animepat;
	BYTE setanime;
	BYTE aNamS; //��׸��������ߍŏ��̃A�h���X
	BYTE aNamN; //��׸��������ߍ��̃A�h���X//�J�E���g��������
	BYTE aNamT; //animecounttime
	BYTE aTyp; //��׸����������
	WORD aSpe; //��ҽ�߰�ށB�P�������B�O�̓X�g�b�v�B
	BYTE a_loop; //�A�j�����[�v
//	BOOL a_loop; //�A�j�����[�v
	
	BYTE muki; //�����@��:1 �k:2 (�k��:3) ��:4 (�k��:6) ��:8 (�쐼:9) (�쓌:12) ��:16 ��:32
	
	BYTE PoE;//�G����//0:�į��(NPC),1:�ʂ蔲��(�������),2<<<<:�傫�����̂͏��������̂�e��(�G,����,���@,�ׯ��)

	BOOL pas;//T�~T�Ȃ瓖����Ȃ� T�~F���Ɠ�����
	BOOL ene;//T�~T�Ȃ瓖����Ȃ�
	
	BYTE size[3];//�傫�����ށ@[0] = 0~256 [1] = �悱�{�� [2] = ���Ĕ{��
	
	BYTE HP[2];//˯��߲��:[0]=now, [1]=max, [2]=animation
	BYTE MP[2];//ϼޯ��߲��:[0]=now, [1]=max, [2]=animation
	WORD SP[2];//�����߲��:[0]=now, [1]=max, [2]=animation

	BYTE AP[2];//�����߲��:[0]=now, [1]=kihon,
	BYTE DP[2];//�ި̪ݽ�߲��:[0]=now, [1]=kihon,

	BYTE equiphand[2];//�葕�� 0:���� 1:�Q��weapondataNo
	BYTE glip[17];//��̈ʒu 0:���@1:���@2:��@3:��@4:�E�@5:�E�@6:���@7:�� 16:Zpos

	BYTE hand_item[2];//��Ɏ����Ă�A�C�e��
	
	WORD speed[3];//����:[0]=maxaccel, [1]=accelcount, [2]=��ڰ�
	BYTE cent_p[3]; //center

	BYTE jump_m;//jamp power
	WORD jump_t;//jump time
	BYTE jump_c;//jump time count
	
	BYTE a_mati;//���̃A�N�V�����̑҂�����
	BYTE a_cnt;//�A�j���J�E���g
	BOOL action[16]; //��܂��ȍs������
	BYTE actionb; //�L��
	BOOL anyact;
//	0 mati, 1 move, 2 break, 3 jump, 4 fall, 5 attack, 6 magical, 7 dam, 8 dead, 9 climb 16 emote
	
//	BYTE HIT[64][3];//64�C�܂ł̓�����֌W��:1 �k:2 ��:4 ��:8 ��:16 ��:32�̂����蔻��
					//[0]:�G���� [1]:���� [2]:�A�C�e��


	BYTE AItype;//0:�����A1<<<<:�ړ��Ƃ��U���Ƃ�����ɂ���
	BYTE eboid;//�ǂ�BOX�o���H//255�Ȃ�(��l�ĉ�toka)//box�͎��g��
	
	
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

BYTE a_sousu = 0;//�L�����N�^����
BYTE c_sort[64];//�L�����N�^�\����

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
	chara[ii].HP[0] = 3;//����
	chara[ii].HP[1] = 25;//MAX
//	chara[ii].HP[2] = 5;//��Ұ���
	chara[ii].MP[0] = 0;//����
	chara[ii].MP[1] = 0;//MAX
//	chara[ii].MP[2] = 0;//��Ұ���
	chara[ii].SP[0] = 0;//�u���X
	chara[ii].SP[1] = 0;//����
	chara[ii].SP[2] = 0;//MAX
	chara[ii].equiphand[0] = 0;
	chara[ii].equiphand[1] = 0;
	chara[ii].jump_m = 64;
	chara[ii].jump_t = 20;
	chara[ii].jump_c = 0x0;
	chara[ii].hold = FALSE;//�g�}��
	chara[ii].speed[0] = 0x100; //max
	chara[ii].speed[1] = 0x100; //count
	chara[ii].speed[2] = 0x020; //bleeki
	chara[ii].PoE = 2;//chara �����菟����������
//	chara[ii].size[0] = chara[ii].atari[1] - chara[ii].atari[0];
//	chara[ii].size[1] = chara[ii].atari[3] - chara[ii].atari[2];
//	chara[ii].size[2] = 32;
	chara[ii].asimoto = 0;//���n���Ă�̂�
	chara[ii].aTyp = 0x00;
	chara[ii].aNamS = 0;//��ҽ���
	chara[ii].aSpe = 0x20;//	��ҽ�߰��
	chara[ii].a_cnt = 0;
	chara[ii].a_loop = TRUE;
	chara[ii].a_mati = 0;
	chara[ii].action[4] = TRUE;//�܂�
	for(j = 0; j < 16; j++){
		chara[ii].Acycle[j] = 0;
		chara[ii].action[j] = FALSE; //��܂��ȍs������
	}
	chara[j].AItype = 0;
	chara[j].eboid = 255;
}

void cuedata_cha(BYTE i, BYTE j)
{
	CopyMemory(&chara[i].active, &chara[j].active, sizeof(chara[0]));
	   //��A���A������
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
	chara[i].HP[0] = chara[j].HP[0];//����
	chara[i].HP[1] = chara[j].HP[1];//MAX
//	chara[i].HP[2] = 5;//��Ұ���
	chara[i].MP[0] = chara[j].MP[0];//����
	chara[i].MP[1] = chara[j].MP[1];//MAX
//	chara[i].MP[2] = 0;//��Ұ���
	chara[i].SP[0] = chara[j].SP[0];//�u���X
	chara[i].SP[1] = chara[j].SP[1];//����
	chara[i].SP[2] = chara[j].SP[2];//MAX
	chara[i].equiphand[0] = chara[j].equiphand[0];
	chara[i].equiphand[1] = chara[j].equiphand[1];
	chara[i].jump_m = chara[j].jump_m;
	chara[i].jump_t = chara[j].jump_t;
	chara[i].jump_c = chara[j].jump_c;
	chara[i].hold = chara[j].hold;//�g�}��
	chara[i].speed[0] = chara[j].speed[0]; //max
	chara[i].speed[1] = chara[j].speed[1]; //count
	chara[i].speed[2] = chara[j].speed[2]; //bleeki
	chara[i].PoE = 2;//chara �����菟����������
	chara[i].size[0] = chara[j].size[0];
	chara[i].size[1] = chara[j].size[1];
	chara[i].size[2] = chara[j].size[2];
	chara[i].asimoto = chara[j].asimoto;//���n���Ă�̂�
	chara[i].aTyp = chara[j].aTyp;
	chara[i].aNamS = chara[j].aNamS;//��ҽ���
	chara[i].aNamN = chara[j].aNamN;//��ҽ���
	chara[i].aNamT = chara[j].aNamT;//��ҽ���
	chara[i].aSpe = chara[j].aSpe;//	��ҽ�߰��
	chara[i].a_cnt = chara[j].a_cnt;
	chara[i].a_loop = chara[j].a_loop;
	chara[i].a_mati = chara[j].a_mati;
	chara[i].actionb = chara[j].actionb;//�܂�
	for(c = 0; c < 16; c++){
		chara[i].Acycle[c] = chara[j].Acycle[c];
		chara[i].action[c] = chara[j].action[c];
	}

	chara[i].huttobi_x = chara[j].huttobi_x;//�ӂ��Ƃ�
	chara[i].huttobi_y = chara[j].huttobi_y;//�ӂ��Ƃ�
	chara[i].huttobi_y = chara[j].huttobi_z;//�ӂ��Ƃ�

	chara[i].accel_x = chara[j].accel_x;//��
	chara[i].accel_y = chara[j].accel_y;//��
	chara[i].accel_z = chara[j].accel_z;//��

	chara[i].AItype =  chara[j].AItype;//ai_box
	chara[i].eboid = chara[j].eboid;
//	int huttobi_y;
//	int huttobi_z;
	
//	int accel_x;//����
//	int accel_y;//
//	int accel_z;//

}


