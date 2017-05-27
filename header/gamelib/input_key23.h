/*キー操作総合*/
/*case 0:画面内キャラ移動*/
/*case 1:画面スクロール*/
/*case 2:メニュー*/

/*key状態 	0000 0000 = default	*/
/*			0000 0001 = enter	*/
/*			0000 0010 = left	*/
/*			0000 0100 = up		*/
/*			0000 1000 = right	*/
/*			0001 0000 = doun	*/
/*			0010 0000 = num 1	*/
/*			0100 0000 = num 2	*/
/*			1000 0000 = num 3	*/
/*		の予定だ				*//*駄目でした。*/

BOOL nocursor[8] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};//
BYTE musikey[5] = {255, 255, 255, 255, 255};//指は5本。同時押しは後を優先。
BYTE osimati = 0;//osippa0
BOOL ositg = FALSE;//押した、離した。クリアスクリーン判断用
BOOL osiki = FALSE;//押した、離した記憶。クリアスクリーン判断用
BOOL CLUSTOP = FALSE;//押しっぱなし連打ストップ

void exit_game(void);


struct keys{
	BOOL left;
	BOOL up;
	BOOL right;
	BOOL down;
	BOOL enter;
	BOOL num1;
	BOOL num2;
	BOOL num3;
	BOOL esc;
	
	BOOL _q;//
	BOOL _2;//
	BOOL _w;//
	BOOL _3;//

	BOOL _e;//
	BOOL _4;//
	BOOL _r;//
	BOOL _5;//

	BOOL _t;//
	BOOL _6;//
	BOOL _y;//
	BOOL _7;//

	BOOL _u;//
	BOOL _8;//
	BOOL _i;//
	BOOL _9;//

	BOOL _o;//
	BOOL _0;//＃ラ
	BOOL _p;//ド
//	BOOL mi;//＃ド
//	BOOL at;//レ
//	BOOL ch;//＃レ
//	BOOL ho;//ミ

	BOOL _a;//高い鍵盤
	BOOL _z;//低い鍵盤
	BOOL _k;//高い鍵盤
	BOOL _m;//低い鍵盤
	
	BOOL f1;
	BOOL f2;
	BOOL f3;
	BOOL f4;
	
	BYTE osi;
	BOOL tame;
	
	} keyState = {FALSE, FALSE, FALSE, FALSE,
					FALSE, FALSE, FALSE, FALSE,
					FALSE, FALSE, FALSE, FALSE,
					FALSE, FALSE, FALSE, FALSE,
					FALSE, FALSE, FALSE,
					FALSE, FALSE, FALSE, FALSE,
					FALSE, FALSE, FALSE, FALSE};

