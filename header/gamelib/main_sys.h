/*********���C���V�X�e��*********/
static BYTE mTime=0, aCount=0x40;

int stabi = 1;//antei count
DWORD cpupow = 0;//cpu power
DWORD dwTime;

/*�t���[���\��*/
//WORD Fcount[10] = {6,6,6,6,6, 6,6,6,6,6};//10��v����
WORD Lcount = 0; //�Q�[���T�C�N���J�E���g
DWORD Famari = 0, Fwari = 10000, Fsyoki = 1;//syokiti 
DWORD Fhenka = 0, Fold = 0;
DWORD P[2] = {0, 0}, I[2] = {0, 0}, D[2] = {1, 0},  Ka = 0;//1:�V���� 2:�Â�
BOOL mo;

//int oldclk, startclk;
WORD mSec = 1000;
WORD showFlame = 0;
WORD vFlame = 60; //�r�f�I�ڰ�60
BOOL getFlame = FALSE;
BYTE tgFlame = 1;
BYTE slp = 10;//Sleep time
//BYTE FCi = 0;//for �p�J�E���g
//BYTE Fwait = 1;
/**************/

/*******rndm*******/
BYTE rp = 0;//rndm�Ăяo����
BYTE redro = 0;//�ĕ`��
BYTE failp = 0;
/******************/

////randam///��{�� value = rndm(Lcount, redro, 1[����], true(+����), max)
int rndm(BYTE times, BYTE option, BOOL hu, WORD m_val){
	long value = 0;//99��tukau
	int int_val;
	WORD bb_val;
	BYTE c, i;
	
	bb_val = m_val + 1;
	failp = 0;
	value = times;
	
	while(bb_val > m_val){
		bb_val = 0;
		for(i = 0; i < option - (failp/2); i++){
//			c = value % 10;//hitoketame
			c = ((redro + 1) + /*(i / 10)*/ + (failp + 1) + (value / 10) * (rp + 1) + (i % 10)) % 10;

			switch(c){
				case 0:value = 1680749523;break;//////////////////////////
				case 1:value = 3529678401;break;////////////��////////////
				case 2:value = 2417305869;break;////////////�@////////////
				case 3:value = 9154282736;break;////////////��////////////
				case 4:value =  792831654;break;////////////�@////////////
				case 5:value = 6835094217;break;////////////��////////////
				case 6:value = 4203956178;break;////////////�@////////////
				case 7:value = 8376120945;break;////////////�@////////////
				case 8:value = 7941563082;break;////////////��////////////
				case 9:value = 5268417390;break;//////////////////////////
				default:break;
			}

			value = value / pow(10, (failp + times) % 10);//redraw�����P���ڂɎ����Ă���
			bb_val += (value % 10) * (pow(10, i));//hitoketame
		
			rp++;if(rp > 249){rp = 0;}
		}
		failp++;if(failp > option){failp = 0;}
		
		if(bb_val < m_val){break;}
	}

	if(hu == TRUE){return(bb_val);}//��׽��

	else if(hu == FALSE){////�}�C�i�X�A��
		int_val	= bb_val;
		if((times + rp) % 2 == 1){
			int_val	*= -1;
		}
		return(int_val);
	
	}

}

////�~16�����O�p�֐�
BYTE HEXcircle(BYTE kakudo/*�p�x16����*/, BYTE vecter, BOOL Ymode){
	BYTE ans,/* ansY,*/ l_kakudo,/*0=0_1=90_2=180_3=270*/ s_kakudo/*0=0, 1=30, 2=45, 3=60, 4=90, 5=120, 6=135, 7=150, ....*/;
	
	l_kakudo = kakudo / 4;
	s_kakudo = kakudo % 8;

	if(Ymode == FALSE){
		switch(s_kakudo){
			case 0: ans = vecter; break;
			case 1: ans = vecter * 0.92/*7*/; break;
			case 2: ans = vecter * 0.71/*1*/; break;
			case 3: ans = vecter * 0.38/*2*/; break;
			case 4: ans = 0; break;
			case 5: ans = vecter * 0.38/*2*/; break;
			case 6: ans = vecter * 0.71/*1*/; break;
			case 7: ans = vecter * 0.92/*7*/; break;
		}
	}
	if(Ymode == TRUE){
		switch(s_kakudo){
			case 0: ans = 0; break;
			case 1: ans = vecter * 0.38/*2*/; break;
			case 2: ans = vecter * 0.71/*1*/; break;
			case 3: ans = vecter * 0.92/*7*/; break;
			case 4: ans = vecter; break;
			case 5: ans = vecter * 0.92/*7*/; break;
			case 6: ans = vecter * 0.71/*1*/; break;
			case 7: ans = vecter * 0.38/*2*/; break;
		}
	}
	return(ans);
}

