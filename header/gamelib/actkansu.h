/*�A�N�V�����n�֐��錾����*/

void action(void); /*��������*/
void mati(void);
BYTE HEXcircle(BYTE kakudo/*�p�x16����*/, BYTE vecter, BOOL Ymode);

void scrollCheck(void);
void menuSelect(void);
void sampling(void);

WORD subposget(BYTE i, BYTE op, BYTE HL){//�k�@���@���@��
	sub_m = 0;
	sub_h = 0;
	
	subOK = FALSE;
//	ZmasH = 0;
	subZmasL = 0;
	subXmas = 0;
	subYmas = 0;
	subXpos = 0;
	subYpos = 0;

	if((chara[i].Xpos >= MAP128[0].Xpos) && (chara[i].Xpos <= MAP128[0].Xpos + (MAP128[0].hirosa[0] * 32)) && (chara[i].Ypos >= MAP128[0].Ypos) && (chara[i].Ypos <= MAP128[0].Ypos + (MAP128[0].hirosa[1] * 32))){

		subZmasL = (((HL) + chara[i].Zpos + chara[i].atari[5] - MAP128[0].Zpos + 0.1) / 64);//asi
		subZmasH = subZmasL + 1;//= ((chara[i].Zpos + chara[i].atari[4] - MAP128[0].Zpos + 0.1) / 64);//atama
		

		switch(op){
			case 0:	subXmas = (chara[i].Xpos + chara[i].cent_p[0] - MAP128[0].Xpos) / 32;//center oti
					subXpos = (chara[i].Xpos + chara[i].cent_p[0] - MAP128[0].Xpos) % 32;
					subYmas = (chara[i].Ypos + chara[i].atari[2] - MAP128[0].Ypos) / 32;//kita oti
					subYpos = (chara[i].Ypos + chara[i].atari[2] - MAP128[0].Ypos) % 32;
					break;

			case 1:	subXmas = (chara[i].Xpos + chara[i].atari[1] - MAP128[0].Xpos) / 32;//nisi oti
					subXpos = (chara[i].Xpos + chara[i].atari[1] - MAP128[0].Xpos) % 32;
					subYmas = (chara[i].Ypos + chara[i].cent_p[1] - MAP128[0].Ypos) / 32;////center oti
					subYpos = (chara[i].Ypos + chara[i].cent_p[1] - MAP128[0].Ypos) % 32;
					break;
			
			case 2:	subXmas = (chara[i].Xpos + chara[i].atari[0] - MAP128[0].Xpos) / 32;//higasi oti
					subXpos = (chara[i].Xpos + chara[i].atari[0] - MAP128[0].Xpos) % 32;
					subYmas = (chara[i].Ypos + chara[i].cent_p[1] - MAP128[0].Ypos) / 32;////center oti
					subYpos = (chara[i].Ypos + chara[i].cent_p[1] - MAP128[0].Ypos) % 32;
					break;

			case 3:	subXmas = (chara[i].Xpos + chara[i].cent_p[0] - MAP128[0].Xpos) / 32;//center oti
					subXpos = (chara[i].Xpos + chara[i].cent_p[0] - MAP128[0].Xpos) % 32;
					subYmas = (chara[i].Ypos + chara[i].atari[3] - MAP128[0].Ypos) / 32;//minami oti
					subYpos = (chara[i].Ypos + chara[i].atari[3] - MAP128[0].Ypos) % 32;
					break;

			case 4:	subXmas = (chara[i].Xpos + chara[i].cent_p[0] - MAP128[0].Xpos) / 32;//center oti
					subXpos = (chara[i].Xpos + chara[i].cent_p[0] - MAP128[0].Xpos) % 32;//center oti
					subYmas = (chara[i].Ypos + chara[i].cent_p[1] - MAP128[0].Ypos) / 32;//center oti
					subYpos = (chara[i].Ypos + chara[i].cent_p[1] - MAP128[0].Ypos) % 32;//center oti
					break;

			
			default:break;
		}
		
//		if(HL){//masu yori ue ikisugi
//			sub_m = mapType[MAP128[0].Put[subXmas + (subYmas * MAP128[0].hirosa[0])][subZmasH]].height[subXpos][subYpos] + (subZmasH * 63) + MAP128[0].Zpos;
//		}
		
//		else{
			sub_m = mapType[MAP128[0].Put[subXmas + (subYmas * MAP128[0].hirosa[0])][subZmasL]].height[subXpos][subYpos] + (subZmasL * 63) + MAP128[0].Zpos;
//		}
		
		subOK = TRUE;

		
		return(sub_m);
	}
}

