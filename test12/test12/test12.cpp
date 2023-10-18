// test12.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "test12.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE g_hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.
HWND hWndMain;


// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TEST12, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEST12));

	// 기본 메시지 루프입니다.
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
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.cbClsExtra = 0; //일종의 예약 영역이다. 특수한 경우 사용되는 여분의 공간
	wcex.cbWndExtra = 0; //일종의 예약 영역이다. 특수한 경우 사용되는 여분의 공간
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //커서 모양
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //윈도우의 배경 색깔 지정
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST12)); //아이콘 모양
	wcex.hInstance = hInstance; // 윈도우 클래스를 사용하는 프로그램의 번호 
	wcex.lpfnWndProc = WndProc; //윈도우의 메시지 처리 함수를 지정
	wcex.lpszClassName = szWindowClass; //윈도우 클래스의 이름을 정의한다
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_TEST12); //이 프로그램이 사용할 메뉴 지정
	wcex.style = 0; //윈도우 스타일 정의

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd; //메세지를 받을 윈도우 핸들

   g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_SYSMENU | WS_MINIMIZEBOX,
	   CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow); //윈도우 창을 보여줌
   UpdateWindow(hWnd);
   //UpdateWindow 함수란 생성된 윈도우의 일부가 다른 윈도우 등에 가려졌거나 리사이즈 되었을 경우,
   //즉 무효화 영역(Invalid Region)에 대해 WM_PAINT 메시지를 해당 윈도우에 보냄으로써 무효화 영역을
   //갱신시켜 주는 함수입니다

   return TRUE;
}

void DrawScreen(HDC hdc);
void MakeNewBrick();
int GetAround(int x, int y, int b, int r);
BOOL MoveDown();
void TestFull();
void PrintTile(HDC hdc, int x, int y, int c);
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit);
//함수 원형

#define BW 10 //게임 판 가로배열
#define BH 20 //게임 판 세로배열
#define random(n)(rand()%n) // 일정 범위안의 난수 생성을 위한 매크로문
#define TS 25 //비트맵 크기

struct Point
{
	int x, y;
};
//매크로 및 지역 변수들

// 각 벽돌의 패턴 배열  
Point Shape[][4][4] = //Shape[벽돌번호][회전번호][일련번호(x,y)]
{
	{ { 0, 0, 1, 0, 2, 0, -1, 0 }, { 0, 0, 0, 1, 0, -1, 0, -2 },
	{ 0, 0, 1, 0, 2, 0, -1, 0 }, { 0, 0, 0, 1, 0, -1, 0, -2 } }, //일자형 블럭
	{ { 0, 0, 1, 0, 0, 1, 1, 1 }, { 0, 0, 1, 0, 0, 1, 1, 1 }, 
	{ 0, 0, 1, 0, 0, 1, 1, 1 }, { 0, 0, 1, 0, 0, 1, 1, 1 } }, //네모 블럭
	{ { 0, 0, -1, 0, 0, -1, 1, -1 }, { 0, 0, 0, 1, -1, 0, -1, -1 },
	{ 0, 0, -1, 0, 0, -1, 1, -1 }, { 0, 0, 0, 1, -1, 0, -1, -1 } }, //S형 블럭
	{ { 0, 0, -1, -1, 0, -1, 1, 0 }, { 0, 0, -1, 0, -1, 1, 0, -1 },
	{ 0, 0, -1, -1, 0, -1, 1, 0 }, { 0, 0, -1, 0, -1, 1, 0, -1 } }, //Z형 블럭
	{ { 0, 0, -1, 0, 1, 0, -1, -1 }, { 0, 0, 0, -1, 0, 1, -1, 1 }, 
	{ 0, 0, -1, 0, 1, 0, 1, 1 }, { 0, 0, 0, -1, 0, 1, 1, -1 } }, //ㄴ형 블럭
	{ { 0, 0, 1, 0, -1, 0, 1, -1 }, { 0, 0, 0, 1, 0, -1, -1, -1 },
	{ 0, 0, 1, 0, -1, 0, -1, 1 }, { 0, 0, 0, -1, 0, 1, 1, 1 } }, //역 ㄱ형 블럭
	{ { 0, 0, -1, 0, 1, 0, 0, 1 }, { 0, 0, 0, -1, 0, 1, 1, 0 },
	{ 0, 0, -1, 0, 1, 0, 0, -1 }, { 0, 0, -1, 0, 0, -1, 0, 1 } }, //ㅗ형 블럭
};
enum { EMPTY, BRICK, WALL = sizeof(Shape) / sizeof(Shape[0]) + 1 };
int board[BW + 2][BH + 2];  //게임판 테두리
int nx, ny; //현재 위치
int brick, rot; //블럭, 회전
int nbrick[2]; //다음에 출력될 블럭
int score; //점수
int bricknum; //블럭 갯수
enum tag_Status{ GAMEOVER, RUNNING, PAUSE }; //tag_Status에 게임 종료(0), 게임 진행(1), 일시정지(2) 배열식으로 선언
tag_Status GameStatus; //tag_Status를 GameStatus로 선언 
int Interval; //시간 사이 간격
HBITMAP hBit[10]; //비트맵 선언