////�������ꌅ���\��////
DWORD jussin(DWORD kazu, BYTE keta, WORD XX, WORD YY, BYTE zure){//�\�����鐔���ƌ��������Ă��܂����B
	BYTE i, j, k, nanketa;//d_kazu ha 256=0�����ǋC�ɂ��Ȃ�
	
	DWORD d_kazu;//�����ӂ�Ă�060723
	for(nanketa = 0; nanketa < keta; nanketa++){
		d_kazu = kazu / pow(10, nanketa);//keta�����P���ڂɎ����Ă���
			i = d_kazu % 10;

		for(j = 0; j < 8; j++){
			for(k = 0; k < 8; k++){
				if(*(NUM[i] + k + (j * 8)) != 0x00ffffff){
					lpBBPixel[/**/XX + k/**/ - (nanketa * 8) + /**/(320 * (8 - j + YY + (nanketa * zure)))/**/] = *(NUM[i] + k + (j * 8));
				}
			}
		}
	}
	return(kazu);
}

void anime_count(void){
	BYTE p;
	/*�^�C�}�[�ȏ���*/
	for(p = 0; p < a_sousu; p++){
		if(!(mTime % (aCount - chara[p].aSpe)) && chara[p].aSpe){
			//if(!(p - 1)){mTime=0;}
		/*aNam0100 aMax0100�̎� aNam0000�ɂ���܂�NAND�A���ˁB*/
			aNam[p]++;
			aNam[p] = (aNam[p] &~ aMax);
			
		}
	}

	mTime++;
}

//////////////////60�t���[���Y�����m�i���x�F�n��j////////////////////
void flame_cnt(void){
	if(Lcount > 59){
		mSec = GetTickCount() - dwTime;
		dwTime = GetTickCount();
		
		if(showFlame > 58 && showFlame < 62 ){//����ĂȂ��ꍇ
			if(stabi > 0){stabi++;}
			if(stabi > 5){ cpupow = Fwari; stabi = 0;}//��{���גl���L���B
		}
		
		else if(stabi > 0){stabi = 1; cpupow = Fwari;}
		//if(stabi < 5){Fwari;}


		if((mSec * 1 > 1016) || (mSec * 1 < 984)){//����Ă�ꍇ
			showFlame =vFlame - (((mSec * 1) - 1000.1) / (1000 / vFlame));//60 + �o(��mS����Ă邩) / (1�ڰщ�mS��)�p�����ڰт���Ă邩�ɂȂ�

			if(mSec * 1 > 1320 && Fwari > 5000){Fwari -= 5000; mSec = 1000;}//>75 or 45>
			if(mSec * 1 < 680){Fwari += 5000; mSec = 1000;}

			if(mSec * 1 > 1160 &&  Fwari > 2500){Fwari -= 2500; mSec = 1000;}//>70 or 50>
			if(mSec * 1 < 840){Fwari += 2500; mSec = 1000;}

			if(mSec * 1 > 1080 &&  Fwari > 1250){Fwari -= 1250; mSec = 1000;}//>65 or 55>
			if(mSec * 1 < 920){Fwari += 1250; mSec = 1000;}
		
			if(mSec * 1 > 1016 &&  Fwari > 512){Fwari -= 512;}//>61 or 59>
			if(mSec * 1 < 984){Fwari += 512;}	//flame����p���B
			
		}

		Lcount = 0;
	}
	
	Lcount++;
}

