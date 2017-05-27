//���E�Ƃ��h�E�N�c�E���̃G���A�ݒ�

BYTE zoneno;
BYTE zonename[24];

struct areamapcels{
//	BYTE areafild[3];//{xMas, yMas, zMas};//�悲�Ƃ̍L��
//	BYTE arange; //���̃G���A�̍L��
	BYTE mapchs[8]; //8�d�}�b�v
	BYTE event[8]; //�C�x���g�^�C�v
	BYTE ebo[16][4];//enemybox
} area[8][8][8];//

BYTE zonebgm = 0;//BGM
BYTE zonebgg = 0;//�w�i

BYTE nowarea[4] = {0, 0, 0, 0};//�ꏊ �n��,�K�w,�k��,����

struct s_message{
	BYTE w[256];//�y�[�W*���[�h//255�����܂ŉ\�H
	BYTE waittype;
	BYTE who[16];
//	WORD change_f[16];
	BYTE change_f[16];//line���Ƃ�face�Ǘ�
	BYTE change_b[16];//line���Ƃ�BGM�ύX
//	BYTE color_type[4];//�J���[
//	BYTE color_point[4][2];//�J���[�͂��߂�ӏ��I���ӏ�
} msgdata[256];//64*8page�܂�

BYTE startmessage = 0;
BYTE messagecnt = 0;//�����ƃy�[�W�p;
BYTE blmessagecnt = 0;//����p;
BYTE linecnt = 0;//��r�p���C��;
BYTE messagewait = 1;//���b�Z�[�W�E�F�C�g
BYTE messagewtcnt = 0; //���b�Z�[�W�E�F�C�g�J�E���g
BYTE mescolorcnt = 0; //���b�Z�[�W�J���[�J�E���g
BYTE msgscrcnt = 0;//���b�Z�[�W�X�N���[���J�E���g
BYTE msgslide = 0;
BYTE facepoint = 0;


void msgsyoki(void){
	BYTE i, j;
	for(i = 0; i < 64; i++){
		for(j = 0; j < 255; j++){
			msgdata[i].w[j] = 0;
		}
		msgdata[i].w[j] = 0;
		msgdata[i].waittype = 255;//nostop
		for(j = 0; j < 16; j++){
			msgdata[i].who[j] = 0;
			msgdata[i].change_f[j] = 0;
		}
	}
}

void areaload(BYTE X, BYTE Y, BYTE Z)//(8chmap)�G���A�ǂݍ���
{
	BYTE ch, i;
	BYTE MAPNO;
	for(ch = 0; ch < 8; ch++){
		MAPNO = area[X][Y][Z].mapchs[ch];//loadmapNo
//		if(MAPNO){
	//		mapload(ch, MAPNO, 1);
			maploadZ(ch, MAPNO, 1);
//		}
	}

	delall_ene();//delete all
	
	for(i = 0; i < 16; i++){//eneboxset
		if(area[nowarea[1]][nowarea[2]][nowarea[3]].ebo[i][0]){//LEVEL����
			set_enemybox(i, 0, 0);
//			stockebo_01(i, 0);
		}
	}

}

