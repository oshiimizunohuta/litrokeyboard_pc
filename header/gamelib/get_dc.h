		/*ウィンドウのデバイスコンテキストをゲット*/
	get_WDC(hdcH, hwndH, hBMPH, hdcBMPH, hOldBMPH)
	HDC hdcH; 
	HWND hwndH;
		hdcH = GetDC(hwndH);

		hBMPH = CreateCompatibleBitmap(hdc, 480, 360);

		hdcBMPH = CreateCompatibleDC(hdc);

		hOldBMPH = SelectObject(hdcBMP, hBMP);
