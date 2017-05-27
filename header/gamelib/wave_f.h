//WAVE FUNCTION�L��
//WAVE//
	static WAVEFORMATEX plsWf;
	static HWAVEOUT hplsWaveOut;
	static WAVEHDR plsWhdr[3];
	static MMTIME mmt;
	DWORD timebyte;//kakikae time byte
	DWORD bcloop = 0;//�������Ă邩//htbyte / SRATE
	DWORD htbyte = 0;//194 783�b = 54.1063889 ���ԂɈ�񖳉�����
	DWORD bctbyte = 0;//194 783�b = 54.1063889 ���ԂɈ�񖳉�����
	DWORD blanktime = 0;//getwaveplaytime ���� �g�`�������݂܂ł̎���
	WORD wvset_msec = 1000;//���b�H
	WORD wpf[5] = {735, 1103, 1470, 2205, 3675};//2, 4, 6, 10�t���[�����A
	

	WORD SRATE = 22050;//22.05KHz��
//	WORD SRATE = 2205;//22.05KHz��
//	WORD SRATE = 11025;//22.05KHz��
	WORD freq = 1;
	WORD maxvol = 16000;
//	WORD maxvol = 16384;
//	WORD maxvol = 30240;
//	BYTE volume_m = 12;//�}�X�^�[�{�����[��(max16//���i��8�̂ق���
	BYTE volume_m = 8;//�}�X�^�[�{�����[��(max16//���i��8�̂ق���//24�͉���ꂠ��
	BYTE volume_s = 8;//�\�t�g�{�����[��(max16
	BYTE now_buff = 0;
	BYTE rebuff = 1;
	BYTE tribuff = 2;
	BYTE selch = 0;
	BYTE selwvbuff = 3;//�o�b�t�@�T�C�Y�Z���N�g
	WORD onkai[96] = {33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, 62, 
					65, 69, 73, 78, 82, 87, 93, 98, 104, 110, 117, 123, 
					131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247, 
					262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 
					523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 
					1047, 1109, 1175, 1246, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 
					2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951, 
					4186, 4435, 4699, 4978, 5274, 5588, 5920, 6272, 6645, 7040, 7459, 7902};
					
	BYTE hanon[5] = {1, 3, 6, 8, 10};
	BYTE onkan[18] = {1, 1, 1, 1,  1, 1, 1, 1};//�L���p���K wave.h�ֈړ�
					

	WORD dly_wait[18] = {0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0};
	BYTE testkey = 33;

	short *lpplsWave[3];
	short WAVEDATA[17][8840];
	short WVDATPOT[8840];//��������R�s�[�p

	BYTE fsdata[8][4] = {64, 8, 2, 1,  64, 16, 4, 1,  1, 4, 8, 32,
					 1, 2, 3, 4,
					1, 2, 3, 4,
					 1, 2, 3, 4,
					1, 2, 3, 4,
					 1, 2, 3, 4};

	BOOL wvstop = 0;

DWORD oscpos = 0;//�I�V���|�W�V����

					 
struct wv_efect{
	BYTE vol;//volume
	BYTE type;
	BYTE t_att;//attenation
	BYTE t_gin;//gain

//	BYTE fms;//freq modulate sub
	BYTE dly;//deray
	BYTE dch;//dechune
	BYTE msec;//nagasa  16

	BYTE fs;//freq modulate sub
	BYTE pws;//palse width sweep 16
	BYTE rfs;//rising falling sweep  16

	WORD freq;//frequancy
	WORD length;//nagasa
	WORD loops;//sansyutu nagasa
	WORD pasloop;//keika nagasa
	WORD playtime;
	BYTE nonkai;//���݂̉��K
//	WORD busytime;//�㏑���֎~����

//	BOOL busy;
	BOOL fchg;
	BOOL maxg;
	BOOL on;
} wv_ch[18] = {8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 1,//wv
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//wv
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//wv
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//wv
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//wv
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//wv
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//wv
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//wv
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//wv
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//wv
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//wv
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//wv
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//se
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//se
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//se
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 0, 0, 0, 0, 0,  0, 0, 0,//se
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 33, 0, 0, 0, 0,  0, 0, 1,//cp
				8, 1, 1, 0,  0, 0, 50, 128, 128, 128,  330, 33, 0, 0, 0, 0,  0, 0, 1};//cp




BYTE wvch_sel(BYTE ch, short vo);

void stopwave(void)
{
	wvstop = (wvstop + 1) % 2;
	waveOutGetPosition(hplsWaveOut,&mmt,sizeof(MMTIME));
	if(mmt.u.cb < (double)SRATE){
		timebyte = mmt.u.cb;
	}
	else{
		timebyte = 320;
	}
}

short get_maxwv(BYTE ch){
	WORD i;
	short data = 0;
	for(i = 0; i < wv_ch[ch].length; i++){
		if(WAVEDATA[ch][i] > data){
			data = WAVEDATA[ch][i];
		}
	}
	
	return data;

}

BOOL fs_env(BYTE ch, BYTE pls, BYTE mns, BOOL fp)
{
	short wleng, freq;
	WORD syu[4] = {8, 20, 40, 80};
	BYTE df[4] = {1, 1, 4, 8};
	BYTE fa;
	BYTE delt;
	BYTE sels;
	BYTE bunn[4] = {2, 4, 8, 16};
	BYTE bunm;
	BYTE envs;
	

	if(pls + mns > 63){
		delt = ((pls + mns - 32) / 4) % 4;
		envs = ((pls + mns) % 32) / 4;
		envs++;
		pls %= 4;
		mns %= 4;
		sels = pls + mns;
		bunm = df[delt];
		df[delt] = 1;
		delt %= 2;
	}
	else{
		envs = 0;
		delt = ((pls + mns - 32) / 4) % 4;
		pls %= 4;
		mns %= 4;
		bunm = df[delt];
		sels = pls + mns;
	}
	

	fa = ((wv_ch[ch].pasloop + (syu[sels] / 2)) / syu[sels]) % bunn[delt];

	if((wv_ch[ch].pasloop % (syu[sels] / bunm)) != 0){return 0;}

	wleng = 0;
	if(fa == 0){
//		freq = df[delt] / syu[sels];
		wleng = df[delt];
	}
	else if(fa == 1){
//		freq = -df[delt] / syu[sels];
		wleng = -df[delt];
	}
	else if(fa == 2){
//		freq = -df[delt] / syu[sels];
		wleng = -df[delt];
	}
	else if(fa == 3){
//		freq = df[delt]  / syu[sels];
		wleng = df[delt];
	}

	if(!wleng){return 0;}
	
	if(fp && wleng < 0){
		wleng += wleng * envs;
	}
	else if(!fp && wleng > 0){
		wleng += wleng * envs;
	}

	
	if(wv_ch[ch].length + wleng > 11025){
		wv_ch[ch].freq = SRATE / 11025;
	}
	else if(wv_ch[ch].length + wleng < 3){wv_ch[ch].freq = SRATE / 2;}
	else{
		wv_ch[ch].freq = SRATE / (wv_ch[ch].length + wleng);
	}
	return 1;
	
}

//���g���ŕϓ��^�C���X�B�[�u
void fsf(BYTE ch, BYTE pls, BYTE mns, BOOL fp, short mdata)
{
	WORD i;
	WORD length;
	WORD loopb, loopn;
	WORD mseco;
	BYTE ppss;
//	short data = 0;
	
	ppss = pls + mns;
	
	if(ppss > 31){
		fs_env(ch, pls, mns, fp);
		 wvch_sel(ch, mdata);
		 return;
	}
//	else if(ppss > 63 && ppss < 96){
//		fs_env(ch, pls, mns, fp, mdata);
//		 wvch_sel(ch, mdata);
		 
//	}
//	if(wv_ch[ch].fs == 128){return;}
	if(wv_ch[ch].pasloop % fsdata[0][(pls + mns) % 4]){wvch_sel(ch, mdata); return;}//���[�v����
	
	if(fp){pls = 1 << (((pls / 4) % 8) * 2); mns = 0;}//8bit�V�t�g�܂�
	else{mns = 1 << (((mns / 4) % 8) * 2); pls = 0;}

	if((wv_ch[ch].freq + pls < 11025) && (wv_ch[ch].freq - mns > 33)){
		length = wv_ch[ch].length;
		
//		data = mdata;
		
		wv_ch[ch].freq += pls - mns;

		wvch_sel(ch, mdata);//�g�`��K����蒼���̂ł��ׂĂ�TUNE�f�[�^�ɉe������
		if(length > wv_ch[ch].length){
			wv_ch[ch].loops = (wv_ch[ch].loops * length) / wv_ch[ch].length;//loop�������H
		}
		else{
//			wv_ch[ch].loops=1;
//			wv_ch[ch].loops = (wv_ch[ch].loops * wv_ch[ch].freq) / freq;//loop�������H			

		}
		
	}
	else{
//		data = mdata;
		if(wv_ch[ch].loops){
			wv_ch[ch].loops = 1;
		}
//		pls = pls || pls;
//		mns = mns || mns;
		wv_ch[ch].freq = ((1 - fp) * 30) + (fp * 11025);
//		wv_ch[ch].freq = 1000;

		if(wv_ch[ch].type == 8 || wv_ch[ch].type == 12){
			wvch_sel(ch, mdata);//�g�`��K����蒼���̂ł��ׂĂ�TUNE�f�[�^�ɉe������
		}
		else{
			wvch_sel(ch, mdata);//�g�`��K����蒼���̂ł��ׂĂ�TUNE�f�[�^�ɉe������
		}
//		wv_ch[ch].freq = (32) + (pls * 11025);


	}
//		wv_ch[ch].fchg = FALSE;
	
}


