//敵箱

struct e_box{
	WORD Xpos;
	WORD Ypos;
	WORD Zpos;
	BYTE stocks[8];//種類
	BYTE tres[2][8];//Itemボナス
	BYTE endles;//空になったとき
	BYTE count_l;//loop_count
	BYTE count_s;//secon_dcount
	BYTE multi;//多数出し
	BOOL empty;//殻
} enemy_box[16];


void e_box_syoki(void){
	BYTE i;
	
	FillMemory(enemy_box, sizeof(enemy_box), 0);
/*
	enemy_box[ii].Xpos = 0;
	enemy_box[ii].Ypos = 0;
	enemy_box[ii].Zpos = 0;
	for(i = 0; i < 8; i++){
		enemy_box[ii].stocks[i] = 0;//種類
		enemy_box[ii].tres[0][i] = 0;//Itemボナス
		enemy_box[ii].tres[1][i] = 0;//Itemボナス
	}
	enemy_box[ii].endles = 0;//空になったとき
	enemy_box[ii].count_l = 0;//loop_count
	enemy_box[ii].count_s = 0;//secon_dcount
	enemy_box[ii].multi = 0;//多数出し
*/	
}
/*
void stockebo_01(BYTE ii, BYTE opt)//boxにenemyをセット
{
	BYTE i;
	for(i = 0; i < (ii / 2) + 1; i++){
		enemy_box[ii].stocks[i] = 4;
	}
//	opt = opt;
}
*/
void delete_ene(BYTE ii)
{
	BYTE i;
	chara[ii].aNamS = 0x00;
	chara[ii].Ypos = 65535;
	chara[ii].action[8] = FALSE;
	for(i = ii; i < a_sousu ; i++){
//		cuedata_c(i, i + 1);
		cuedata_cha(i, i + 1);
	}
	a_sousu--;
}

void delall_ene(void)
{
	BYTE j;
	for(j = a_sousu - 1; j > 0; j--){
		delete_ene(j);
	}
}

void enemy01(BYTE ii){
	//eneNo2
/*	chara[ii].atari[0] = 9;//higasi
	chara[ii].atari[1] = 23;//nisi
	chara[ii].atari[2] = 25;//kita
	chara[ii].atari[3] = 31;//minami
	chara[ii].atari[4] = 20;//atama
	chara[ii].atari[5] = 10;//asi
*/

	chara[ii].ataritype = 1;
	chara[ii].aNamS = 4;//アドレス
	chara[ii].aNamN = 0;//
	chara[ii].aNamT = 0;//
	chara[ii].aTyp = 0;//
	chara[ii].HP[0] = 2;//現在
	chara[ii].HP[1] = 2;//MAX
	chara[ii].MP[0] = 0;//現在
	chara[ii].MP[1] = 0;//MAX
	chara[ii].SP[0] = 128;//ブレス
	chara[ii].SP[1] = 128;//現在
	chara[ii].SP[2] = 128;//MAX
	chara[ii].equiphand[0] = 0;
	chara[ii].equiphand[1] = 0;
	chara[ii].jump_m = 200;
	chara[ii].jump_t = 120;
	chara[ii].jump_c = 0x0;
	chara[ii].speed[0] = 0x480; //max
	chara[ii].speed[1] = 0x50; //count
	chara[ii].speed[2] = 0x40; //bleeki
	chara[ii].PoE = 2;//chara あたり勝ち負け判定
/*	chara[ii].size[0] = chara[ii].atari[1] - chara[ii].atari[0];
	chara[ii].size[1] = chara[ii].atari[3] - chara[ii].atari[2];
	chara[ii].size[2] = 16;
*/	chara[ii].aSpe = 4;//	ｱﾆﾒｽﾋﾟｰﾄﾞ
	chara[ii].a_cnt = 0;
	chara[ii].a_loop = TRUE;
	chara[ii].animepat = 4;
	chara[ii].setanime = 0;
	chara[ii].action[0] = FALSE;
	chara[ii].action[1] = FALSE;
	chara[ii].action[2] = TRUE;//のげ
	chara[ii].AItype = 1;
}

void select_ene(BYTE ii, BYTE numnum){
//	switch(numnum / 2){
	switch(numnum){
		case 2: enemy01(ii); break;
		default :break;
	}
}

