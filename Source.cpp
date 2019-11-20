#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <ctime>
#include <thread>
#include <conio.h>
using namespace std;
int HEIGH_CONSOLE = 20, WIDTH_CONSOLE = 60;// ?? r?ng và ?? cao c?a màn hình console
int SPEED;// toc do
bool STATE; // trang thai
int MOVING;//bien xac dinh huong di chuyen 
POINT *X1,*X2; //mang chua 2 thanh truot nguoi choi
int dX = 15;
POINT B;//qua bong
int i;//huong bong 1,2,3,4
void Nocursortype()//ham lam an con tro cho nhap trong consoleWindow
{
	CONSOLE_CURSOR_INFO Info;
	Info.bVisible = FALSE;
	Info.dwSize = 20;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
}
void FixConsoleWindow()
{
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);
}
void GotoXY(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void ResetData()
{
	SPEED = 200;
	B = { 30,2 }; // vi tri qua bong
	i = 4;
	// tao 2 thanh truot
	if (X1 == NULL)//thanh tren
	{
		X1 = new POINT[dX];
		int a = 25;
		for (int i = 0; i < dX; i++) {
			X1[i].x = 25 + i;
			X1[i].y = 1;
		}
	}
	if (X2 == NULL)//
	{
		X2 = new POINT[dX];
		int a = 25;
		for (int i = 0; i < dX; i++) {
			X2[i].x = 25 + i;
			X2[i].y = HEIGH_CONSOLE - 1;
		}
	}
}

void DrawBoard(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0)
{
	GotoXY(x, y); cout << '=';
	for (int i = 1; i < width; i++) cout << '=';
	cout << '=';
	GotoXY(x, height + y); cout << '=';
	for (int i = 1; i < width; i++)cout << '=';
	cout << '=';
	for (int i = y + 1; i < height + y; i++)
	{
		GotoXY(x, i); cout << '|';
		GotoXY(x + width, i); cout << '|';
	}
	GotoXY(curPosX, curPosY);
}
void StartGame()
{
	system("cls");
	ResetData(); // khoi tao du lieu goc
	DrawBoard(0, 0, WIDTH_CONSOLE, HEIGH_CONSOLE); // tao màn hình game
	GotoXY((WIDTH_CONSOLE + 1) / 2, HEIGH_CONSOLE + 1);
	GotoXY(1, HEIGH_CONSOLE + 1);
	STATE = true;//cho thread chay
}
//Hàm don dep tai nguyen
void GabageCollect()
{
	delete[] X1;
	delete[] X2;
}
//Hàm thoát game
void ExitGame(HANDLE t) {
	system("cls");
	TerminateThread(t, 0);
	GabageCollect();
}
//Hàm dung game
void PauseGame(HANDLE t)
{
	SuspendThread(t);
}
bool kttren(int a, int b)//truyen B.x va B.y-1
{
	for (int i = 0; i < dX; i++)
	{
		if ((a == X1[i].x) && (b == X1[i].y)) return true;
	}
	return false;
}
bool ktduoi(int a, int b)//truyen B.x va B.y+1
{
	for (int i = 0; i < dX; i++)
	{
		if ((a == X2[i].x) && (b == X2[i].y)) return true;
	}
	return false;
}
//Hàm ve qua bong
void DrawBall()
{
	GotoXY(B.x, B.y);
	cout << "0";
}
void bongcheo(){
	//bong di cheo dau chia "/".
	GotoXY(B.x, B.y);
	cout << " ";
	if (i == 1 ) {//di duoi len
		B.x++;
		B.y--;
	}
	if (i == 2)//di tren xuong
	{
		B.x--;
		B.y++;
	}
	//bong di cheo dau "\". 
	if (i == 3) {//di duoi len
		B.x--;
		B.y--;
	}
	if (i == 4)//di tren xuong
	{
		B.x++;
		B.y++;
	}
	DrawBall();
}
void DrawBarX1()//ve thanh truot
{
	GotoXY(X1[0].x, X1[0].y);
	cout << "o";
	for (int i = 1; i < dX-1; i++)
	{
		GotoXY(X1[i].x, X1[i].y);
		cout << ":";
	}
	GotoXY(X1[dX-1].x, X1[dX-1].y);
	cout << "o";
}
void DrawBarX2()//ve thanh truot
{
	GotoXY(X2[0].x, X2[0].y);
	cout << "o";
	for (int i = 1; i < dX - 1; i++)
	{
		GotoXY(X2[i].x, X2[i].y);
		cout << ":";
	}
	GotoXY(X2[dX - 1].x, X2[dX - 1].y);
	cout << "o";
}
void MoveRightX1() {
	if (X1[dX - 1].x < WIDTH_CONSOLE - 3)
	{
		GotoXY(X1[0].x, X1[0].y);
		cout << " ";
		GotoXY(X1[1].x, X1[1].y);
		cout << " ";
		GotoXY(X1[2].x, X1[2].y);
		cout << " ";
		for (int i = 0; i < dX; i++)
		{
			X1[i].x += 3;
		}
		DrawBarX1();
	}
	if (X1[dX - 1].x == WIDTH_CONSOLE - 3)
	{
		GotoXY(X1[0].x, X1[0].y);
		cout << " ";
		GotoXY(X1[1].x, X1[1].y);
		cout << " ";
		for (int i = 0; i < dX; i++)
		{
			X1[i].x += 2;
		}
		DrawBarX1();
	}
	if (X1[dX - 1].x == WIDTH_CONSOLE - 2)
	{
		GotoXY(X1[0].x, X1[0].y);
		cout << " ";
		for (int i = 0; i < dX; i++)
		{
			X1[i].x ++;
		}
		DrawBarX1();
	}
}
void MoveRightX2() {
	if (X2[dX - 1].x < WIDTH_CONSOLE - 3)
	{
		GotoXY(X2[0].x, X2[0].y);
		cout << " ";
		GotoXY(X2[1].x, X2[1].y);
		cout << " ";
		GotoXY(X2[2].x, X2[2].y);
		cout << " ";
		for (int i = 0; i < dX; i++)
		{
			X2[i].x += 3;
		}
		DrawBarX2();
	}
	if (X2[dX - 1].x == WIDTH_CONSOLE - 3)
	{
		GotoXY(X2[0].x, X2[0].y);
		cout << " ";
		GotoXY(X2[1].x, X2[1].y);
		cout << " ";
		for (int i = 0; i < dX; i++)
		{
			X2[i].x+=2;
		}
		DrawBarX2();
	}
	if (X2[dX - 1].x == WIDTH_CONSOLE - 2)
	{
		GotoXY(X2[0].x, X2[0].y);
		cout << " ";
		for (int i = 0; i < dX; i++)
		{
			X2[i].x++;
		}
		DrawBarX2();
	}
}
void MoveLeftX1() {
	if (X1[0].x > 3)
	{
		GotoXY(X1[dX - 1].x, X1[dX - 1].y);
		cout << " ";
		GotoXY(X1[dX - 2].x, X1[dX - 2].y);
		cout << " ";
		GotoXY(X1[dX - 3].x, X1[dX - 3].y);
		cout << " ";
		for (int i = 0; i < dX; i++)
		{
			X1[i].x -= 3;
		}
		DrawBarX1();
	}
	if (X1[0].x == 3)
	{
		GotoXY(X1[dX - 1].x, X1[dX - 1].y);
		cout << " ";
		GotoXY(X1[dX - 2].x, X1[dX - 2].y);
		cout << " ";
		for (int i = 0; i < dX; i++)
		{
			X1[i].x-=2;
		}
		DrawBarX1();
	}
	if (X1[0].x == 2)
	{
		GotoXY(X1[dX-1].x, X1[dX-1].y);
		cout << " ";
		for (int i = 0; i < dX; i++)
		{
			X1[i].x--;
		}
		DrawBarX1();
	}
}
void MoveLeftX2() {
	if (X2[0].x > 3)
	{
		GotoXY(X2[dX - 1].x, X2[dX - 1].y);
		cout << " ";
		GotoXY(X2[dX - 2].x, X2[dX - 2].y);
		cout << " ";
		GotoXY(X2[dX - 3].x, X2[dX - 3].y);
		cout << " ";
		for (int i = 0; i < dX; i++)
		{
			X2[i].x -= 3;
		}
		DrawBarX2();
	}
	if (X2[0].x == 3)
	{

		GotoXY(X2[dX - 1].x, X2[dX - 1].y);
		cout << " ";
		GotoXY(X2[dX - 2].x, X2[dX - 2].y);
		cout << " ";
		for (int i = 0; i < dX; i++)
		{
			X2[i].x-=2;
		}
		DrawBarX2();
	}
	if (X2[0].x == 2)
	{
		GotoXY(X2[dX - 1].x, X2[dX - 1].y);
		cout << " ";
		for (int i = 0; i < dX; i++)
		{
			X2[i].x--;
		}
		DrawBarX2();
	}
}
bool ketthuc(int n)
{
	if ((n == 0) || (n == HEIGH_CONSOLE))
	{
		return true;
	}
	return false;
}
void ProcessDead()
{
	STATE = false;
	GotoXY(0, HEIGH_CONSOLE + 2);
	if (B.y == 0) {
		printf("---- PLAYER 2 WIN ----\nNhap Y de bat dau lai va phim khac de ket thuc");
	}
	if (B.y == HEIGH_CONSOLE) {
		printf("---- PLAYER 1 WIN ----\nNhap Y de bat dau lai va phim khac de ket thuc");
	}
}
void MoveBall()
{
	//dung tuong phai
	if ((B.x == WIDTH_CONSOLE - 1) && (i == 4)) { i = 2; }
	if ((B.x == WIDTH_CONSOLE - 1) && (i == 1)) { i = 3; }
	//dung tuong trai
	if ((B.x == 2) && (i == 2)) { i = 4; }
	if ((B.x == 2) && (i == 3)) { i = 1; }
	//dung thanh tren
	if (kttren(B.x, B.y - 1) && (i == 1)) { i = 4; SPEED -= 20; }
	if (kttren(B.x, B.y - 1) && (i == 3)) { i = 2; SPEED -= 20; }
	//dung thanh duoi
	if (ktduoi(B.x, B.y + 1) && (i == 4)) { i = 1; SPEED -= 20; }
	if (ktduoi(B.x, B.y + 1) && (i == 2)) { i = 3; SPEED -= 20; }
	bongcheo();
	if(SPEED>0) Sleep(SPEED);
}
void SubThread()
{
	while (1)
	{
		if (STATE) 
		{
			switch (MOVING) //Kiem tra bien moving
			{
			case 'A':
				MoveLeftX1();
				break;
			case 'D':
				MoveRightX1();
				break;
			case 75:
				MoveLeftX2();
				break;
			case 77:
				MoveRightX2();
				break;
			}
			DrawBarX1();
			DrawBarX2();
			MOVING = ' ';// Tam khóa không cho di chuyen, cho nhan phím tu hàm main
			MoveBall();
			DrawBall();
			if (ketthuc(B.y))
			{
				ProcessDead();
			}
			Sleep(100);//Hàm ngu 
		}
	}
}
void main() {
	int temp;
	FixConsoleWindow();
	Nocursortype();
	srand(time(NULL));
	StartGame();
	thread t1(SubThread);
	while (1)
	{
		temp = toupper(_getch());
		if (STATE == 1)
		{
			if (temp == 27) {
				ExitGame(t1.native_handle());
				return;
			}
			else if (temp == 'P') {
				PauseGame(t1.native_handle());
			}
			else {
				ResumeThread((HANDLE)t1.native_handle());
				if (temp == 'D' || temp == 'A' || temp == 75 || temp == 77)
				{
					MOVING = temp;
				}
			}
		}
		else
		{
			if (temp == 'Y') StartGame();
			else {
				ExitGame(t1.native_handle());
			}
		}
	}
}