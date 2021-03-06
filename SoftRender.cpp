//SoftRender.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SoftRender.h"
#include <conio.h>

#define MAX_LOADSTRING 100

// 测试
float cameraPositionX = 0;
float cameraPositionY = 0;


// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 绘图设备库
Graphics grapchics;

// 当前要绘制的设备目标环境（前置缓冲区）
HDC hdc = NULL;
// 双缓存中的后备缓冲区（每次在绘图库中绘制完成后，要将后备缓冲区交换到前置缓冲区中）
HDC screenHDC = NULL;

// 窗口宽高度
int screenWidth = 800;
int screenHeight = 600;

// 窗口起始位置
int screenPosX = 300;
int screenPosY = 200;

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL PreTranslateMessage(MSG* pMsg);	// 处理键盘输入

void InitGraphics(HDC hdc) {
	grapchics = Graphics(hdc,screenWidth,screenHeight);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SOFTRENDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOFTRENDER));

	MSG msg = {0};

	// 主消息循环: 
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else {
			DoRender();
		}
	}

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOFTRENDER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SOFTRENDER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      screenPosX, screenPosY, screenWidth, screenHeight, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   // 初始化设备目标环境
   hdc = GetDC(hWnd);
   screenHDC = CreateCompatibleDC(hdc);

   //===============================
   // 创建ScreenHDC缓冲区
   BITMAPINFO bitmapInfo = { { sizeof(BITMAPINFOHEADER),800, 600, 1, 32, BI_RGB, 800 * 600 * 4, 0, 0, 0, 0 } };
   LPVOID ptr;
   //创建设备无关的位图
   HBITMAP bitmapHandler = CreateDIBSection(screenHDC, &bitmapInfo, DIB_RGB_COLORS, &ptr, 0, 0);
   if (bitmapHandler == NULL)
	   return FALSE;
   HBITMAP screenObject = (HBITMAP)SelectObject(screenHDC, bitmapHandler);

   // 初始化绘图库
   InitGraphics(screenHDC);

   // 显示并更新窗口
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
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
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC tempHdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
	// 处理键盘输入
	case WM_KEYDOWN: {
		int wd = LOWORD(wParam);
		// 分析菜单选择: 
		switch (wd)
		{
		case VK_LEFT:
			cameraPositionX -= 0.1f;
			break;
		case VK_UP:
			cameraPositionY += 0.1f;
			break;
		case VK_RIGHT:
			cameraPositionX += 0.1f;
			break;
		case VK_DOWN:
			cameraPositionY -= 0.1f;
			break;
		}
		break;
	}
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
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

// 测试
int angle = 1;
void DoRender() {
	grapchics.Clear();

	// 三角形的三个顶点
	Vector3 v1 = Vector3(0, 0,0);
	Vector3 v2 = Vector3(1 * 0.2f,1 * 0.2f,0);
	Vector3 v3 = Vector3(2 * 0.2f,0,0);

	Vector3 worldPotion = Vector3(0,0,0);
	Vector3 scale = Vector3(1,2,2);
	Vector3 rotation = Vector3(0, (angle++) % 359 + 1, 0);

	// M矩阵
	Matrix mMatrix = grapchics.GetModelMatrix(worldPotion,rotation,scale);

	// 摄像机的Near/Far参数
	int Near = 1;int Far = 10;
	int right = 1;int left = -1;
	int bottom = -1; int top = 1;

	Vector3 cameraPostision = Vector3(cameraPositionX, cameraPositionY,-3);
	Vector3 centerPosition = Vector3(0,0,0);
	Vector3 upDir = Vector3(0,1,0);

	// V矩阵
	Matrix vMatrix = grapchics.GetViewMatrixWithUVN(cameraPostision,centerPosition,upDir);
	
	// P矩阵
	Matrix pMatrix = grapchics.GetProjectionMatrixWithFrustum(30,Near,Far,right,left,bottom,top);

	// 获得MVP矩阵(将向量当做列矩阵看待,故矩阵乘法是右乘,所以MVP的顺序反应到乘法中是PVM)
	Matrix mvp = pMatrix * vMatrix * mMatrix;

	grapchics.DrawPrimitive(v1,v2,v3,mvp);

	//====================================
	// 测试,输出当前摄像机位置
	TCHAR szBuffer[40];
	int iLength = wsprintf(
		szBuffer, 
		L"x:%d,y:",
		(int)cameraPositionX,
		(int)cameraPositionY
	);
	TextOut(screenHDC,10,10, szBuffer, iLength);
	//=====================================

	//双缓冲,将screenHDC缓冲区拷贝到HDC设备目标环境中
	BitBlt(hdc, 0, 0, screenWidth, screenHeight, screenHDC, 0, 0, SRCCOPY);
}


BOOL PreTranslateMessage(MSG* pMsg)//Dlg为对话框类
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_UP:
			printf("UP\n"); break;
		case VK_DOWN:
			printf("DOWN\n"); break;
		case VK_LEFT:
			printf("LEFT\n"); break;
		case VK_RIGHT:
			printf("RIGHT\n"); break;
		default:
			break;
		}
	}
	return true;
}
