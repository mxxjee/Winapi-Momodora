// DefaultWindow.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "DefaultWindow.h"
#include "CMainGame.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND g_hWnd;
Graphics* g_pGraphics = nullptr;

bool bColRender = false;                        //콜라이더 렌더여부
float g_fVolume = 1.f;

ULONG_PTR g_GdiPlusToken;                   //GDI + 초기화 내용 저장


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void InitGDIPlus();
void ShutdownGDIPlus();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,         // 메모리에 할당되는 실 객체, 현재 프로그램의 고유 식별 번호 저장 용도
                     _In_opt_ HINSTANCE hPrevInstance,  // 전에 실행되었던 인스턴스 핸들(사용 안하기 때문에 NULL)
                     _In_ LPWSTR    lpCmdLine,          // 
                     _In_ int       nCmdShow)           // 실행한 창의 스타일 옵션

{
    srand(unsigned(time(NULL)));

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEFAULTWINDOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEFAULTWINDOW));

    MSG msg;
    msg.message = WM_NULL;


    InitGDIPlus();
    CMainGame       MainGame;
    MainGame.Initialize();

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
            {
                ShutdownGDIPlus();
                break;
            }

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        else
        {                       // 114
            MainGame.Update();
            MainGame.Late_Update();
            MainGame.Render();
        }

       

    }

    return (int) msg.wParam;
}  // 



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;       // 윈도우 창 생성을 필요로한 구조체 정보

    wcex.cbSize = sizeof(WNDCLASSEX);       // 자기 자신의 사이즈를 저장

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    // 창 스타일 지정 : 가로 다시 그리기 | 세로 다시 그리기
    // 창의 수직 또는 수평의 크기가 변경될 경우 창을 다시 그려라하는 옵션

    wcex.lpfnWndProc    = WndProc;
    // 메세지 처리기 함수 이름을 저장할 포인터

    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    // 윈도우가 특수한 목적으로 사용하는 여분의 공간, 일종의 예약 영역, 기본값 0

    wcex.hInstance      = hInstance;
    // 프로그램 번호 설정

    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEFAULTWINDOW));
    // 윈도우 창 표현 아이콘(시작 표시줄)

    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    // 마우스 커서

    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    // 배경 색상

    wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_DEFAULTWINDOW);
    // 창 상단에 메뉴

    wcex.lpszClassName  = szWindowClass;
    // 실행 파일 이름

    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    // 윈도우 상단에 있는 작은 아이콘 모양

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   RECT rc{ 0, 0, WINCX, WINCY };

   // rc = rc + 기본 윈도우 설정 값 + 메뉴 바 사이즈 크기 고려 여부

   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   HWND hWnd = CreateWindowW(szWindowClass,         // 앞서 정의한 윈도우 클래스의 전달한 클래스 이름을 넣어줌
                            szTitle,                // 창 상단의 출력할 문자열
                            WS_OVERLAPPEDWINDOW,    // 윈도우 창 스타일 지정(기본 값으로 사용되는 창 모양)
                            CW_USEDEFAULT, 0,       // 창 출력 좌표(x,y 좌표)
                            rc.right - rc.left,
                            rc.bottom - rc.top,       // 창 가로, 세로 사이즈
                            nullptr,        // 부모 윈도우 주소
                            nullptr,        // 윈도우에서 사용할 메뉴 핸들
                            hInstance,      // 윈도우를 만드는 주체
                            nullptr);       // 운영체제가 특수한 목적으로 사용

   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;
  

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

// 함수 호출 규약 : 함수의 실행이 종료되면 할당되었던 stack 메모리를 반환해야 하는데 함수를 호출한 호출원(caller)에서 
// 정리할 것인지 아니면 호출 받은 피호출자(callee)에서 할 것인가에 따른 약속, 규약

// __cdecl    : c / c++ 언어의 기본 호출 규약, caller가 stack 을 정리, 가변 인자 함수의 호출 규약
// __stdcall  : winapi 기본 호출 규약, callee가 stack 을 정리, 고정 인자 함수의 규약
// __fastcall : 함수 호출을 빠르게 처리하기 위한 호출 규약, ecx, edx 라는 레지스터를 통해 함수 코드를 저장
// __thiscall : 


// 1. KERNEL : 메모리를 관리하고 프로그램을 실행시키는 명령어 집합
// 2. USER : 유저 인터페이스와 윈도우 관리 명령어 집합
// 3. GDI : 화면 처리와 그래픽 담당

RECT    rc{ 100, 100, 200, 200 };
list<RECT>  BulletList;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
   // case WM_CREATE:
   //     
   //     SetTimer(hWnd, 0, 0, 0);
   //     // 타이머를 설치
   //     // 1. 창 핸들
   //     // 2. 타이머의 ID
   //     // 3. 타이머의 주기(기본 값으로 0을 지정하면 1 / 1000 초)
   //     // 4. NULL인 경우, 3번에서 설정한 주기대로 WM_TIMER 메세지 발생
   //     break;
   //
   // case WM_TIMER:
   //     InvalidateRect(hWnd, 0, TRUE);
   //     break;

        // TRUE : 그려져 있는 않는 모든 부분 갱신
        // FALSE : 다시 그린 부분만 갱신

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            switch (wmId)
            {
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

    //case WM_PAINT:
    //    {
    //        PAINTSTRUCT ps;
    //
    //        HDC hdc = BeginPaint(hWnd, &ps);
    //        
    //        // 커서의 위치를 이동시키는 함수(기존 커서 위치를 반환)
    //        //MoveToEx(hdc, 100, 100, nullptr);
    //        //LineTo(hdc, 200, 100);
    //        //LineTo(hdc, 200, 200);
    //        //LineTo(hdc, 100, 200);
    //        //LineTo(hdc, 100, 100);
    //        //
    //        //LineTo(hdc, 200, 200);
    //        //
    //        //MoveToEx(hdc, 200, 100, nullptr);
    //        //LineTo(hdc, 100, 200);
    //
    //        //LineTo(hdc, 200, 200);
    //        //LineTo(hdc, 200, 300);
    //
    //        // 사각형 그리기 함수
    //        //Rectangle(hdc, 100, 100, 200, 200);
    //
    //        // 원 그리기 함수
    //        //Ellipse(hdc, 300, 300, 400, 400);
    //
    //        Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
    //
    //        for (auto& rcBullet : BulletList)
    //        {
    //            Ellipse(hdc, rcBullet.left,
    //                rcBullet.top,
    //                rcBullet.right,
    //                rcBullet.bottom);
    //
    //            
    //            rcBullet.top -= 10; 
    //            rcBullet.bottom -= 10;
    //        }
    //
    //
    //
    //        EndPaint(hWnd, &ps);
    //    }
    //    break;

    case WM_KEYDOWN:

        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;

        //case VK_RIGHT:
        //
        //    rc.left += 10;
        //    rc.right += 10;
        //    break;
        //
        //case VK_LEFT:
        //
        //    rc.left -= 10;
        //    rc.right -= 10;
        //    break;
        //
        //case VK_UP:
        //
        //    rc.top    -= 10;
        //    rc.bottom -= 10;
        //    break;
        //
        //case VK_DOWN:
        //
        //    rc.top += 10;
        //    rc.bottom += 10;
        //    break;
        //
        //case VK_SPACE:
        //    BulletList.push_back(rc);
        //    
        //    break;
        //
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

void InitGDIPlus()
{

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    Gdiplus::GdiplusStartup(&g_GdiPlusToken, &gdiplusStartupInput, nullptr);
}

void ShutdownGDIPlus()
{
    Gdiplus::GdiplusShutdown(g_GdiPlusToken);
}