//�g���䕝�ŕϓ�/
void pwsf(BYTE ch, BYTE pls, BYTE mns, BOOL fp, short mdata)
{
	WORD i, j, ofsl;
	WORD ratebun_p, dlen;
	WORD bunn;
//	short data;
	BYTE fa, flx, ppss;
	BYTE du_data[20] = {20, 30, 50, 90,  20, 40, 80, 60,
						20, 30, 50, 90,  20, 30, 50, 90,
						20, 20, 20, 20};
	
	
//	if(wv_ch[ch].pws == 128){return;}
//	if(wv_ch[ch].pasloop % fsdata[1][(pls + mns) % 4]){return;}//���[�v����

	ppss = ((pls + mns) / 4) % 4;
	if(ppss == 0){flx = 64;}//�e��
	else if(ppss == 1){flx = 32;}
	else if(ppss == 2){flx = 8;}
	else if(ppss == 3){flx = 2;}//�ׂ���
//	else{ppss = ppss / 0;}

	if(fp){
//		flx = pls % 4;
//		pls = 8 << (pls % 4);//8bit�V�t�g�܂�
		pls = 8 >> (pls % 4);//8bit�V�t�g�܂�
//		pls = 8 << ((pls / 4) % 4);//8bit�V�t�g�܂�
	}
	else{
//		flx = mns % 4;
//		mns = 8 << (mns % 4);
		mns = 8 >> (mns % 4);
//		mns = 8 << ((mns / 4) % 4);
	}
	ppss = pls + mns;
//	ppss= 4;
//	bunn =  (4 * wv_ch[ch].freq) / (2);//bunn���傫���قǍׂ���
//	bunn =  (4 * wv_ch[ch].freq) / flx;
	bunn =  (1024) / flx;

	if(bunn < 8){
		bunn = 8;
	}

//	data = 0;
//	for(i = 0; i < wv_ch[ch].length; i++){
//		if(WAVEDATA[ch][i] > data){
//			data = WAVEDATA[ch][i];
//		}
//	}

//	flx0: p-1-50%
//	flx1: p-1-100%
//	flx2: p-1-100%, n-1-50%
//	flx3: p-1-100%, n-1-100%

//	dlen = wv_ch[ch].length * 2;//32����
	dlen = wv_ch[ch].length * du_data[wv_ch[ch].type] / 10;//32����

	ofsl = bunn / 2;
	fa = (((wv_ch[ch].pasloop / ppss) + ofsl) % (bunn * 2)) / bunn;//turn

	ofsl = bunn / 2;
	ratebun_p = ((((wv_ch[ch].pasloop / ppss) + ofsl) % bunn) + 1) * dlen / bunn;//�����ӂ�Ȃ̂ŁA�����Čv�Z������������
//	ratebun_p = ((((wv_ch[ch].pasloop / ppss) + ofsl) % bunn)) * dlen / bunn;//�����ӂ�Ȃ̂ŁA�����Čv�Z������������

	
	if(ratebun_p < 2){
		ratebun_p = 2;
	}

//	bunn = dlen / bunn; // 1loop�Ԃ��

	wv_ch[16].vol = wv_ch[ch].vol;
	wv_ch[17].vol = wv_ch[ch].vol;

	wv_ch[16].freq = (SRATE / ratebun_p);

//	if(wv_ch[16].freq > 8000){
//		ratebun_p = 8000;
//	}
//	if(ratebun_p < dlen){
//		wv_ch[17].freq = SRATE / (dlen + 1 - ratebun_p);
//	}
//	else{wv_ch[17].freq = 8000;}
	wv_ch[16].type = wv_ch[ch].type;
	wv_ch[17].type = wv_ch[ch].type;

	if(wv_ch[16].freq == 0){return;}
	if(wv_ch[16].freq > 11025){return;}


	wvch_sel(16, mdata);//�g�`��K����蒼���̂ł��ׂĂ�TUNE�f�[�^�ɉe������


	if(fp){//pls
		if(fa == 0){//�\
			for(i = 0; i < wv_ch[16].length; i++){//�g�`�O��
				if(i > wv_ch[ch].length - 2){break;}//�g�`�S��High�ɂ͂����Ȃ�
				WAVEDATA[ch][i] = WAVEDATA[16][i];
			}
	
			if(i < dlen){//�O�g�`2�{�̒��������Ȃ�
				wv_ch[17].freq = SRATE / (dlen + 1 - (i * 2));
			}
			else{wv_ch[17].freq = 8000;}
			if(wv_ch[17].freq){
				wvch_sel(17, mdata);//�㔼�g�`���
			}
	
			for(j = 0; j + i < wv_ch[ch].length; j++){//�g�`�㔼
				WAVEDATA[ch][i + j] = -WAVEDATA[17][j];
			}
		}
		if(fa == 1){//��
			for(i = 0; i < wv_ch[16].length; i++){
				if(i > wv_ch[ch].length - 2){break;}
				WAVEDATA[ch][i] = -WAVEDATA[16][i];
			}
			if(i < dlen){
				wv_ch[17].freq = SRATE / (dlen + 1 - (i * 2));
			}
			else{wv_ch[17].freq = 8000;}
			if(wv_ch[17].freq){
				wvch_sel(17, mdata);//�g�`��K����蒼���̂ł��ׂĂ�TUNE�f�[�^�ɉe������
			}
			for(j = 0; j + i < wv_ch[ch].length; j++){
				WAVEDATA[ch][i + j] = WAVEDATA[17][j];
			}
		}
	}
	else{//mns
		if(fa == 0){
			for(i = 0; i < wv_ch[16].length; i++){
				if(i == wv_ch[ch].length - 2){break;}
//				WAVEDATA[ch][wv_ch[ch].length - i - 1] = WAVEDATA[16][i];
				WAVEDATA[ch][wv_ch[ch].length - i - 1] = WAVEDATA[16][i];
			}
	
			if(i < dlen){
				wv_ch[17].freq = SRATE / (dlen + 1 - (i * 2));
			}
			else{wv_ch[17].freq = 8000;}
			if(wv_ch[17].freq){
				wvch_sel(17, mdata);//�g�`��K����蒼���̂ł��ׂĂ�TUNE�f�[�^�ɉe������
			}
	
			for(j = 0; j + i < wv_ch[ch].length; j++){
				WAVEDATA[ch][wv_ch[ch].length - i - j - 1] = -WAVEDATA[17][j];
			}
		}
		if(fa == 1){
			for(i = 0; i < wv_ch[16].length; i++){
				if(i == wv_ch[ch].length - 2){break;}
				WAVEDATA[ch][wv_ch[ch].length - i - 1] = -WAVEDATA[16][i];
			}
			if(i < dlen){
				wv_ch[17].freq = SRATE / (dlen + 1 - (i * 2));
			}
			else{wv_ch[17].freq = 8000;}
			if(wv_ch[17].freq){
				wvch_sel(17, mdata);//�g�`��K����蒼���̂ł��ׂĂ�TUNE�f�[�^�ɉe������
			}
			for(j = 0; j + i < wv_ch[ch].length; j++){
				WAVEDATA[ch][wv_ch[ch].length - i - j - 1] = WAVEDATA[17][j];
			}
		}
	}
}