void mainposget(BYTE i, BYTE op){
//	mainOK = FALSE;
	Zmas = 0;
	XmasE = 0; XmasW = 0;
	YmasN = 0; YmasS = 0;
	XposE = 0; XposW = 0;
	YposN = 0; YposS = 0;
	
//	Zmas = (chara[i].Zpos - MAP128[0].Zpos) / 64;

//	Xmas = (chara[i].Xpos + chara[i].atari[0]) / 32;
			
/*	XmasE = (chara[i].Xpos + chara[i].atari[0]) / 32;
	XmasW = (chara[i].Xpos + chara[i].atari[1]) / 32;
	YmasN = (chara[i].Ypos + chara[i].atari[2]) / 32;
	YmasS = (chara[i].Ypos + chara[i].atari[3]) / 32;
	XposE = (chara[i].Xpos + chara[i].atari[0]) % 32;
	XposW = (chara[i].Xpos + chara[i].atari[1]) % 32;
	YposN = (chara[i].Ypos + chara[i].atari[2]) % 32;
	YposS = (chara[i].Ypos + chara[i].atari[3]) % 32;
*/			
//		subOK = TRUE;
	
}


void ene_move(void){
	BYTE i;
	for(i = 0; i < a_sousu; i++){
		ene_AI(i);
		//if(chara[i].PoE == 2){
		//	ene_AI(i);
		//}
	}
}

int huttobi(BYTE i){
	act[i].accel_x = act[i].huttobi_x;
	act[i].accel_y = act[i].huttobi_y;
	act[i].accel_z = act[i].huttobi_z;

	act[i].accel_z++;//�������Ȃ����accel�����͂��Ȃ�

	act[i].Acycle[0]--;
}

int damage(BYTE j, BYTE i){
	CopyMemory(lpPCM, eSnd[2].aSnd, 22050);
	PlaySound(lpBuf, NULL, SND_MEMORY | SND_ASYNC);

	chara[j].HP[0] -= 4;
//	if(chara[j].HIT[i][1]){
		act[j].Acycle[0] = 10;
		act[j].huttobi_x = (act[j].accel_x - (act[i].accel_x * chara[i].PoE)) * -1;
		if((act[j].huttobi_x < 1000) && (act[j].huttobi_x > 0) && (act[j].huttobi_x > act[j].huttobi_y)){act[j].huttobi_x = 1000;}//��������іh�~
		if(act[j].huttobi_x > -1000 && act[j].huttobi_x < 0 && (act[j].huttobi_x < act[j].huttobi_y)){act[j].huttobi_x = -1000;}

		act[j].huttobi_y = (act[j].accel_y - (act[i].accel_y * chara[i].PoE)) * -1;
		if(act[j].huttobi_y < 1000 && act[j].huttobi_y > 0 && (act[j].huttobi_x < act[j].huttobi_y)){act[j].huttobi_y = 1000;}
		if(act[j].huttobi_y > -1000 && act[j].huttobi_y < 0 && (act[j].huttobi_x > act[j].huttobi_y)){act[j].huttobi_y = -1000;}

		act[j].huttobi_z = 500;

//		act[j].accel_z = 1080;//�ݒ�G����I���[�I�I
		act[j].spring_m = 0;
		act[j].spring_c = 0;
		
//	}
	chara[j].HIT[i][0] = 0x00;//pri_hantei(3F�ɂȂ����甭��)
	chara[j].HIT[i][1] = 0x00;//sec_hantei(3F�ɂȂ�O�̓������ԁB�������������𔻒�)
}


////////���킠����////////
//     chara0buki[j]
//         ��
// ������������������
// 0 1 2 3 4 5 6 7 8 .....[i]
//���L�L�L�L�L�L�L�L

//     chara1buki[j]
//         ��
// ������������������
// 0 1 2 3 4 5 6 7 8 .....[i]
//�L���L�L�L�L�L�L�L


