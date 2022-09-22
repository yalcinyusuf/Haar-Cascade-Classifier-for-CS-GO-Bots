#pragma once
#include <Windows.h>
#include <opencv2\opencv.hpp>

void sendClick(HWND window, short isReleased, POINT pt)
{
	if (window)
	{
		Sleep(100);
		SendMessage(window, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
		std::cout << "Clicked down." << std::endl;

		if(isReleased)
		{
			Sleep(20);
			
			SendMessage(window, WM_LBUTTONUP, 0, MAKELPARAM(pt.x, pt.y));
			std::cout << "Clicked up." << std::endl;
		}
	}
	else
		std::cout << "Window is not found!" << std::endl;
}


void sendMouseEvent(HWND window, int x, int y)
{
	POINT pt; //cursor location

	pt.x = x;
	pt.y = y;
	std::cout << "Cursor Window Pos:" << pt.x << "," << pt.y << std::endl;
	//GetCursorPos(&pt); //O anki mouse pozisyonunu dondurur
	ClientToScreen(window, &pt); // Game penceresinin koordinatini, Ana desktop penceresinin koordinatina donusturme

	SetCursorPos(pt.x, pt.y);
	std::cout << "Cursor Main Pos:" << pt.x << "," << pt.y << std::endl;
	
	//ScreenToClient(window, &pt); // Mouse genel koordinatini pencerenin koordinatina donusturme

	//RECT windowsize;
	//GetClientRect(window, &windowsize); // get the height and width of the screen
	//std::cout << "sag: " << windowsize.right << "alt: " << windowsize.bottom << std::endl;
	sendClick(window, 1, pt); //Click

}

cv::Mat hwnd2mat(HWND hwnd)
{
	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	cv::Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);

	srcheight = 720;// windowsize.bottom;
	srcwidth = 1280;//windowsize.right;
	height = windowsize.bottom / 1;  //change this to whatever size you want to resize to
	width = windowsize.right / 1;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

																									   // avoid memory leak
	DeleteObject(hbwindow);
	DeleteDC(hwindowCompatibleDC);
	ReleaseDC(hwnd, hwindowDC);

	return src;
}