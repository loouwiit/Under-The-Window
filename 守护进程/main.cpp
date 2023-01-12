#include <iostream>
#include <Windows.h>
#include "Window_Infomation_Class.h"
#include "resource.h"
//#include <SFML/Graphics.hpp>
//#include "Operator.h"

/*
* 守护进程
* 当用户看不到壁纸时暂停ffplay窗口
*/

/*
* 0.0.0.1 初号版本
* 
* next
*/

struct Window_And_HWND
{
	Window_Infomation* Window_Infomation_ptr;
	HWND Window_HWND;
};

constexpr unsigned int update_Time = 5;
constexpr char Progream_Version[] = "0.0.0.1";
constexpr char endl = '\n';
constexpr bool Ignore_Other_Window = true;
#ifdef _DEBUG
constexpr bool Show_Console = true;
#else
constexpr bool Show_Console = false;
#endif
constexpr float Target_Screen_Rate = 0.81f;

//sf::RenderWindow window;
//sf::Font font;

Window_Infomation* now_Window = nullptr;
time_t next_Time = 0;
HWND Console_HWND = 0;
unsigned target_Screen_Pixels = 0;
bool running = true;
bool auto_Pause = true;
bool hPause = false;
bool paused = false;

void Initialize();
void Enetialize();

void update_All_Window();
void pause_All_Window();
void Repair_W1_W2_Error(Window_Infomation* Window_Infomation_ptr);
void Get_Child_Window(Window_Infomation* Window_Infomation_ptr, HWND Parent_Window_WHND = Window_Infomation::Get_PM_Window_HWND());//枚举并连接PM的子窗口
BOOL CALLBACK Enum_Child_Widow(HWND Window_HWND, LPARAM lparam);//枚举的回调函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//子菜单的回调函数
void tray();
void update_Menu(HWND hwnd);
void loop();

int main()
{
	Initialize();

	tray();

	Enetialize();

	return 0;
}

void Initialize()
{
	std::cout << Progream_Version << endl;
#ifdef _DEBUG
	std::cout << "Debug版本" << endl;
#endif // _DEBUG
#ifndef _WIN64
	std::cout << "32位版本" << endl;
#endif // _WIN64

	Console_HWND = FindWindow(L"ConsoleWindowClass", NULL);//获取句柄
	if (Console_HWND != NULL && !Show_Console) ShowWindow(Console_HWND, SW_HIDE);//隐藏窗口
	Window_Infomation::Set_Self_Window_Exist(false);

	int desktop_Size[2] = { 0,0 };
	Window_Infomation::Get_Desktop_Size(desktop_Size[0], desktop_Size[1]);
	target_Screen_Pixels = (unsigned int)(desktop_Size[0] * desktop_Size[1] * Target_Screen_Rate);

	Window_Infomation::Set_Self_Window_Exist(false);//不存在窗口
	now_Window = new Window_Infomation;
	Get_Child_Window(now_Window); //读取全部窗口

	//退回最前
	while (now_Window->Get_Last_Window_ptr() != nullptr) now_Window = now_Window->Get_Last_Window_ptr();
	if (now_Window->Get_Window_HWND() == 0 && now_Window->Get_Next_Window_ptr() != nullptr)
	{
		//删除0
		now_Window = now_Window->Get_Next_Window_ptr();
		delete now_Window->Get_Last_Window_ptr();
		now_Window->Set_Last_Window_ptr(nullptr);
	}
}

void Enetialize()
{
	//if (paused) pause_All_Window();
#ifdef _DEBUG
	system("pause");
#endif
}


