//�I���W�i��BMP�����o��
WORD fit_col(DWORD color, LPDWORD pal);//�Ȃ����v���g�^�C�v�����Ő擪�ɏo���Ȃ�

//�I���W�i��BMP�����o��

//void dump_bmp(LPDWORD palad, LPDWORD pixad, BYTE xblock, BYTE yblock, BYTE mas, BYTE FNo, LPBYTE op)//op=.gdt
void dump_bmp(LPDWORD palad, LPDWORD pixad, BYTE xblock, BYTE yblock, BYTE mas, BYTE FNo, char *op)//op=.gdt
{
	char type[5];
	char no[3] = {'0', '0', '0'};
	char names[16];
	char *name;
	
	HANDLE hFile;
	OPENFILENAME ofn;
	DWORD D, i;
	DWORD FileSize;

	WORD fitcol, backfcol;//256��������
	BYTE cnt;
//	BYTE memcol, k, val = 0;
	BYTE k, val = 0;

	strcpy(names, "BMPFILES/");


	for(i = 0; i < 3; i++){
		switch(i){
			case 0:	val = (FNo) / 100;break;//100
			case 1:	val = (FNo % 100) / 10;break;//10
			case 2:	val = (FNo) % 10;break;//1
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
	for(i =0; i < 3; i++){
		names[9 + i] = no[i];
	}

//	strcpy(type, &op);//�g���q�Z�b�g

	for(; i < 7; i++){//4moji
//		names[9 + i] = type[i - 3];
		names[9 + i] = op[i - 3];
	}
	names[9 + i] = NULL;
	
	name = &(names[0]);


//	val = 0;
	hFile = CreateFile(name, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	WriteFile(hFile, &xblock, 1, &D, NULL);//block��
	WriteFile(hFile, &yblock, 1, &D, NULL);//block��
	WriteFile(hFile, &mas, 1, &D, NULL);//xy�}�X
	WriteFile(hFile, palad, 256 * 4, &D, NULL);//�p���b�g
	
	D = 0;
//	memcol = 0;
	backfcol = 0;
	cnt = 0;
	//���k
	for(i = 0; i < (DWORD)(xblock * yblock * mas * mas); i++){
		fitcol = fit_col(*(pixad + i), palad);//fit�����p���b�g
//		memcol = fitcol;
		if(!i){
			WriteFile(hFile, &fitcol, 1, &D, NULL);
			backfcol = fitcol;
		}//�ŏ�

		if(255 < fitcol){//�t�B�b�g���Ȃ�������
			MessageBox(NULL,"���s�B", name, MB_OK);
			return;
		}
		else{
			if(backfcol == fitcol){//�����F��������
				if(cnt == 255){
					WriteFile(hFile, &cnt, 1, &D, NULL);//dataNO
					cnt = 0;
					backfcol = fitcol;
				}
				else{
					cnt++;
				}
			}
			else{
				WriteFile(hFile, &cnt, 1, &D, NULL);//dataNO
				cnt = 0;
				backfcol = fitcol;
			}
			if(!cnt){WriteFile(hFile, &fitcol, 1, &D, NULL); cnt++;}//dataNO
		}
	}
	WriteFile(hFile, &cnt, 1, &D, NULL);//�Ō�
	CloseHandle(hFile);

}


WORD fit_col(DWORD color, LPDWORD pal)//�t�B�b�g�����J���[�p���b�g��Ԃ�
{
	WORD cad = 0;
	
	while(cad < 256){
//		if(*(pal + cad) == color){return cad;}
		if(*(pal + cad) == color){break;}
		else{cad++;}
	}
	return cad;
}