void zoneload(BYTE L, BYTE debug)//locate, gamemode, 
{
////////////////name_entry//////////////

	char no[2] = {'0', '0'};
	char type[4];
	char names[16];
	char *name;
	
	HANDLE hFile;
	DWORD D = 0;
	OPENFILENAME ofn;
	DWORD FileSize;
	
	BYTE zerocount;
	BYTE i, j, k, num, val = 0;
	BYTE ch0, ret;//0������H
	strcpy(names, "ZONEDATA/");

	for(i = 0; i < 2; i++){//2���ǂ�
		switch(i){//�e�����Ƃ̐���
			case 0:	val = (L) / 10;break;//locate 2����
			case 1:	val = (L) % 10;break;//locate 1����
			default: break;
		}
		
		switch(val){//���l���當���f�[�^��
			case 0: no[i] = '0';break;
			case 1: no[i] = '1';break;
			case 2: no[i] = '2';break;
			case 3: no[i] = '3';break;
			case 4: no[i] = '4';break;
			case 5: no[i] = '5';break;
			case 6: no[i] = '6';break;
			case 7: no[i] = '7';break;
			case 8: no[i] = '8';break;
			case 9: no[i] = '9';break;
			default: no[i] = 'X'; break;
		}
	}

	for(i = 0; i < 2; i++){//"MAPDATA/" + "XXX"
		names[9 + i] = no[i];//�ԍ�
	}

	strcpy(type, ".zdt");//zone�f�[�^
	for(; i < 6; i++){//4����
		names[9 + i] = type[i - 2];//i��2����n�܂�
	}
	
	names[9 + i] = NULL;//end [15]
	name = &(names[0]);

	hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FileSize = GetFileSize(hFile, NULL);
	
	if(FileSize + 1 == 0){
		MessageBox(NULL,"�ǂݍ��ݎ��s�BAREADAEA������܂���B", name,MB_OK);
		CloseHandle(hFile);
		return;
	}//�ǂݎ��s

	ReadFile(hFile, &zoneno, 1, &D, NULL);//ZONE No
	ReadFile(hFile, zonename, 24, &D, NULL);//ZONE Name
	ReadFile(hFile, &zonebgm, 1, &D, NULL);//ZONE No
	ReadFile(hFile, &zonebgg, 1, &D, NULL);//ZONE Name
	
	zerocount = 0;
	for(i = 0; i < 8; i++){
	for(j = 0; j < 8; j++){
	for(k = 0; k < 8; k++){
		if(!zerocount){
			ReadFile(hFile, area[k][j][i].mapchs, 2, &D, NULL);//�A��2BYTE��荞��
			zerocount = area[k][j][i].mapchs[1];//2�Ԗڂɂ�0���J��Ԃ������͂����Ă���B1�ԖڂɃf�[�^������Ƃ��̓N���A�����̂ň��S
		}
		if(zerocount){
			zerocount--;
		}
		if(area[k][j][i].mapchs[0]){//�f�[�^����
			ReadFile(hFile, area[k][j][i].mapchs + 2, 6, &D, NULL);//�c���6�o�C�g��荞��
			ReadFile(hFile, area[k][j][i].event, 8, &D, NULL);
			for(num = 0; num < 16; num++){
				ReadFile(hFile, area[k][j][i].ebo[num], 4, &D, NULL);//�G�f�[�^
			}
			zerocount = 0;//
			if(debug == 1){//MAPEDITOR�p
				nowarea[1] = k;
				nowarea[2] = j;
				nowarea[3] = i;
			}
		}
		else{//�f�[�^�Ȃ�
//			if(!zerocount){
//				zerocount = area[k][j][i].mapchs[1];//2�Ԗڂɂ�0���J��Ԃ������͂����Ă���
//			}
			for(num = 0; num < 8; num++){
				area[k][j][i].mapchs[num] = 0;//���̃G���A�͂��ׂ�0
				area[k][j][i].event[num] = 0;
				area[k][j][i].ebo[num][0] = 0;
				area[k][j][i].ebo[num][1] = 0;
				area[k][j][i].ebo[num][2] = 0;
				area[k][j][i].ebo[num][3] = 0;
				area[k][j][i].ebo[num + 8][0] = 0;
				area[k][j][i].ebo[num + 8][1] = 0;
				area[k][j][i].ebo[num + 8][2] = 0;
				area[k][j][i].ebo[num + 8][3] = 0;
			}
		}
	}
	}
	}

	CloseHandle(hFile);
		
	D = 0;
//	MessageBox(NULL,"�ǂݍ��ݎ��s�BMAPDATA������܂���B", zonename,MB_OK);

	areaload(nowarea[1], nowarea[2], nowarea[3]);
//	MessageBox(NULL,"ZONEdata�ǂݍ��݊�����",name ,MB_OK);
}

