////�A�C�e���f�[�^////
struct item_data{
	WORD Xpos;
	WORD Ypos;
	WORD Zpos;
	
	WORD XCpos;
	WORD YCpos;

	int XpSyo;//�}�b�v�����_�ȉ�
	int YpSyo;
	int ZpSyo;

//	BYTE jump_m;//jamp power
//	WORD jump_t;//jump time
//	BYTE jump_c;//jump time count
	BYTE asimoto;//���n
	
	WORD dest_time;//���Ŏ���
	BOOL tenmetu;
	
	BYTE num;
	BYTE piece;//��
		
//	int huttobi_x;//�ӂ��Ƃ�
//	int huttobi_y;
//	int huttobi_z;
	
	int accel_x;//����
	int accel_y;//
	int accel_z;//
	
} item[128];

struct i_box{
	WORD maxweight;//omosa
	WORD nowweight;//���ݏd��
	BYTE maxshelf;//�I
	BYTE nowshelf;//���ݒI��
	BYTE stocks[2][64];//0:�A�C�e����ށA1:�X�g�b�N
} item_box[4];

BYTE i_sousu;
BYTE i_sort[128];//�L�����N�^�\����

void item_syoki(BYTE ii){
	
	item[ii].num = 0;
	item[ii].piece = 1;//��

	item[ii].Xpos = 65535;
	item[ii].Ypos = 65535;
	item[ii].Zpos = 65535;

	item[ii].XCpos = 0;
	item[ii].YCpos = 0;

	item[ii].XpSyo = 0;//�}�b�v�����_�ȉ�
	item[ii].YpSyo = 0;
	item[ii].ZpSyo = 0;
//	item[i].jump_m = item[j].jump_m;
//	item[i].jump_t = item[j].jump_t;
//	item[i].jump_c = item[j].jump_c;
	item[ii].asimoto = 8;//���n���Ă�̂�

	item[ii].dest_time = 0;//���Ŏ���
	item[ii].tenmetu = FALSE;

//	item[i].huttobi_x = item[j].huttobi_x;//�ӂ��Ƃ�
//	item[i].huttobi_y = item[j].huttobi_y;//�ӂ��Ƃ�
//	item[i].huttobi_y = item[j].huttobi_z;//�ӂ��Ƃ�

	item[ii].accel_x = 0;//��
	item[ii].accel_y = 0;//��
	item[ii].accel_z = 0;//��

	i_sort[ii] = ii;

}

void cuedata_i(BYTE i, BYTE j){//i << j
	BYTE c;
	item[i].num = item[j].num;
	item[i].piece = item[j].piece;//��
	item[i].Xpos = item[j].Xpos;
	item[i].Ypos = item[j].Ypos;
	item[i].Zpos = item[j].Zpos;

	item[i].XCpos = item[j].XCpos;
	item[i].YCpos = item[j].YCpos;

	item[i].XpSyo = item[j].XpSyo;//�}�b�v�����_�ȉ�
	item[i].YpSyo = item[j].YpSyo;
	item[i].ZpSyo = item[j].ZpSyo;
//	item[i].jump_m = item[j].jump_m;
//	item[i].jump_t = item[j].jump_t;
//	item[i].jump_c = item[j].jump_c;
	item[i].asimoto = item[j].asimoto;//���n���Ă�̂�
	item[i].dest_time = item[j].dest_time;//���Ŏ���
	item[i].tenmetu = item[j].tenmetu;

//	item[i].huttobi_x = item[j].huttobi_x;//�ӂ��Ƃ�
//	item[i].huttobi_y = item[j].huttobi_y;//�ӂ��Ƃ�
//	item[i].huttobi_y = item[j].huttobi_z;//�ӂ��Ƃ�

	item[i].accel_x = item[j].accel_x;//��
	item[i].accel_y = item[j].accel_y;//��
	item[i].accel_z = item[j].accel_z;//��
//	int huttobi_y;
//	int huttobi_z;
	
//	int accel_x;//����
//	int accel_y;//
//	int accel_z;//

}

void delete_item(BYTE ii){
	BYTE i;
	item[ii].num = 0;
	item[ii].Ypos = 65535;
	item[ii].piece = 0;
	for(i = ii; i < i_sousu; i++){
		cuedata_i(i, i + 1);
	}
//	clean_item();
	i_sousu--;
}

//void itemsort(BYTE i, BYTE out, BYTE opt)//�A�C�e���𐮗�
//{
//}

void itemget(BYTE i, BYTE opt)//�A�C�e�����Q�b�g
{
	BYTE j;
	if(item_box[0].nowshelf < item_box[0].maxshelf){
		if(item_box[0].nowweight < item_box[0].maxweight){
			item_box[0].stocks[0][item_box[0].nowshelf] = item[i].num;//�A�C�e����
			item_box[0].stocks[1][item_box[0].nowshelf] = item[i].piece;//�A�C�e����

			item_box[0].nowshelf++;//���ݎ莝�����{			
			delete_item(i);
		}
	}

}

//
void itemdrop(BYTE i, BYTE type, BYTE out, BYTE opt)//�A�C�e�����̂Ă�
{
	BYTE j;
	for(j = 0; j < 127; j++){
		if(item[j].num == 0){break;}
	}
	if(j > 126){return;}
	if(opt){
//		item[j].num = chara[i].hand_item[0];
		item[j].num = 81;
		item[j].piece = 1;
		item[j].Xpos = chara[i].Xpos + 8;
		item[j].Ypos = chara[i].Ypos + 8;
		item[j].Zpos = chara[i].Zpos + 8;
		item[j].asimoto = chara[i].asimoto;
		item[j].accel_x = chara[i].accel_x;
		item[j].accel_y = chara[i].accel_y;
		item[j].accel_z = chara[i].accel_z + 2000;
		i_sousu++;
	}
	else{
		out = item_box[0].stocks[0][i];
		
		item_box[0].stocks[1][item_box[0].nowshelf] -= out;
		if(item_box[0].stocks[1][item_box[0].nowshelf] == 0){
			item_box[0].stocks[0][item_box[0].nowshelf] = 0;
		}
	}
}