//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
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
	//윈도우가 처음 만들어질때 보내는 메세지
	// 이 메시지 처리 루틴에서 주로 프로그램 초기화를 수행하며 실행을
	// 위한 준비를 한다. 여기서는 메모리 DC를 만들고 두개의 비트맵을
	// 미리 읽어둔다.
	{
	case WM_CREATE: //윈도우가 처음 만들어질때 사용
		hWndMain = hWnd;
		SetRect(&crt, 0, 0, (BW + 12)*TS, (BH + 2)*TS); // crt의 크기를 조정.
		AdjustWindowRect(&crt, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE); 
		SetWindowPos(hWndMain, NULL, 0, 0, crt.right - crt.left, crt.bottom - crt.top,SWP_NOMOVE | SWP_NOZORDER);
		// 윈도우 여백 설정(윈도우의 크기 설정)
		//윈도우 크기 조절 못하게 설정(최대화 삭제)

		GameStatus = GAMEOVER;  //게임의 상태는 게임 종료 상태
		srand(GetTickCount()); //컴퓨터의 시간을 1초단위에서 미리세컨드로 나눠줌
		for (i = 0; i < 10; i++)
		{
			hBit[i] = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1 + i));
		}

		return 0;
		
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDM_GAME_START: //게임 시작을 눌렀을때
		{
			if (GameStatus != GAMEOVER) //게임 종료가 아닐때
				break; //빠져나온다
			for (x = 0; x < BW + 2; x++) //가로로 +2칸
			{
				for (y = 0; y < BH + 2; y++) // 세로로 +2칸
				{
					board[x][y] = (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) ? WALL : EMPTY;
				}//board[x][y] 배열에 안에 이게 WALL인지 EMPTY인지 판별해서 저장함
			}
			score = 0; //점수를 0으로 초기화
			bricknum = 0; //벽 갯수를 0으로 초기화
			GameStatus = RUNNING; //게임 상태를 진행중으로 선언
			nbrick[0] = random(sizeof(Shape) / sizeof(Shape[0])); //랜덤하게 7개의 블록중 1개를 생성
			MakeNewBrick(); //함수호출
			Interval = 1000; //블록 내려오는 초를 조절 시스템은 천당 1초로 기억함
			SetTimer(hWnd, 1, Interval, NULL); //1초에 1칸씩 내려가도록 적용시킴
			break;

		}


		case IDM_GAME_PAUSE: //일시 정지를 눌렀을때
			if (GameStatus == RUNNING) 
			{
				GameStatus = PAUSE;
				KillTimer(hWnd, 1);
				//게임이 실행중일시 일시정지로 바꾸고 타이머를 멈춤
			}
			else if (GameStatus == PAUSE)
			{
				GameStatus = RUNNING;
				SetTimer(hWnd, 1, Interval, NULL);
				//게임이 일시정지 일시 실행중으로 바꾸고 타이머 시작
			}

			break;
		case IDM_GAME_EXIT: //게임 종료
			DestroyWindow(hWnd); //창을 종료
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
		}//게임 실행상태가 아니거나 벽돌이 없으면 메모리 반환 (키가 먹지 않음)
		switch (wParam)
		{
		case VK_LEFT: //왼쪽키를 눌렀을때
			if (GetAround(nx - 1, ny, brick, rot) == EMPTY) //빈공간일시에 이동
			{
				nx--; //왼쪽으로이동 
				InvalidateRect(hWnd, NULL, FALSE); 
				//지웠다가 다시 그려줌 NULL값은 전체영역 FALSE는 그린 부분만 지정
			}
			break;
		case VK_RIGHT: //오른쪽키를 눌렀을때
			if (GetAround(nx + 1, ny, brick, rot) == EMPTY) //빈공간일 시에 이동
			{
				nx++; //오른쪽으로 이동
				InvalidateRect(hWnd, NULL, FALSE);
				//지웠다가 다시 그려줌 NULL값은 전체영역 FALSE는 그린 부분만 지정
			}
			break; 
		case VK_UP: //위쪽 키를 눌렀을때
			trot = (rot == 3 ? 0 : rot + 1); 
			//시뮬레이션을 해봄 3일경우 0으로 0일경우 rot+1 해서 1로 1일 경우 rot+1을 해서 2로
				if (GetAround(nx, ny, brick, trot) == EMPTY) //만약 회 전할 공간이 있을시에 회전함
				{
					rot = trot; //회전
					InvalidateRect(hWnd, NULL, TRUE);
					//지웠다가 다시 그려줌 NULL값은 전체영역 TRUE는 배경 전체로 지정
				}
				break; 
		case VK_DOWN: //아래쪽 키를 눌렀을때
			if (MoveDown() == TRUE)
			//1칸 아래로 이동
			{
				MakeNewBrick();
				//
			}
					break;

		case VK_SPACE: //스페이스바를 눌렀을때
			while (MoveDown() == FALSE)
			//맨 아래로 이동
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
	case WM_DESTROY: //윈도우를 메모리에서 파괴
		KillTimer(hWndMain, 1);
		//타이머 종료
		for (i = 0; i < 11; i++)
		{
			DeleteObject(hBit[i]);
			//hbit[i]를 삭제
		}
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void DrawScreen(HDC hdc)
//화면 전체를 그린다. 게임판과 게임 이름, 벽까지 한꺼번에 그린다.
{
	int x, y, i;
	TCHAR str[128]; //배열 선언

	for (x = 0; x < BW + 1; x++)
	{
		PrintTile(hdc, x, 0, WALL);
		PrintTile(hdc, x, BH+1, WALL);
	}//위, 아래에 테두리 그림
	
	for (y = 0; y < BH + 2; y++)
	{
		PrintTile(hdc, 0, y, WALL);
		PrintTile(hdc, BW+1, y, WALL);
	}//양쪽 테두리 그림


	for (x = 1; x < BW + 1; x++)
	{
		for (y = 1; y < BH + 1; y++)
		{
			PrintTile(hdc, x, y, board[x][y]); 
		}
	}//게임판 그림

	if (GameStatus != GAMEOVER && brick != -1) // 만약 게임이 끝나지 않았고 블록 숫자가 없지 않았다면
	{
		for (i = 0; i < 4; i++)
		{
			PrintTile(hdc, nx + Shape[brick][rot][i].x, ny + Shape[brick][rot][i].y, brick + 1);
		}
	}//이동중인 벽돌 그려주기



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

	} //다음 넥스트 블록 게임판 그림 그려주기

	if (GameStatus != GAMEOVER) //게임이 끝나지 않았을시에
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
	}//다음 벽돌 그림

	lstrcpy(str, TEXT("Tetris")); //Tetris 출력
	TextOut(hdc, (BW + 4)*TS, 30, str, lstrlen(str));
	wsprintf(str, TEXT("점수 : %d   "), score); //점수 출력
	TextOut(hdc, (BW + 4)*TS, 60, str, lstrlen(str));
	wsprintf(str, TEXT("벽돌 : %d   개"), bricknum); //벽돌 갯수 출력
	TextOut(hdc, (BW + 4)*TS, 80, str, lstrlen(str));
	//정보 출력
}