void set_ene(BYTE i, BYTE opt)//enemyセット
{
	BYTE j, k;
	for(j = 0; j < 63; j++){
		if(chara[j].aNamS == 0){break;}//0x20空きスロットを検索
	}
	if(j > 62){return;}//開いてなければ抜け
	
	for(k = 0; k < 8; k++){
		if(enemy_box[i].stocks[k] != 0x0){break;}//何かないか検索
	}
	if(k > 7 && enemy_box[i].endles == 0x0){
		enemy_box[i].empty = 1;
		return;
	}//何もない、エンドレスキャラがなければ抜け
	
	chara[j].Xpos = enemy_box[i].Xpos;
	chara[j].Ypos = enemy_box[i].Ypos;
	chara[j].Zpos = enemy_box[i].Zpos;
	if(k < 8){//ストック内
		chara[j].aNamS = enemy_box[i].stocks[k];
		chara[j].hand_item[0] = enemy_box[i].tres[0][k];//アイテム種
		chara[j].hand_item[1] = enemy_box[i].tres[1][k];//アイテム個数
		enemy_box[i].stocks[k] = 0;//出し終わり
	}
	else{//品切れ。カス出し
		chara[j].aNamS = enemy_box[i].endles;
		chara[j].hand_item[0] = 0;//アイテム種
		chara[j].hand_item[1] = 0;//アイテム個数
	}
	chara[j].eboid = i;//出身
	select_ene(j, chara[j].aNamS);

	a_sousu++;
//	opt = opt;
}

void boxstock_pat(BYTE ebn, BYTE pat)
{
	BYTE i;

//	FillMemory(stockpat, sizeof(stockpat), 0);

	for(i = 0; i < 8; i++){
		if((pat % 8) > i){
//			enemy_box[ebn].stocks[i] = 2;
			enemy_box[ebn].stocks[i] = (pat / 8) + 2;
		}
		else{
			enemy_box[ebn].stocks[i] = 0;
		}
	}
}


void set_enemybox(BYTE no, BYTE type, BYTE opt)//敵セット
{
	BYTE j, lvl;
	
	lvl = area[nowarea[1]][nowarea[2]][nowarea[3]].ebo[no][0];
	for(j = 1; j < 63; j++){
		if(chara[j].aNamS == 0){break;}
	}
	if(j > 62){return;}
	if(type == 0){//box
		chara[j].aNamS = 1;
		chara[j].aNamN = 0;
		chara[j].Xpos = (area[nowarea[1]][nowarea[2]][nowarea[3]].ebo[no][1]) * 32;
		chara[j].Ypos = (area[nowarea[1]][nowarea[2]][nowarea[3]].ebo[no][2]) * 32;
		chara[j].Zpos = (area[nowarea[1]][nowarea[2]][nowarea[3]].ebo[no][3] + 1) * 32;
		if(mapdraw  == 2){
			chara[j].Ypos -= MAP128[0].hirosa[2] * 32;
		}
		enemy_box[no].Xpos = chara[j].Xpos;
		enemy_box[no].Ypos = chara[j].Ypos;
		enemy_box[no].Zpos = chara[j].Zpos;
		chara[j].HP[0] = 2;//現在
		chara[j].HP[1] = 2;//MAX
		chara[j].MP[0] = 0;//現在
		chara[j].MP[1] = 0;//MAX
		chara[j].SP[0] = 128;//ブレス
		chara[j].SP[1] = 128;//現在
		chara[j].SP[2] = 128;//MAX
 		chara[j].asimoto = 8;
		chara[j].accel_x = 0;
		chara[j].accel_y = 0;
		chara[j].accel_z = 100;
		chara[j].muki = 1;
		chara[j].AItype = 255;//ai_box
		chara[j].eboid = no;//出身
		enemy_box[no].count_l = 20;
		enemy_box[no].count_s = 0;
		if(no / 4 == 1){enemy_box[no].multi = 2;}
		else{enemy_box[no].multi = 2;}
		if(no / 4 == 3){enemy_box[no].endles = 1;}
		else{enemy_box[no].endles = 0;}
		enemy_box[no].empty = FALSE;
		
		boxstock_pat(no, lvl);//LVによるパターンセット
		a_sousu++;
//		opt = opt;
	}
}