//�����オ�藧����ϓ�
void rfsf(BYTE ch, BYTE pls, BYTE mns, BOOL fp, short mdata)
{
	WORD i, j, start, nexi, nowi, nowj, nexj;
	BYTE trig;
	BYTE fa;
	BYTE bunn;
	BYTE ppss;
	BYTE deapth;
	BYTE dcnt;
	
	if(mdata == 0){return;}


	ppss = (pls % 4) + (mns % 4);
	ppss++;

	deapth = (pls + mns) / 4;
	deapth++; 

	if(fp){
//		pls = 8 >> (pls % 4);//8bit�V�t�g�܂�
		pls++;
	}
	else{
//		mns = 8 >> (mns % 4);
		mns++;
	}


	
//	if(wv_ch[ch].rfs == 128){return;}
	if(wv_ch[ch].length < 2 + (deapth * 2)){return;}
	if(wv_ch[16].length < 2 + (deapth * 2)){return;}
	if(wv_ch[17].length < 2 + (deapth * 2)){return;}
	if(wv_ch[ch].vol == 0){return;}
	
	bunn = 12 * ppss;
	
	trig = ((wv_ch[ch].pasloop / 64) % (bunn - 4)) + 1;
//	trig = ((wv_ch[ch].pasloop / 1024) % (bunn - 4)) + 1;
//	trig = ((wv_ch[ch].pasloop / 4) % bunn) + 1;


//	if(trig - 1 < bunn / 2){
//		fa = 0;
//	}
//	else if(trig > bunn / 2){
//		fa = 1;
//		trig -= bunn / 2;
//	}
	
	if(trig + 1 < bunn / 2){
		fa = 0;
	}
//	else if(trig + 2 > bunn / 2){
	else{
		fa = 1;
		trig += 2;
		trig -= bunn / 2;
	}

	
	for(i = 0; i < wv_ch[ch].length; i++){//�����オ�茟��1
		start = (wv_ch[ch].length - i - 1) % wv_ch[ch].length;
		if(WAVEDATA[ch][start] < 0){break;}
	}
	for(i = 0; i < wv_ch[ch].length; i++){//�����オ�茟��2
		if(WAVEDATA[ch][(start + i) % wv_ch[ch].length] > 0){break;}
	}
	
//	i = (wv_ch[ch].length + start + i - 1) % wv_ch[ch].length;
	i = (wv_ch[ch].length + start + i) % wv_ch[ch].length;
	

	for(j = 0; j < wv_ch[ch].length; j++){//����������(�^��)����1�Ŗ��Ȃ�
		start = (j + i + 1) % wv_ch[ch].length;
		if(WAVEDATA[ch][start] < 0){break;}
	}

//	j = (wv_ch[ch].length + start - 1) % wv_ch[ch].length;
	j = (wv_ch[ch].length + start) % wv_ch[ch].length;
	
	

	bunn /= 2;
	if(ppss == 4){//�Œ肵�đ�`��ڎw��
		deapth *= 2;
//		trig = bunn / (deapth + 1);
		trig = bunn / (deapth);
//		trig = trig * (deapth / 2);
//		trig = 12;
//		trig = 12;
		fa = 0;
	}

//			for(i = 0; i < 1; i++){//�g�`�O��

	
//				nowi = (i + deapth - 1) % wv_ch[ch].length;
//				nexi = (wv_ch[ch].length + i - 1) % wv_ch[ch].length;
	nowi = i;
	nexi = (wv_ch[ch].length + i - 1) % wv_ch[ch].length;
	nowj = j;
	nexj = (wv_ch[ch].length + j - 1) % wv_ch[ch].length;
	for(dcnt = 0; dcnt < deapth; dcnt++){
		if(!fa){

//				WAVEDATA[ch][i] = WAVEDATA[ch][i] + (((WAVEDATA[ch][(i + 1) % wv_ch[ch].length] - WAVEDATA[ch][i]) / bunn) * trig);//�Ő�f�[�^�i�����オ��j
//				WAVEDATA[ch][j] = WAVEDATA[ch][j] - (((WAVEDATA[ch][j] - WAVEDATA[ch][(j + 1) % wv_ch[ch].length]) / bunn) * trig);//������
//				WAVEDATA[ch][i] = WAVEDATA[ch][(wv_ch[ch].length + i - 1) % wv_ch[ch].length] + (((WAVEDATA[ch][i] - WAVEDATA[ch][(wv_ch[ch].length + i - 1) % wv_ch[ch].length]) / bunn) * trig);//�Ő�f�[�^�i�����オ��j
//				WAVEDATA[ch][j] = WAVEDATA[ch][j] - (((WAVEDATA[ch][j] - WAVEDATA[ch][(j + 1) % wv_ch[ch].length]) / bunn) * trig);//������
			WAVEDATA[ch][(nowi + 1) % wv_ch[ch].length] = WAVEDATA[ch][nowi];//�O�f�[�^���p���i�ő�VOL���ێ��j
			WAVEDATA[ch][nowi] = WAVEDATA[ch][nexi] + (((WAVEDATA[ch][nowi] - WAVEDATA[ch][nexi]) / bunn) * trig);//�Ő�f�[�^�i�����オ��j
			WAVEDATA[ch][nowj] = WAVEDATA[ch][nexj] - (((WAVEDATA[ch][nexj] - WAVEDATA[ch][nowj]) / bunn) * trig);//������
		}
		else{
//				WAVEDATA[ch][i] = WAVEDATA[ch][(i + 1) % wv_ch[ch].length] - (((WAVEDATA[ch][(i + 1) % wv_ch[ch].length] - WAVEDATA[ch][i]) / bunn) * trig);//�Ő�f�[�^�i�����オ��j
			WAVEDATA[ch][(nowi + 1) % wv_ch[ch].length] = WAVEDATA[ch][nowi];//�O�f�[�^���p���i�ő�VOL���ێ��j
			WAVEDATA[ch][nowi] = WAVEDATA[ch][nowi] - (((WAVEDATA[ch][nowi] - WAVEDATA[ch][nexi]) / bunn) * trig);//�Ő�f�[�^�i�����オ��j
			WAVEDATA[ch][nowj] = WAVEDATA[ch][nowj] + (((WAVEDATA[ch][nexj] - WAVEDATA[ch][nowj]) / bunn) * trig);//������
		}
		
//		i = (wv_ch[ch].length + i - 1) % wv_ch[ch].length;
		if(ppss < 4){
			nexi = (nexi + 1) % wv_ch[ch].length;
			nexj = (nexj + 1) % wv_ch[ch].length;
		}
		else{
			trig += bunn / (deapth);
		}
		nowi = (nowi + 1) % wv_ch[ch].length;
		nowj = (nowj + 1) % wv_ch[ch].length;
//		trig += deapth;
//		i = (wv_ch[ch].length + i - 1) % wv_ch[ch].length;
//		j = (wv_ch[ch].length + j - 1) % wv_ch[ch].length;
	}

}