void fcnt2(void){
	WORD gp = 50, gi = 1, gd = 10;
	short Psa;
	

	if(Lcount >= vFlame){
		mSec = GetTickCount() - dwTime;
		dwTime = GetTickCount();//60�T�����Ƃ��̎���
		if(showFlame > 59 && showFlame < 61 ){//����ĂȂ��ꍇ
			if(stabi > 0){stabi++;}
			if(stabi > 5){cpupow = Fwari; stabi = 0;}//��{���גl���L���B
		}
		else if(stabi > 0){stabi = 1; cpupow = Fwari;}//���ꂽ�����l���P�Ɂi�O�͈���Œ��ԁj�A

		showFlame = (vFlame * 1000) / (mSec * 1);//����ƋC�Â��܂����B2006�N��

		if(mSec * 1 < 1000){//60f/s������Ă�ꍇ
			P[0] = 1000 - (mSec * 1);//���(�ړI�Ƃ̍�) ���̂Ƃ�onetime P[0] = 700���炢��
			Fold = Fwari - Fold;//�O�񕉉וω���
			D[1] = P[0] - P[1];//��x�ŕω�����60f�Ԃ̎��� 700 - 500 => 200
			if(D[1] < 1){ D[1] = 1;}

			Ka = (P[0] * 50);// + (gd * (D[0])) + (gi * I[0]);

			if((Fwari + Ka) > 0){Fwari += Ka;}
			else{Fwari = 1;}

			I[1] = P[1];
			P[1] = P[0];//�O��̍��ɋL��
			mo = 0;
		
		}
		
		else if(mSec * 1 > 1000){
			P[0] = (mSec * 1) - 1000;//���(�ړI�Ƃ̍�) ���̂Ƃ�onetime P[0] = 700���炢��
			if(Fold > Fwari){
				Fold = Fold - Fwari;//�O�񕉉וω���
			}
			else{Fold = 0;}
			
			D[1] = P[1] - P[0];//��x�ŕω�����60f�Ԃ̎��� 700 - 500 => 200
			if(D[1] < 1){ D[1] = 1;}
			Ka = (P[0] * 50);
			if(Fwari - Ka > 0){Fwari = Fwari - Ka;}
			else{Fwari = 1;}
			P[1] = P[0];//�O��̍��ɋL��
			mo = 1;
		}

		Lcount = 0;
	
//	jussin(showFlame, 6, 40, 81, 0);

//	if(mo == 0){
//		jussin(Ka, 6, 40, 81, 0);
//		jussin(Fold, 8, 65, 90, 0);
//		jussin(D[1], 10, 65, 99, 0);
//		jussin(P[0], 10, 65, 108, 0);
//	}
	
//	if(mo == 1){
//		jussin(Ka, 6, 40, 81, 3);
//		jussin(Fold, 8, 65, 90, 3);
//		jussin(D[1], 10, 65, 99, 3);
//		jussin(Fwari + Ka, 10, 65, 108, 3);
//	}
	
	Lcount++;

}
	
/*			Fhenka = Fwari - Fsyoki;//�ݐϕ��וω���
			Fold = Fwari - Fold;//�O�񕉉וω���

			P[0] = 1000 - (mSec * 1);//���(�ړI�Ƃ̍�) ���̂Ƃ�onetime P[0] = 700���炢��
			if(K && P[0] > 0){
				D[1] = P[0] - P[1];//��x�ŕω�����60f�Ԃ̎��� 700 - 500 => 200
				if(D[1] < 0 ){
					D[1] = 1;
				}
//				if(D[0] == 1){
					D[0] = Fold / (D[1] + 0.01);//���וω��ł����玞�Ԃ��ω��������H (4000 / 200) *500 
//				}
//				else{D[0] = 0;}
			}
			I[1] = P[1];
			P[1] = P[0];//�O��̍��ɋL��

			showFlame = (vFlame * 1000) / (mSec * 1);//����ƋC�Â��܂����B2006�N��
			K = (P[0] * D[0]);// + (gd * (D[0])) + (gi * I[0]);

			if((Fwari += K) > 0){Fwari += K;}
			else{Fwari = 1;}

*/	
}
	
/////////////////////������������������///////////////////////
void mati(void){
	for(Famari = 0; Famari < cpupow; Famari++){
		lpBBPixel[Famari / 100]=lpBBPixel[Famari / 100];
//WaitForInputIdle
	}
Famari = 0;
//	while(Famari > cpupow){
//		Famari++;
//	}
	Sleep(10);
//	WaitForInputIdle(Famari);
}

DWORD wait_set(BYTE st, DWORD cp){
	HANDLE hFile;
	DWORD D;
	BYTE mode;
	
	OPENFILENAME ofn;
	DWORD FileSize = 0;
	
	char box_ms[64];
	
	if(keyState.num1){mode = 0;}
	else if(keyState.num2){mode = 1;}
	else if(keyState.num3){mode = 2;}
		
	switch(mode){
		////���莞��������
		case 0: if(stabi == 0){hFile = CreateFile("huka.pre", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					WriteFile(hFile, &cpupow, (DWORD)sizeof(cpupow), &D, NULL);
					wsprintf(box_ms,"���݂̈���x �� %d, CPU Power = %7d", st, cp);
					MessageBox(NULL,"CpuPower�����o������",box_ms,MB_OK);
				}
				else{
					wsprintf(box_ms,"���݂̈���x �� %d, CPU Power = %7d", st, cp);
					MessageBox(NULL,"���肵�Ă܂����",box_ms,MB_OK);
				}
				break;
				
		////�ǂݍ���
		case 1: 
				hFile = CreateFile("huka.pre", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				FileSize = GetFileSize(hFile, NULL);
				if(FileSize == -1){MessageBox(NULL,"�ǂݍ��ݎ��s�Bhuka.pre������܂���B","FileSize�� '-1' �ł�",MB_OK);
					break;}//�ǂݎ��s

				stabi = 0;//���łɈ���Ƃ�				
				ReadFile(hFile, &cp, FileSize, &D, NULL);
				
				wsprintf(box_ms,"���݂̈���x �� %d, CPU Power = %7d %d", st, cp, FileSize);
				MessageBox(NULL,"CpuPower�ǂݍ��݊���",box_ms,MB_OK);
				//�ǂݐ���
				break;
				
		////���Z�b�g
		case 2: stabi = 1; 
				wsprintf(box_ms,"���݂̈���x �� %d, CPU Power = %7d", st, cp);
				MessageBox(NULL,"���ב��胊�Z�b�g",box_ms,MB_OK);
				break;
		
		default: break;
	}
	
	CloseHandle(hFile);
	return cp;

}

void exit_game(void){
//	if (lpDIB32 != NULL) {

//		HeapFree(GetProcessHeap(), 0, lpDIB32);
//		HeapFree(GetProcessHeap(), 0, lpBuf);
//	}
//	KillTimer(hWnd, 1);

	
	
	PostQuitMessage(0);

}