void MakeNewBrick() //다음 블록을 가져오는 함수
{
	bricknum++; //블록 갯수를 세기 위해
	brick = nbrick[0]; //미리보기 블록을 현재 블록으로 가져옴
	nbrick[0] = nbrick[1];
	nbrick[1] = nbrick[2];


	nbrick[2] = random(sizeof(Shape) / sizeof(Shape[0]));//sizeof(Shape) / sizeof(Shape[0])을 해서 그중 1개를 미리보기 블록으로 랜덤으로 생성

	nx = BW / 2; //다음 블록좌표를 중앙으로
	ny = 3;  //다음 블롣 좌표를 3칸 아래로
	rot = 0; //처음 회전 상태 0으로 초기화

	InvalidateRect(hWndMain, NULL, TRUE);
	if (GetAround(nx, ny, brick, rot) != EMPTY) //만약 GetAround(벽돌이동 회전 가능성 조사)함수 주위에 빈공간이 아니라면
	{
		KillTimer(hWndMain, 1);
		//타이머를 제거
		GameStatus = GAMEOVER;
		//벽돌 생성 위치에 이미 벽돌이 있으면 게임 종료
		MessageBox(hWndMain, 
			TEXT("게임이 끝났습니다. 다시 시작하려면 게임시작")
			TEXT("항목(S)를 선택해 주십시오."), 
			TEXT("알림"), MB_OK);
	}
}