void bukiatari(void){
	BYTE i = 0, j = 0, p = 0;
	BYTE l_kakudo;
	BYTE Le/*ngth*/, En/*d Length*/, Wr/*idth*//*ight*/, Wl/*idth*//*eft*/;
	WORD left_a[2], right_a[2], up_a[2], down_a[2], head_a[2], foot_a[2];//0:chara 1:buki
	
//	DWORD lpB[4];
	
	for(j = 0; j < a_sousu; j++){//buki�����L����
		if((a_sousu == 0)){break;} //�P��ׂ��������ĂȂ��Ƃ�����
		for(i = 0; i < a_sousu; i++){//0�Ԗڔ��肵�Ȃ�����
			if(j != i && (weapon[j].hantei == TRUE)){//����̎�����L�����𔻒肵�Ȃ�,���펝���ĂȂ��ЂƂ�

				left_a[0] = chara[i].Xpos + chara[i].atari[0];//�񔻒�L����

				right_a[0] = chara[i].Xpos + chara[i].atari[1];//�񔻒�L����

				up_a[0] = chara[i].Ypos + chara[i].atari[2];//�񔻒�L����

				down_a[0] = chara[i].Ypos + chara[i].atari[3];//�񔻒�L����
				
				head_a[1] = (chara[j].glip[16]/*z glip*/ + chara[j].Zpos) + weapon[j].atari[4];//����L����
				head_a[0] = chara[i].Zpos + chara[i].atari[4];//�񔻒�L����

				foot_a[1] = (chara[j].glip[16] + chara[j].Zpos) + weapon[j].atari[5];//����L����
				foot_a[0] = chara[i].Zpos + chara[i].atari[5];//�񔻒�L����
			
				l_kakudo = weapon[j].kakudo / 4;

				Le = HEXcircle(weapon[j].kakudo, weapon[j].atari[0], FALSE);
				En = HEXcircle(weapon[j].kakudo, weapon[j].atari[1], FALSE);
				Wr = HEXcircle(weapon[j].kakudo, weapon[j].atari[2], TRUE);
				Wl = HEXcircle(weapon[j].kakudo, weapon[j].atari[3], TRUE);
				switch(l_kakudo){
					////////������
					case 0: Le = HEXcircle(weapon[j].kakudo, weapon[j].atari[0], FALSE);
							En = HEXcircle(weapon[j].kakudo, weapon[j].atari[1], FALSE);
							Wr = HEXcircle(weapon[j].kakudo, weapon[j].atari[2], TRUE);
							Wl = HEXcircle(weapon[j].kakudo, weapon[j].atari[3], TRUE);

							left_a[1] = /*���ӁF�\�����Ă���v�Z*/weapon[j].Xpos + /*glip����̈ʒu*/weapon[j].glip[0] - Le + Wr;//����buki

							right_a[1] = weapon[j].Xpos + weapon[j].glip[0] - En + Wl;//����L����

							Wr = HEXcircle(weapon[j].kakudo, weapon[j].atari[2], FALSE);//weapon[j].atari[2];//HEXcircle(weapon[j].kakLedo, weapon[j].atari[2], FALSE);
							Wl = HEXcircle(weapon[j].kakudo, weapon[j].atari[3], FALSE);//weapon[j].atari[3];//HEXcircle(weapon[j].kakudo, weapon[j].atari[3], FALSE);
							Le = HEXcircle(weapon[j].kakudo, weapon[j].atari[0], TRUE);
							En = HEXcircle(weapon[j].kakudo, weapon[j].atari[1], TRUE);//���Ȃ��H

							up_a[1] = weapon[j].Ypos + weapon[j].glip[8] - Wr - Le;//����L����

							down_a[1] = weapon[j].Ypos + weapon[j].glip[8] + Wl - En;//����L����
							break;

					////////�����
					case 1: Le = HEXcircle(weapon[j].kakudo, weapon[j].atari[0], TRUE);
							En = HEXcircle(weapon[j].kakudo, weapon[j].atari[1], TRUE);
							Wr = HEXcircle(weapon[j].kakudo, weapon[j].atari[2], FALSE);
							Wl = HEXcircle(weapon[j].kakudo, weapon[j].atari[3], FALSE);
							
							up_a[1] = weapon[j].Ypos + weapon[j].glip[9] - Le + Wl;//����L����

							down_a[1] = weapon[j].Ypos + weapon[j].glip[9] - En + Wr;//����L����

							Wr = HEXcircle(weapon[j].kakudo, weapon[j].atari[2], TRUE);
							Wl = HEXcircle(weapon[j].kakudo, weapon[j].atari[3], TRUE);
							Le = HEXcircle(weapon[j].kakudo, weapon[j].atari[0], FALSE);
							En = HEXcircle(weapon[j].kakudo, weapon[j].atari[1], FALSE);

							left_a[1] = weapon[j].Xpos + weapon[j].glip[1] - Wl + En;

							right_a[1] = weapon[j].Xpos + weapon[j].glip[1] + Wr + Le;//����L����

							break;

					////////�E����
					case 2: Le = HEXcircle(weapon[j].kakudo, weapon[j].atari[0], FALSE);
							En = HEXcircle(weapon[j].kakudo, weapon[j].atari[1], FALSE);
							Wr = HEXcircle(weapon[j].kakudo, weapon[j].atari[2], TRUE);
							Wl = HEXcircle(weapon[j].kakudo, weapon[j].atari[3], TRUE);

							left_a[1] = weapon[j].Xpos + weapon[j].glip[2] + En + Wl;

							right_a[1] = weapon[j].Xpos + weapon[j].glip[2] + Le + Wr;//����L����

							Wr = HEXcircle(weapon[j].kakudo, weapon[j].atari[2], FALSE);//weapon[j].atari[2];//HEXcircle(weapon[j].kakLedo, weapon[j].atari[2], FALSE);
							Wl = HEXcircle(weapon[j].kakudo, weapon[j].atari[3], FALSE);//weapon[j].atari[3];//HEXcircle(weapon[j].kakudo, weapon[j].atari[3], FALSE);
							Le = HEXcircle(weapon[j].kakudo, weapon[j].atari[0], TRUE);
							En = HEXcircle(weapon[j].kakudo, weapon[j].atari[1], TRUE);//���Ȃ��H

							up_a[1] = weapon[j].Ypos + weapon[j].glip[10] - Wl + En;//����L����

							down_a[1] = weapon[j].Ypos + weapon[j].glip[10] + Wr + Le;//����L����
							break;

					////////������
					case 3: Le = HEXcircle(weapon[j].kakudo, weapon[j].atari[0], TRUE);
							En = HEXcircle(weapon[j].kakudo, weapon[j].atari[1], TRUE);
							Wr = HEXcircle(weapon[j].kakudo, weapon[j].atari[2], FALSE);
							Wl = HEXcircle(weapon[j].kakudo, weapon[j].atari[3], FALSE);
						
							up_a[1] = weapon[j].Ypos + weapon[j].glip[11] + En - Wr;//����L����

							down_a[1] = weapon[j].Ypos + weapon[j].glip[11] + Le + Wl;//����L����

							Wr = HEXcircle(weapon[j].kakudo, weapon[j].atari[2], TRUE);
							Wl = HEXcircle(weapon[j].kakudo, weapon[j].atari[3], TRUE);
							Le = HEXcircle(weapon[j].kakudo, weapon[j].atari[0], FALSE);
							En = HEXcircle(weapon[j].kakudo, weapon[j].atari[1], FALSE);

							left_a[1] = weapon[j].Xpos + weapon[j].glip[3] - Wr - Le;

							right_a[1] = weapon[j].Xpos + weapon[j].glip[3] + Wl - En;//����L����

							break;
					default: break;
				}
				

				if(left_a[0] <= right_a[1]){chara[i].HIT[j][1] = chara[i].HIT[j][1] | 0x01;}//X���W��������
				else{chara[i].HIT[j][1] = chara[i].HIT[j][1] &~ 0x01;}
				
				if(right_a[0] >= left_a[1]){chara[i].HIT[j][1] = chara[i].HIT[j][1] | 0x04;}//X��������
				else{chara[i].HIT[j][1] = chara[i].HIT[j][1] &~ 0x04;}
				
				if(up_a[0] <= down_a[1]){chara[i].HIT[j][1] = chara[i].HIT[j][1] | 0x02;}//Y���W�k������
				else{chara[i].HIT[j][1] = chara[i].HIT[j][1] &~ 0x02;}
				
				if(down_a[0] >= up_a[1]){chara[i].HIT[j][1] = chara[i].HIT[j][1] | 0x08;}//Y���W�삠����
				else{chara[i].HIT[j][1] = chara[i].HIT[j][1] &~ 0x08;}
				
				if(foot_a[0] <= head_a[1]){chara[i].HIT[j][1] = chara[i].HIT[j][1] | 0x10;}//Z���W��������
				else{chara[i].HIT[j][1] = chara[i].HIT[j][1] &~ 0x10;}
				
				if(head_a[0] >= foot_a[1]){chara[i].HIT[j][1] = chara[i].HIT[j][1] | 0x20;}//Z���W�゠����
				else{chara[i].HIT[j][1] = chara[i].HIT[j][1] &~ 0x20;}///////1111 0001 : 1110 / 1110 0001: 1110 / 0001 0001: 0000				

				if((chara[i].HIT[j][1] == 0x3f) && (weapon[j].PoE > chara[i].PoE)){
					damage(i,j);//huttobe-

				}
//				chara[i].HIT[j][1] = 0x00;//�����L������hit�Ȃ����B��޺���μ�

				if(j == 0){
					up_a[1] = up_a[1] - MAP.Ypos - chara[0].Zpos/2;
					left_a[1] = left_a[1] - MAP.Xpos;
					testP[0] = left_a[1] + (up_a[1] * 320);
					testP[1] = left_a[1] + 1 + (up_a[1] * 320);
					testP[2] = left_a[1] + ((up_a[1] + 1) * 320);
					testP[3] = left_a[1] + 1 + ((up_a[1] + 1) * 320);
					
					down_a[1] = down_a[1] - MAP.Ypos - chara[0].Zpos/2;
					right_a[1] = right_a[1] - MAP.Xpos;

					testP[4] = right_a[1] + (down_a[1] * 320);
					testP[5] = right_a[1] + 1 + (down_a[1] * 320);
					testP[6] = right_a[1] + ((down_a[1] + 1) * 320);
					testP[7] = right_a[1] + 1 + ((down_a[1] + 1) * 320);

				}
			}
		}
	}
}


