// test12.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "test12.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE g_hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.
HWND hWndMain;


// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST12, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST12));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.cbClsExtra = 0; //������ ���� �����̴�. Ư���� ��� ���Ǵ� ������ ����
	wcex.cbWndExtra = 0; //������ ���� �����̴�. Ư���� ��� ���Ǵ� ������ ����
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //Ŀ�� ���
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //�������� ��� ���� ����
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST12)); //������ ���
	wcex.hInstance = hInstance; // ������ Ŭ������ ����ϴ� ���α׷��� ��ȣ 
	wcex.lpfnWndProc = WndProc; //�������� �޽��� ó�� �Լ��� ����
	wcex.lpszClassName = szWindowClass; //������ Ŭ������ �̸��� �����Ѵ�
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TEST12); //�� ���α׷��� ����� �޴� ����
	wcex.style = 0; //������ ��Ÿ�� ����

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd; //�޼����� ���� ������ �ڵ�

   g_hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_SYSMENU | WS_MINIMIZEBOX,
	   CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow); //������ â�� ������
   UpdateWindow(hWnd);
   //UpdateWindow �Լ��� ������ �������� �Ϻΰ� �ٸ� ������ � �������ų� �������� �Ǿ��� ���,
   //�� ��ȿȭ ����(Invalid Region)�� ���� WM_PAINT �޽����� �ش� �����쿡 �������ν� ��ȿȭ ������
   //���Ž��� �ִ� �Լ��Դϴ�

   return TRUE;
}

void DrawScreen(HDC hdc);
void MakeNewBrick();
int GetAround(int x, int y, int b, int r);
BOOL MoveDown();
void TestFull();
void PrintTile(HDC hdc, int x, int y, int c);
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
//�Լ� ����

#define BW 10 //���� �� ���ι迭
#define BH 20 //���� �� ���ι迭
#define random(n)(rand()%n) // ���� �������� ���� ������ ���� ��ũ�ι�
#define TS 25 //��Ʈ�� ũ��

struct Point
{
	int x, y;
};
//��ũ�� �� ���� ������

// �� ������ ���� �迭  
Point Shape[][4][4] = //Shape[������ȣ][ȸ����ȣ][�Ϸù�ȣ(x,y)]
{
	{ { 0, 0, 1, 0, 2, 0, -1, 0 }, { 0, 0, 0, 1, 0, -1, 0, -2 },
	{ 0, 0, 1, 0, 2, 0, -1, 0 }, { 0, 0, 0, 1, 0, -1, 0, -2 } }, //������ ��
	{ { 0, 0, 1, 0, 0, 1, 1, 1 }, { 0, 0, 1, 0, 0, 1, 1, 1 }, 
	{ 0, 0, 1, 0, 0, 1, 1, 1 }, { 0, 0, 1, 0, 0, 1, 1, 1 } }, //�׸� ��
	{ { 0, 0, -1, 0, 0, -1, 1, -1 }, { 0, 0, 0, 1, -1, 0, -1, -1 },
	{ 0, 0, -1, 0, 0, -1, 1, -1 }, { 0, 0, 0, 1, -1, 0, -1, -1 } }, //S�� ��
	{ { 0, 0, -1, -1, 0, -1, 1, 0 }, { 0, 0, -1, 0, -1, 1, 0, -1 },
	{ 0, 0, -1, -1, 0, -1, 1, 0 }, { 0, 0, -1, 0, -1, 1, 0, -1 } }, //Z�� ��
	{ { 0, 0, -1, 0, 1, 0, -1, -1 }, { 0, 0, 0, -1, 0, 1, -1, 1 }, 
	{ 0, 0, -1, 0, 1, 0, 1, 1 }, { 0, 0, 0, -1, 0, 1, 1, -1 } }, //���� ��
	{ { 0, 0, 1, 0, -1, 0, 1, -1 }, { 0, 0, 0, 1, 0, -1, -1, -1 },
	{ 0, 0, 1, 0, -1, 0, -1, 1 }, { 0, 0, 0, -1, 0, 1, 1, 1 } }, //�� ���� ��
	{ { 0, 0, -1, 0, 1, 0, 0, 1 }, { 0, 0, 0, -1, 0, 1, 1, 0 },
	{ 0, 0, -1, 0, 1, 0, 0, -1 }, { 0, 0, -1, 0, 0, -1, 0, 1 } }, //���� ��
};
enum { EMPTY, BRICK, WALL = sizeof(Shape) / sizeof(Shape[0]) + 1 };
int board[BW + 2][BH + 2];  //������ �׵θ�
int nx, ny; //���� ��ġ
int brick, rot; //��, ȸ��
int nbrick[2]; //������ ��µ� ��
int score; //����
int bricknum; //�� ����
enum tag_Status{ GAMEOVER, RUNNING, PAUSE }; //tag_Status�� ���� ����(0), ���� ����(1), �Ͻ�����(2) �迭������ ����
tag_Status GameStatus; //tag_Status�� GameStatus�� ���� 
int Interval; //�ð� ���� ����
HBITMAP hBit[10]; //��Ʈ�� ����










//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	RECT crt;
	int trot = 0;
	HDC hdc;
	PAINTSTRUCT ps;
	int x, y;

	switch (message)
	//�����찡 ó�� ��������� ������ �޼���
	// �� �޽��� ó�� ��ƾ���� �ַ� ���α׷� �ʱ�ȭ�� �����ϸ� ������
	// ���� �غ� �Ѵ�. ���⼭�� �޸� DC�� ����� �ΰ��� ��Ʈ����
	// �̸� �о�д�.
	{
	case WM_CREATE: //�����찡 ó�� ��������� ���
		hWndMain = hWnd;
		SetRect(&crt, 0, 0, (BW + 12)*TS, (BH + 2)*TS); // crt�� ũ�⸦ ����.
		AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE); 
		SetWindowPos(hWndMain, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,SWP_NOMOVE | SWP_NOZORDER);
		// ������ ���� ����(�������� ũ�� ����)
		//������ ũ�� ���� ���ϰ� ����(�ִ�ȭ ����)

		GameStatus = GAMEOVER;  //������ ���´� ���� ���� ����
		srand(GetTickCount()); //��ǻ���� �ð��� 1�ʴ������� �̸�������� ������
		for (i = 0; i < 10; i++)
		{
			hBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1 + i));
		}

		return 0;
		
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_GAME_START: //���� ������ ��������
		{
			if (GameStatus != GAMEOVER) //���� ���ᰡ �ƴҶ�
				break; //�������´�
			for (x = 0; x < BW + 2; x++) //���η� +2ĭ
			{
				for (y = 0; y < BH + 2; y++) // ���η� +2ĭ
				{
					board[x][y] = (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) ? WALL : EMPTY;
				}//board[x][y] �迭�� �ȿ� �̰� WALL���� EMPTY���� �Ǻ��ؼ� ������
			}
			score = 0; //������ 0���� �ʱ�ȭ
			bricknum = 0; //�� ������ 0���� �ʱ�ȭ
			GameStatus = RUNNING; //���� ���¸� ���������� ����
			nbrick[0] = random(sizeof(Shape) / sizeof(Shape[0])); //�����ϰ� 7���� ����� 1���� ����
			MakeNewBrick(); //�Լ�ȣ��
			Interval = 1000; //��� �������� �ʸ� ���� �ý����� õ�� 1�ʷ� �����
			SetTimer(hWnd, 1, Interval, NULL); //1�ʿ� 1ĭ�� ���������� �����Ŵ
			break;

		}


		case IDM_GAME_PAUSE: //�Ͻ� ������ ��������
			if (GameStatus == RUNNING) 
			{
				GameStatus = PAUSE;
				KillTimer(hWnd, 1);
				//������ �������Ͻ� �Ͻ������� �ٲٰ� Ÿ�̸Ӹ� ����
			}
			else if (GameStatus == PAUSE)
			{
				GameStatus = RUNNING;
				SetTimer(hWnd, 1, Interval, NULL);
				//������ �Ͻ����� �Ͻ� ���������� �ٲٰ� Ÿ�̸� ����
			}

			break;
		case IDM_GAME_EXIT: //���� ����
			DestroyWindow(hWnd); //â�� ����
			break;
		}
		return 0;		

	case WM_TIMER: 
		if (MoveDown() == TRUE)
		{
			MakeNewBrick();
		}
		return 0;

	case WM_KEYDOWN:
		if (GameStatus != RUNNING || brick == -1)
		{
			return 0;
		}//���� ������°� �ƴϰų� ������ ������ �޸� ��ȯ (Ű�� ���� ����)
		switch (wParam)
		{
		case VK_LEFT: //����Ű�� ��������
			if (GetAround(nx - 1, ny, brick, rot) == EMPTY) //������Ͻÿ� �̵�
			{
				nx--; //���������̵� 
				InvalidateRect(hWnd, NULL, FALSE); 
				//�����ٰ� �ٽ� �׷��� NULL���� ��ü���� FALSE�� �׸� �κи� ����
			}
			break;
		case VK_RIGHT: //������Ű�� ��������
			if (GetAround(nx + 1, ny, brick, rot) == EMPTY) //������� �ÿ� �̵�
			{
				nx++; //���������� �̵�
				InvalidateRect(hWnd, NULL, FALSE);
				//�����ٰ� �ٽ� �׷��� NULL���� ��ü���� FALSE�� �׸� �κи� ����
			}
			break; 
		case VK_UP: //���� Ű�� ��������
			trot = (rot == 3 ? 0 : rot + 1); 
			//�ùķ��̼��� �غ� 3�ϰ�� 0���� 0�ϰ�� rot+1 �ؼ� 1�� 1�� ��� rot+1�� �ؼ� 2��
				if (GetAround(nx, ny, brick, trot) == EMPTY) //���� ȸ ���� ������ �����ÿ� ȸ����
				{
					rot = trot; //ȸ��
					InvalidateRect(hWnd, NULL, TRUE);
					//�����ٰ� �ٽ� �׷��� NULL���� ��ü���� TRUE�� ��� ��ü�� ����
				}
				break; 
		case VK_DOWN: //�Ʒ��� Ű�� ��������
			if (MoveDown() == TRUE)
			//1ĭ �Ʒ��� �̵�
			{
				MakeNewBrick();
				//
			}
					break;

		case VK_SPACE: //�����̽��ٸ� ��������
			while (MoveDown() == FALSE)
			//�� �Ʒ��� �̵�
			{
				;
			}
			MakeNewBrick();
			break;
		}

		return 0;



	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		DrawScreen(hdc);
		//DrawBitmap(hdc,10,10,)
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: //�����츦 �޸𸮿��� �ı�
		KillTimer(hWndMain, 1);
		//Ÿ�̸� ����
		for (i = 0; i < 11; i++)
		{
			DeleteObject(hBit[i]);
			//hbit[i]�� ����
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void DrawScreen(HDC hdc)
//ȭ�� ��ü�� �׸���. �����ǰ� ���� �̸�, ������ �Ѳ����� �׸���.
{
	int x, y, i;
	TCHAR str[128]; //�迭 ����

	for (x = 0; x < BW + 1; x++)
	{
		PrintTile(hdc, x, 0, WALL);
		PrintTile(hdc, x, BH+1, WALL);
	}//��, �Ʒ��� �׵θ� �׸�
	
	for (y = 0; y < BH + 2; y++)
	{
		PrintTile(hdc, 0, y, WALL);
		PrintTile(hdc, BW+1, y, WALL);
	}//���� �׵θ� �׸�


	for (x = 1; x < BW + 1; x++)
	{
		for (y = 1; y < BH + 1; y++)
		{
			PrintTile(hdc, x, y, board[x][y]); 
		}
	}//������ �׸�

	if (GameStatus != GAMEOVER && brick != -1) // ���� ������ ������ �ʾҰ� ��� ���ڰ� ���� �ʾҴٸ�
	{
		for (i = 0; i < 4; i++)
		{
			PrintTile(hdc, nx + Shape[brick][rot][i].x, ny + Shape[brick][rot][i].y, brick + 1);
		}
	}//�̵����� ���� �׷��ֱ�



	for (x = BW + 3; x <= BW + 11; x++)
	{
		for (y = BH -13; y <= BH + 8; y++)
		{
			if (x == BW + 3 || x == BW + 11 || y == BH - 13 || y == BH + 1)
			{
				PrintTile(hdc, x, y, WALL);
			}
			else
			{
				PrintTile(hdc, x, y, 0);
			}
		}

	} //���� �ؽ�Ʈ ��� ������ �׸� �׷��ֱ�

	if (GameStatus != GAMEOVER) //������ ������ �ʾ����ÿ�
	{
		for (i = 0; i < 4; i++)
		{
			PrintTile(hdc, BW + 7 + Shape[nbrick[0]][0][i].x, BH - 2 + Shape[nbrick[0]][0][i].y, nbrick[0] + 1);
		}
		for (i = 0; i < 4; i++)
		{
			PrintTile(hdc, BW + 7 + Shape[nbrick[1]][0][i].x, BH - 6 + Shape[nbrick[1]][0][i].y, nbrick[1] + 1);
		}
		for (i = 0; i < 4; i++)
		{
			PrintTile(hdc, BW + 7 + Shape[nbrick[2]][0][i].x, BH - 10 + Shape[nbrick[2]][0][i].y, nbrick[2] + 1);
		}
	}//���� ���� �׸�

	lstrcpy(str, TEXT("Tetris")); //Tetris ���
	TextOut(hdc, (BW + 4)*TS, 30, str, lstrlen(str));
	wsprintf(str, TEXT("���� : %d   "), score); //���� ���
	TextOut(hdc, (BW + 4)*TS, 60, str, lstrlen(str));
	wsprintf(str, TEXT("���� : %d   ��"), bricknum); //���� ���� ���
	TextOut(hdc, (BW + 4)*TS, 80, str, lstrlen(str));
	//���� ���
}

void MakeNewBrick() //���� ����� �������� �Լ�
{
	bricknum++; //��� ������ ���� ����
	brick = nbrick[0]; //�̸����� ����� ���� ������� ������
	nbrick[0] = nbrick[1];
	nbrick[1] = nbrick[2];


	nbrick[2] = random(sizeof(Shape) / sizeof(Shape[0]));//sizeof(Shape) / sizeof(Shape[0])�� �ؼ� ���� 1���� �̸����� ������� �������� ����

	nx = BW / 2; //���� �����ǥ�� �߾�����
	ny = 3;  //���� ��� ��ǥ�� 3ĭ �Ʒ���
	rot = 0; //ó�� ȸ�� ���� 0���� �ʱ�ȭ

	InvalidateRect(hWndMain, NULL, TRUE);
	if (GetAround(nx, ny, brick, rot) != EMPTY) //���� GetAround(�����̵� ȸ�� ���ɼ� ����)�Լ� ������ ������� �ƴ϶��
	{
		KillTimer(hWndMain, 1);
		//Ÿ�̸Ӹ� ����
		GameStatus = GAMEOVER;
		//���� ���� ��ġ�� �̹� ������ ������ ���� ����
		MessageBox(hWndMain, 
			TEXT("������ �������ϴ�. �ٽ� �����Ϸ��� ���ӽ���")
			TEXT("�׸�(S)�� ������ �ֽʽÿ�."), 
			TEXT("�˸�"), MB_OK);
	}
}


int GetAround(int x, int y, int b, int r)
//�̵����� ������ �ֺ��� �����ϴ� ���� �ƴϹǷ� �μ��� ���޵� ��ġ�� ���� ����� �����Ѵ�.(����� ��ǥ)
{
	int i, k = EMPTY; // i, k�� ��������� ����

	for (i = 0; i < 4; i++) 
	{
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	} //max(a,b) (((a) > (b)) ? (a) : (b)) ū���� ����
	  //���� �����ڴ� ���� ������ �Ǻ��Ѵ�.
	//

	return k;
}

BOOL MoveDown()
//���� �ٴڿ� ��Ҵ��� �Ǻ��ϴ� �Լ� ������� TestFull �Լ�(�������� �� ä���� ���� ã�� ����)�� ȣ���� �� TRUE�� �����Ѵ�.
{
	if (GetAround(nx, ny + 1, brick, rot) != EMPTY) //������ ��ġ�� ������� �ƴ϶��
	{
		TestFull(); //�������� �� ä���� ���� ã�� ����
		return TRUE; //TURE�� ���� �ִ� FALSE�� ���� ����.
	}

	ny++; //y���� ��ĭ ������
	InvalidateRect(hWndMain, NULL, FALSE);
	UpdateWindow(hWndMain);
	//��� �׷��� �������� ����� �����ش�

	return FALSE;
}


void TestFull()
//�������� �� ä���� ���� ã�� �����Ѵ�
{
	int i, x, y, ty;
	int count = 0;
	static int arScoreInc[] = {0,1,3,8,20}; //�޺�����

	for (i = 0; i < 4; i++)
	{
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = brick + 1;
	}
	//�迭�� ���� ������ ������ ���

	brick = -1; //���� ������� Ű�� �ȸ����� ���� �ʱ�ȭ

	for (y = 1; y < BH + 1; y++)
	{
		for (x = 1; x < BW + 1; x++)//������ �˻�
		{
			if (board[x][y] == EMPTY) //���� �����ǿ� ������� �������
				break; //��������
		}

		if (x == BW + 1) //���� x�� BW+1�� ���
		{
			count++; 
			for (ty = y; ty>1; ty--)
			{
				for (x = 1; x < BW + 1; x++)
				{
					board[x][ty] = board[x][ty - 1]; //�ش� ������ �����Ѵ�.
				}
			}

			InvalidateRect(hWndMain, NULL, FALSE);
			UpdateWindow(hWndMain);
			//UpdateWindow �Լ��� ������ �������� �Ϻΰ� �ٸ� ������ � �������ų� �������� �Ǿ��� ���, 
			//�� ��ȿȭ ����(Invalid Region)�� ���� WM_PAINT �޽����� �ش� �����쿡 �������ν� ��ȿȭ ������
			//���Ž��� �ִ� �Լ��Դϴ�.
			//�迭�� ���� ������ ������ ���
			Sleep(150);
			//�� ���� �� 1000/150�� ���� �����ð�
		}
	}
	score += arScoreInc[count]; // ���ھ� ������ �����ش�
	
	if (bricknum % 10 == 0 && Interval > 200)//��� ����10���� ������ �������� 0�϶� 
											//�׸��� �������� �ð��� 200 �̻��Ͻÿ�
	{
		Interval -= 50; //�������� �ð��� ���� �ٿ���
		SetTimer(hWndMain, 1, Interval, NULL); //���� �ð� ����
	}
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;
	
	

	MemDC = CreateCompatibleDC(hdc);
	//�޸� DC�� ���鶧 ����ϴ� �Լ�
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	//�޸𸮿� ����� ��Ʈ�� ������ ������
	bx = bit.bmWidth - 23;//��Ʈ�� ����
	by = bit.bmHeight - 23; //��Ʈ�� ����

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);
	//BitBlt �Լ��� DC���� �������� ��� ���縦 �����Ѵ�. 
	//�޸� DC�� ǥ�鿡 �׷��� �ִ� ��Ʈ���� ȭ�� DC�� ���������ν� ��Ʈ���� ȭ������ ����Ѵ�.

	SelectObject(MemDC, OldBitmap);
	//SelectObject�� ������ ������ �ִ� ������ ��ȯ
	DeleteDC(MemDC);
	//����� �������ÿ� �޸�DC����
}

void PrintTile(HDC hdc, int x, int y, int c)
{
	DrawBitmap(hdc, x*TS, y * TS, hBit[c]); //DrawBitMap �Լ� ȣ�� �ؼ� Ÿ�� �������� �ٲ���
	return;
}




// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
