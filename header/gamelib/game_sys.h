/*********���C���V�X�e��*********/
static BYTE mTime=0, aCount=0x40;//iranakunariso--

BYTE mode = 0;;

int stabi = 1;//antei count
DWORD cpupow = 0;//cpu power
DWORD dwTime;

/*�t���[���\��*/
//WORD Fcount[10] = {6,6,6,6,6, 6,6,6,6,6};//10��v����
WORD Lcount = 0; //�Q�[���T�C�N���J�E���g
WORD Vcount = 0; //�r�f�I�T�C�N���J�E���gmain�֐��ɂ͂����Ă�
DWORD Famari = 0, Fwari = 10000, Fsyoki = 1;//syokiti iranakunaruyotei
DWORD Fhenka = 0, Fold = 0;
DWORD P[2] = {0, 0}, I[2] = {0, 0}, D[2] = {1, 0},  Ka = 0;//1:�V���� 2:�Â�
BOOL mo;

//int oldclk, startclk;
WORD mSec = 1000;
WORD showFlame = 0;
WORD showVFlame = 0;
WORD vFlame = 60; //�r�f�I�ڰ�60
BOOL slowF = 0;
BYTE sFlame = 60;
BOOL drawF = TRUE;//�t���[������ɂ��`��\���
BOOL getFlame = FALSE;
BYTE tgFlame = 1;
BOOL hispk = TRUE;//�����x�Q�[�����x
BYTE slp = 10;//Sleep time
WORD slpcnt = 0;//Sleep count
WORD showslp = 0;//Sleep count
DWORD dmycnt = 0;//Dummyloop count
DWORD showdmy = 0;//Dummyloop count

DWORD looptime;//1�T�̎���
DWORD counttime = 0;//60�T����܂ŃJ�E���g��������
DWORD aveFtime = 0;//60�T�����σ^�C��

BOOL mastermode = TRUE;//�}�X�^�[
WORD mastercode = 8678;//�}�X�^�[

/**************/

void fcnt2(void){
	WORD gp;
	short Psa;
	

	if(Lcount >= vFlame){
//		mSec = GetTickCount() - dwTime;//60�T�����Ƃ��̎���
		mSec = timeGetTime() - dwTime;//60�T�����Ƃ��̎���
//		dwTime = GetTickCount();//���݂̎���
		dwTime = timeGetTime();//���݂̎���
		if(showFlame > 59 && showFlame < 61){//����ĂȂ��ꍇ
			if(stabi > 0){stabi++;}
			if(stabi > 5){cpupow = Fwari; stabi = 0;}//��{���גl���L���B
		}
		else if(stabi > 0){stabi = 1; cpupow = Fwari;}//���ꂽ�����l���P�Ɂi�O�͈���Œ��ԁj�A

		showFlame = (vFlame * 1000) / (mSec * 1);//����ƋC�Â��܂����B2006�N��

		Lcount = 0;
		showVFlame = Vcount;//����ƋC�Â��܂����B2006�N��
		Vcount = 0;
	}
//	jussin(showFlame, 4, 1, 1, 0);
	
	Lcount++;

}
void fcnt3(void){
	if(Lcount >= vFlame){
		mSec = timeGetTime() - dwTime;//60�T�����Ƃ��̎���
		dwTime = timeGetTime();//���݂̎���
		showFlame = (vFlame * 1000) / (mSec * 1);//����ƋC�Â��܂����B2006�N��
		Lcount = 0;
	}
//	jussin(showFlame, 4, 1, 1, 0);
	
	Lcount++;

}

void gameslow(void){
//	slowcnt--;
	slowF = TRUE;
	if(Lcount % (60 / sFlame)){slowF = FALSE;}
//	sFlame = 60;

}
/////////////////////�҂�///////////////////////
void mati(void){
	DWORD time = timeGetTime() - looptime;
	if(1032 < time){vFlame = 15;}
	else if(1016 < time){vFlame = 30;}
	else{vFlame = 60;}
	while(1000 / 60 > timeGetTime() - looptime){
		if(time > 1){Sleep(1);}
//		time = timeGetTime() - looptime;
	}//���ԍ���ςݏd�˂�
	looptime = timeGetTime();

}
void mati2(void){
	DWORD nextTime = (Lcount * 1000 / 60);
	DWORD time = timeGetTime() - looptime;
	BOOL noslp = TRUE;

	aveFtime += time;
	if(Lcount == 60){
		aveFtime /= 60;
		aveFtime = 0;
	}
	if(Lcount == 1){
		counttime = 0;
		showslp = slpcnt;
		slpcnt = 0;
		showdmy = dmycnt;
		dmycnt = 0;
	}

	if(64 < time){vFlame = 10;}
	else if(50 < time){vFlame = 15;}
	else if(32 < time){vFlame = 20;}
	else if(16 < time){vFlame = 30;}
	else{vFlame = 60;}

	while(nextTime > time + counttime){
//		if(time > 1){Sleep(1); noslp = FALSE; ++slpcnt;}
		if(nextTime - time - counttime > (DWORD)hispk){Sleep(1); noslp = FALSE; ++slpcnt;}
		++dmycnt;
//		noslp = FALSE;
		time = timeGetTime() - looptime;
	}//���ԍ���ςݏd�˂�
	counttime += time;
	looptime = timeGetTime();
	drawF = TRUE;
	if(Lcount % (60 / vFlame)){drawF = FALSE;}
//	if(Lcount % (60 / 60)){drawF = FALSE;}
	if(noslp){drawF = FALSE;}

}


void masterlisence(void)//�}�X�^�[�F��
{
////////////////name_entry//////////////
	HANDLE hFile;
	DWORD D;
	OPENFILENAME ofn;
	DWORD FileSize;
	
	WORD mas = 0;
	
	char *name = "masterkey";

	hFile = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	FileSize = GetFileSize(hFile, NULL);
	
	if(FileSize + 1 == 0){
		CloseHandle(hFile);
		mastermode = FALSE;
		mastercode = 0;
		return;
	}//�ǂݎ��s

	ReadFile(hFile, &mas, 2, &D, NULL);//ZONE No

	if(mas == mastercode){
		mastermode = TRUE;
	}
	else{
		mastermode = FALSE;
	}
	CloseHandle(hFile);
		
	D = 0;
//	MessageBox(NULL,"ZONEdata�ǂݍ��݊�����",name ,MB_OK);
}

void createmasterkey(void){

	char *name = "masterkey";

	HANDLE hFile;
	OPENFILENAME ofn;
	DWORD D;
	DWORD FileSize;

////////////////name_entry//////////////

	hFile = CreateFile(name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//	FileSize = GetFileSize(hFile, NULL);

	WriteFile(hFile, &mastercode, 2, &D, NULL);//dataNO

	CloseHandle(hFile);
	MessageBox(NULL,"�������݁B", name,MB_OK);

}