int inkey(void)
{

//	switch(scroll){
//		case 0:	paintDo = 0;break;

//		case 1: paintDo = 0;break;

//		default : break;
//	}
	
	if (GetAsyncKeyState(VK_LEFT) < 0 && keyState.left != TRUE) {
		keyState.left = TRUE; keyState.tame = TRUE;//cntMode();
	}
	if (GetAsyncKeyState(VK_UP) < 0 && keyState.up != TRUE) {
		keyState.up = TRUE; keyState.tame = TRUE;//aCount = 0;
	}
	if (GetAsyncKeyState(VK_RIGHT) < 0 && keyState.right != TRUE) {
		keyState.right = TRUE; keyState.tame = TRUE; //cntMode();
	}
	if (GetAsyncKeyState(VK_DOWN) < 0 && keyState.down != TRUE) {
		keyState.down = TRUE; keyState.tame = TRUE;//cntMode();//aCount = 20;
	}
	if (GetAsyncKeyState(VK_RETURN) < 0 && keyState.enter != TRUE){
		keyState.enter = TRUE; keyState.tame = TRUE;// scroll = scroll ^ 0x0010; paintDo = 1; maski = TRUE; 
	}

	if (GetAsyncKeyState(VK_NUMPAD1) < 0 && keyState.num1 != TRUE){
		keyState.num1 = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState(VK_NUMPAD2) < 0 && keyState.num2 != TRUE){
		keyState.num2 = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState(VK_NUMPAD3) < 0 && keyState.num3 != TRUE){
		keyState.num3 = TRUE; keyState.tame = TRUE;
	}
	
	if (GetAsyncKeyState('Q') < 0 && keyState._q != TRUE){
		keyState._q = TRUE; keyState.tame = TRUE;
	}	
	if (GetAsyncKeyState('2') < 0 && keyState._2 != TRUE){
		keyState._2 = TRUE; keyState.tame = TRUE;
	}	
	if (GetAsyncKeyState('W') < 0 && keyState._w != TRUE){
		keyState._w = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('3') < 0 && keyState._3 != TRUE){
		keyState._3 = TRUE; keyState.tame = TRUE;
	}	
	if (GetAsyncKeyState('E') < 0 && keyState._e != TRUE){
		keyState._e = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('4') < 0 && keyState._4 != TRUE){
		keyState._4 = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('R') < 0 && keyState._r != TRUE){
		keyState._r = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('5') < 0 && keyState._5 != TRUE){
		keyState._5 = TRUE; keyState.tame = TRUE;
	}	
	if (GetAsyncKeyState('T') < 0 && keyState._t != TRUE){
		keyState._t = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('6') < 0 && keyState._6 != TRUE){
		keyState._6 = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('Y') < 0 && keyState._y != TRUE){
		keyState._y = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('7') < 0 && keyState._7 != TRUE){
		keyState._7 = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('U') < 0 && keyState._u != TRUE){
		keyState._u = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('8') < 0 && keyState._8 != TRUE){
		keyState._8 = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('I') < 0 && keyState._i != TRUE){
		keyState._i = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('9') < 0 && keyState._9 != TRUE){
		keyState._9 = TRUE; keyState.tame = TRUE;
	}	
	if (GetAsyncKeyState('O') < 0 && keyState._o != TRUE){
		keyState._o = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('0') < 0 && keyState._0 != TRUE){
		keyState._0 = TRUE; keyState.tame = TRUE;
	}	
	if (GetAsyncKeyState('P') < 0 && keyState._p != TRUE){
		keyState._p = TRUE; keyState.tame = TRUE;
	}
//	if (GetAsyncKeyState(VK_OEM_MINUS) < 0 && keyState.mi != TRUE){
//		keyState.mi = TRUE; keyState.tame = TRUE;
//	}
//	if (GetAsyncKeyState(VK_OEM_3) < 0 && keyState.at != TRUE){
//		keyState.at = TRUE; keyState.tame = TRUE;
//	}
//	if (GetAsyncKeyState(VK_OEM_7) < 0 && keyState.ch != TRUE){
//		keyState.ch = TRUE; keyState.tame = TRUE;
//	}
//	if (GetAsyncKeyState(VK_OEM_4) < 0 && keyState.ho != TRUE){
//		keyState.ho = TRUE; keyState.tame = TRUE;
//	}
	if (GetAsyncKeyState('A') < 0 && keyState._a != TRUE){
		keyState._a = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('Z') < 0 && keyState._z != TRUE){
		keyState._z = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('K') < 0 && keyState._k != TRUE){
		keyState._k = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState('M') < 0 && keyState._m != TRUE){
		keyState._m = TRUE; keyState.tame = TRUE;
	}
	
	if (GetAsyncKeyState(VK_F1) < 0 && keyState.f1 != TRUE){
		keyState.f1 = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState(VK_F2) < 0 && keyState.f2 != TRUE){
		keyState.f2 = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState(VK_F3) < 0 && keyState.f3 != TRUE){
		keyState.f3 = TRUE; keyState.tame = TRUE;
	}
	if (GetAsyncKeyState(VK_F4) < 0 && keyState.f4 != TRUE){
		keyState.f4 = TRUE; keyState.tame = TRUE;
	}
	
	
	if(keyState.tame == TRUE){keyState.osi = 0x20;/* act_wait();*/}//一押しで一度のみ0x20
	
//	if(/*keyState.tame == TRUE*/1){cntMode();}//押しっぱなしならアクション。だがやめとく
	if (GetAsyncKeyState(VK_ESCAPE) < 0 ){
		keyState.esc = TRUE; keyState.tame = TRUE;
//		exit_game();// PostQuitMessage(0);
	}
	
	
	if (GetAsyncKeyState(VK_LEFT) == 0) {
		keyState.left = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState(VK_UP) == 0) {
		keyState.up = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState(VK_RIGHT) == 0) {
		keyState.right = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState(VK_DOWN) == 0) {
		keyState.down = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState(VK_RETURN) == 0){
		keyState.enter = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState(VK_NUMPAD1) == 0){
		keyState.num1 = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState(VK_NUMPAD2) == 0){
		keyState.num2 = FALSE; keyState.tame = FALSE;//if(scroll != 0x10 || scroll != 0x11){act_jump();}
	}
	if (GetAsyncKeyState(VK_NUMPAD3) == 0){
		keyState.num3 = FALSE; keyState.tame = FALSE;
	}
	
	if (GetAsyncKeyState('Q') == 0){
		keyState._q = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('2') == 0){
		keyState._2 = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('W') == 0){
		keyState._w = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('3') == 0){
		keyState._3 = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('E') == 0){
		keyState._e = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('4') == 0){
		keyState._4 = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('R') == 0){
		keyState._r = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('5') == 0){
		keyState._5 = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('T') == 0){
		keyState._t = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('6') == 0){
		keyState._6 = FALSE; keyState.tame = FALSE;
	}	
	if (GetAsyncKeyState('Y') == 0){
		keyState._y = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('7') == 0){
		keyState._7 = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('U') == 0){
		keyState._u = FALSE; keyState.tame = FALSE;
	}
	
	if (GetAsyncKeyState('8') == 0){
		keyState._8 = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('I') == 0){
		keyState._i = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('9') == 0){
		keyState._9 = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('O') == 0){
		keyState._o = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('0') == 0){
		keyState._0 = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('P') == 0){
		keyState._p = FALSE; keyState.tame = FALSE;
	}
//	if (GetAsyncKeyState(VK_OEM_MINUS) == 0){
//		keyState.mi = FALSE; keyState.tame = FALSE;
//	}
//	if (GetAsyncKeyState(VK_OEM_3) == 0){
//		keyState.at = FALSE; keyState.tame = FALSE;
//	}
//	if (GetAsyncKeyState(VK_OEM_7) == 0){
//		keyState.ch = FALSE; keyState.tame = FALSE;
//	}
//	if (GetAsyncKeyState(VK_OEM_4) == 0){
//		keyState.ho = FALSE; keyState.tame = FALSE;
//	}
	
	if (GetAsyncKeyState('A') == 0){
		keyState._a = FALSE; keyState.tame = FALSE;
	}	
	if (GetAsyncKeyState('Z') == 0){
		keyState._z = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('K') == 0){
		keyState._k = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState('M') == 0){
		keyState._m = FALSE; keyState.tame = FALSE;
	}
	
	if (GetAsyncKeyState(VK_F1) == 0){
		keyState.f1 = FALSE; keyState.tame = FALSE;
	}	
	if (GetAsyncKeyState(VK_F2) == 0){
		keyState.f2 = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState(VK_F3) == 0){
		keyState.f3 = FALSE; keyState.tame = FALSE;
	}
	if (GetAsyncKeyState(VK_F4) == 0){
		keyState.f4 = FALSE; keyState.tame = FALSE;
	}

	if (GetAsyncKeyState(VK_ESCAPE) == 0 ){
		keyState.esc = FALSE; keyState.tame = FALSE;
//		exit_game();// PostQuitMessage(0);
	}

	return 0;
}