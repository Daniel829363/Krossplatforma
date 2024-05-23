// autotetris.cpp: ¶¨ÒåÓ¦ÓÃ³ÌÐòµÄÈë¿Úµã¡£
//

#include "stdafx.h"
#include "autotetris.h"
#define MAX_LOADSTRING 100

// È«¾Ö±äÁ¿: 
HINSTANCE hInst;                                // µ±Ç°ÊµÀý
WCHAR szTitle[MAX_LOADSTRING];                  // ±êÌâÀ¸ÎÄ±¾
WCHAR szWindowClass[MAX_LOADSTRING];            // Ö÷´°¿ÚÀàÃû

// ´Ë´úÂëÄ£¿éÖÐ°üº¬µÄº¯ÊýµÄÇ°ÏòÉùÃ÷: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ÔÚ´Ë·ÅÖÃ´úÂë¡£

	// ³õÊ¼»¯È«¾Ö×Ö·û´®
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_AUTOTETRIS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Ö´ÐÐÓ¦ÓÃ³ÌÐò³õÊ¼»¯: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AUTOTETRIS));

	MSG msg;

	// Ö÷ÏûÏ¢Ñ­»·: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  º¯Êý: MyRegisterClass()
//
//  Ä¿µÄ: ×¢²á´°¿ÚÀà¡£
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AUTOTETRIS));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_AUTOTETRIS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   º¯Êý: InitInstance(HINSTANCE, int)
//
//   Ä¿µÄ: ±£´æÊµÀý¾ä±ú²¢´´½¨Ö÷´°¿Ú
//
//   ×¢ÊÍ: 
//
//        ÔÚ´Ëº¯ÊýÖÐ£¬ÎÒÃÇÔÚÈ«¾Ö±äÁ¿ÖÐ±£´æÊµÀý¾ä±ú²¢
//        ´´½¨ºÍÏÔÊ¾Ö÷³ÌÐò´°¿Ú¡£
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // ½«ÊµÀý¾ä±ú´æ´¢ÔÚÈ«¾Ö±äÁ¿ÖÐ

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_BORDER | WS_SYSMENU,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
VOID CALLBACK TimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	// ¶¨Ê±Æ÷
	checkTetris(hWnd, VK_DOWN);
}
// º¯Êý£º Æô¶¯ÓÎÏ·
VOID InitGame(HWND hWnd)
{
	// ³õÊ¼»¯²ÎÊý
	btCurGrid.init(PAINT_YNODE, GAME_XNODE);
	iInterval = AI_SWITCH ? AI_DEFAULT_INTERVAL : DEFAULT_INTERVAL;
	iCurDif = SIMPLE;
	iCurScore = 0;
	// Éú³ÉÊ×¸öÏÂÂäÍ¼ÐÎ
	curTetris.random();
	// Éú³ÉÏÂÒ»¸öÏÂÂäÍ¼ÐÎ
	nextTetris.random();
	// Æô¶¯¶¨Ê±Æ÷
	KillTimer(hWnd, TIMER_ID);

	SetTimer(hWnd, TIMER_ID, iInterval, TimeProc);

}

// º¯Êý£º ³õÊ¼»¯´°ÌåµÄÎ»ÖÃºÍ´óÐ¡
VOID InitWndPostionSize(HWND hWnd)
{
	RECT rect;
	int nWinX, nWinY, nClientX, nClientY;
	//»ñÈ¡´°¿Ú´óÐ¡  
	GetWindowRect(hWnd, &rect);
	nWinX = rect.right - rect.left;
	nWinY = rect.bottom - rect.top;
	//»ñÈ¡¿Í»§Çø´óÐ¡  
	GetClientRect(hWnd, &rect);
	nClientX = rect.right - rect.left;
	nClientY = rect.bottom - rect.top;

	// »ñÈ¡µ±Ç°ÆÁÄ»µÄ¿í¸ß£¬½«´°¿Ú·ÅÖÃÖÐÐÄÇøÓò
	UINT iScreenX = GetSystemMetrics(SM_CXSCREEN);
	UINT iScreenY = GetSystemMetrics(SM_CYSCREEN);
	// µ÷Õû´°¿ÚÎ»ÖÃÓë´óÐ¡
	MoveWindow(hWnd, (iScreenX - PAINT_WIDTH) / 2, (iScreenY - PAINT_HEIGHT) / 3, PAINT_WIDTH + nWinX - nClientX, PAINT_HEIGHT + nWinY - nClientY, true);
}