void wave_modulation(short mdata, BYTE ch, BYTE mod)
{
	BOOL mns, pls;
	WORD i;
	short data = 0;
	WORD fspls, fsmns, fsx, length;
	BYTE fmtype;//0 freqpitch, 1 notepitch, 2 pwpitch, 3 vibpitch, 4,5,6,7,enbelope
	BYTE mval[4];
//	if(wv_ch[ch].fs == 128){return;}

	if(wv_ch[ch].type == 9){return;}
	if(wv_ch[ch].type == 10){return;}
	if(wv_ch[ch].type == 11){return;}
	if(wv_ch[ch].type == 13){return;}
	if(wv_ch[ch].type == 14){return;}
	if(wv_ch[ch].type == 15){return;}
	if(wv_ch[ch].type > 19){return;}
//	if(wv_ch[ch].type == 9){return;}

//	switch(mod){
//		case 0: mval = wv_ch[ch].fs; break;
//		case 1: mval = wv_ch[ch].pws; break;
//		case 2: mval = wv_ch[ch].rfs; break;
//	}
	mval[0] = wv_ch[ch].fs;
	mval[1] = wv_ch[ch].pws;
	mval[2] = wv_ch[ch].rfs;
	mval[3] = 0;
	
//	if(mval == 128 && mod){
//	if(wv_ch[ch].fs == 128 && wv_ch[ch].pws == 128 && wv_ch[ch].rfs == 128){
	if(mod == 0){
		if(mval[0] == 128){
			if(wv_ch[ch].type == 8 || wv_ch[ch].type == 12){
				wvch_sel(ch, mdata);//�g�`��K����蒼���̂ł��ׂĂ�TUNE�f�[�^�ɉe������
			}
			else{
				wvch_sel(ch, mdata);//�g�`��K����蒼���̂ł��ׂĂ�TUNE�f�[�^�ɉe������
			}

			return;
		}
	}
	
	
//	if(wv_ch[ch].fs < 128){//�}�C�i�X
//		fsmns = 128 - wv_ch[ch].fs;
	if(mval[mod] < 128){//�}�C�i�X
		fsmns = 128 - mval[mod];
		fspls = 0;
		pls = FALSE;
		mns = TRUE;
	}
//	else if(wv_ch[ch].fs > 128){//�v���X
//		fspls = wv_ch[ch].fs - 128;
	else if(mval[mod] > 128){//�v���X
		fspls = mval[mod] - 128;
		fsmns = 0;
		mns = FALSE;
		pls = TRUE;
	}
	else{
		fsx = 0;
		fsmns = 0;
		fspls = 0;
	}
	
	if(fsmns){
		fsmns--;//�͂���1����͂���0��
		fmtype = fsmns / 16;
//		fsx = fsdata[fmtype][fsmns % 4];//���T����?
		fsx = fsdata[mod][fsmns % 4];//���T����?
//		fsx = fsdata[mod][fsmns / 4];//���T����?

	}
	else if(fspls){
		fspls--;
		fmtype = fspls / 16;

//		fsx = fsdata[fmtype][fspls % 4];//���T����?
		fsx = fsdata[mod][fspls % 4];//���T����?
//		fsx = fsdata[mod][fspls / 4];//���T����?
	}

	if(mval[mod] != 128){
//		if((wv_ch[ch].pasloop % fsx) == 0){
			switch(mod){
				case 0:fsf(ch, fspls, fsmns, pls, mdata); break;//���g���ϓ�
				case 1:pwsf(ch, fspls, fsmns, pls, mdata);break;//���ϓ�
				case 2:rfsf(ch, fspls, fsmns, pls, mdata);break;//�����オ�艺����ϓ�
//		case 3:dchf();break;//�ʑ��ϓ�
//		case 4:fse();break;//���g���G���x���[�v
//		case 5:ntse();break;//���K�G���x���[�v
//		case 6:pwme();break;//�f���[�e�B�[��G���x���[�v
//		case 7:fse();break;//�U���G���x���[�v
			}
//		}
		return;
	}
	

/*		for(i = 0; i < wv_ch[ch].length; i++){
			if(WAVEDATA[ch][i] > data){
				data = WAVEDATA[ch][i];
			}
		}
		if(wv_ch[ch].type == 8 || wv_ch[ch].type == 12){
			wvch_sel(ch, mdata);//�g�`��K����蒼���̂ł��ׂĂ�TUNE�f�[�^�ɉe������
		}
		else{
			wvch_sel(ch, mdata);//�g�`��K����蒼���̂ł��ׂĂ�TUNE�f�[�^�ɉe������
		}
*/
	
}

void wave_efects_t(BYTE ch)
{
//	DWORD chbuffpos = 0;
	WORD i;
	short data = 0;
	short swapdata = 0;
	BYTE dch = 0;////1loop����
	BYTE dlx = 0;//��loop����
	BYTE ofs = 32;//�O��
	BYTE cent = 16;//�؂�ւ��l
	
//	if(wv_ch[ch].dch < 65){return;}
	if(wv_ch[ch].dch < ofs + 1){return;}
	
	
//	volumes = wv_ch[ch].t_att * volume_m;
	if(wv_ch[ch].dch){//delay�ݒ肠���
//		dlx = 97 - wv_ch[ch].dch;
		dlx = ofs + cent + 1 - wv_ch[ch].dch;
//		if(wv_ch[ch].dch && (wv_ch[ch].dch < 96)){//�������Ƃ�
		if(wv_ch[ch].dch && (wv_ch[ch].dch < ofs + cent + 1)){//�������Ƃ�
			dch = 1;
		}
//		else if(wv_ch[ch].dch > 95){//������Ƃ�
		else if(wv_ch[ch].dch > ofs + cent - 1){//������Ƃ�
			dlx = 1;
			dch = wv_ch[ch].dch - ofs - cent;
		}
		
		for(i = 0; i < wv_ch[ch].length; i++){
			if(WAVEDATA[ch][i] > data){
				data = WAVEDATA[ch][i];
			}
//			WVDATPOT[i] = WAVEDATA[ch][i];//�R�s�[
			WAVEDATA[16][i] = WAVEDATA[ch][i];//�R�s�[
		}
	}
	else{
		dch = 0;
	}
	
	for(i = 0; i < wv_ch[ch].length; i++){
		if(dch){
//			WAVEDATA[ch][wv_ch[ch].length - 1 - ((i + (dch * wv_ch[ch].pasloop / dlx)) % wv_ch[ch].length)] = WVDATPOT[i];//�R�s�[
			WAVEDATA[ch][wv_ch[ch].length - 1 - ((i + (dch * wv_ch[ch].pasloop / dlx)) % wv_ch[ch].length)] = WAVEDATA[16][i];//�R�s�[
		}
	}
}

void wave_efects(BYTE ch)
{
//	DWORD chbuffpos = 0;
	WORD i;
	short data = 0;
	short swapdata = 0;
	BYTE dch = 0;////1loop����
	BYTE dlx = 0;//��loop����
	BYTE ofs = 32;
	BYTE cent = 16;
	
//	if((wv_ch[ch].dch - 1) % 64 > ofs){return;}
	if((wv_ch[ch].dch) % 64 > ofs){return;}

	if(wv_ch[ch].dch){//delay�ݒ肠���
		dlx = cent + 1 - wv_ch[ch].dch;
		if(wv_ch[ch].dch && (wv_ch[ch].dch < cent)){//�������Ƃ�
			dch = 1;
		}
//		else if(wv_ch[ch].dch > 31){//�������
		else if(wv_ch[ch].dch > cent - 1){//�������
			dch = wv_ch[ch].dch - cent + 1;
			dlx = 1;
		}
		
		for(i = 0; i < wv_ch[ch].length; i++){
			if(WAVEDATA[ch][i] > data){
				data = WAVEDATA[ch][i];
			}
			WVDATPOT[i] = WAVEDATA[ch][i];//�R�s�[
		}
	}
	else{
		dch = 0;
	}
	
	for(i = 0; i < wv_ch[ch].length; i++){
		if(dch){
			WAVEDATA[ch][(i + (dch * wv_ch[ch].pasloop / dlx)) % wv_ch[ch].length] = WVDATPOT[i];//�R�s�[
		}
	}
}

void dch_env(BYTE ch)
{
	if(!wv_ch[ch].dch){return;}//����
	if(wv_ch[ch].dch < 33){return;}//�x��
	if(wv_ch[ch].dch < 65){return;}//�i��
	
	

}

short wave_yoin(BYTE ch, WORD at, BYTE delt)
{
	short volumes;
//	BYTE dv[3] = {20, 40, 60};//���l�܂ŉ�����
	BYTE dv[3] = {15, 20, 40};//���l�܂ŉ�����
	BYTE syu;
	

	at = (at % 16) + 1;
//	syu = (at % 16) + 1;
	syu = 160 - (at * 8);

//	if(wv_ch[ch].pasloop % syu){return 0;}
//	else{
//		volumes_b = dv[delt] * volume_m * 32;
//	}

	
	if(wv_ch[ch].pasloop < syu){
		return 0;
	}
	else{
//		if(wv_ch[ch].vol - dv[delt] > 0){
		if(((10 * wv_ch[ch].vol) / dv[delt]) > 0){
			return (((10 * wv_ch[ch].vol) / dv[delt]) * volume_m * 32);
		}
		else{
			return (2 * volume_m * 32);
		}
	}
}
short wave_env(BYTE ch, WORD syu, BYTE delt)
{
	short volumes;
	short dv[3] = {24, 52, 128};
//	BYTE delt = env / 64;
//	BYTE syu = 20;
	BYTE fa;
	
	fa = ((wv_ch[ch].pasloop / syu)) % 4;
	
	if(!fa){
		volumes = dv[delt] * volume_m / syu;
//		volumes = delt * (wv_ch[ch].pasloop % syu) / volume_m;
//		volumes = (wv_ch[ch].pasloop % syu);
	}
	else if(fa == 1){
		volumes = -dv[delt] * volume_m / syu;
//		volumes = delt * ((syu - wv_ch[ch].pasloop % syu)) / volume_m;
//		volumes = ((syu - wv_ch[ch].pasloop % syu));
	}
	else if(fa == 2){
		volumes = -dv[delt] * volume_m / syu;
//		volumes = delt * -((wv_ch[ch].pasloop % syu)) / volume_m;
//		volumes = -((wv_ch[ch].pasloop % syu));
	}
	else if(fa == 3){
		volumes = dv[delt] * volume_m / syu;
//		volumes = delt * -((syu - wv_ch[ch].pasloop % syu)) / volume_m;
//		volumes = -((syu - wv_ch[ch].pasloop % syu));
	}
	if(!volumes){
		if(wv_ch[ch].pasloop % (2) == 0){
			if(fa == 0 || fa == 3){
				volumes = 1;
			}
			else{
				volumes = -1;
			}
		}
	}

	return volumes;

}