////////�����蔻��////////
void atarihantei(void){
	BYTE i = 0, j = 0;
	WORD half_x1,bottom_y1,half_x2,bottom_y2;
	WORD left_a[2], right_a[2], up_a[2], down_a[2], head_a[2], foot_a[2];
	
	for(j = 0; j < a_sousu - 1; j++){
		if(a_sousu == 0){break;} //�P��ׂ��������ĂȂ��Ƃ�����
		for(i = 1; i < a_sousu; i++){//0�Ԗڔ��肵�Ȃ�����
			if(j < i){//�����L�����𔻒肵�Ȃ�
//				chara[j].HIT[i][1] = chara[j].HIT[i][0]; //�O��̍��W��v���L��

				left_a[0] = chara[j].Xpos + chara[j].atari[0];//����L����
				left_a[1] = chara[i].Xpos + chara[i].atari[0];//�񔻒�L����

				right_a[0] = chara[j].Xpos + chara[j].atari[1];//����L����
				right_a[1] = chara[i].Xpos + chara[i].atari[1];//�񔻒�L����

				up_a[0] = chara[j].Ypos + chara[j].atari[2];//����L����
				up_a[1] = chara[i].Ypos + chara[i].atari[2];//�񔻒�L����

				down_a[0] = chara[j].Ypos + chara[j].atari[3];//����L����
				down_a[1] = chara[i].Ypos + chara[i].atari[3];//�񔻒�L����

				head_a[0] = chara[j].Zpos + chara[j].atari[4];//����L����
				head_a[1] = chara[i].Zpos + chara[i].atari[4];//�񔻒�L����

				foot_a[0] = chara[j].Zpos + chara[j].atari[5];//����L����
				foot_a[1] = chara[i].Zpos + chara[i].atari[5];//�񔻒�L����


				if(left_a[0] <= right_a[1]){chara[j].HIT[i][0] = chara[j].HIT[i][0] | 0x01;}//X���W��������
				else{chara[j].HIT[i][0] = chara[j].HIT[i][0] &~ 0x01;}
				
				if(right_a[0] >= left_a[1]){chara[j].HIT[i][0] = chara[j].HIT[i][0] | 0x04;}//X��������
				else{chara[j].HIT[i][0] = chara[j].HIT[i][0] &~ 0x04;}
				
				if(up_a[0] <= down_a[1]){chara[j].HIT[i][0] = chara[j].HIT[i][0] | 0x02;}//Y���W�k������
				else{chara[j].HIT[i][0] = chara[j].HIT[i][0] &~ 0x02;}
				
				if(down_a[0] >= up_a[1]){chara[j].HIT[i][0] = chara[j].HIT[i][0] | 0x08;}//Y���W�삠����
				else{chara[j].HIT[i][0] = chara[j].HIT[i][0] &~ 0x08;}
				
				if(foot_a[0] <= head_a[1]){chara[j].HIT[i][0] = chara[j].HIT[i][0] | 0x10;}//Z���W��������
				else{chara[j].HIT[i][0] = chara[j].HIT[i][0] &~ 0x10;}
				
				if(head_a[0] >= foot_a[1]){chara[j].HIT[i][0] = chara[j].HIT[i][0] | 0x20;}//Z���W�゠����
				else{chara[j].HIT[i][0] = chara[j].HIT[i][0] &~ 0x20;}///////1111 0001 : 1110 / 1110 0001: 1110 / 0001 0001: 0000				
				if(chara[j].HIT[i][0] == 0x3f){
/*					switch(chara[j].HIT[i][1]){
						case 0x1f://��ɏ�Q���a�B
						case 0x2f://���ɏ�Q���a�B
						case 0x37://��ɏ�Q���a�B
						case 0x3b://���ɏ�Q���a�B
						case 0x3d://�k�ɏ�Q���a�B
						case 0x3e://���ɏ�Q���a�B
*/						if(chara[i].PoE > chara[j].PoE){//������������j�̕���
							damage(j,i);
							
						}
						if(chara[i].PoE < chara[j].PoE){//������������i�̕���
							damage(i,j);

						}
						
						
//						break;
					
//						default:/*damage(j,i)*/;break;
//					}
					
					//chara[j].HIT[i][1] = chara[j].HIT[i][0]; //�O��̍��W��v���L��
				}
			}
		}
	}
}

