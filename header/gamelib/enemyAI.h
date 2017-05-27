//////////////////////////////////////////////////////
///////////////////////enemy AI///////////////////////
///////////////////////全    集///////////////////////
//////////////////////////////////////////////////////
void a_cyclecount(BYTE i)
{
	BYTE j;
	for(j = 0; j < 15; j++){
		if(chara[i].Acycle[j]){
			chara[i].Acycle[j]--;
		}
	}
}

void awayblock(BYTE ii){//マップ外はみ出し防止
	if(chara[ii].Xpos + 31 > MAP128[0].hirosa[0] * 32){
		chara[ii].Xpos = (MAP128[0].hirosa[0] - 31) * 32;
	}
	if(chara[ii].Xpos > 65533){
		chara[ii].Xpos = 0;
	}
	if(chara[ii].Ypos + 31 > (MAP128[0].hirosa[1] + MAP128[0].hirosa[2]) * 32){
		chara[ii].Ypos = (MAP128[0].hirosa[1] + MAP128[0].hirosa[2]) * 32;
	}
	if(chara[ii].Ypos < (MAP128[0].hirosa[2]) * 32){
		chara[ii].Ypos = MAP128[0].hirosa[2] * 32;
	}
}

void ai_hold(BYTE ii){
	if(!chara[ii].Acycle[2]){
		chara[ii].Acycle[2] = 250;
		chara[ii].aNamN = 4;
		chara[ii].a_loop = 0;//noloop(attack anime)
	}
}

void ai_butaway(BYTE ii){//こうもり型動作
	if(chara[ii].Acycle[7] || !chara[ii].HP[0]){
		chara[ii].SP[0] = 0;
		chara[ii].action[1] = FALSE;
		chara[ii].action[2] = TRUE;//逃げ
//		a_cyclecount(ii, 0);//アクションサイクルカウンター
		return;
	}//ダメージ時ひるむ
	
	if(chara[ii].action[0]){//上で待ち伏せ＆スタミナ回復まち
		chara[ii].asimoto = 8;//天井に着天
	   	chara[ii].accel_z = 0;
		if(chara[ii].SP[0] >= chara[ii].SP[1]){
			chara[ii].SP[0] == chara[ii].SP[1];
			if(chara[0].Xpos > chara[ii].Xpos + 128){return;}
			if(chara[0].Xpos < chara[ii].Xpos - 96){return;}
			if(chara[0].Ypos > chara[ii].Ypos + 128){return;}
			if(chara[0].Ypos < chara[ii].Ypos - 96){return;}
			chara[ii].action[0] = FALSE;//待ち伏せやめて
			chara[ii].action[1] = TRUE;//襲う
			chara[ii].aSpe = 4;
			chara[ii].a_loop = TRUE;
			chara[ii].aNamN = 0;
			chara[ii].aNamT = 0;
		}
		else{
			chara[ii].SP[0] += 1;//回復
		}
	}
	else if(chara[ii].action[1] && chara[ii].SP[0]){//スタミナ続く限りおそう
		chara[ii].SP[0]--;
		if(chara[ii].Xpos < chara[0].Xpos + 4){
			chara[ii].accel_x += chara[ii].speed[1];
		}
		if(chara[ii].Xpos > chara[0].Xpos - 4){
	   		chara[ii].accel_x -= chara[ii].speed[1];
		}
		if(chara[ii].Ypos < chara[0].Ypos + 4){
	   		chara[ii].accel_y += chara[ii].speed[1];
		}
		if(chara[ii].Ypos > chara[0].Ypos - 4){
	   		chara[ii].accel_y -= chara[ii].speed[1];
		}
	   	chara[ii].accel_z += 100;

		if(chara[ii].SP[0] == 0){//スタミナ切れ
			chara[ii].action[1] = FALSE;
			chara[ii].action[2] = TRUE;//逃げ
		}
	}
	else if(chara[ii].action[2]){//逃げながら上昇
		if(chara[ii].Xpos >= chara[0].Xpos && chara[ii].Xpos < chara[0].Xpos + 64){
			chara[ii].accel_x += chara[ii].speed[1] / 2;
		}
		if(chara[ii].Xpos < chara[0].Xpos && chara[ii].Xpos > chara[0].Xpos - 48){
	   		chara[ii].accel_x -= chara[ii].speed[1] / 2;
		}
		if(chara[ii].Ypos >= chara[0].Ypos && chara[ii].Ypos < chara[0].Ypos + 64){
	   		chara[ii].accel_y += chara[ii].speed[1] / 2;
		}
		if(chara[ii].Ypos < chara[0].Ypos && chara[ii].Ypos > chara[0].Ypos - 48){
	   		chara[ii].accel_y -= chara[ii].speed[1] / 2;
		}
	   	chara[ii].accel_z += 400;
//		chara[ii].Acycle[1] = chara[ii].Acycle[0];
		if((chara[ii].accel_z > 10000) && (chara[ii].actionb == chara[ii].Zpos % 0x100)){
			chara[ii].action[2] = FALSE;
			chara[ii].action[0] = TRUE;//まち
			chara[ii].accel_x = 0;
			chara[ii].accel_y = 0;
			chara[ii].a_loop = FALSE;
			chara[ii].aNamN = 6;
			chara[ii].aSpe = 255;
		}
	}
	else{chara[ii].action[2] = TRUE;}
		
	chara[ii].actionb = chara[ii].Zpos % 0x100;
}
void boxcount(BYTE ii){
	if(chara[ii].eboid != 255){//eneboxは無視
		enemy_box[chara[ii].eboid].count_s++;//
	}
}
void ai_box(BYTE id){
	if(enemy_box[id].count_s >= enemy_box[id].multi){//出現空きなし
		enemy_box[id].count_l = 0;
	}
	else{//空きあり
		enemy_box[id].count_l++;
		if(enemy_box[id].count_l > 240){
			set_ene(id, 0);//敵セット
			enemy_box[id].count_l = 0;
		}
	}
	enemy_box[id].count_s = 0;
}