void wave_att(BYTE ch){//
	WORD i;
	short volumes;
	short honrai = wv_ch[ch].vol * (volume_m) * 32;
	BYTE g = 0;
	BYTE lend = 1;
	BYTE atn;
	BYTE delt;
	BYTE fa;
	WORD syu[4] = {4, 48, 128, 512};
	BYTE sysel;
	BOOL maxflg;//0�ɂ���\��
	

	if(!wv_ch[ch].t_att){return;}
	if(!wv_ch[ch].maxg){return;}

	if(wv_ch[ch].t_att > 207){
		atn = wv_ch[ch].t_att - 208;
		delt = ((atn / 16) % 3);//���l�܂ŉ�����A��͂��̂܂�
		volumes = wave_yoin(ch, atn, delt);
		if(volumes){
			for(i = 0; i < wv_ch[ch].length; i++){
				WAVEDATA[ch][i] = volumes;
			}
			return;
		}
		else{
//			atn = wv_ch[ch].t_att % 16;//���X�ɒႭ//loop���K��l�𒴂���܂ł�
//			atn *= 4;
			delt = (atn / 16);//�Œ�l�ɂ����ATT�l���ω�
			atn = atn % 16;//���X�ɒႭ//loop���K��l�𒴂���܂ł�
			atn *= delt + 1;
			volumes = (atn) * volume_m / 10;
		}
	}
	else if(wv_ch[ch].t_att > 63){//64���
		atn = wv_ch[ch].t_att - 64;
		sysel = (atn / 48);
		fa = wv_ch[ch].pasloop % syu[sysel];
		delt = ((atn / 16) % 3);//�Œ�l�͂P����
		atn = wv_ch[ch].t_att % 16;//�Œ�l�͂P����
		if(fa || !atn){
			volumes = wave_env(ch, syu[sysel], delt);
			
		}
		else if(atn){
			volumes = (atn * syu[sysel]) * volume_m / 20;//4����
			if((wv_ch[ch].pasloop / syu[sysel]) % 4 == 2){
				maxflg = TRUE;
			}
		}
	}
	else{//64�܂�
		atn = wv_ch[ch].t_att % 64;//�Œ�l�͂P����
		volumes = (atn) * volume_m / 10;
		if(!volumes && (wv_ch[ch].pasloop % (atn * volume_m)) == 0){
			volumes = 1;
			maxflg = TRUE;
		}
	}


	if(wv_ch[ch].type > 7 && wv_ch[ch].type < 16 && wv_ch[ch].t_att < 0){
		g = 1;
		g = 0;
	}
	for(i = 0; i < wv_ch[ch].length; i++){
//		if(g){
//			if(WAVEDATA[ch][i] < 0){
//				volumes = -WAVEDATA[ch][i] * -wv_ch[ch].t_att / (volume_m * 4);
//			}
//			else{
//				volumes = WAVEDATA[ch][i]  * -wv_ch[ch].t_att / (volume_m * 4);
//			}
//		}
		if(WAVEDATA[ch][i] > 0){//plus
//				if(WAVEDATA[ch][i] - volumes > 0 && WAVEDATA[ch][i] - volumes < maxvol){
			if(WAVEDATA[ch][i] - volumes > 0){
				WAVEDATA[ch][i] -= volumes;
			}
			else if(maxflg){WAVEDATA[ch][i] = 0;}
			else{WAVEDATA[ch][i] = 1;}
		}
		else if(WAVEDATA[ch][i] < 0){//minus
//				if(WAVEDATA[ch][i] + volumes < 0 && WAVEDATA[ch][i] + volumes > -maxvol){
			if(WAVEDATA[ch][i] + volumes < 0){
				WAVEDATA[ch][i] += volumes;
			}
			else if(maxflg){WAVEDATA[ch][i] = 0;}
			else{WAVEDATA[ch][i] = -1;}
		}
		else{WAVEDATA[ch][i] = 0;}//0��0
	}
}

void wave_gain(BYTE ch){//
	WORD i;
	short volumes;
	short honrai = wv_ch[ch].vol * (volume_m) * 32;
	BYTE g = 1;
	BYTE lend = 1;
	WORD syu[4] = {16, 48, 128, 256};
	BYTE sysel;
	BYTE amp;
	BYTE delt;
	BYTE fa;
	BOOL maxflg;

	if(!wv_ch[ch].t_gin){return;}
	if(wv_ch[ch].maxg){return;}

	if(wv_ch[ch].t_gin > 63){
//	volumes = (wv_ch[ch].t_gin * wv_ch[ch].t_gin) + volume_m;
		amp = wv_ch[ch].t_gin - 64;
		sysel = (amp / 48);
//		fa = wv_ch[ch].pasloop % syu[sysel];
		fa = (wv_ch[ch].pasloop / syu[sysel]) % 4;
		delt = ((amp / 16) % 3);//�Œ�l�͂P����
		amp = wv_ch[ch].t_gin % 16;//�Œ�l�͂P����
//		if(fa || !amp){
		if(fa != 3 || !amp){
			volumes = wave_env(ch, syu[sysel], delt);
			maxflg = FALSE;
		}
		else if(amp){
			volumes = wave_env(ch, syu[sysel], delt);
			volumes += (amp) * volume_m / 4;//gain4�{���炢
//			if(fa == 3){
				maxflg = TRUE;
//			}
		}
	}
	else{//64�܂�
		amp = wv_ch[ch].t_gin % 64;//�Œ�l�͂P����
		volumes = (amp) * volume_m / 10;
//		if(!volumes && (wv_ch[ch].pasloop % (amp * volume_m)) == 0){
//			volumes = 1;
			maxflg = TRUE;
//		}
	}

	
	if(wv_ch[ch].type > 7 && wv_ch[ch].type < 16 && wv_ch[ch].t_gin){
//		g = 1;
		g = 2;
		honrai *= g;
		volumes *= g;
	}
	for(i = 0; i < wv_ch[ch].length; i++){
		if(WAVEDATA[ch][i] > 0){
			if(WAVEDATA[ch][i] > 0 && WAVEDATA[ch][i] + volumes < honrai){
				WAVEDATA[ch][i] += volumes;
				if(WAVEDATA[ch][i] < 1){WAVEDATA[ch][i] = volume_m;}
			}
			else if(maxflg){WAVEDATA[ch][i] = honrai; wv_ch[ch].maxg = TRUE; wvch_sel(ch, honrai); return;}
			else{WAVEDATA[ch][i] += volumes;}
		}
		else if(WAVEDATA[ch][i] < 0){
			if(WAVEDATA[ch][i] < 0 && WAVEDATA[ch][i] - volumes > -honrai){
				WAVEDATA[ch][i] -= volumes;
				if(WAVEDATA[ch][i] > -1){WAVEDATA[ch][i] = -volume_m;}
			}
			else if(maxflg){WAVEDATA[ch][i] = -honrai; wv_ch[ch].maxg = TRUE; wvch_sel(ch, honrai); return;}
			else{WAVEDATA[ch][i] -= volumes;}
		}
		else{WAVEDATA[ch][i] = 0;}
	}
}