void c_check(void){
	BYTE i;
	a_sousu = 0;
	for(i = 0; chara[i].active == TRUE; i++){
		a_sousu++;
	}
}

void debugdisp(void){
	switch(menu[MNo]){
		case 0: dDisp[0] = dDisp[4 - (c_on * 2)]; dDisp[1] = dDisp[5 - (c_on * 2)];break;//scroll
		case 1: dDisp[0] = dDisp[10];  dDisp[1] = dDisp[11]; break;//disp
		case 2: dDisp[0] = dDisp[10];  dDisp[1] = dDisp[11]; break;//jump
		case 3: dDisp[0] = dDisp[10];  dDisp[1] = dDisp[11]; if(maski){sampling();}break;//sound//kankeinai hennsuu wo motidasitemasuyo
		case 4: dDisp[0] = dDisp[10 + (tgFlame * 2)];  dDisp[1] = dDisp[11 + (tgFlame * 2)]; break;//flame
		case 5: dDisp[0] = dDisp[10];  dDisp[1] = dDisp[11]; break;//chara
		case 6: dDisp[0] = dDisp[10];  dDisp[1] = dDisp[11]; break;//mapcreate
		case 7: dDisp[0] = dDisp[10];  dDisp[1] = dDisp[11]; break;
	}
}