void update_All_Window()
{
	//退回最前
	Window_Infomation* temp = now_Window;
	while (temp->Get_Last_Window_ptr() != nullptr) temp = temp->Get_Last_Window_ptr();

	//输出所有窗口
	while (temp != nullptr)
	{
		temp->Update_Window_HWND();
		//std::cout << (long long)temp->Get_Window_HWND() << '|';
		if (temp->Get_Window_HWND() != 0 && IsWindow(temp->Get_Window_HWND()))
		{
			//std::cout << "exist" << endl;
			temp = temp->Get_Next_Window_ptr();
		}
		else
		{
			//std::cout << "unexist" << endl;

			if (now_Window->Get_Window_HWND() != 0)
				now_Window->Set_Window_HWND(0);

			if (now_Window->Get_Next_Window_ptr() == nullptr)
				break;//最后一个，直接跳出循环

			Window_Infomation* delete_Window = temp;

			if (temp->Get_Last_Window_ptr() != nullptr)
				temp->Get_Last_Window_ptr()->Set_Next_Window_ptr(temp->Get_Next_Window_ptr());
			if (temp->Get_Next_Window_ptr() != nullptr)
				temp->Get_Next_Window_ptr()->Set_Last_Window_ptr(temp->Get_Last_Window_ptr());

			temp = temp->Get_Next_Window_ptr();
			delete delete_Window;
			delete_Window = nullptr;
		}
	}
	Repair_W1_W2_Error(nullptr);
}

void pause_All_Window()
{
	if (hPause && paused) return;
	paused = !paused;
	//std::cout << "pause all\n";
	while (now_Window->Get_Last_Window_ptr() != nullptr)
		now_Window = now_Window->Get_Last_Window_ptr();
	for (Window_Infomation* temp = now_Window; temp != nullptr; temp = temp->Get_Next_Window_ptr())
	{
		temp->Pause_ffplay();
	}
}

void Repair_W1_W2_Error(Window_Infomation* Window_Infomation_ptr)
{
	if (Window_Infomation_ptr != nullptr)
	{
		Window_Infomation_ptr->Repair_W1_W2();
		return;
	}

	if (!Window_Infomation::Get_W1_W2_Error()) return;//没有错误

	Window_Infomation::Hide_PM_W2_Window();//隐藏W2
	//遍历全部
	for (Window_Infomation_ptr = now_Window; Window_Infomation_ptr != nullptr;
		Window_Infomation_ptr = Window_Infomation_ptr->Get_Last_Window_ptr())
	{
		//自己及左边
		Window_Infomation_ptr->Repair_W1_W2();
	}
	for (Window_Infomation_ptr = now_Window->Get_Next_Window_ptr(); Window_Infomation_ptr != nullptr;
		Window_Infomation_ptr = Window_Infomation_ptr->Get_Next_Window_ptr())
	{
		//右边
		Window_Infomation_ptr->Repair_W1_W2();
	}

	Window_Infomation::Reset_W1_W2_Error();
}

void Get_Child_Window(Window_Infomation* Window_Infomation_ptr, HWND Parent_Window_HWND)
{
	//移到最前，提高性能
	while (Window_Infomation_ptr->Get_Last_Window_ptr() != nullptr)
	{
		Window_Infomation_ptr = Window_Infomation_ptr->Get_Last_Window_ptr();
	}

	Window_And_HWND Param = { Window_Infomation_ptr,Parent_Window_HWND };
	EnumChildWindows(Parent_Window_HWND, Enum_Child_Widow, (LPARAM)&Param);
}

BOOL CALLBACK Enum_Child_Widow(HWND Window_HWND, LPARAM lparam)
{
	if (GetAncestor(Window_HWND, GA_PARENT) == ((Window_And_HWND*)lparam)->Window_HWND) //子一代窗口记录
	{
		//加入window队列
		Window_Infomation* Now_Window_ptr = ((Window_And_HWND*)lparam)->Window_Infomation_ptr;

		//查重 移到最前
		while (Now_Window_ptr->Get_Last_Window_ptr() != nullptr)
		{
			Now_Window_ptr = Now_Window_ptr->Get_Last_Window_ptr();
		}
		//遍历到最后
		while (Now_Window_ptr->Get_Next_Window_ptr() != nullptr)
		{
			if (Now_Window_ptr->Get_Window_HWND() == Window_HWND)
			{
				return TRUE;//直接返回
			}
			Now_Window_ptr = Now_Window_ptr->Get_Next_Window_ptr();
		}
		//最后一个是遍历不上的
		if (Now_Window_ptr->Get_Window_HWND() == Window_HWND)
		{
			return TRUE;//直接返回
		}
		//没有重复的->创建连接

		Window_Infomation* Temp_Window_ptr = new Window_Infomation(Window_HWND);//隐含Update_HWND和Update_Infomation

		if (Ignore_Other_Window && !Temp_Window_ptr->Is_ffplay_Window())
		{
			//可选是否仅限ffplay窗口
			delete Temp_Window_ptr;
			Temp_Window_ptr = nullptr;
			return TRUE;
		}

		//连接
		Now_Window_ptr->Set_Next_Window_ptr(Temp_Window_ptr);
		Temp_Window_ptr->Set_Last_Window_ptr(Now_Window_ptr);
		Temp_Window_ptr = nullptr;
	}

	//枚举下一个
	return TRUE;
}