void continue_combcho(void){
	DWORD i = 0, longest = 0;
	DWORD chbuffpos = 0;
//	WORD rpf = wpf[selwvbuff] / 30;//2�t���[���������byte(2BYTE)count
	WORD rpf = wpf[selwvbuff] / 6;//10�t���[���������byte(2BYTE)count
	WORD lpf = rpf * ((Lcount + 1) % 6);//���݂̃o�b�t�@�ʒu
	WORD onef = wpf[selwvbuff] / 60; //1�t���[���̃o�C�g�������byte(2BYTE)count
	BYTE ch;
	BOOL nextbuff = FALSE;//���̃o�b�t�@�ɂ��܂�
	BYTE b;
	short mdata;
	
	if((Lcount + 1) % 6 == 0){nextbuff = TRUE;}
	if((Lcount + 1) % 6 == 5){rpf = wpf[selwvbuff] - (rpf * 5);}//�]�蕪��␳
	b = (now_buff + nextbuff) % 2;

//	for(ch = 0; ch < 4; ch++){
	for(ch = 0; ch < 16; ch++){
		for(i = 0; i < (DWORD)rpf; i++){//�Đ��I���o�b�t�@�֎����z���g�`�f�[�^�쐬
			if(!wv_ch[ch].length){break;}//���t�I���܂ł���CH�͏㏑���ł��Ȃ�
			if(!wv_ch[ch].on){break;}
			chbuffpos = (i + wv_ch[ch].playtime) % wv_ch[ch].length;//�g���ʒu

				if(chbuffpos == 0){
					if(!wv_ch[ch].loops){break;}
					else{wv_ch[ch].loops--;}
//						wave_modulation(4000, ch, 0);//pulsewidthseep
					
				}//loop���炷�B�炷�Ƃ��͂P�͕K�v
//				if(i % onef == onef - 1){
				if(i % onef == 0){
//					if(i){
//						if(!wv_ch[ch].loops){break;}
//						else{
						wv_ch[ch].pasloop++;
////						wave_fsimple(ch);
//						wave_modulation(ch, 0);
//						wave_modulation(ch, 1);
						wave_att(ch);
						wave_gain(ch);
						mdata = get_maxwv(ch);//�g�`�̒��_

						wave_modulation(mdata, ch, 0);//freqsweep//�K���g�`����蒼���|�C���g

						wave_modulation(mdata, ch, 1);//pulsewidthsweep
						wave_modulation(mdata, ch, 2);//Distortionsweep
						wave_efects(ch);
						wave_efects_t(ch);
//					}
				}

				if(i >= longest){
					lpplsWave[b][i + lpf] = WAVEDATA[ch][chbuffpos];
				}
				else{
					lpplsWave[b][i + lpf] += WAVEDATA[ch][chbuffpos];
				}

				chbuffpos = (chbuffpos + 1) % wv_ch[ch].length;//�o�b�t�@�I���p
//			}
		}
		wv_ch[ch].playtime = chbuffpos;//playtime�����o���ASRATE�܂ł������炻���܂ł����Z�Blength�ɓ��B�����炻���܂ł����Z�ƂȂ�B
		if(longest <= i){
			longest = i;
		}
	}
	for(i = longest; i < (DWORD)rpf; i++){//
		lpplsWave[b][i + lpf] = 0;
//		lpplsWave[tribuff][i] = 0;
	}
	
}
void wvpnt(void)//wave reset rewrite
{
	int i, j;
	DWORD DRATE = wpf[selwvbuff] * 2;//22050WORD �� 2�b�ŗ����B ���t���[����������
	//22050WORD �� 2�b�ŗ����B ���t���[����������
	BYTE ch;
	
	waveOutGetPosition(hplsWaveOut,&mmt,sizeof(MMTIME));//���Ԃ́H
	htbyte = (mmt.u.cb % DRATE) / 2;//BYTE --> WORD

	if((Lcount % 6)== 0){//�s���߂����烊�Z�b�g
		bcloop = htbyte /wpf[selwvbuff];//����ځH
		waveOutPrepareHeader(hplsWaveOut, &plsWhdr[rebuff], sizeof(WAVEHDR));
		waveOutWrite(hplsWaveOut,&plsWhdr[rebuff],sizeof(WAVEHDR));
		rebuff = now_buff;//�����ł��������Ȃ�
		now_buff = (now_buff + 1) % 2;//buffNO�؂�ւ�
	}
	continue_combcho();

	bctbyte = htbyte;//�O��̎���

}



void noizwave(BYTE ch, short vo)
{
	int i, j, p, c, len;
	DWORD DRATE;
	WORD volume_a, volume_sys;
//	short backdata = 0;
	BYTE d_div = 1;//delay�����_�ȉ��̓J�b�g
//	BYTE quotelen[64] = {64, 116, 4, 58, 102, 28, 6, 82,
//						 40, 16, 78, 50, 92, 36, 104, 130,//�g������
//	 					 24, 52, 94, 106, 60, 38, 126, 44,
//						 108, 84, 118, 30, 128, 80, 12, 18,
//						 124, 96, 76, 2, 26, 46, 68, 110,
//						 62, 22, 86, 120, 48, 112, 74, 122,
//						 72, 98, 14, 54, 32, 20, 114, 88,
//						 56, 10, 34, 100, 90, 42, 8, 70};//�g������
	BYTE quotelen[64] = {96, 14, 32, 192, 4, 255, 64, 128,
						 200, 36, 100, 10, 15, 68, 136, 16,//�g������
						 144, 104, 18, 208, 72, 6, 11, 40,
						 7, 30, 108, 44, 216, 20, 152, 76,
						 160, 48, 224, 112, 17, 80, 13, 22,
						 232, 116, 24, 168, 52, 9, 12, 84,
						 176, 240, 8, 88, 5, 26, 56, 120,
						 248, 60, 184, 92, 19, 124, 28, 17};//�g������
						
	BYTE duty;//�f���[�e�B��A�����ł̓X�L�b�v�ϓ���
	WORD fdl;
	WORD fdc;
	BYTE skip;
	BYTE han;
	BYTE maxlen;
	BYTE minlen;
	WORD datalen;
	
	len = quotelen[0];	//�g��
	
	if(vo > maxvol + (volume_m * 256)){vo = maxvol + (volume_m * 256);}
	if(vo == 0){volume_a = 0;}
	else{
		volume_sys = (volume_m) * 32;//�V�X�e���{�����[��
		if(wv_ch[ch].t_att < 0){volume_a = volume_sys * 2;}
		else{volume_a = vo;}
	}
	
	if(wv_ch[ch].type == 16){duty = 2;}
	else if(wv_ch[ch].type == 17){duty = 4;}
	else if(wv_ch[ch].type == 18){duty = 8;}
	else if(wv_ch[ch].type == 19){duty = 64;}//�Z������ v1111
	if(wv_ch[ch].type == 16){duty = 4;}
	else if(wv_ch[ch].type == 17){duty = 8;}
	else if(wv_ch[ch].type == 18){duty = 16;}
	else if(wv_ch[ch].type == 19){duty = 64;}//�Z������ v1111

	j = 0;
	han = 0;
	
	if(wv_ch[ch].freq < 33){
		wv_ch[ch].freq = 33;
	}
	maxlen = 8192 / wv_ch[ch].freq;//max:248+1

//	maxlen = DRATE / wv_ch[ch].freq;//max:248+1
	if(!maxlen){maxlen = 2;}
	wv_ch[ch].length = (SRATE * duty) / wv_ch[ch].freq;
	
	if(wv_ch[ch].length > 8840){
		wv_ch[ch].length = 8840;
	}
	
//	c = (wv_ch[ch].pasloop) % 64;//����
	c = 0;//����
	skip = (wv_ch[ch].pasloop) % duty;

//	p = (wv_ch[ch].pasloop) % 64;
	p = (wv_ch[ch].pasloop) % duty;

	wv_ch[ch].length = 256 + (duty * 4);
//	wv_ch[ch].length = 512;
	
	
	for(i = 0; i < wv_ch[ch].length; i++){	//�Đ�������̃o�b�t�@�֔g�`�f�[�^�쐬
			WAVEDATA[ch][i] = volume_a * (1 - (han * 2));
		if((j % len == 0)){
			p = (p + skip) % 64;
			len = (quotelen[p] % maxlen) + 1;
			han = (han + 1) % 2;
			
			j = 0;
			c++;
			if(c > duty){
				skip = (((skip + quotelen[p]) * 2) % 16) + 1;
				c = 0;
			}
			
		}
		j++;
		
		if(wv_ch[ch].length / 2 == i){
			if(ch > 15){
				wv_ch[ch].length = i;
				return;
			}
		}

	}
//	wv_ch[ch].maxg = FALSE;//gain stop�t���O
//	wv_ch[ch].playtime = 0;
//	wv_ch[ch].loops = ((SRATE) / wv_ch[ch].length) * wv_ch[ch].msec / 1000;
//	dly_wait[ch] = wv_ch[ch].dly * SRATE / 1000;
}