void zonesave(BYTE L)
{
////////////////name_entry//////////////
	HANDLE hFile;
	DWORD D;
	OPENFILENAME ofn;
	DWORD FileSize;
	
	BYTE i, j, k, num, val = 0;
	BYTE zerocount;
	char no[2] = {'0', '0'};
	char type[4];
	char names[16];
	char *name;
	strcpy(names, "ZONEDATA/");

	for(i = 0; i < 2; i++){//2���ǂ�
		switch(i){//�e�����Ƃ̐���
			case 0:	val = (L) / 10;break;//locate 2����
			case 1:	val = (L) % 10;break;//locate 1����
			default: break;
		}
		
		switch(val){//���l���當���f�[�^��
			case 0: no[i] = '0';break;
			case 1: no[i] = '1';break;
			case 2: no[i] = '2';break;
			case 3: no[i] = '3';break;
			case 4: no[i] = '4';break;
			case 5: no[i] = '5';break;
			case 6: no[i] = '6';break;
			case 7: no[i] = '7';break;
			case 8: no[i] = '8';break;
			case 9: no[i] = '9';break;
			default: no[i] = 'X'; break;
		}
	}

	for(i = 0; i < 2; i++){//"MAPDATA/" + "XXX"
		names[9 + i] = no[i];//�ԍ�
	}

	strcpy(type, ".zdt");//zone�f�[�^
	for(; i < 6; i++){//4����
		names[9 + i] = type[i - 2];//i��2����n�܂�
	}
	
	names[9 + i] = NULL;//end [15]
	name = &(names[0]);

	hFile = CreateFile(name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	
	WriteFile(hFile, &L, 1, &D, NULL);//ZONE No
	WriteFile(hFile, zonename, 24, &D, NULL);//ZONE Name
	WriteFile(hFile, &zonebgm, 1, &D, NULL);//ZONE BGMu
	WriteFile(hFile, &zonebgg, 1, &D, NULL);//ZONE BGGr
	
	zerocount = 0;
	for(i = 0; i < 8; i++){
	for(j = 0; j < 8; j++){
	for(k = 0; k < 8; k++){
		if(area[k][j][i].mapchs[0]){
			if(zerocount){
				WriteFile(hFile, &zerocount, 1, &D, NULL);//�������������H
			}
			zerocount = 0;//

			WriteFile(hFile, area[k][j][i].mapchs, 8, &D, NULL);//�`�����l��:8BYTE
			WriteFile(hFile, area[k][j][i].event, 8, &D, NULL);//�C�x���g:8BYTE
			
			for(num = 0; num < 16; num++){
				WriteFile(hFile, area[k][j][i].ebo[num], 4, &D, NULL);//�G�f�[�^
			}
		}
		else{
			if(!zerocount){
				WriteFile(hFile, &area[k][j][i].mapchs[0], 1, &D, NULL);
			}
			
			zerocount++;//�󔒃f�[�^�J�E���g
			if(zerocount == 255){
				MessageBox(NULL,"255ERROR�m�F?", name,MB_OK);
				WriteFile(hFile, &zerocount, 1, &D, NULL);//�������������H
				zerocount = 0;
			}
			else if((i == 7) && (j == 7) && (k == 7)){
				MessageBox(NULL,"end�����m�F", name,MB_OK);
				WriteFile(hFile, &zerocount, 1, &D, NULL);//�������������H
				zerocount = 0;
			}

		}
	}
	}
	}
	
			
	CloseHandle(hFile);

	MessageBox(NULL,"ZONEdata�����o��������", name,MB_OK);
}

void messageload(BYTE L){
////////////////name_entry//////////////
	HANDLE hFile;
	DWORD D;
	OPENFILENAME ofn;
	DWORD FileSize;
	
	BYTE zerocount;
	WORD i, j, k, val = 0;
	BYTE num;
	BYTE ch0, ret;//0������H
	BYTE kiokumsg[3] = {0, 0, 0};//�Ⴄ�f�[�^����Ƃ�
	BYTE msgdatano;
	WORD recnt = 0;
	
	char no[2] = {'0', '0'};
	char type[4];
	char names[16];
	char *name;
	strcpy(names, "MSGDATA/");

	for(i = 0; i < 2; i++){//2���ǂ�
		switch(i){//�e�����Ƃ̐���
			case 0:	val = (L) / 10;break;//locate 2����
			case 1:	val = (L) % 10;break;//locate 1����
			default: break;
		}
		
		switch(val){//���l���當���f�[�^��
			case 0: no[i] = '0';break;
			case 1: no[i] = '1';break;
			case 2: no[i] = '2';break;
			case 3: no[i] = '3';break;
			case 4: no[i] = '4';break;
			case 5: no[i] = '5';break;
			case 6: no[i] = '6';break;
			case 7: no[i] = '7';break;
			case 8: no[i] = '8';break;
			case 9: no[i] = '9';break;
			default: no[i] = 'X'; break;
		}
	}

	for(i = 0; i < 2; i++){//"MAPDATA/" + "XXX"
		names[8 + i] = no[i];//�ԍ�
	}

	strcpy(type, ".msg");//zone�f�[�^
	for(; i < 6; i++){//4����
		names[8 + i] = type[i - 2];//i��2����n�܂�
	}
	
	names[8 + i] = NULL;//end [15]
	name = &(names[0]);

	hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FileSize = GetFileSize(hFile, NULL);
	
	if(FileSize + 1 == 0){
		MessageBox(NULL,"�ǂݍ��ݎ��s�BMSGDAEA������܂���B", name,MB_OK);
		CloseHandle(hFile);
		return;
	}//�ǂݎ��s

	ReadFile(hFile, &msgdatano, 1, &D, NULL);//msgdatano

	for(i = 0; i < 256; i++){
//		kiokumsg[0] = 255 - kiokumsg[1];//�Ⴄ�f�[�^�����Ƃ�
		kiokumsg[0] = 0;//�Ⴄ�f�[�^�����Ƃ�
		kiokumsg[1] = 1;//�Ⴄ�f�[�^�����Ƃ�
		for(j = 0; j < 256; j++){
			if(kiokumsg[0] != kiokumsg[1]){//�O��̃R�s�[���c���Ă邩�A�����̂΂₢
				ReadFile(hFile, &kiokumsg[0], 1, &D, NULL);//kioku1
			}
			ReadFile(hFile, &kiokumsg[1], 1, &D, NULL);//kioku2
			if(kiokumsg[0] == kiokumsg[1]){//��r
				ReadFile(hFile, &recnt, 1, &D, NULL);//�J��Ԃ��J�E���g
				msgdata[i].w[j] = kiokumsg[0];
//				while(recnt){
				for(; recnt > 0; recnt--){
					j++;
					msgdata[i].w[j] = kiokumsg[0];
					if(j == 255){break;}
					if(recnt == 1){j--;}
				}
//				msgdata[i].w[j] = kiokumsg[0];//�s����
//				j--;//�����
				kiokumsg[0] = 255 - kiokumsg[1];//�Ⴄ�f�[�^�����Ƃ�
			}
			else{//�Ⴄ�ꍇ
				msgdata[i].w[j] = kiokumsg[0];
				kiokumsg[0] = kiokumsg[1];
			}
		}
		kiokumsg[0] = 0;
		kiokumsg[1] = 0;
		
		for(j = 0; j < 16; j++){
			ReadFile(hFile, &kiokumsg[0], 1, &D, NULL);//kao
			if(kiokumsg[0]){
				msgdata[i].who[j] = kiokumsg[0];
			}
			else{//0��������
//				ReadFile(hFile, &kiokumsg[1], j + 1, &D, NULL);//kao
				ReadFile(hFile, msgdata[i].change_f, j + 1, &D, NULL);//kao
				j++;
//				kiokumsg[1] = j;
//				j = 0;
				while(j != 16){
//				while(0){
					msgdata[i].who[j] = kiokumsg[0];
					msgdata[i].change_f[j] = kiokumsg[0];
					j++;
					kiokumsg[1]--;
					if(!kiokumsg[1]){break;}
				}
				break;
			}
		}
		ReadFile(hFile, &msgdata[i].waittype, 1, &D, NULL);//waittype
		ReadFile(hFile, msgdata[i].change_b, 16, &D, NULL);//waittype
	}
	ReadFile(hFile, &val, 2, &D, NULL);//dataNO

	CloseHandle(hFile);

}

void messagesave(BYTE L){

	char no[3] = {'0', '0', '0'};
	char type[4];
	char names[15];
	char back[4];
	char *name;

	HANDLE hFile;
	OPENFILENAME ofn;
	DWORD D;
	DWORD FileSize;

	WORD i, j, k, val = 0;
	BYTE zerocount;

////////////////name_entry//////////////
	strcpy(names, "MSGDATA/");
	strcpy(back, "bak/");

	for(i = 0; i < 2; i++){
		switch(i){
			case 0:	val = (L) / 10;break;
			case 1:	val = (L) % 10;break;
			default: break;
		}
		switch(val){
			case 0: no[i] = '0';break;
			case 1: no[i] = '1';break;
			case 2: no[i] = '2';break;
			case 3: no[i] = '3';break;
			case 4: no[i] = '4';break;
			case 5: no[i] = '5';break;
			case 6: no[i] = '6';break;
			case 7: no[i] = '7';break;
			case 8: no[i] = '8';break;
			case 9: no[i] = '9';break;
			default: no[i] = 'X'; break;
		}
	}

	
	for(i =0; i < 2; i++){
		names[8 + i] = no[i];
	}
	strcpy(type, ".msg");

	for(; i < 6; i++){
		names[8 + i] = type[i - 2];
	}
	
	name = &(names[0]);
////////////////name_entry//////////////

	val = L;
	hFile = CreateFile(name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//	FileSize = GetFileSize(hFile, NULL);

	WriteFile(hFile, &val, 1, &D, NULL);//dataNO
	for(i = 0; i < 256; i++){
		zerocount = 0;
		for(j = 0; j < 256; j++){
//			if(msgdata[i].w[j] == 255){//endmsg
//				break;
//			}
			if(msgdata[i].w[j] != msgdata[i].w[j - zerocount]){//���f�[�^�I���
				if(zerocount > 1){//��������
					WriteFile(hFile, &zerocount, 1, &D, NULL);//�������������H
				}
				WriteFile(hFile, &msgdata[i].w[j], 1, &D, NULL);//����Ă����f�[�^����
				zerocount = 1;//0�ł͐H���Ⴄ
			}
			else{//�����f�[�^
				if(zerocount < 2){
					WriteFile(hFile, &msgdata[i].w[j], 1, &D, NULL);//�������PBYTE����((A, A) * zerocount)
				}
				zerocount++;//
			}
//			WriteFile(hFile, &msgdata[i].w[j], 1, &D, NULL);//�PBYTE����
			
		}
		if(zerocount > 1){//��������
			zerocount--;
			WriteFile(hFile, &zerocount, 1, &D, NULL);//�������������H
		}
		if(zerocount == 0){//��������0����
			zerocount = 255;
			WriteFile(hFile, &zerocount, 1, &D, NULL);//255�H
			zerocount = 0;
		}
//		zerocount = 255;
//		WriteFile(hFile, &zerocount, 1, &D, NULL);//endfile

		for(j = 0; j < 16; j++){
//			if(msgdata[i].who[j]){
				WriteFile(hFile, &msgdata[i].who[j], 1, &D, NULL);//face
//				zerocount = 1;
//			}
//			else{
			if(!msgdata[i].who[j]){
//				WriteFile(hFile, &msgdata[i].who[j], 1, &D, NULL);//face=00
				j++;
				break;//�����
			}
		}
//		val = j;//�񐔂��L��
		if(j){
//			WriteFile(hFile, &msgdata[i].change_f, j * 2, &D, NULL);//face
			WriteFile(hFile, msgdata[i].change_f, j, &D, NULL);//face
		}
		WriteFile(hFile, &msgdata[i].waittype, 1, &D, NULL);//waittype
		WriteFile(hFile, msgdata[i].change_b, 16, &D, NULL);//waittype
	}
	val = 256 - L + msgdata[0].w[L];//�t����dataNo����̕���
	WriteFile(hFile, &val, 2, &D, NULL);//dataNO//*WORD
	CloseHandle(hFile);
	MessageBox(NULL,"�������݁B", name,MB_OK);

}


void wldsyoki(void){
	BYTE i, j, k;
	BYTE mch;
	
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			for(k = 0; k < 8; k++){
//				area[k][j][i].arange = 0;///??
//				area[k][j][i].areafild[0] = 0;
//				area[k][j][i].areafild[1] = 0;
//				area[k][j][i].areafild[2] = 0;

				for(mch = 0; mch < 8; mch++){
					area[k][j][i].mapchs[mch] = 0;//ch
					area[k][j][i].event[mch] = 0;
					area[k][j][i].ebo[0][mch] = 0;//LV
					area[k][j][i].ebo[1][mch] = 0;//X
					area[k][j][i].ebo[2][mch] = 0;//Y
					area[k][j][i].ebo[3][mch] = 0;//Z
					area[k][j][i].ebo[0][mch + 8] = 0;
					area[k][j][i].ebo[1][mch + 8] = 0;
					area[k][j][i].ebo[2][mch + 8] = 0;
					area[k][j][i].ebo[3][mch + 8] = 0;
				}
			}
		}
	}
	for(i = 0; i < 32; i++){
		zonename[i] = 0;
	}
}

void TREASURE_001(void)
{
	
}
//[0]������A[1]�{�݂Ƃ��A[2]���I�Ȃ�A[3][4][5][6][7]
void event_set(void)
{
	BYTE i;
	switch(area[nowarea[1]][nowarea[2]][nowarea[3]].event[0]){
		case 1: TREASURE_001();break;
		default: break;
	}

}