LPCTSTR szAppName = TEXT("守护进程");
LPCTSTR szWndName = TEXT("where is it?");
HMENU hMenu = nullptr;
HINSTANCE hInstance = 0;
constexpr char no_Window_Text[] = "未找到窗口";
constexpr char have_Window_Text[] = "已找到窗口";
constexpr unsigned long long ID_AUTO = 40001;
constexpr unsigned long long ID_HPAUSE = 40002;
constexpr unsigned long long ID_PAUSED = 40003;
constexpr unsigned long long ID_SEARCH = 40004;
constexpr unsigned long long ID_CHANGE = 40005;
constexpr unsigned long long ID_EXIT = 40006;
void tray()
{
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	hInstance = GetModuleHandle(NULL);
	int iCmdShow = SW_SHOW;

	HWND handle = FindWindow(NULL, szWndName);
	if (handle != NULL)
	{
		//MessageBox(NULL, TEXT("Application is already running"), szAppName, MB_ICONERROR);
		MessageBox(NULL, TEXT("已有一个守护线程"), szAppName, MB_ICONERROR);
		return;
	}

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		//MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		MessageBox(NULL, TEXT("注册失败"), szAppName, MB_ICONERROR);
		return;
	}

	// 此处使用WS_EX_TOOLWINDOW 属性来隐藏显示在任务栏上的窗口程序按钮
	hwnd = CreateWindowEx(WS_EX_TOOLWINDOW,
		szAppName, szWndName,
		WS_POPUP,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (running)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
				DispatchMessage(&msg);
		}

		loop();
		Sleep(100);
	}
	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	NOTIFYICONDATA nid;
	UINT WM_TASKBARCREATED;

	// 不要修改TaskbarCreated，这是系统任务栏自定义的消息
	WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));
	switch (message)
	{
	case WM_CREATE:
		nid.cbSize = sizeof(nid);
		nid.hWnd = hwnd;
		nid.uID = 0;
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.uCallbackMessage = WM_USER;
		//nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		lstrcpy(nid.szTip, szAppName);

		hMenu = CreatePopupMenu();
		AppendMenuA(hMenu, auto_Pause ? MF_CHECKED : MF_UNCHECKED, ID_AUTO, "自动暂停");
		AppendMenuA(hMenu, hPause ? MF_CHECKED : MF_UNCHECKED, ID_HPAUSE, "强制暂停");
		AppendMenuA(hMenu, paused ? MF_CHECKED : MF_UNCHECKED, ID_PAUSED, "已暂停");
		AppendMenuA(hMenu, MF_SEPARATOR, NULL, NULL); //水平线
		AppendMenuA(hMenu, MF_STRING, ID_SEARCH, now_Window->Get_Window_HWND() == 0 ? no_Window_Text : have_Window_Text);
		AppendMenuA(hMenu, MF_STRING, ID_CHANGE, "切换暂停");
		AppendMenuA(hMenu, MF_STRING, ID_EXIT, "退出");

		Shell_NotifyIcon(NIM_ADD, &nid);
		break;
	case WM_USER:
		switch (lParam)
		{
		case WM_LBUTTONDOWN:
		{
			POINT pt;
			GetCursorPos(&pt);

			SetForegroundWindow(hwnd);

			update_Menu(hwnd);

			int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hwnd, NULL);
			switch (cmd)
			{
			case ID_AUTO:
			{
				auto_Pause = !auto_Pause;
				break;
			}
			case ID_HPAUSE:
			{
				hPause = !hPause;
				pause_All_Window();
				break;
			}
			case ID_PAUSED:
			{
				if (!hPause)
				{
					paused = !paused;
				}
				break;
			}
			case ID_SEARCH:
			{
				Get_Child_Window(now_Window); //读取全部窗口

				//退回最前
				while (now_Window->Get_Last_Window_ptr() != nullptr) now_Window = now_Window->Get_Last_Window_ptr();
				if (now_Window->Get_Window_HWND() == 0 && now_Window->Get_Next_Window_ptr() != nullptr)
				{
					//删除0
					now_Window = now_Window->Get_Next_Window_ptr();
					delete now_Window->Get_Last_Window_ptr();
					now_Window->Set_Last_Window_ptr(nullptr);
				}

				break;
			}
			case ID_CHANGE:
			{
				if (hPause) paused = !paused;
				pause_All_Window();
				break;
			}
			case ID_EXIT:
			{
				running = false;
				break;
			}
			}
			break;
		}
		case WM_RBUTTONDOWN:
		{
			//std::cout << "right pause\n";
			pause_All_Window();
			break;
		}
		}
		break;
	case WM_DESTROY:
		Shell_NotifyIcon(NIM_DELETE, &nid);
		PostQuitMessage(0);
		break;
	default:
		/*
		 * 防止当Explorer.exe 崩溃以后，程序在系统系统托盘中的图标就消失
		 *
		 * 原理：Explorer.exe 重新载入后会重建系统任务栏。当系统任务栏建立的时候会向系统内所有
		 * 注册接收TaskbarCreated 消息的顶级窗口发送一条消息，我们只需要捕捉这个消息，并重建系
		 * 统托盘的图标即可。
		 */
		if (message == WM_TASKBARCREATED)
			SendMessage(hwnd, WM_CREATE, wParam, lParam);
		break;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void update_Menu(HWND hwnd)
{
	MENUITEMINFO info;
	info.cbSize = sizeof(info);
	info.fMask = MIIM_STATE;
	info.fState = auto_Pause ? MFS_CHECKED : MFS_UNCHECKED;
	SetMenuItemInfo(hMenu, ID_AUTO, FALSE, &info);

	info.fState = hPause ? MFS_CHECKED : MFS_UNCHECKED;
	SetMenuItemInfo(hMenu, ID_HPAUSE, FALSE, &info);

	info.fState = paused ? MFS_CHECKED : MFS_UNCHECKED;
	SetMenuItemInfo(hMenu, ID_PAUSED, FALSE, &info);

	wchar_t buffer[11] = L"";
	info.fMask = MIIM_STRING;
	setlocale(LC_ALL, "chs");
	mbstowcs_s(NULL, buffer, now_Window->Get_Window_HWND() == 0 ? no_Window_Text : have_Window_Text, sizeof(buffer) / sizeof(wchar_t));
	info.dwTypeData = buffer;
	SetMenuItemInfo(hMenu, ID_SEARCH, FALSE, &info);

	DrawMenuBar(hwnd);
}

void loop()
{
	if (auto_Pause && time(nullptr) > next_Time)
	{
		next_Time = time(nullptr) + (update_Time - 1u);
		//std::cout << "time:" << time(nullptr) <<endl
		//	<< "next_Time:" << next_Time << endl;

		update_All_Window();

		HWND active_HWND = GetForegroundWindow();
		RECT active_Rect;
		unsigned int mul = 0;
		if (active_HWND != Window_Infomation::Get_W12_Window_HWND(1) && active_HWND != Window_Infomation::Get_W12_Window_HWND(2))
		{
			GetWindowRect(active_HWND, &active_Rect);
			mul = (active_Rect.right - active_Rect.left) * (active_Rect.bottom - active_Rect.top);
		}
		if (mul > target_Screen_Pixels != paused)
			pause_All_Window(); //占比大和暂停不相同时调用
		//else
		//	std::cout << "now " << (paused ? "paused" : "play") << endl;
	}
}
