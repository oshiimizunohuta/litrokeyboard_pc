////////////buki
void bukiposition(void){
	BYTE i, j;
	BYTE l_kakudo, s_kakudo;
	
	for(i = 0; i < a_sousu; i++){
		for(j = 0; j < 1; j++)//とりあえず武器のみ
		if(chara[i].equips[j] != 0){
			l_kakudo = weapon[i].kakudo / 4;//0 ~ 3のはず//武器向き
//			s_kakudo = weapon[i].kakudo % 4;//向きの中の角度

			weapon[i].Xpos = chara[i].Xpos + chara[i].glip[chara[i].muki - 1] - weapon[i].glip[l_kakudo];//buki x軸表示位置
			weapon[i].Ypos = chara[i].Ypos + chara[i].glip[chara[i].muki + 7] - weapon[i].glip[l_kakudo + 8];//buki y軸表示位置
		}
	}
}

void hurimawasi(BYTE chno){
	BYTE muki = chara[chno].muki;
	
	if(chara[0].action == 128){
//		chara[chno].glip[muki - 1] = 20;
//		chara[chno].glip[muki + 7] = 21;

		weapon[chno].kakudo++;
		if(weapon[chno].kakudo > 15){
			weapon[chno].kakudo = 0;
		}
		weapon[chno].back = FALSE;
	}
	else if(chara[chno].action == 2){
	switch(muki){
		case 1:switch(aNam[chno]){
					case 0: chara[chno].glip[muki - 1] = 20;
							chara[chno].glip[muki + 7] = 21;
							weapon[chno].kakudo = 6;
							weapon[chno].back = TRUE;
							break;
							
					case 1: chara[chno].glip[muki - 1] = 13;
							chara[chno].glip[muki + 7] = 22;
							weapon[chno].kakudo = 4;
							weapon[chno].back = TRUE;
							break;
							
					case 2: chara[chno].glip[muki - 1] = 8;
							chara[chno].glip[muki + 7] = 26;
							weapon[chno].kakudo = 15;
							weapon[chno].back = TRUE;
							break;

					case 3: chara[chno].glip[muki - 1] = 12;
							chara[chno].glip[muki + 7] = 24;
							weapon[chno].kakudo = 7;
							weapon[chno].back = FALSE;
							break;
							
					default:break;
		}break;

		case 2:switch(aNam[chno]){
					case 0: chara[chno].glip[muki - 1] = 24;
							chara[chno].glip[muki + 7] = 22;
							weapon[chno].kakudo = 3;
							weapon[chno].back = FALSE;
							break;
							
					case 1: chara[chno].glip[muki - 1] = 21;
							chara[chno].glip[muki + 7] = 19;
							weapon[chno].kakudo = 7;
							weapon[chno].back = TRUE;
							break;
							
					case 2: chara[chno].glip[muki - 1] = 13;
							chara[chno].glip[muki + 7] = 16;
							weapon[chno].kakudo = 3;
							weapon[chno].back = TRUE;
							break;
							
					case 3: chara[chno].glip[muki - 1] = 20;
							chara[chno].glip[muki + 7] = 24;
							weapon[chno].kakudo = 3;
							weapon[chno].back = TRUE;
							break;
							
					default:break;
		}break;

		case 4:switch(aNam[chno]){
					case 0: chara[chno].glip[muki - 1] = 11;
							chara[chno].glip[muki + 7] = 21;
							weapon[chno].kakudo = 2;
							weapon[chno].back = FALSE;
							break;
							
					case 1: chara[chno].glip[muki - 1] = 18;
							chara[chno].glip[muki + 7] = 28;
							weapon[chno].kakudo = 14;
							weapon[chno].back = FALSE;
							break;
							
					case 2: chara[chno].glip[muki - 1] = 23;
							chara[chno].glip[muki + 7] = 26;
							weapon[chno].kakudo = 7;
							weapon[chno].back = FALSE;
							break;
							
					case 3: chara[chno].glip[muki - 1] = 19;
							chara[chno].glip[muki + 7] = 26;
							weapon[chno].kakudo = 1;
							weapon[chno].back = TRUE;
							break;
							
					default:break;
		}break;

		case 8:switch(aNam[chno]){
					case 0: chara[chno].glip[muki - 1] = 7;
							chara[chno].glip[muki + 7] = 21;
							weapon[chno].kakudo = 5;
							weapon[chno].back = TRUE;
							break;
							
					case 1: chara[chno].glip[muki - 1] = 16;
							chara[chno].glip[muki + 7] = 28;
							weapon[chno].kakudo = 15;
							weapon[chno].back = TRUE;
							break;
							
					case 2: chara[chno].glip[muki - 1] = 16;
							chara[chno].glip[muki + 7] = 27;
							weapon[chno].kakudo = 10;
							weapon[chno].back = FALSE;
							break;
							
					case 3: chara[chno].glip[muki - 1] = 12;
							chara[chno].glip[muki + 7] = 25;
							weapon[chno].kakudo = 7;
							weapon[chno].back = FALSE;
							break;
							
					default:break;
		}break;
		
		default:break;
		
	}
	}
}