/*�Q�[���S�ʕϐ�*/

static WORD CposX = 0, CposY = 0, CposZ = 0, MposX = 0, MposY = 0, MposZ = 0x80, mapX = 0, mapY = 0, mapZ = 0; /*��ʕ\���p�L�����ʒu�ϐ�*/
WORD UniPosX[64] = {0x0000}, UniPosY[64] = {0x0000}, UniPosZ[64] = {0x0000};/*�Q�[�����̃L�����ʒu�ϐ�*/
WORD pix_page = 0;
BYTE a_sousu = 3;
BYTE p_gra = 0x10;
BYTE false_time = 0;
BYTE list_ofs = 0;//menu list���炵�\��

LPDWORD lpBBPixel;//�I�d�v�I

BYTE fwork = 0;

//debug�p�H
BYTE scroll = 0x00, MNo = 0x0000, menu[2] = {0x00, 0x00};
BYTE dSize = 0x0001;
BOOL c_on = FALSE;
BOOL dsamp = FALSE;
BYTE sno = 0;

DWORD testP[16];
////////


BYTE jNum = 0x01, jMax = 0x01;

BYTE d_kazu;//�����\���p�ϐ�
BOOL maski = TRUE;//�}�X�N���܂����B
int paintDo = 0;
BOOL osippa = FALSE;



float Cc = 0;//�}�b�v�X�N���[���X�s�[�h

BYTE c_sort[128];


/*���C���|�W�V����*/
WORD XmasW, XmasE, YmasN, YmasS, Zmas;
BYTE XposW, XposE, YposN, YposS;

/*�T�u�|�W�V����*/
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
	

}atyp = {0,1,2,3,	4,5,6,7,	8,9,10,11,	18,13};//move �� attack��4�̊Ԃ��J���Ă��邱�ƁB�A�^�b�N�֐��Ŏg�p��

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
	
	WORD Xpos;//�x�[�X�}�b�v�ɑ΂��Ă�X�|�W�V����
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
	
	int huttobi_x;//�ӂ��Ƃ�
	int huttobi_y;
	int huttobi_z;
	
	int accel_x;//����
	int accel_y;//
	int accel_z;//
	
	BYTE asimoto;
	
	BYTE Acycle[5];//0:huttobi 1:attack_false
	} act[128];


struct cha_data{
	WORD Xpos;//�}�b�v�ʒu
	WORD Ypos;
	WORD Zpos;
	
	int XpSyo;//�}�b�v�����_�ȉ�
	int YpSyo;
	int ZpSyo;
	
	
	WORD XCpos;//��׈ʒu
	WORD YCpos;
	WORD ZCpos;

	BYTE aNam; //��׸��������߉��ԖځH
	BYTE aTyp; //��׸���è������
	BYTE aSpe; //��ҽ�߰��
	
	BYTE muki; //�����@��:1 �k:2 ��:4 ��:8 ��:16 ��:32
	
	BYTE PoE;//�G����//0:�į��(NPC),1:�ʂ蔲��(�������),2<<<<:�傫�����̂͏��������̂�e��(�G,����,���@,�ׯ��)
	LPDWORD pxData;//��׸��摜�f�[�^�擪�A�h
	BYTE size[3];//�傫�����ށ@[0] = 0~256 [1] = �悱�{�� [2] = ���Ĕ{��
	
	WORD HP[3];//˯��߲��:[0]=now, [1]=max, [2]=animation
	
	BYTE equips[5];//���� 0:����
	BYTE glip[17];//��̈ʒu 0:���@1:���@2:��@3:��@4:�E�@5:�E�@6:���@7:�� 16:Zpos

	
	WORD speed[3];//����:[0]=maxaccel, [1]=accelcount, [2]=��ڰ�
	BYTE atari[7];//[0,1]=X [2,3]=Y [4,5]=Z �� ֺ ��� �̂����蔻��͈́Btigau?*�S��/2�̒l��
	BYTE cent_p[3]; //center

	BYTE jump;
	
	
	BYTE a_mati;//���̃A�N�V�����̑҂�����
	BYTE action; //��܂��ȍs������
	
	BYTE HIT[64][2];//64�C�܂ł̓�����֌W��:1 �k:2 ��:4 ��:8 ��:16 ��:32�̂����蔻��


	BOOL active;//���ݒ��̃L����
	BYTE AItype;//0:�����A1<<<<:�ړ��Ƃ��U���Ƃ�����ɂ���
} chara[64];

struct weapon_data{
	char name;
	BYTE ID;
	
	WORD Xpos,Ypos,Zpos;
	WORD XCpos,YCpos;//pixel�|�W�V����
	
	BYTE glip[16];
	BYTE muki; //0:������ 1:�E����
	BYTE pxData;//�擪�A�h
	BYTE kakudo; //0:�O�x 1:�R�O�x 2:�S�T�x 3:�U�O�x
	BOOL back; //�L�����̌�납�H
	
	BOOL hantei; //0:������Ȃ� 1:������
	BYTE PoE;//�G��������

	BYTE atari[6];//bukiatari�B glip���炢������Ă邩�ʒu  ��|������G��
//	BYTE cent_p[3];//atari�p���S
	BYTE agi[8];//0:�U�肩�Ԃ�@1:�U���������@2:�]���U�������@3:�����U��؂�,255������΂����
} weapon[64];