void act_menu(void){
BYTE page_max = 2, menu_max = 8;

	if(keyState.osi <= 1 || (keyState.osi >= 0x20)){
		
		if(keyState.left == TRUE){/*menu[i] = menu[i] + 7;*/}
		else if(keyState.up == TRUE){
			if(menu[MNo] > 0){menu[MNo]--;}
			else{menu[MNo] += menu_max -1; list_ofs = menu_max - 7;}
			
			if(menu[MNo] - list_ofs < 0){
				list_ofs--;
			}

			maski = TRUE;//mask���������B
			tgFlame = 0x00;//debug�����\��������B
			dsamp = FALSE;
		}
		else if(keyState.right == TRUE){/*if(menu[MNo] / 6 < page_max){menu[MNo] += 7;}else{menu[i] -= 7;}*/}
		else if(keyState.down == TRUE){
			if(menu[MNo] < menu_max){menu[MNo]++;}
			else{menu[MNo] -= menu_max; list_ofs = 0;}//max�ɂȂ����Ƃ��O��

			if(menu[MNo] - list_ofs > 6){
				list_ofs++;
			}
			
			maski = TRUE; tgFlame = 0x00; dsamp = FALSE;
		}
		else if((keyState.num1 == TRUE) || (keyState.num2 == TRUE) || (keyState.num3 == TRUE)){
			menuSelect();
		}
//		else if(keyState.tame == FALSE){keyState.osi = 0x21;}
	
		if(!keyState.osi){keyState.osi = 0x21;}
		if(keyState.osi == 1){keyState.osi = 0x04;}
	}
	
	debugdisp();
	keyState.osi--;
}

void act_wait(void){
	switch(chara[0].muki){
		case 0:	chara[0].aTyp = atyp.do_w; chara[0].aSpe = 0x20; weapon[0].kakudo = 0; break;
		case 1:	chara[0].aTyp = atyp.le_w; chara[0].aSpe = 0x20;
				weapon[0].kakudo = 15; weapon[0].back = TRUE;
				chara[0].glip[0] = 17; chara[0].glip[8] = 26;
				break;
				
		case 2:	chara[0].aTyp = atyp.up_w; chara[0].aSpe = 0x20;
				weapon[0].kakudo = 5; weapon[0].back = FALSE;
				chara[0].glip[chara[0].muki - 1] = 24; chara[0].glip[chara[0].muki + 7] = 24;
				break;
				
		case 4:	chara[0].aTyp = atyp.ri_w; chara[0].aSpe = 0x20;
				weapon[0].kakudo = 9; weapon[0].back = FALSE;
				chara[0].glip[chara[0].muki - 1] = 14; chara[0].glip[chara[0].muki + 7] = 26;
				break;

		case 8:	chara[0].aTyp = atyp.do_w; chara[0].aSpe = 0x20;
				weapon[0].kakudo = 13; weapon[0].back = TRUE;
				chara[0].glip[chara[0].muki - 1] = 8; chara[0].glip[chara[0].muki + 7] = 24;
				break;

		default:break;
	}
	chara[0].a_mati = 0;
	chara[0].action = 0;//�܂�

}