void trywave2(BYTE ch, short vo)
{
//	WAVEDATA[4][22050] = 
	int i, j,len,lens[8];
//	WORD volume_a, volume_sys, volume_b;
	WORD volume_a, volume_b;
	short backdata = 0;
//	BYTE priaddpos[2][16] = {7, 15, 3, 11,  1, 13, 5, 9,  6, 8, 0, 14,  4, 10, 2, 12,//���g���i0�O�p�p�A1���p�j
	BYTE priaddpos[2][16] = {8, 15, 3, 11, 1, 13, 5, 9,  6, 7, 14, 4,  10, 2, 12, 0,//���g���i0�O�p�p�A1���p�j
//	BYTE priaddpos[2][16] = {8, 15, 3, 11,  1, 13, 5, 9,  6, 0, 7, 14,  4, 10, 2, 12,//���g���i0�O�p�p�A1���p�j
//	BYTE priaddpos[2][16] = {0, 15, 7, 3,  12, 1, 8, 14,  6, 2, 13, 5,  10, 4, 9, 11,//���g���i0�O�p�p�A1���p�j
							0, 15, 7, 3,  12, 1, 8, 14,  6, 2, 13, 5,  10, 4, 9, 11};//�D��J�b�g�ʒu�B�g�`��4����1�̔��������肩��
	BYTE quotelen[32];//�g���l�������]��z��
	BYTE count32;
//	BYTE d_div = 1;//delay�����_�ȉ��̓J�b�g
	BYTE duty;//�f���[�e�B��
	BOOL noko = FALSE;//�S�m�R�M��
	BOOL half = TRUE;//
//	WORD fdl;
//	WORD fdc;
	BYTE part;
	BYTE updwcnt = 8;//up down down up
	BOOL ud = TRUE;//up down down up
	BOOL fb = FALSE;//�O�g�`��g�`
	BOOL modori = FALSE;

	
	if(wv_ch[ch].type > 11){//�̂��ݒ�
		noko = TRUE;
	}

	len = SRATE / wv_ch[ch].freq;//�g��
	wv_ch[ch].length = 2205;

//	volume_a = 0;
//	if(vo == 0){volume_a = 0; volume_sys = 0;}//������΃[��
	if(vo > maxvol + (volume_m * 256)){vo = maxvol + (volume_m * 256);}

	if(vo == 0){volume_b = 0;}//������΃[��
	else{

//		volume_sys = (volume_m) * 32;
		volume_b = vo / 8;//
	}
//	�O�p
//// -1111 -2222 -3333 -4444 -5555 -6666 -7777 -8888 _
//// -7777 -6666 -5555 -4444 -3333 -2222 -1111 -0000 _
//// +1111 +2222 +3333 +4444 +5555 +6666 +7777 +8888 _
//// +7777 +6666 +5555 +4444 +3333 +2222 +1111 +0000 _

//	�S��
//// -111h -1111 -222h -2222 -333h -3333 -444h -4444 _
//// -555h -5555 -666h -6666 -777h -7777 -888h -8888 _
//// +777h +7777 +666h +6666 +555h +5555 +444h +4444 _
//// +333h +3333 +222h +2222 +111h +1111 +000h +0000 _
	for(i = 0; i < 8; i++){
		lens[i] = len / 32; //�g��32����
	}
	for(i = 0; i < 16; i++){//���g�������Ă���
//		�S��
		if((len - (i * 2)) % 32){//+1�`+31����΁ii�̕������]��������Ă���͂��j
			quotelen[priaddpos[noko][i]] = 1;//�]���32�����g�`�ɂP�������Ă��\��
		}
		else{//�O������
			for(; i < 16; i++){//���Ƃ͂O
				quotelen[priaddpos[noko][i]] = 0;
				quotelen[priaddpos[noko][i] + 16] = 0;
			}
			break;
		}//�]��0�Ȃ甲��
//		�㔼
		if((len - (i * 2) - 1) % 32){//+1�`+31����΁ii�̕������]��������Ă���͂��j
			quotelen[priaddpos[noko][i] + 16] = 1;//�]���32�����g�`�ɂP�������Ă��\��
		}
		else{//�㔼����
			quotelen[priaddpos[noko][i] + 16] = 0;
			for(i++; i < 16; i++){//���Ƃ͂O
				quotelen[priaddpos[noko][i]] = 0;
				quotelen[priaddpos[noko][i] + 16] = 0;
			}
			break;
		}//�]��0�Ȃ甲��
	}
//	count32 = 31;//�ŏ��ɍ��f�[�^
	count32 = 0;//�ŏ��ɍ��f�[�^
//
//	if(len == 0){return;}
//	if(len - wv_ch[ch].dch){len -= wv_ch[ch].dch;}//���g���Ⴍ(�g���Z��)���Ă�������
//	fdl = 0;
	
//	maximam = maxvol * 4;
//	volume_sys = wv_ch[ch].vol * volume_sys;
	
	if(wv_ch[ch].type == 8){duty = 1;}
	else if(wv_ch[ch].type == 9){duty = 2;}
	else if(wv_ch[ch].type == 10){duty = 4;}
	else if(wv_ch[ch].type == 11){duty = 8;}

	if(wv_ch[ch].type > 11){//�̂��ݒ�
		noko = TRUE; duty = 1;
		ud = FALSE;
		fb = TRUE;
		updwcnt = 8;
		half = FALSE;
	}
	else{
		ud = TRUE;
		fb = TRUE;
		updwcnt = 0;
		part = 0;
//		half = FALSE;
	}
	volume_a = volume_b * (updwcnt + ((noko && half) * (0.5 - fb)));

	j = 0;
	for(i = 0; i < wv_ch[ch].length; i++){	//�Đ�������̃o�b�t�@�֔g�`�f�[�^�쐬

		if(j * duty >= (lens[0] + quotelen[(count32) % 32])){
			j = 0;
			if(lens[0] + quotelen[(count32) % 32] == 0){//�ق�Ƃ�j=0�̌�ɂ��ꂽ�����ǑODATA�������Ă��܂��̂ł����Ɍ���
				modori = TRUE;////�f�[�^��߂��Ĕg�`���k�߂�
			}

			if(!noko){
				if(updwcnt == 0 || updwcnt == 8){//��ΒlUP�EDOWN�؂�ւ�
					if(part == 3){
						fb = FALSE;
						ud = FALSE;
						part = 0;
					}
					else if(part == 2){
						fb = FALSE;
						ud = TRUE;
						part++;
					}
					else if(part == 1){
						fb = TRUE;
						ud = FALSE;
						part++;
					}
					else if(part == 0){
						fb = TRUE;
						ud = TRUE;
						part++;
					}
/*					if(fb){
						ud = (ud + 1) % 2;
					}
				}
				if(updwcnt == 0){//�O���㔼�`�F���W
//					if(!ud){
						fb = (fb + 1) % 2;
//					}
*/				}
				updwcnt = (updwcnt + (2 * ud) - 1);//�J�E���g���Ă���
				if(ch > 15){
					if(part == 3){
						wv_ch[ch].length = i;
						return;
					}
				}
			}
			else{
				half = (half + 1) % 2;//�K���ʂ�0 -> 0.5 -> 1 -> 1.5
				if(half){
					if(updwcnt == 0){//�O���㔼�`�F���W
						fb = (fb + 1) % 2;
					}
				}
				if(!half){
					if(updwcnt == 0){//�O���㔼�`�F���W
						ud = (ud + 1) % 2;
					}
					updwcnt = (updwcnt + (2 * ud) - 1);//�J�E���g���Ă���

				}
				if(ch > 15){
					if(!fb){
						wv_ch[ch].length = i;
						return;
					}
				}
			}
//			volume_a = volume_sys * volume_b * (updwcnt + ((noko && half) * (0.5 - (fb))));
			volume_a = volume_b * (updwcnt + ((noko && half) * (0.5 - fb)));

			count32 = (1 + count32) % (32);
//			count32 = (31 + count32) % (32);
		}
		if(modori){
//			if(i && updwcnt != 8){
			if(i){
				i--;
				modori = FALSE;
			}
		}
		WAVEDATA[ch][i] = volume_a * ((2 * fb) - 1);
		j++;//�g���J�E���g//23nooko
		
		if(i && i % len == 0){//���̎�����������Ȃ�
//			wv_ch[ch].maxg = FALSE;
//			wv_ch[ch].playtime = 0;
			wv_ch[ch].length = i;
//			wv_ch[ch].loops = ((SRATE) / wv_ch[ch].length) * wv_ch[ch].msec / 1000;
//			dly_wait[ch] = wv_ch[ch].dly * SRATE / 1000;
			return;
		}
	}

}



