#include <opencv2/opencv.hpp>
#include <iostream>
#include "Desktop.h"

using namespace std;
using namespace cv;

int main()
{
	HWND hwndWindow = FindWindow(0, "Counter-Strike: Global Offensive - Direct3D 9");
	Mat imgSrc, imgDst, imgGray;
	
	CascadeClassifier csCascade;
	vector<Rect> enemies;
	csCascade.load("cascade12.xml");

	if (hwndWindow == NULL)
		return -1;
	while (true)
	{
		imgSrc = hwnd2mat(hwndWindow);
		if (imgSrc.empty())
			break;
		cvtColor(imgSrc, imgGray, COLOR_BGR2GRAY);
		csCascade.detectMultiScale(imgGray, enemies);

		if (enemies.size() <= 0)
			0; // cout << "enemy can not found" << endl;
		else
		{
			cout << enemies.size() << " enemy is found" << endl;
			for (auto enemy : enemies)
			{
				rectangle(imgSrc, enemy, Scalar(0, 0, 255), 2);
			}
		}

		//resize(imgSrc, imgDst, Size(720, 1280)); // Oyun penceresi normalde,,, 1024'e 576 donuyor //Size(512, 380)
		imshow("Game", imgSrc);

		for (auto enemy : enemies)
		{
			mouse_event(MOUSEEVENTF_MOVE, enemy.x - 512 + enemy.width * 0.5, enemy.y - 288 + enemy.height * 0.08, 0, NULL);
			sendMouseEvent(hwndWindow, enemy.x + (enemy.width * 0.5), enemy.y + (enemy.height * 0.08));
		}
		if (waitKey(2) == 27)
			break;

		//imshow("Game", imgSrc);
	}

	return 0;
}