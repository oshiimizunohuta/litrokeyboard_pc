		/*�E�B���h�E�̃f�o�C�X�R���e�L�X�g���Q�b�g*/
	get_WDC(hdcH, hwndH, hBMPH, hdcBMPH, hOldBMPH)
	HDC hdcH; 
	HWND hwndH;
		hdcH = GetDC(hwndH);

		hBMPH = CreateCompatibleBitmap(hdc, 480, 360);

		hdcBMPH = CreateCompatibleDC(hdc);

		hOldBMPH = SelectObject(hdcBMP, hBMP);