/*���Ԑ���*/
void junhyouji(void){
	BYTE i = 0, j = 0, kaku = 0, count = 0;////i = �\�����鏇�ԁAj = loop�p�J�E���g�Akaku = �ŏ���ۑ��Acount = 0�ɂȂ����甲��
		for(i = 0; i < a_sousu; i++){
			for(j = (i + 1); j < a_sousu; j++){
				if(chara[c_sort[j]].Ypos < chara[c_sort[i]].Ypos){
					kaku = c_sort[j];	//�őO�ɂ�����̂�I��
					count = j;			//���Ԗڂɓ���ւ�����
				}
			}
			if(count){
				c_sort[count] = c_sort[i];	//�ŏ��̏ꏊ�ɓ����
				c_sort[i] = kaku;			//�ŏ��������ɓ����
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
	
	/*��׼��*/
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
		
		chara[i].HP[0] = 65535;//����
		chara[i].HP[1] = 65535;//MAX
		chara[i].HP[2] = 65535;//��Ұ���
		
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
		
		chara[0].PoE = 1;//chara �����菟����������
		chara[1].PoE = 2;//chara �����菟����������
		chara[2].PoE = 2;//chara �����菟����������
		
		chara[i].size[0] = 32;
		chara[i].size[1] = 32;
		chara[i].size[2] = 32;
		
		act[i].asimoto = 0;//���n���Ă�̂�
	}
	
	//��è�޷�׏���//
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
	
	/*anime�擪*/
	for(i = 0; i < a_sousu; i++){
		switch(i){
			case 0: chara[i].aNam = 0x04;break;
			case 1:	chara[i].aNam = 0x02 + 0x0020;break;
			case 2:	chara[i].aNam = 0x04 + 0x0020;break;
		}
	}
	/*��ҽ�߰��*/
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

	
	/*���ԏ���*/
	for(i = 0; i < 128; i++){
		c_sort[i] = i;
	}
	for(i = 0; i < 31; i++){//ϯ�����ߗv�f
		mapType[i].touka = FALSE;
		
	}
	
	for(i = 0; i < 31; i++){//ϯ�����ߗv�f
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
					for(j = 0; j < 31; j++){//��E�i���S��
						for(k = j; k < 31 - j; k++){//0����n�܂遨1����n�܂�E�E�E/15����n�܂���16�ŏI���i�Ō�k��1�񂾂��ϓ��j
						mapType[i].height[k][j] = j + 16;//��i�E��
						mapType[i].height[j][31 - k] = j + 16;// ������
						mapType[i].height[31 - j][k] = j + 16;//�E�񉺂�
						mapType[i].height[31 - k][31 - j] = j + 16;//���i����
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
					
			case 19: mapType[i].gravit = p_gra;//�E������㔼
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[k][j] = 63 - k;
						}
					}
					break;

			case 20: mapType[i].gravit = p_gra;//�E�����艺��
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
					
			case 25: mapType[i].gravit = p_gra;//�E�������
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k ++){
							mapType[i].height[31 - k][j] = k + 32;
						}
					}
					break;

			case 26: mapType[i].gravit = p_gra;//�E�����艺
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[31 - k][j] = k;
						}
					}
					break;

			case 27: mapType[i].gravit = p_gra;//�̂ڂ�؂ꉺ
					mapType[i].touka = TRUE;
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[k][j] = k;
						}
					}
					break;

			case 28: mapType[i].gravit = p_gra;//�̂ڂ�؂��
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[k][j] = k + 32;
						}
					}
					break;

			case 29: mapType[i].gravit = p_gra;//������؂��
					for(j = 0; j < 32; j++){
						for(k = 0; k < 32; k++){
							mapType[i].height[31 - k][j] = k + 32;
						}
					}
					break;

			case 30: mapType[i].gravit = p_gra;//������؂ꉺ
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
	chara[0].action = 0;//�܂�

	////////����f�[�^����////////
	weapon[0].ID = 1;
	weapon[0].agi[0] = 20;//40;
	weapon[0].agi[1] = 80;//60;
	weapon[0].agi[2] = 20;//60;
	weapon[0].agi[3] = 20;
	weapon[0].agi[4] = 255;

	weapon[0].muki = 3;
	weapon[0].kakudo = 0;
	weapon[0].pxData = 0;
	
	//atari �ʒu
	weapon[0].atari[0] = 25;//nisi
	weapon[0].atari[1] = 13;//higasi
	weapon[0].atari[2] = 3;//kita
	weapon[0].atari[3] = 3;//minami
	weapon[0].atari[4] = 1;//ue
	weapon[0].atari[5] = 1;//sita

	//��������
	for(i = 0; i < 16; i++){
		switch(i){
			//������
			case 0: weapon[0].glip[i] = 25; break;/*x*/	case 8: weapon[0].glip[i] = 26; break;//y
			//�����
			case 1: weapon[0].glip[i] = 6; break;/*x*/	case 9: weapon[0].glip[i] = 25; break;//y
			//�E����
			case 2: weapon[0].glip[i] = 6; break;/*x*/	case 10: weapon[0].glip[i] = 5; break;//y
			//������
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
	
	///�v���C���[����////
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
















































