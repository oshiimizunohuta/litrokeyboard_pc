////////////sound
LPPCMWAVEFORMAT lpPWFom;
short * lpPCM;
LPBYTE lpBuf;

struct EfectSound{
	WORD dsize;
	short aSnd[22050];
	WORD fre;
	WORD mxvl;
	WORD block;
	WORD end;
	BYTE pic;

}eSnd[50];


void nijikyoku(){
}

//	Å^Å_Å^Å_Å^Å_Å^Å_ //éOäpîg!!//  Å^Å_Å^Å_Å^Å_Å^Å_ //
short sankaku(sndNo){
	WORD bpf, qubpf, delta;//1é¸ä˙Ç†ÇΩÇËÇÃblockêîÅA4ï™ÇÃ1é¸ä˙ÇÃÇ ÉçÉbÉNêîÅAïœâªó 
	WORD i, j;
	
	BOOL hilo = FALSE, idcr = TRUE;// 
	
	BOOL fend = FALSE;
	
	bpf = eSnd[sndNo].block / eSnd[sndNo].fre;//é¸ä˙ÅHÇPHzâΩÉuÉçÉbÉNÅH
	qubpf = bpf / 4;//4Ç≈äÑÇÈÅB
	delta = eSnd[sndNo].mxvl / qubpf;//blockÇ†ÇΩÇËÇÃïœâªó 
	
	for(i = 0; i < eSnd[sndNo].block / qubpf; i++){
		for(j = 0; j < qubpf; j++){
			if(j + (i * qubpf) > eSnd[sndNo].end){fend = TRUE; break;}//dataendÇ‹Ç≈Ç´ÇΩÇÁî≤ÇØ
				eSnd[sndNo].aSnd[j + (i * qubpf)] = -(j * delta);//(qubpf * (idcr) * (-1)) + (j * delta) * (-idcr) ;
		}
		if(i % 4 == 0){hilo = FALSE; idcr = TRUE;}//íÜÅÑè„
		else if(i % 4 == 1){hilo = TRUE; idcr = FALSE;}//è„ÅÑíÜ
		else if(i % 4 == 2){hilo = FALSE; idcr = FALSE;}//íÜÅÑâ∫
		else if(i % 4 == 3){hilo = TRUE; idcr = TRUE;}//â∫ÅÑíÜ
		
		if(fend == TRUE){break;}//Ç≥ÇÁÇ…î≤ÇØ
	}
			
		
}

	////////ecco modurater ver0.1//////////
	
void ecco(sndNo){
	
}