void act_jump(void)
{
	BYTE i;

	switch(keyState.num3){
		if(chara[0].action == 3){
			chara[0].aTyp = atyp.jump_m;
		}

		case TRUE:
			if(!jMax){
				act[0].spring_c = 0x10;//���W�����v
			}
			for(i = 0; i < 0x1; i++){
				false_time = 0;
				
				if(/*!act.spring_c &&*/ jNum){//�c��W�����v�񐔂����
					act[i].spring_m = chara[i].jump;//�W�����v�͂���ȃ}�b�N�X��
					jNum--;//�񐔌��炷
					act[i].asimoto = 0;
					
					chara[0].action = 3;
					chara[0].aSpe = 0x00;
					aNam[0] = 0;
					chara[0].a_mati = 0;
					chara[0].aTyp = atyp.jump_m;
					//sound
					CopyMemory(lpPCM, eSnd[5].aSnd, 22050);
					PlaySound(lpBuf, NULL, SND_MEMORY | SND_ASYNC);

				}
				if(/*act.spring_c < */act[0].spring_m){//��ȃ}�b�N�X����Ȃ��
					act[0].spring_c += act[0].spring_m / 0x01;//����
					act[0].spring_m -= chara[0].jump / 0x08;//�W�����v�͌���
				}

			}break;
			
		case FALSE:

			for(i = 0; i < 0x1; i++){
				act[0].spring_m = 0;

				if(act[0].asimoto == 1){
					false_time++;//�n�ʂɑ������Ă���̎���/
					if(chara[0].action == 3){chara[0].action = 1;}
					if(false_time > 1 ){
						jNum = jMax;
					}
				}
			
//				subposget(i);
//				if(chara[i].Zpos == mapType[MAP128[0].Put[subXmas + (subYmas * MAP128[0].hirosa)][Zmas]].height[subXpos][subYpos] + (Zmas * 64) + MAP128[0].Zpos && (subOK == TRUE)){
//					jNum = jMax;
//				}
			}
			break;
	}

}

void move_left(void){
	act[0].accel_x = act[0].accel_x - chara[0].speed[1];
	if(keyState.up == FALSE && keyState.down == FALSE){
	
		if(chara[0].muki != 0x01){
			aNam[0] = 0;
			chara[0].muki = 0x01;
			act_wait();
		}

	//	keyState.left = FALSE;
		if(chara[0].action != 3){//jump������A�j���Œ�
			chara[0].aTyp = atyp.le_m;
			chara[0].aSpe = 0x38;
		}
	}

}

void move_up(void){
	act[0].accel_y = act[0].accel_y - chara[0].speed[1];

	if(keyState.left == FALSE && keyState.right == FALSE){
		if(chara[0].muki != 0x02){
			aNam[0] = 0;
			chara[0].muki = 0x02;
			act_wait();
		}
	
	//	keyState.up = FALSE;
		if(chara[0].action != 3){
			chara[0].aTyp = atyp.up_m;
			chara[0].aSpe = 0x38;
		}
	}
}

void move_right(void){
	act[0].accel_x = act[0].accel_x + chara[0].speed[1];

	if(keyState.up == FALSE && keyState.down == FALSE){
		if(chara[0].muki != 0x04){
			aNam[0] = 0;
			chara[0].muki = 0x04;
			act_wait();
		}


	//	keyState.right = FALSE;
		if(chara[0].action != 3){
			chara[0].aTyp = atyp.ri_m;
			chara[0].aSpe = 0x38;
		}
	}
}

void move_down(void){
	act[0].accel_y = act[0].accel_y + chara[0].speed[1];
	
	if(keyState.left == FALSE && keyState.right == FALSE){
		if(chara[0].muki != 0x08){
			aNam[0] = 0;
			chara[0].muki = 0x08;
			act_wait();
		}

//		keyState.down = FALSE;
		if(chara[0].action != 3){
			chara[0].aTyp = atyp.do_m;
			chara[0].aSpe = 0x38;
		}
	}
}

