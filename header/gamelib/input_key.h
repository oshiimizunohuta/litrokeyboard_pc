/*�L�[���쑍��*/
/*case 0:��ʓ��L�����ړ�*/
/*case 1:��ʃX�N���[��*/
/*case 2:���j���[*/

/*key��� 	0000 0000 = default	*/
/*			0000 0001 = enter	*/
/*			0000 0010 = left	*/
/*			0000 0100 = up		*/
/*			0000 1000 = right	*/
/*			0001 0000 = doun	*/
/*			0010 0000 = num 1	*/
/*			0100 0000 = num 2	*/
/*			1000 0000 = num 3	*/
/*		�̗\�肾				*//*�ʖڂł����B*/

BOOL nocursor[8] = {TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE};//
BYTE osimati = 0;//osippa0
BOOL ositg = FALSE;//�������A�������B�N���A�X�N���[�����f�p
BOOL osiki = FALSE;//�������A�������L���B�N���A�X�N���[�����f�p
BOOL CLUSTOP = FALSE;//�������ςȂ��A�ŃX�g�b�v

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
	
	BYTE osi;
	BOOL tame;
	
	} keyState = {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

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
	if(keyState.tame == TRUE){keyState.osi = 0x20;/* act_wait();*/}//�ꉟ���ň�x�̂�0x20
	
//	if(/*keyState.tame == TRUE*/1){cntMode();}//�������ςȂ��Ȃ�A�N�V�����B������߂Ƃ�
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
	

	return 0;
}