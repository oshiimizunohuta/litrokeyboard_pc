//�r�f�I�ݒ�

//�r�b�g�}�b�v�R���e�L�X�g�AOld�ȑO�֘A�t�����ꂽ���͍̂폜�p
//StretchDIBits�֐��ň���
	HDC hdcmapBMP;
	HBITMAP hmapBMP, hmapOldBMP;//�}�b�v�X�N���[���p
	BITMAPINFO biBBInfo;

	HDC hdcCmapBMP;
	HBITMAP hCmapBMP, hCmapOldBMP;//���̃}�b�v�X�N���[���p

	HBITMAP hOld;//����Ȃ��H

	HDC hdcstaBMP;
	HBITMAP hstaBMP, hstaOldBMP;//�X�e�[�^�X�E�B���h�E�p
	BITMAPINFO staBInfo;
	LPDWORD lpstaBPixel;

	HDC hdcmesBMP;
	HBITMAP hmesBMP, hmesOldBMP;//���b�Z�[�W�E�B���h�E�p
	BITMAPINFO mesBInfo;

	LPDWORD lpmesBPixel;//�I

//�o�b�N�o�b�t�@�B�����ɏW�߂ĕ\������B
HBITMAP hBBBMP, hOldBBBMP;
HDC hdcBB;

LPDWORD lpBBPixel;//�d�v

BYTE dSize = 1;

WORD Dwidth = 640;//bitmap�̈� 
WORD Dheight = 480;
DWORD MAXPIX = 307200;
DWORD MAXPIX_msg = 115200;
DWORD MAXPIX_sta = 92160;
BYTE Mwidth = 14;//���\���}�X
BYTE Mheight = 10;//���\���}�X
WORD Xdsize;//Mwidth * 32
WORD Ydsize;//

WORD STAwidth = 352, STAheight = 288;//�X�e�[�^�X������
WORD MESwidth = 640, MESheight = 180;//���b�Z�[�W������

WORD cub128 = 16384, cub256 = 65536;


void setpixx(DWORD disp, DWORD bcolor, LPDWORD lpd, BYTE multi)
{
	BYTE i, j;
	DWORD dispP;//�\��
//	WORD dwidth = Dwidth;

//	lpBBPixel[disp] = *bmpix;
	for(i = 0; i < multi; i++){
		if((disp + (i * Dwidth)) > MAXPIX){break;}//�傫������l�͖���
		for(j = 0; j < multi; j++){
//			lpBBPixel[disp + j + (i * Dwidth)] = bcolor;//��i�ځH
			lpd[disp + j + (i * Dwidth)] = bcolor;//��i�ځH
		}
	}

}


////�������ꌅ���\��////060730
DWORD jussin(DWORD kazu, BYTE keta, WORD XX, WORD YY, DWORD collor1, DWORD collor2)
{//�\�����鐔���ƌ��������Ă��܁B
	BYTE i, j, k, nanketa;//d_kazu ha 256=0�����ǋC�ɂ��Ȃ�
	BOOL minus = FALSE;
	
	DWORD d_kazu;//�����ӂ�Ă�060723
	DWORD dispP;
	LPDWORD mendoi;
	
	if(keta < 6 && kazu > 65535){kazu *= -1; minus = TRUE;}
	for(nanketa = 0; nanketa < keta; nanketa++){
		d_kazu = kazu / pow(10, nanketa);//keta�����P���ڂɎ����Ă���
		i = d_kazu % 10;
		for(j = 0; j < 8; j++){
			for(k = 0; k < 8; k++){
				dispP = XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (8 - j + YY));
				if(dispP > MAXPIX){break;}
//				if(*(NUM[i] + k + (j * 8)) == 0x00ffff00){
				if(*(pix.data[pix.numb_ad + i] + k + (j * 8)) == 0x00ffff00){
//					if(minus){lpBBPixel[XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (8 - j + YY))] = 0xeeeeee;}
					if(minus){lpBBPixel[dispP] = 0xeeeeee;}
					else{lpBBPixel[XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (8 - j + YY))] = collor1;}
				}
//				if(*(NUM[i] + k + (j * 8)) == 0x00808000){
				if(*(pix.data[pix.numb_ad + i] + k + (j * 8)) == 0x00808000){
					if(minus){lpBBPixel[XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (8 - j + YY))] = 0xeeeeee;}
					else{lpBBPixel[XX + k - ((nanketa - keta + 1) * 8) + (Dwidth * (8 - j + YY))] = collor2;}
				}
			}
		}
	}
	return 0;
}


void icon_draw(LPDWORD lppix, BYTE dib, int x, int y, BYTE size, BYTE reva, BYTE multi)
{
	BYTE i, j, hamofx, hamofy;
	DWORD disps;//�\��
	LPDWORD lpsel;

	if(x < 0){hamofx = -x; x = 0;}
	else{hamofx = 0;}
	if(y < 0){hamofy = -y; y = 0;}
	else{hamofy = 0;}
	disps = x + (y * Dwidth);
	
	switch(dib){
		case 0: lpsel = lpBBPixel; break;
		case 1: lpsel = lpstaBPixel; break;
		case 2: lpsel = lpmesBPixel; break;
		default : lpsel = lpBBPixel; break;
	}
	switch(reva){
		case 0://����
		lppix += (size * (size - 1 - hamofy)) + hamofx;
		for(i = hamofy; i < size; i++){
			if(y + i < Dheight){
				for(j = hamofx; j < size; j++){
					if(*lppix && (x + j < Dwidth)){
						setpixx(disps, *lppix, lpsel, multi);
					}
					disps++;
					lppix++;
				}
			}
			disps += (Dwidth - j + hamofx);
			lppix -= size + size - hamofx;
		}break;
		
		case 1://���E���]
		lppix += (size * size) - 1;
		for(i = 0; i < size; i++){
			for(j = 0; j < size; j++){
				if(*lppix && (x + j < Dwidth)){
					setpixx(disps, *lppix, lpsel, multi);
				}
				disps++;
				lppix--;
			}
			disps += (Dwidth - j);
//			lppix -= j + j;
		}break;
		
	}
}