void act_attack(void){
	if(keyState.num1 == TRUE && act[0].Acycle[1] > 1){//num2�������Ƃ��Anum2�𗣂������Ԃ�1�T�C�N�������Ƃ�
		chara[0].action = 128;
		act[0].Acycle[1] = 0;
//		aNam[0] = 0;
		chara[0].a_mati = 0;
		weapon[0].hantei = TRUE;
		
		CopyMemory(lpPCM, eSnd[1].aSnd, 22050);

//		PlaySound(lpBuf, NULL, SND_MEMORY | SND_ASYNC | SND_LOOP);
		PlaySound(lpBuf, NULL, SND_MEMORY | SND_ASYNC);

		hurimawasi(0);
	}
	
	if(chara[0].action == 128){

		if(keyState.num1 == FALSE){
		//chara[0].action = 0;
		act[0].Acycle[1]++;
		}
	}

	if(keyState.num2 == TRUE && act[0].Acycle[1] > 1 && chara[0].action != 2){//num2�������Ƃ��Anum2�𗣂������Ԃ�1�T�C�N�������Ƃ�
		chara[0].action = 2;
		act[0].Acycle[1] = 0;
		aNam[0] = 0;
		chara[0].a_mati = 0;
		weapon[0].hantei = TRUE;

		switch(chara[0].muki){//attack���A�������Ƃɐ�����ѐݒ�
			case 0x01:	chara[0].aTyp = atyp.le_a;
						if(keyState.left == TRUE){
							act[0].huttobi_x = act[0].accel_x - 800;
							act[0].huttobi_y = act[0].accel_y ;
							act[0].huttobi_z = 100;
							act[0].spring_c *= -0.5;
							act[0].Acycle[0] = 10;
						} break;

			case 0x02:	chara[0].aTyp = atyp.up_a;
						if(keyState.up == TRUE){
							act[0].huttobi_x = act[0].accel_x;
							act[0].huttobi_y = act[0].accel_y - 800;
							act[0].huttobi_z = 100;
							act[0].spring_c *= -0.5;
							act[0].Acycle[0] = 10;
						} break;

			case 0x04:	chara[0].aTyp = atyp.ri_a;
						if(keyState.right == TRUE){
							act[0].huttobi_x = act[0].accel_x + 800;
							act[0].huttobi_y = act[0].accel_y;
							act[0].huttobi_z = 100;
							act[0].spring_c *= -0.5;
							act[0].Acycle[0] = 10;
						} break;

			case 0x08:	chara[0].aTyp = atyp.do_a;
						if(keyState.down == TRUE){
							act[0].huttobi_x = act[0].accel_x;
							act[0].huttobi_y = act[0].accel_x + 800;
							act[0].huttobi_z = 100;
							act[0].spring_c *= -0.5;
							act[0].Acycle[0] = 10;
						} break;

			default: break;
		}
		weapon[0].muki = chara[0].muki;
		hurimawasi(0);
	}
	
	if(chara[0].action == 2){//attack ��
		chara[0].aSpe = 0x00;
		
/*		if((chara[0].aTyp < atyp.le_a) || (chara[0].aTyp > atyp.do_a)){//attack�A�j���ȊO�i�����j�̂Ƃ�:c_aT < 4 || 7 > c_aT
				aNam[0] = 0;//�K�v�Ȃ��H
				chara[0].a_mati = 0;
				hurimawasi(0);
		}
*/
		chara[0].a_mati += weapon[0].agi[aNam[0]];
		
		if(chara[0].a_mati > 200){//
			aNam[0]++;
			if(aNam[0] == 1){
				CopyMemory(lpPCM, eSnd[4].aSnd, 22050);
				PlaySound(lpBuf, NULL, SND_MEMORY | SND_ASYNC);
			}
			
			chara[0].a_mati = 0;

			hurimawasi(0);

			if(weapon[0].agi[aNam[0]] == 255){//255end����
				chara[0].action = 0;
				chara[0].aTyp -= 4;
				aNam[0] = 3;
				weapon[0].hantei = FALSE;
			}
		}
	}
	
	if(keyState.num2 == FALSE && keyState.num1 == FALSE){
		act[0].Acycle[1]++;
	}


}

void act_move(void){
	BOOL movetta = FALSE;

//	chara[0].action = 0;
	if(keyState.left == TRUE){move_left(); movetta = TRUE;}
	if(keyState.up == TRUE){move_up(); movetta = TRUE;}
	if(keyState.right == TRUE){move_right(); movetta = TRUE;}
	if(keyState.down == TRUE){move_down(); movetta = TRUE;}

	if(movetta == TRUE && chara[0].action != 3){ chara[0].action = 1;}
//	act_check();
}

void enterkey(void){
	if((keyState.enter == TRUE) && (osippa == FALSE)){
		scroll = scroll ^ 0x0010; paintDo = 1; maski = TRUE; osippa = TRUE;
	}
	else if((keyState.enter == FALSE) && (osippa == TRUE)){
		osippa = FALSE;
	}
}

void act_check(){
	if(chara[0].action != 2 && chara[0].action != 128){act_move();}
	if(chara[0].action != 2 && chara[0].action != 128){act_jump();}
	act_attack();
	if(chara[0].action == 0){act_wait();}
	if(chara[0].action == 1){chara[0].action = 0;}
}

void act_scroll(void)
{
	if(keyState.left == TRUE /*&& MAP.Xmas > 0*/ && MAP.Xpos > 0){MAP.Xpos = MAP.Xpos - 2; keyState.left = FALSE;}
	if(keyState.up == TRUE  /*&& MAP.Ymas > 0 */&& MAP.Ypos > 0){MAP.Ypos = MAP.Ypos - 2; keyState.up = FALSE;}
	if(keyState.right == TRUE /*&& MAP.Xmas < 24 - 8*/ && MAP.Xpos < 512){MAP.Xpos = MAP.Xpos + 2; keyState.right = FALSE;}
	if(keyState.down == TRUE /*&& MAP.Ymas < 18 - 6*/ && MAP.Ypos < 512){MAP.Ypos = MAP.Ypos + 2; keyState.down = FALSE;}
	if(keyState.num2 == TRUE){act_jump(); keyState.num2 = FALSE;}
	
}

void cntMode(void){
	switch(scroll){
		case 0x00:	act_check(); break;
		case 0x01:act_scroll(); break;
		
		case 0x0010:
		case 0x0011:act_menu(); break;
		default: break;
	}
	enterkey();

}
/////////////////////
/////////////////////
/////////////////////////////////////////////////////////