// º¯Êý£º »æÖÆ±³¾°
VOID DrawBackGround(HDC hdc)
{
	HPEN hPenNull = (HPEN)GetStockObject(NULL_PEN);         // HPEN NULL
	HBRUSH hBrushGray = (HBRUSH)GetStockObject(GRAY_BRUSH); // hBRUSH GRAY
	SelectObject(hdc, hPenNull);
	SelectObject(hdc, hBrushGray); // ±³¾°ÑÕÉ«	
	// ÓÎÏ·ÇøÓò´óÐ¡ (HDC,×óÉÏ½ÇX,×óÉÏ½ÇY£¬ÓÒÏÂ½ÇX£¬ÓÒÏÂ½ÇY)
	Rectangle(hdc, GAME_POSITION.iLeftX, GAME_POSITION.iLeftY, GAME_POSITION.iRightX, GAME_POSITION.iRightY);
	// ÐÅÏ¢Çø´óÐ¡
	Rectangle(hdc, INFO_POSITION.iLeftX, INFO_POSITION.iLeftY, INFO_POSITION.iRightX, INFO_POSITION.iRightY);
	DeleteObject(hBrushGray);
	DeleteObject(hPenNull);

}
// º¯Êý£º »æÖÆÐÅÏ¢À¸
VOID DrawInfo(HDC hdc)
{
	// ÔèãóðûÏÂÂä·½¿é
	RECT rect;
	HFONT hFont = CreateFont(23, 0, 0, 0, FW_THIN, 0, 0, 0, UNICODE, 0, 0, 0, 0, L"÷î");
	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(255, 255, 0));

	NEXT_FONT_POSITION.setRect(rect);
	DrawText(hdc, TEXT("Ôèãóðû"), _tcslen(TEXT("Ôèãóðû")), &rect, 0);

	LEVEL_FONT_POSITION.setRect(rect);
	TCHAR tcDif[10];
	LPCTSTR lpctDif = TEXT("×òî-òî: %d");
	wsprintf(tcDif, lpctDif, iCurDif);
	DrawText(hdc, tcDif, _tcslen(tcDif), &rect, 0);

	SCORE_FONT_POSITION.setRect(rect);
	TCHAR tcScore[10];
	LPCTSTR lpctScore = TEXT("Î÷êè: %d");
	wsprintf(tcScore, lpctScore, iCurScore);
	DrawText(hdc, tcScore, _tcslen(tcScore), &rect, 0);

	DeleteObject(hFont);
}
// ¸ù¾Ý×ø±ê»ñÈ¡µ±Ç°½ÚµãÎ»ÖÃ
RectPosition getRectPosition(BYTE x, BYTE y)
{
	RectPosition rectPs;
	rectPs.iLeftX = BLOCK_SIZE + y * NODE_SIZE;
	rectPs.iLeftY = BLOCK_SIZE + x * NODE_SIZE;
	rectPs.iRightX = rectPs.iLeftX + NODE_SIZE;
	rectPs.iRightY = rectPs.iLeftY + NODE_SIZE;
	return rectPs;
}
// º¯Êý£º»æÖÆ·½¿é
VOID DrawTetris(HDC hdc)
{
	RectPosition ps;
	HPEN hPen = (HPEN)GetStockObject(NULL_PEN);
	SelectObject(hdc, hPen);
	// »æÖÆGameÇøÓò
	for (int i = 0; i < PAINT_YNODE; i++)
	{
		for (int j = 0; j < GAME_XNODE; j++)
		{
			if (btCurGrid[i][j] > 0)
			{
				// »æÖÆµ±Ç°½Úµã
				SelectObject(hdc, getHbrush(btCurGrid[i][j]));
				ps = getRectPosition(i, j);
				Rectangle(hdc, ps.iLeftX, ps.iLeftY, ps.iRightX, ps.iRightY);
			}
		}
	}
	// »æÖÆÕýÔÚÏÂÂäÍ¼ÐÎ
	SelectObject(hdc, getHbrush(curTetris.color));
	for (int i = 0; i < 4; i++)
	{
		ps = getRectPosition(curTetris.positions[i].x, curTetris.positions[i].y);
		Rectangle(hdc, ps.iLeftX, ps.iLeftY, ps.iRightX, ps.iRightY);
	}
	// »æÖÆÏÂÒ»´ÎÏÂÂäÍ¼ÐÎ
	int move_x = 8 * NODE_SIZE + BLOCK_SIZE;
	int move_y = 2 * NODE_SIZE + BLOCK_SIZE;
	SelectObject(hdc, getHbrush(nextTetris.color));
	for (int i = 0; i < 4; i++)
	{
		ps = getRectPosition(nextTetris.positions[i].x, nextTetris.positions[i].y);
		Rectangle(hdc, ps.iLeftX + move_x, ps.iLeftY + move_y, ps.iRightX + move_x, ps.iRightY + move_y);
	}
}
// µ÷ÕûÓÎÏ·ÄÑ¶È
VOID setLevel(HWND hWnd)
{
	BOOL SimpleToGeneral = iCurDif < GENERAL && iCurScore >= GENERAL_SCORE;
	BOOL GeneralToDifficult = iCurDif < DIFFICULT && iCurScore >= DIFFICULT_SCORE;
	BOOL DifficultToEst = iCurDif < DIFFICULTEST && iCurScore >= DIFFICULTEST_SCORE;

	// ÉèÖÃÄÑ¶È
	if (SimpleToGeneral || GeneralToDifficult || DifficultToEst)
	{
		iCurDif++;
		iInterval -= 250;
		KillTimer(hWnd, TIMER_ID);
		SetTimer(hWnd, TIMER_ID, iInterval, TimeProc);
	}
}
// ÏÂÂäÍê³É
VOID finishDown(HWND hWnd)
{
	// ¸üÐÂbtCurGridµ±Ç°½çÃæÊý×é
	btCurGrid.addCurTerisToGrid(curTetris);
	// ¸üÐÂÏÂÂä·½¿é£¬ÖØÐÂÉú³ÉnextTetris
	curTetris = nextTetris;
	nextTetris.random();
	// Ïû³ý
	iCurScore += btCurGrid.clearLine();
	// ÅÐ¶ÏÉú³ÉÎ»ÖÃÊÇ·ñ±»Õ¼ÓÃ
	if (btCurGrid.isExist(curTetris))
	{
		// TODO ÓÎÏ·½áÊø
		KillTimer(hWnd, TIMER_ID);
		MessageBox(hWnd, TEXT("Óïñ"), TEXT("èãðà îêîí÷åíà"), MB_OK);
		return;
	}
	// µ÷ÕûÓÎÏ·ÄÑ¶È(aiÄ£Ê½²»ÐèÒª)
	if (!AI_SWITCH)
	{
		setLevel(hWnd);
		InvalidateRect(hWnd, NULL, TRUE);
	}
	else
	{
		InvalidateRect(hWnd, NULL, TRUE);
		// Èç¹ûÎªAIÄ£Ê½£¬¼ÌÐø¼ÆËã
		AIHandle(hWnd);
	}
}