void plswave(BYTE ch, short vo)
{
	WORD i, j, len, nowad;
//	WORD volume_a, volume_sys;
	WORD volume_a;
	WORD maxdlt;//�ω���
	short backwv;//��O
//	short backdata = 0;
	BYTE d_div = 1;//delay�����_�ȉ��̓J�b�g
	BYTE duty;//�f���[�e�B��
	WORD noko;//���m�R�M��
	WORD fds;//�����炷��
	WORD fdr;//���g���Ɉ��
	WORD fdl;//���Ł�WORD�����
	WORD fdc;
	BOOL ltr = FALSE;

//	htbyte = timebyte / 2;//�������݈ʒu
	
	len = SRATE / wv_ch[ch].freq;	//�g��

	wv_ch[ch].length = 2205;
	
	backwv = 0;
	j = 0;
//	if(wv_ch[ch].vol == 0){volume_a = 0;}
//	else{
//		if(wv_ch[ch].t_att < 0){volume_a = volume_sys * 2;}
//		else{volume_a = wv_ch[ch].vol * volume_sys;}
//	}
	if(vo > maxvol + (volume_m * 256)){vo = maxvol + (volume_m * 256);}
	if(vo == 0){volume_a = 0;}
	else{
//		volume_sys = (volume_m) * 32;//�V�X�e���{�����[��
		volume_a = vo;
	}
	
	if(wv_ch[ch].type == 0){duty = 20; noko = 0; maxdlt = 65535;}
	else if(wv_ch[ch].type == 1){duty = 30; noko = 0; maxdlt = 65535;}
	else if(wv_ch[ch].type == 2){duty = 50; noko = 0; maxdlt = 65535;}
	else if(wv_ch[ch].type == 3){duty = 90; noko = 0; maxdlt = 65535;}

	else if(wv_ch[ch].type == 4){duty = 20; ltr = TRUE;}
	else if(wv_ch[ch].type == 5){duty = 40; ltr = TRUE;}
	else if(wv_ch[ch].type == 6){duty = 80; ltr = TRUE;}
	else if(wv_ch[ch].type == 7){duty = 13; ltr = TRUE;}
	
	if(ltr){
		 noko = (volume_a / len); maxdlt = (volume_a / 4);
	}
	
	for(i = 0; i < wv_ch[ch].length; i++){	//�Đ�������̃o�b�t�@�֔g�`�f�[�^�쐬
		if(ltr == 8){
			if(i % len < 10 * len / duty){//�g�`��
				WAVEDATA[ch][i] = -volume_a + (noko * i);
				if(-volume_a + backwv < -maxdlt){
					if(WAVEDATA[ch][i] < -volume_a){
						WAVEDATA[ch][i] = -volume_a + (noko * i);
					}
				}
			}
			else{//�g�`��
				if(ch > 15){
					wv_ch[ch].length = i;
					return;
				}
				WAVEDATA[ch][i] = volume_a - (noko * j);
				if(volume_a - backwv > maxdlt){
					if(WAVEDATA[ch][i] > volume_a){
						WAVEDATA[ch][i] = volume_a - (noko * j);
					}
				}
				j++;
			}
		}
		else{
			
			if(i % len < 10 * len / duty){//�g�`��
				WAVEDATA[ch][i] = volume_a - (noko * i);
				if(volume_a - backwv > maxdlt){
	//				WAVEDATA[ch][i] = maxdlt + backwv;
					if(WAVEDATA[ch][i] > volume_a){
						WAVEDATA[ch][i] = volume_a - (noko * i);
					}
				}
			}
			else{//�g�`��
				if(ch > 15){
					wv_ch[ch].length = i;
					return;
				}
				WAVEDATA[ch][i] = -volume_a + (noko * j);
				if(-volume_a + backwv < -maxdlt){
	//				WAVEDATA[ch][i] = backwv - maxdlt;
					if(WAVEDATA[ch][i] < -volume_a){
						WAVEDATA[ch][i] = -volume_a + (noko * j);
					}
				}
				j++;
			}
		}

		backwv = WAVEDATA[ch][i];
		if(i && i % len == 0){//���̎�����������Ȃ�
//			wv_ch[ch].maxg = FALSE;
//			wv_ch[ch].playtime = 0;
			wv_ch[ch].length = i;
//			wv_ch[ch].loops = ((SRATE ) / wv_ch[ch].length) * wv_ch[ch].msec / 1000;
//			dly_wait[ch] = wv_ch[ch].dly * SRATE / 1000;
			return;
		}
	}
		
}

//void voidwave(BYTE ch, short vo){//�z��O�̓N���A
void voidwave(BYTE ch){//�z��O�̓N���A
	WORD i, j;
	
	for(i = 0; i < wv_ch[ch].length; i++){	//�Đ�������̃o�b�t�@�֔g�`�f�[�^�쐬
		WAVEDATA[ch][i] = 0;
//		if(i && i % len == 0){//���̎�����������Ȃ�
//			wv_ch[ch].maxg = FALSE;
//			wv_ch[ch].playtime = 0;
//			wv_ch[ch].length = i;
//			wv_ch[ch].loops = ((SRATE ) / wv_ch[ch].length) * wv_ch[ch].msec / 1000;
//			dly_wait[ch] = wv_ch[ch].dly * SRATE / 1000;
//			return;
//		}
	}
}


BYTE wvch_sel(BYTE ch, short vo)
{
	BYTE i;
	BYTE tr = 0;
//	for(i = 0; i < 4; i++){
//		getwaveplaytime(i);
//		if(wv_ch[ch].on && !wv_ch[ch].busy){
		if(wv_ch[ch].on){

			if(wv_ch[ch].type < 8){
//				wv_set(i);
				plswave(ch, vo);
				tr = 1;
			}
			else if(wv_ch[ch].type < 16){
				if(wv_ch[ch].type == 8 || wv_ch[ch].type == 12){
					trywave2(ch, vo);
					tr = 1;
				}
//				else{voidwave(ch, vo); wv_ch[ch].loops = 0;}
				else{voidwave(ch); wv_ch[ch].loops = 0;}
			}
			
			else if(wv_ch[ch].type < 20){
				noizwave(ch, vo);
				tr = 1;
			}
		}
//	}
//	combch_rb();
//	combch_r();
	return(tr);
}


short wvbase_set(BYTE ch){
//	WORD i, j, len, nowad;
	WORD volume_a, volume_sys;

	volume_sys = (volume_m) * 32;//�V�X�e���{�����[��
	htbyte = timebyte / 2;//�������݈ʒu
	
//	len = SRATE / wv_ch[ch].freq;	//�g��

	wv_ch[ch].length = 2205;
	
	wv_ch[ch].playtime = 0;

	if(wv_ch[ch].vol == 0){volume_a = 0;}
	else{
//		if(wv_ch[ch].t_att < 0){volume_a = volume_sys * 2;}
		if(wv_ch[ch].t_gin){
			volume_a = volume_sys * 2;
			wv_ch[ch].maxg = FALSE;
		}
		else{
			volume_a = wv_ch[ch].vol * volume_sys;
			wv_ch[ch].maxg = TRUE;
		}
		if(wv_ch[ch].type > 3 && wv_ch[ch].type < 8){
			volume_a *= 1.25;
		}
		else if(wv_ch[ch].type == 8){
			volume_a *= 2;
		}
		else if(wv_ch[ch].type == 12){
			volume_a *= 1.5;
		}
		
	}
	return volume_a;

}

void wv_loop(BYTE ch)
{
//	if(wv_ch[ch].type > 10){return;}
	wv_ch[ch].loops = ((SRATE * 16 / wv_ch[ch].length) * wv_ch[ch].msec) / 1000;
	if(!wv_ch[ch].loops){wv_ch[ch].loops = 1;}
	wv_ch[ch].pasloop = 0;
//	wv_ch[ch].loops = 10;
	dly_wait[ch] = wv_ch[ch].dly * SRATE / 200;
}

void wv_test(void)//wavetest
{
//	BYTE seton = 0;
	BYTE ch;
	short vo;
//	for(ch = 0; ch < 4; ch++){
	for(ch = 0; ch < 16; ch++){
		if(wvch_sel(ch, wvbase_set(ch))){
			wv_loop(ch);
		}
		
//		vo = wvbase_set(ch);
//		seton = wvch_sel(ch, vo);//������������
//		if(seton){
//			wv_loop(ch);
//		}
	}
}

////�q��
/*
	for(ch = 0; ch < 4; ch++){
		for(i = 0; i < (rpf * 4); i++){
			if(i + htbyte >= SRATE * 2){break;}
			if(i + wv_ch[ch].playtime >= SRATE * 2){break;}
			lpplsWave[now_buff][i + htbyte] = WAVEDATA[ch][i + wv_ch[ch].playtime];
		}
		if(wv_ch[ch].playtime < SRATE * 2){
			wv_ch[ch].playtime += rpf;
		}
		else{
			wv_ch[ch].playtime = SRATE * 2;
		}
	}
*/