void soundsyoki(void){
	WORD i, j, k, l;
	WORD fre, fblock = 22050, dblock, vlm, hexB, bprad, skiprad;
	WORD cp;

	float bpf, bpf2, pic, henkaryo;
	
	WORD count, kioku;
	
	eSnd[0].dsize = 22050;///blocky

	
	lpBuf = (LPBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 44 + eSnd[0].dsize);
	
//	aSnd = (short *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 22050);

	
	lpPWFom = (LPPCMWAVEFORMAT)(lpBuf + 20);
	lpPCM = (short *)(lpBuf + 44);//
	
	/*WAVEÉwÉbÉ_*/
	lpBuf[0] = 'R';
	lpBuf[1] = 'I';
	lpBuf[2] = 'F';
	lpBuf[3] = 'F';
	
	*((LPDWORD)(lpBuf + 4)) = 36 + eSnd[0].dsize;//filesize
	
	lpBuf[8] = 'W';
	lpBuf[9] = 'A';
	lpBuf[10] = 'V';
	lpBuf[11] = 'E';
	lpBuf[12] = 'f';
	lpBuf[13] = 'm';
	lpBuf[14] = 't';
	lpBuf[15] = ' ';
	
	*((LPDWORD)(lpBuf + 16)) = 16;//formatinfosize
	
	/*PCMWAVEFORMATê›íË*/
	lpPWFom -> wf.wFormatTag = WAVE_FORMAT_PCM;
	lpPWFom -> wf.nChannels = 1;
	lpPWFom -> wf.nSamplesPerSec = fblock;
	lpPWFom -> wf.nAvgBytesPerSec = 22050;
	lpPWFom -> wf.nBlockAlign = 2;
	lpPWFom -> wBitsPerSample = 16;
	
	lpBuf[36] = 'd';
	lpBuf[37] = 'a';
	lpBuf[38] = 't';
	lpBuf[39] = 'a';
	
	/*datasize*/
	*((LPDWORD)(lpBuf + 40)) = eSnd[0].dsize;//formatinfosize

	////soundÇO
	for(i = 0; i < 16; i++){
		for(j = 0; j < eSnd[0].dsize; j++){
		
			eSnd[i].aSnd[j] = 0;
		}
		
		eSnd[i].fre = 1;
		eSnd[i].block = 8000;
		eSnd[i].mxvl = 1000;
	}

	
	eSnd[1].fre = 300;
	eSnd[1].block = 8000;
	eSnd[1].mxvl = 4000;

	////squ+16sin	
	for(i = 0; i < 300; i++){
		for(j = 0; j < 16; j++){
			eSnd[1].aSnd[j + (i * 32)] = -8000 + (i * 40);
			eSnd[1].aSnd[j + (i * 32) + 16] = 8000 - (i * 40);
		}
	}

	
	for(i = 0; i < 4000; i++){
		eSnd[1].aSnd[i] = HEXcircle((i % 128)/8, eSnd[1].aSnd[i] / 40, TRUE);
		eSnd[1].aSnd[i] *= 40;
//		lpPCM[i] =(i * 1000) ;
	}
	for(i; i < 8000; i++){
		eSnd[1].aSnd[i] = HEXcircle((i % 128)/8, -(eSnd[1].aSnd[i] / 40), TRUE);
		eSnd[1].aSnd[i] *= 40;
//		lpPCM[i] =(i * 1000) ;
	}
//1sec atari 22050block.....
//1Hz de 22050block tukau......
////10Hz nara 1syuu 2205block?
//////sarani 16bunkatu site "piza hitokire" 125block

	////jump
	dblock =4056;//zentai blocksiyouryou
	fre = 80;
	bpf = fblock / fre;//1syuuki atarino block siyouryou
	vlm = 8000;//vol
	hexB = bpf / 16;//onaji kakudo wo tukau blocksuu
	
	pic = 8;

	eSnd[2].fre = 80;
	eSnd[2].block = 4056;
	eSnd[2].mxvl = 3000;
	
	for (i = 0; dblock > (i * bpf); i++){//zentai ni tassurumade
		for(k = 0; k < 32; k++){//circle/16 //zenbu owatte 1 syuuki
			if(k < 16){eSnd[2].aSnd[(i * bpf) + (hexB * k)] = HEXcircle(k, eSnd[2].mxvl / 400, FALSE) * 400;}
			if(k > 15){eSnd[2].aSnd[(i * bpf) + (hexB * k)] = -HEXcircle(k % 16, eSnd[2].mxvl / 400, FALSE) * 400;}
			

			for(l = 1; l < (hexB) - (i * pic); l++){//onaji kakudo wo copyyy
				if((k * hexB) + l >= dblock){break;}//zentai block koetara end
				eSnd[2].aSnd[(i * bpf) + (k * hexB) + l] = eSnd[2].aSnd[(i * bpf) + (k * hexB)]; //(16 * hexB => bpf)
			}
		}
	}
	
/*	for(i = 0; i < dblock; i++){
		eSnd[2].aSnd[i] = ;
		eSnd[2].aSnd[i] /= 2;
	}
	*/
/*	bprad = bpf / 360;//sono syuuki deno 1do atarino block siyousuu
	skiprad = 0;
	if(bprad < 1){bprad = 1; skiprad = 360 / bpf;}
	
	for (i = 0; dblock > (i * bpf); i++){//zentai ni tassurumade
		for(k = 0; k < 360 / skiprad; k++){//circle/16 //zenbu owatte 1 syuuki
			eSnd[2].aSnd[(i * bpf) + ( * k)] = math.sin(k + skiprad);

			for(l = 1; l < (bprad); l++){//onaji kakudo wo copyyy
				if((k * hexB) + l >= dblock){break;}//zentai block koetara end
				eSnd[2].aSnd[(k * hexB) + l] = eSnd[2].aSnd[(k * hexB)]; //(16 * hexB => bpf)
			}
		}
	}
*/	
	eSnd[3].fre = 300;
	eSnd[3].block = 4000;
	eSnd[3].mxvl = 1000;
	eSnd[3].pic = 0;

	bpf = eSnd[3].block / eSnd[3].fre;
	
	for(i = 0; i < eSnd[3].fre; i++){
		if(j + bpf > eSnd[3].block){break;}
		for(j = 0; j < (bpf / 2); j++){
			if(j + bpf > eSnd[3].block){break;}

			eSnd[3].aSnd[j + (i * bpf)] = -eSnd[3].mxvl;
			eSnd[3].aSnd[j + (i * bpf) + (bpf/2)] = eSnd[3].mxvl;
			
		}
	}
//fullblock=22050
	eSnd[4].fre = 200;
	eSnd[4].block = 2000;
	eSnd[4].end = 2000;
	eSnd[4].mxvl = 3200;
	eSnd[4].pic = 1;
	
	eSnd[5].fre = 300;
	eSnd[5].block = 9000;
	eSnd[5].end = 8500;
	eSnd[5].mxvl = 3200;
	eSnd[5].pic = 1;
	
	eSnd[6].fre = 3000;
	eSnd[6].block = 2000;
	eSnd[6].end = 2000;
	eSnd[6].mxvl = 3200;
	eSnd[6].pic = 1;
	
	eSnd[7].fre = 100;
	eSnd[7].block = 12000;
	eSnd[7].end = 10000;
	eSnd[7].mxvl = 3200;
	eSnd[7].pic = 1;

	eSnd[8].fre = 20;
	eSnd[8].block = 12000;
	eSnd[8].end = 8500;
	eSnd[8].mxvl = 3200;
	eSnd[8].pic = 1;
	
	eSnd[9].fre = 400;
	eSnd[9].block = 22000;
	eSnd[9].end = 8500;
	eSnd[9].mxvl = 3200;
	eSnd[9].pic = 1;
	
	eSnd[10].fre = 400;
	eSnd[10].block = 22000;
	eSnd[10].end = 1200;
	eSnd[10].mxvl = 3200;
	eSnd[10].pic = 1;
	
	//…≤ΩﬁâπóÃàÊ
	
	eSnd[11].fre = 8;
	eSnd[11].block = 2500;
	eSnd[11].mxvl = 3000;
	eSnd[11].end = 300;
	
	//éOäpîgâπóÃàÊ

	eSnd[16].fre = 200;
	eSnd[16].block = 5000;
	eSnd[16].mxvl = 2500;
	eSnd[16].end = 5000;

	////////ham sound modurater ver0.1//////////
	for(k = 4; k < 11; k++){
		bpf = eSnd[k].block / eSnd[k].fre;//é¸ä˙ÅHÇPHzâΩÉuÉçÉbÉNÅH
		henkaryo = (bpf / eSnd[k].fre) * 2;//20/1000->2/100->0.020*2(1é¸ä˙Ç≈0.040blockïœâª)
		bpf2 = 0;
		count = 0;
	
		for(i = 0; i < eSnd[k].fre; i++){//20000/1000->20block, 20000block/20block ->1000kai
			if(j + bpf > eSnd[k].block){break;}

			bpf2 = bpf;//picnasi

			kioku = count;//ëOâÒÇÃãLâØ

			if(eSnd[k].pic){
				bpf2 = bpf + (bpf - (henkaryo * i));//é¸ä˙ïœâª(40-80-120)
			}
			if(bpf2 < 2){bpf2 = 2;}

			for(j = 0; j < (bpf2 / 2); j++){
				if(count < eSnd[k].end){

					eSnd[k].aSnd[kioku + j] = -eSnd[k].mxvl; count++;
					eSnd[k].aSnd[kioku + j + (bpf2/2)] = eSnd[k].mxvl; count++;
				}
			}
		}
	}
	for(i = 0; i < 8; i++){
		CopyMemory((LPDWORD)eSnd[10].aSnd + (i * eSnd[10].end), (LPDWORD)(eSnd[10].aSnd), eSnd[10].end);
	}


//	eSnd[11].fre = 8;
//	eSnd[11].block = 2500;
//	eSnd[11].mxvl = 3000;
//	eSnd[11].end = 300;

	for(i = 0; i < eSnd[11].end; i++){
		eSnd[11].aSnd[i] = rndm(i, 4, FALSE, eSnd[11].mxvl);

	}
	
	for(i = 0; i < 10; i++){
//		CopyMemory((LPDWORD)eSnd[11].aSnd + (i * eSnd[11].end), (LPDWORD)(eSnd[11].aSnd), eSnd[11].end);
	}
	
	for(i = eSnd[11].block; i > 0; i--){
		for(j = 0; j < eSnd[11].fre; j++){
			eSnd[11].aSnd[(i * eSnd[11].fre) - j] = eSnd[11].aSnd[i];
//		eSnd[11].aSnd[(i * 8) - 1] = eSnd[11].aSnd[i];
//		eSnd[11].aSnd[(i - ) * 3] = eSnd[11].aSnd[i - 1];
		}
	}
	
	sankaku(16);
}


void set_sound(LPDWORD PCM, short option){
	WORD i, j;
	
	
	for(i = 0; i < 22050; i++){
		*(PCM + 1) = 0;
	}
	
}