void ene_ai(BYTE i)//敵の動き
{
	//ai_hold
	//ai_appearhold
	//ai_homing
	//ai_hop
	//ai_LRhop
	//ai_UDhop
	//ai_LRwalk
	//ai_UDwalk
	//ai_circle
	//ai_square
	//ai_hitaway
	//ai_rndmwalk
	//ai_linewalk
	//ai_linehop
	//ai_linedash
	//ai_rndmdash
	//ai_w_dhmg
	//ai_away
	//ai_box
	if(chara[i].hold){return;}
	switch(chara[i].AItype){
		case 0:break;
		case 1://誘導
//				if(chara[i].Acycle[7]){chara[i].Acycle[7]--; break;}//ダメージ時ひるむ
//				if(chara[i].Acycle[7] || !chara[i].HP[0]){break;}//ダメージ時ひるむ

//				if(chara[i].Xpos < chara[0].Xpos + 10){
//					chara[i].accel_x += chara[i].speed[1];
//				}
//				if(chara[i].Xpos > chara[0].Xpos - 10){
//			   		chara[i].accel_x -= chara[i].speed[1];
//				}
//				if(chara[i].Ypos < chara[0].Ypos + 10){
//			   		chara[i].accel_y += chara[i].speed[1];
//				}
//				if(chara[i].Ypos > chara[0].Ypos - 10){
//			   		chara[i].accel_y -= chara[i].speed[1];
//				}
				ai_butaway(i);
				break;
			   
		case 2://右往左往とび
//				if(chara[i].Acycle[7]){chara[i].Acycle[7]--; break;}//ダメージ時ひるむ
				if(!chara[i].Acycle[2] && !chara[i].Acycle[3] && chara[i].asimoto){
					chara[i].Acycle[2] = 350;
				}
		
				if(chara[i].asimoto){
					chara[i].accel_x = 0;
				}
				
				if(chara[i].Acycle[2] && chara[i].asimoto){
					if(chara[i].Acycle[2] == 1){
						chara[i].Acycle[3] = 350;
						chara[i].huttobi_x = 1200;
						chara[i].huttobi_y = 0;
						chara[i].huttobi_z = 1300;
						chara[i].Acycle[0] = 20;
						chara[i].asimoto = 0;
					}
//					chara[i].Acycle[2]--;
				}
				
				
				if(chara[i].Acycle[3] && chara[i].asimoto){
					if(chara[i].Acycle[3] == 1){
						chara[i].Acycle[2] = 350;
						chara[i].huttobi_x = -1200;
						chara[i].huttobi_y = 0;
						chara[i].huttobi_z = 1300;
						chara[i].Acycle[0] = 20;
						chara[i].asimoto = 0;
					}
//					chara[i].Acycle[3]--;
				}
				
					
				break;
				
		case 3://円を描く
//				if(chara[i].Acycle[7]){chara[i].Acycle[7]--; break;}//ダメージ時ひるむ
				if(!chara[i].Acycle[0] && !chara[i].Acycle[2] && !chara[i].Acycle[3] && !chara[i].Acycle[4] && !chara[i].Acycle[5] && chara[i].asimoto){
					chara[i].Acycle[2] = 30;
					chara[i].Acycle[4] = chara[i].Acycle[2] / 3;
					chara[i].asimoto = 0;
				}
				
				if(chara[i].Acycle[2] && chara[i].asimoto){//止まった＆着地＆待機NO2
					
//					chara[i].Acycle[2]--;
					chara[i].accel_x += chara[i].speed[1];
					if(chara[i].Acycle[2] == 1){
						chara[i].Acycle[2] = 0;
						chara[i].Acycle[3] = 40;
					}
				}
				
				else if(chara[i].Acycle[3] && chara[i].asimoto){//止まった＆着地＆待機NO2
					
//					chara[i].Acycle[3]--;
					chara[i].accel_x -= chara[i].speed[1];
					if(chara[i].Acycle[3] == 1){
						chara[i].Acycle[3] = 0;
						chara[i].Acycle[2] = 40;
					}
				}
				
				if(chara[i].Acycle[4] && chara[i].asimoto){//止まった＆着地＆待機NO2
					
//					chara[i].Acycle[4]--;
					chara[i].accel_y += chara[i].speed[1];
					if(chara[i].Acycle[4] == 1){
						chara[i].Acycle[4] = 0;
						chara[i].Acycle[5] = 40;
					}
				}
				
				else if(chara[i].Acycle[5] && chara[i].asimoto){//止まった＆着地＆待機NO2
					
//					chara[i].Acycle[5]--;
					chara[i].accel_y -= chara[i].speed[1];
					if(chara[i].Acycle[5] == 1){
						chara[i].Acycle[5] = 0;
						chara[i].Acycle[4] = 40;
					}
				}
				
				break;
				
		case 255:ai_box(chara[i].eboid); break;//ebox
				

		}
}