// ¼ì²éÍ¼ÐÎÏÂÂäÊÇ·ñÔ½½ç/Íê³É / Ïû³ý
VOID checkTetris(HWND hWnd, INT type)
{
	int maxX, minX, maxY, minY;
	Tetris temp = curTetris;
	switch (type)
	{
	case VK_LEFT: // ×ó
		temp.left();
		break;
	case VK_RIGHT: // ÓÒ
		temp.right();
		break;
	case VK_UP: // ÉÏ
		temp.rotate();
		break;
	case VK_DOWN: // ÏÂ(²úÉú¿ìËÙÏÂ½µÐ§¹û)
		temp.down();
		break;
	}
	temp.getMaxMin(minX, maxX, minY, maxY);

	// ÅÐ¶ÏÊÇ·ñÔ½½ç
	if (maxX > 19)
	{
		// ´¥Åöµ×²¿£¬ÏÂÂä½áÊø
		finishDown(hWnd);
		return;
	}
	if (minX < 0 || maxY > 9 || minY < 0)
	{
		// ´¥Åö×óÓÒ±ß½ç
		return;
	}
	// ÅÐ¶ÏÎ»ÖÃÊÇ·ñ±»Õ¼ÓÃ
	if (btCurGrid.isExist(temp))
	{
		if (type == VK_DOWN)
		{
			// ÏÂÂäµ½ÒÑ´æÔÚ½ÚµãÉÏ·½£¬ÏÂÂä½áÊø
			finishDown(hWnd);
		}
		return;
	}
	// ²Ù×÷ÔÊÐí
	curTetris = temp;
	InvalidateRect(hWnd, NULL, TRUE);
}
//
//  º¯Êý: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ä¿µÄ:    ´¦ÀíÖ÷´°¿ÚµÄÏûÏ¢¡£
//
//  WM_COMMAND  - ´¦ÀíÓ¦ÓÃ³ÌÐò²Ëµ¥
//  WM_PAINT    - »æÖÆÖ÷´°¿Ú
//  WM_DESTROY  - ·¢ËÍÍË³öÏûÏ¢²¢·µ»Ø
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		// ³õÊ¼»¯´°ÌåµÄÎ»ÖÃºÍ´óÐ¡
		InitWndPostionSize(hWnd);
		// Æô¶¯ÓÎÏ·
		InitGame(hWnd);
	}
	break;
	case WM_KEYDOWN:
	{
		if (!AI_SWITCH) // ÔÚ×Ô¶¯ÓÎÏ·Ä£Ê½ÏÂ ¼üÅÌ²»¿ÉÓÃ
		{
			checkTetris(hWnd, wParam);
		}
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// ·ÖÎö²Ëµ¥Ñ¡Ôñ: 
		switch (wmId)
		{
		case IDM_COMMON:
			AI_SWITCH = false;
			InitGame(hWnd);
			break;
		case IDM_AI:
			AI_SWITCH = true;
			InitGame(hWnd);
			AIHandle(hWnd);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// »æÖÆ±³¾°
		DrawBackGround(hdc);
		// »æÖÆÐÅÏ¢À¸
		DrawInfo(hdc);
		// »æÖÆÍ¼ÐÎ
		DrawTetris(hdc);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ¡°¹ØÓÚ¡±¿òµÄÏûÏ¢´¦Àí³ÌÐò¡£
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

// º¯Êý£ºÆô¶¯AIÄ£Ê½
VOID AIHandle(HWND hWnd)
{
	// Pierre Dellacherie »ñÈ¡×î¼ÑÂä½Åµã
	getBestPosition();
	// ×Ô¶¯ÒÆ¶¯µ½×î¼ÑÎ»ÖÃ²¢½áÊøÏÂÂä
	aiMoveTetris(hWnd);
}
// »ñÈ¡×î¼ÑÆÀ·Ö¼°ÆäÎ»ÖÃ
VOID getBestPosition()
{
	bestScore = 0x80000000;
	int maxX, minX, maxY, minY;
	algoTetris = curTetris;
	Tetris temp;
	//»ñÈ¡Íø¸ñË®Æ½Ãæ
	getHorizonIndex();
	// Í¼ÐÎÓÐËÄÖÖ×´Ì¬
	for (int i = 0; i < 4; i++)
	{
		algoTetris.getMaxMin(minX, maxX, minY, maxY);
		int num = 10 - (maxY - minY); // Ã¿´Î±ä»»²úÉúnumÖÖÇé¿ö
		for (int j = 0; j < num; j++)
		{
			// »¹Ô­Íø¸ñ
			algoGrid = btCurGrid;
			temp = algoTetris;
			// ½«base_tetris·Åµ½algoGridµÄÖ¸¶¨Î»ÖÃÖÐ, ·µ»ØÐèÒªÏò×óÒÆ¶¯´ÎÊý
			int moveLeft = putTetrisToGrid(j);
			// ÆÀ·Ö
			int score = Dellacherie();
			if (score > bestScore)
			{
				bestScore = score;
				bestTetris = algoTetris;
				needRotate = i;
				needMoveLeft = moveLeft;
			}
			// »¹Ô­ algoTetri
			algoTetris = temp;
		}
		algoTetris.rotate();
	}
}
VOID CALLBACK AITimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	if (needRotate)
	{
		// ÏÈ±äÐÎ
		needRotate--;
		curTetris.rotate();
	}
	else if (needMoveLeft > 0)
	{
		// ×óÒÆ¶¯
		curTetris.left();
		needMoveLeft--;
	}
	else if (needMoveLeft < 0)
	{
		// ÓÒÒÆ¶¯
		curTetris.right();
		needMoveLeft++;
	}
	// ÖØÐÂ»æÖÆ½çÃæ
	InvalidateRect(hWnd, NULL, TRUE);
	// ½áÊø¶¨Ê±Æ÷
	if (needRotate == 0 && needMoveLeft == 0)
	{
		KillTimer(hWnd, AI_TIMER_ID);
	}
}
// ¿ìËÙ²âÊÔÓÃ¶¨Ê±Æ÷
VOID CALLBACK TESTTimeProc(HWND hWnd, UINT message, UINT idTimer, DWORD dwTime)
{
	// ¸üÐÂbtCurGridµ±Ç°½çÃæÊý×é
	btCurGrid.addCurTerisToGrid(bestTetris);
	// ¸üÐÂÏÂÂä·½¿é£¬ÖØÐÂÉú³ÉnextTetris
	curTetris = nextTetris;
	nextTetris.random();
	// Ïû³ý
	iCurScore += btCurGrid.clearLine();
	// ÅÐ¶ÏÉú³ÉÎ»ÖÃÊÇ·ñ±»Õ¼ÓÃ
	if (btCurGrid.isExist(curTetris))
	{
		// TODO ÓÎÏ·½áÊø
		KillTimer(hWnd, 3);
		MessageBox(hWnd, TEXT("ÓÎÏ·½áÊø"), TEXT("ÏûÏ¢"), MB_OK);
		return;
	}
	// Èç¹ûÎªAIÄ£Ê½£¬¼ÌÐø¼ÆËã
	AIHandle(hWnd);
}
// ½«·½¿éÒÆ¶¯µ½×î¼ÑÎ»ÖÃ
VOID aiMoveTetris(HWND hWnd)
{
	SetTimer(hWnd, AI_TIMER_ID, iInterval / 5, AITimeProc);
	// SetTimer(hWnd, 3, 0 ,TESTTimeProc); // ¿ìËÙ²âÊÔÊ¹ÓÃ
}
// »ñÈ¡Íø¸ñË®Æ½Ïß
VOID getHorizonIndex()
{
	for (int i = 0; i < GAME_XNODE; i++)
	{
		for (int j = 0; j < PAINT_YNODE - 1; j++)
		{
			if (btCurGrid[j + 1][i])
			{
				horizonIndex[i] = j;
				break;
			}
			if (j == (PAINT_YNODE - 2))
			{
				horizonIndex[i] = j + 1;
			}
		}
	}

}
INT putTetrisToGrid(int col)
{
	int left_bottom_index = algoTetris.getLeftBottom(); // »ñÈ¡×î×óÏÂ½ÇµÄÔªËØÏÂ±ê

	int maxDistanceRow = horizonIndex[col] - algoTetris.positions[left_bottom_index].x; // ÐèÒªÏòÏÂÒÆ¶¯µÄ¾àÀë
	int maxDistanceCol = algoTetris.positions[left_bottom_index].y - col; // ÐèÒªÏò×óÒÆ¶¯µÄ¾àÀë
	// µ÷ÕûÏòÏÂÒÆ¶¯¾àÀë
	for (int j = 0; j < 4; j++)
	{
		if (j != left_bottom_index)
		{
			int colDistance = algoTetris.positions[j].y - algoTetris.positions[left_bottom_index].y;
			int rowDistance = horizonIndex[col + colDistance] - algoTetris.positions[j].x;
			if (rowDistance < maxDistanceRow)
			{
				maxDistanceRow = rowDistance;
			}
		}
	}
	// ½«algoTetrisÒÆ¶¯µ½Ö¸¶¨Î»Î»ÖÃ
	for (int j = 0; j < 4; j++)
	{
		algoTetris.positions[j].x += maxDistanceRow;
		algoTetris.positions[j].y -= maxDistanceCol;
		algoGrid[algoTetris.positions[j].x][algoTetris.positions[j].y] = algoTetris.color;
	}
	return maxDistanceCol;
}
// AIÆÀ·ÖËã·¨
INT Dellacherie()
{
	INT lh_score, re_score, rt_score, ct_score, nh_score, ws_score;
	lh_score = getLH();
	re_score = getRE();
	rt_score = getRT();
	ct_score = getCT();
	nh_score = getNH();
	ws_score = getWS();
	return lh_score * LH_WEIGHT + re_score * RE_WEIGHT + rt_score * RT_WEIGHT + ct_score * CT_WEIGHT + nh_score * NH_WEIGHT + ws_score * WS_WEIGHT;
}
// ¸ß¶ÈÆÀ·Ö
INT getLH()
{
	int maxX, minX, maxY, minY;
	algoTetris.getMaxMin(minX, maxX, minY, maxY);
	return PAINT_YNODE - maxX;
}
// Ïû³ýÐÐ*¹±Ï×·½¿éÊýÆÀ·Ö
INT getRE()
{
	int  status, clearline = 0, cells = 0;

	for (int i = PAINT_YNODE - 1; i >= 0; i--)
	{
		status = algoGrid.lineStatus(i);
		if (status == 0)
		{
			continue;
		}
		if (status == 1)
		{
			// Âú
			clearline++; // ÐÐ+1
			// ´ËÐÐ¹±Ï×·½¿éÊý
			for (int j = 0; j < 4; j++)
			{
				if (algoTetris.positions[j].x == i)
				{
					cells++;
				}
			}
		}
		else
		{
			// ¿Õ
			break;
		}
	}
	return clearline * cells;
}
// ÐÐ±ä»¯ÂÊÆÀ·Ö
INT getRT()
{
	// TODO ¿ÉÒÔÓÅ»¯£¬Óöµ½¿ÕÐÐ£¬ÒÔÉÏÐÐÖ±½ÓÃ¿ÐÐ+2
	int lineChane = 0, lastStatus;
	for (int i = PAINT_YNODE - 1; i >= 0; i--)
	{
		lastStatus = 1; // ×óÇ½±Ú×´Ì¬ÎªÓÐÐ§£¬µ±×÷´æÔÚÍ¼ÐÎ
		for (int j = 0; j < GAME_XNODE; j++)
		{
			// ÓëÉÏÒ»¸ö¸ñ×Ó²»Í¬
			if (!(lastStatus * algoGrid[i][j]) && lastStatus != algoGrid[i][j])
			{
				lineChane++;
				lastStatus = algoGrid[i][j];
			}
			// µ±Ç°ÐÐ×îºóÒ»¸ö½ÚµãÎª¿Õ
			if (j == (GAME_XNODE - 1) && algoGrid[i][j] == 0)
			{
				lineChane++;
			}
		}
	}
	return lineChane;
}
// ÁÐ±ä»¯ÂÊÆÀ·Ö
INT getCT()
{
	int colChane = 0, lastStatus;
	for (int i = 0; i < GAME_XNODE; i++)
	{
		lastStatus = 1; // ×óÇ½±Ú×´Ì¬ÎªÓÐÐ§£¬µ±×÷´æÔÚÍ¼ÐÎ
		for (int j = 0; j < PAINT_YNODE; j++)
		{
			// ÓëÉÏÒ»¸ö¸ñ×Ó²»Í¬
			if (!(lastStatus * algoGrid[j][i]) && lastStatus != algoGrid[j][i])
			{
				colChane++;
				lastStatus = algoGrid[j][i];
			}
			// µ±Ç°ÐÐ×îºóÒ»¸ö½ÚµãÎª¿Õ
			if (j == (PAINT_YNODE - 1) && algoGrid[j][i] == 0)
			{
				colChane++;
			}
		}
	}
	return colChane;
}
// ¿Õ¶´ÊýÆÀ·Ö
INT getNH()
{
	int holes = 0;
	bool holeFlag = false; // ÊÇ·ñ¿ÉÒÔ¿ªÊ¼¼ÆËã¿Õ¶´
	for (int i = 0; i < GAME_XNODE; i++)
	{
		holeFlag = false;
		for (int j = 0; j < PAINT_YNODE; j++)
		{
			if (!holeFlag)
			{
				if (algoGrid[j][i])
				{
					holeFlag = true;
				}
			}
			else
			{
				if (algoGrid[j][i] == 0)
				{
					holes++;
				}
			}
		}
	}
	return holes;
}
// ¾®ÉîÆÀ·Ö
INT getWS()
{
	// TODO: ¾®ÉîÇó½â²»¾«È·
	int wells = 0;
	int center, left, right;
	for (int i = 0; i < PAINT_YNODE; i++)
	{
		for (int j = 0; j < GAME_XNODE; j++)
		{

			center = algoGrid[i][j];
			if (j > 0)
			{
				left = algoGrid[i][j - 1];
			}
			else
			{
				left = 1;
			}
			if (j < GAME_XNODE - 1)
			{
				right = algoGrid[i][j + 1];
			}
			else
			{
				right = 1;
			}
			// ÊÇ·ñÎª¾®
			if (left && right && !center)
			{
				for (int k = i; k < PAINT_YNODE; k++)
				{
					if (algoGrid[k][j] == 0)
					{
						wells++;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	return wells;
}
