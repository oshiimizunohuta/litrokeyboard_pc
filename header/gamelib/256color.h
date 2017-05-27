/********8ビットDIB********/
	LPBYTE lpDIB256c, lpPixel256c; /*8ﾋﾞｯﾄｱﾄﾞﾚｽ　まあBYTE形ポインタでしょ。*/
	LPBITMAPINFO lpbiInfo256c;
	LPRGBQUAD lpRGB256c;
	

void get_256c(void){
	
	lpDIB256c = (LPBYTE)HeapAlloc(GetProcessHeap(),
			HEAP_ZERO_MEMORY, sizeof(BITMAPINFO) + 255 * 
			sizeof(RGBQUAD) + 256 * 256); //縦かけ横
			
	lpbiInfo256c = (LPBITMAPINFO)lpDIB256c;
	lpPixel256c = lpDIB256c + sizeof(BITMAPINFO) + 255 * sizeof(RGBQUAD);
	lpRGB256c = (LPRGBQUAD)(lpbiInfo256c->bmiColors);
			
	lpbiInfo256c->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbiInfo256c->bmiHeader.biWidth = 128;
	lpbiInfo256c->bmiHeader.biHeight = -256;
	lpbiInfo256c->bmiHeader.biPlanes = 1;
	lpbiInfo256c->bmiHeader.biBitCount = 8;
	lpbiInfo256c->bmiHeader.biCompression = BI_RGB;

	
}

void p256change(void){
	int i;
	lpRGB256c[0].rgbRed = 0x12;
	lpRGB256c[0].rgbGreen = 0x00;
	lpRGB256c[1].rgbBlue = 0xaa;

	for(i = 0; i <= 2568; i++){
		//lpPixel256c =(LPBYTE)chrPix[i];
		FillMemory(lpPixel256c + i, 1, 0x000001);
		
//		CopyMemory(lpPixel256c[i],
	//		     (LPBYTE)chrPix[0] + i - 1, 3);
	}
}