int GetAround(int x, int y, int b, int r)
//이동중인 벽돌의 주변을 조사하는 것이 아니므로 인수로 전달된 위치의 벽돌 모양을 참조한다.(블록의 좌표)
{
	int i, k = EMPTY; // i, k를 빈공간으로 선언

	for (i = 0; i < 4; i++) 
	{
		k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
	} //max(a,b) (((a) > (b)) ? (a) : (b)) 큰값을 리턴
	  //삼항 연산자는 참과 거짓을 판별한다.
	//

	return k;
}

BOOL MoveDown()
//만약 바닥에 닿았는지 판별하는 함수 닿았으면 TestFull 함수(수평으로 다 채워진 줄을 찾아 삭제)를 호출한 후 TRUE를 리턴한다.
{
	if (GetAround(nx, ny + 1, brick, rot) != EMPTY) //벽돌의 위치가 빈공간이 아니라면
	{
		TestFull(); //수평으로 다 채줘진 줄을 찾아 삭제
		return TRUE; //TURE면 벽이 있다 FALSE면 벽이 없다.
	}

	ny++; //y값을 한칸 내려줌
	InvalidateRect(hWndMain, NULL, FALSE);
	UpdateWindow(hWndMain);
	//즉시 그려서 내려가는 모양을 보여준다

	return FALSE;
}


void TestFull()
//수평으로 다 채워진 줄을 찾아 삭제한다
{
	int i, x, y, ty;
	int count = 0;
	static int arScoreInc[] = {0,1,3,8,20}; //콤보점수

	for (i = 0; i < 4; i++)
	{
		board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = brick + 1;
	}
	//배열에 현재 떨어진 벽돌을 기록

	brick = -1; //블럭이 사라질때 키가 안먹히기 위해 초기화

	for (y = 1; y < BH + 1; y++)
	{
		for (x = 1; x < BW + 1; x++)//보드판 검사
		{
			if (board[x][y] == EMPTY) //만약 게임판에 빈공간이 있을경우
				break; //빠져나옴
		}

		if (x == BW + 1) //만약 x가 BW+1일 경우
		{
			count++; 
			for (ty = y; ty>1; ty--)
			{
				for (x = 1; x < BW + 1; x++)
				{
					board[x][ty] = board[x][ty - 1]; //해당 윗줄을 복사한다.
				}
			}

			InvalidateRect(hWndMain, NULL, FALSE);
			UpdateWindow(hWndMain);
			//UpdateWindow 함수란 생성된 윈도우의 일부가 다른 윈도우 등에 가려졌거나 리사이즈 되었을 경우, 
			//즉 무효화 영역(Invalid Region)에 대해 WM_PAINT 메시지를 해당 윈도우에 보냄으로써 무효화 영역을
			//갱신시켜 주는 함수입니다.
			//배열에 현재 떨어진 벽돌을 기록
			Sleep(150);
			//줄 삭제 후 1000/150초 동안 지연시간
		}
	}
	score += arScoreInc[count]; // 스코어 점수를 더해준다
	
	if (bricknum % 10 == 0 && Interval > 200)//블록 숫자10으로 나눠서 나머지가 0일때 
											//그리고 내려오는 시간이 200 이상일시에
	{
		Interval -= 50; //내려오는 시간을 점점 줄여줌
		SetTimer(hWndMain, 1, Interval, NULL); //줄인 시간 적용
	}
}

void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;
	
	

	MemDC = CreateCompatibleDC(hdc);
	//메모리 DC를 만들때 사용하는 함수
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit, sizeof(BITMAP), &bit);
	//메모리에 저장된 비트맵 정보를 가져옴
	bx = bit.bmWidth - 23;//비트맵 가로
	by = bit.bmHeight - 23; //비트맵 세로

	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);
	//BitBlt 함수는 DC간의 영역끼리 고속 복사를 수행한다. 
	//메모리 DC의 표면에 그려져 있는 비트맵을 화면 DC로 복사함으로써 비트맵을 화면으로 출력한다.

	SelectObject(MemDC, OldBitmap);
	//SelectObject는 이전에 가지고 있던 정보를 반환
	DeleteDC(MemDC);
	//사용이 끝났을시에 메모리DC삭제
}

void PrintTile(HDC hdc, int x, int y, int c)
{
	DrawBitmap(hdc, x*TS, y * TS, hBit[c]); //DrawBitMap 함수 호출 해서 타일 형식으로 바꿔줌
	return;
}




// 정보 대화 상자의 메시지 처리기입니다.
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
