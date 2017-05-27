/*********オリジナル計算関数*********/

/*******rndm*******/
BYTE rp = 0;//rndm呼び出し回数
BYTE redro = 0;//再描回数
BYTE failp = 0;
/******************/

////randam///基本は value = rndm(Lcount, redro, 1[桁数], true(+符号), max)
int rndm(BYTE times, BYTE option, BOOL hu, WORD m_val){
	long value;//99億tukau
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
				case 1:value = 3529678401;break;////////////ら////////////
				case 2:value = 2417305869;break;////////////　////////////
				case 3:value = 9154282736;break;////////////ん////////////
				case 4:value =  792831654;break;////////////　////////////
				case 5:value = 6835094217;break;////////////だ////////////
				case 6:value = 4203956178;break;////////////　////////////
				case 7:value = 8376120945;break;////////////　////////////
				case 8:value = 7941563082;break;////////////む////////////
				case 9:value = 5268417390;break;//////////////////////////
				default:break;
			}

			value = value / pow(10, (failp + times) % 10);//redraw桁を１桁目に持っていく
			bb_val += (value % 10) * (pow(10, i));//hitoketame
		
			rp++;if(rp > 249){rp = 0;}
		}
		failp++;if(failp > option){failp = 0;}
		
		if(bb_val < m_val){break;}
	}

	if(hu == TRUE){return(bb_val);}//ﾌﾟﾗｽﾉﾐ

	else if(hu == FALSE){////マイナスアリ
		int_val	= bb_val;
		if((times + rp) % 2 == 1){
			int_val	*= -1;
		}
		return(int_val);
	
	}
	return(1);

}

////円16等分三角関数
BYTE HEXcircle(BYTE kakudo/*角度16等分*/, BYTE vecter, BOOL Ymode){
	BYTE ans,/* ansY,*/ l_kakudo,/*0=0_1=90_2=180_3=270*/ s_kakudo/*0=0, 1=30, 2=45, 3=60, 4=90, 5=120, 6=135, 7=150, ....*/;
	
//	l_kakudo = kakudo / 4;
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
