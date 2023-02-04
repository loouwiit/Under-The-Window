#include <iostream>
#include <fstream>
#include <Windows.h>
#include "Window_Infomation_Class.h"
#include "resource.h"
#include "massage.h"
//#include <SFML/Graphics.hpp>
//#include "Operator.h"

/*
* 守护进程
* 当用户看不到壁纸时暂停ffplay窗口
*/

/*
* 0.0.0.1 初号版本
* 0.0.0.2 添加关闭功能 右键功能同左键 中键功能为切换 修改暂停机制，暂停需要超过81%持续5秒，恢复需要2秒
* 0.0.0.3 添加白名单和黑名单 （遗忘添加版本号） 2023/1/14
* 0.0.0.4 修复隐藏控制台后不恢复的bug 2023/1/14
* 0.0.0.5 修复保存白名单时错误的问题 修复读取错误数值后死循环的bug 2023/2/1
* 0.0.0.6 更正窗口标题 添加shouconsole参数 字符串匹配将不匹配大小写 2023/2/2
* 0.0.0.7 修复部分情况下字符串匹配死循环的bug 修复大小写匹配错误的bug 接受部分消息（来自桌面之下的ME_SEARCH） 守护进程匹配更加准确 替换中键为强制暂停 2023/2/3
* 0.0.0.8 接受ME_CHANGED消息，调用强制暂停 2023/2/4
* 0.0.0.9 把W1、W2窗口设定为白名单，不会错误识别桌面为大窗口 修复显示窗口模式下直接退出的bug 2023/2/4
* 
* next
*/

#define	Message(STRING) MessageBox(NULL, STRING, L"守护进程", MB_YESNO)

struct Window_And_HWND
{
	Window_Infomation* Window_Infomation_ptr;
	HWND Window_HWND;
};

constexpr char Progream_Version[] = "0.0.0.9";
constexpr unsigned File_Version = 1;
constexpr char List_Path[] = ".\\桌面之下\\list.txt";
constexpr unsigned int update_Time = 1;
constexpr unsigned int continue_Time[2] = { 2,5 }; //恢复和暂停
constexpr char endl = '\n';
constexpr bool Ignore_Other_Window = true;
constexpr float Target_Screen_Rate = 0.81f;
#ifdef _DEBUG
bool Defalut_Show_Console = true;
#else
bool Defalut_Show_Console = false;
#endif

//sf::RenderWindow window;
//sf::Font font;

Window_Infomation* now_Window = nullptr;
time_t next_Time = 0;
HWND Console_HWND = 0;
bool Show_Console = Defalut_Show_Console;
char null_List_char[] = "null";
char** black_List = nullptr;
char** white_List = nullptr;
unsigned black_List_Number = 0;
unsigned white_List_Number = 0;
unsigned target_Screen_Pixels = 0;
bool running = true;
bool auto_Pause = true;
bool hPause = false;
bool paused = false;

void Initialize();
void Enetialize();
void Save_List(const char Path[]);
void Load_List(const char Path[]);

void search_All_Window();
void update_All_Window();
void pause_All_Window();
void close_All_Window();
void Repair_W1_W2_Error(Window_Infomation* Window_Infomation_ptr);
void Get_Child_Window(Window_Infomation* Window_Infomation_ptr, HWND Parent_Window_WHND = Window_Infomation::Get_PM_Window_HWND());//枚举并连接PM的子窗口
BOOL CALLBACK Enum_Child_Widow(HWND Window_HWND, LPARAM lparam);//枚举的回调函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//子菜单的回调函数
bool string_Compere(const char STR1[], const char STR2[]);
bool text_In(const char string[], char** string_List, unsigned list_Count);
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

	int desktop_Size[2] = { 0,0 };
	Window_Infomation::Get_Desktop_Size(desktop_Size[0], desktop_Size[1]);
	target_Screen_Pixels = (unsigned int)(desktop_Size[0] * desktop_Size[1] * Target_Screen_Rate);

	Load_List(List_Path);//读取配置文件
	Console_HWND = FindWindow(L"ConsoleWindowClass", NULL);//获取句柄
	if (Console_HWND != NULL && !Show_Console) ShowWindow(Console_HWND, SW_HIDE);//隐藏窗口

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
	if (Console_HWND != NULL && !Show_Console) ShowWindow(Console_HWND, SW_SHOW);//恢复窗口
	//if (paused) pause_All_Window();
	Save_List(List_Path);
	if (Show_Console) system("pause");
}

void Save_List(const char Path[])
{
	std::ofstream file;
	file.open(Path);
	if (!file.is_open())
	{
		system("md 桌面之下");
		file.open(Path);

		if (!file.is_open())
		{
			Message(L"写入文件失败");
			return;
		}
	}

	file << "version=" << File_Version << ';' << endl;
	for (unsigned i = 0; i < white_List_Number; i++)
		file << "whiteList=" << white_List[i] << ';' << endl;
	for (unsigned i = 0; i < black_List_Number; i++)
		file << "blackList=" << black_List[i] << ';' << endl;
	if (Show_Console) file << "showConsole=true;" << endl;
	file << "end" << endl;
}

void Load_List(const char Path[])
{
	std::ifstream file;
	char buffer[500] = "";
	std::streampos position = 0;

	file.open(Path);
	if (!file.is_open()) return;

	file.getline(buffer, sizeof(buffer), '=');

	if (string_Compere(buffer, "version"))
	{
		file.getline(buffer, ';');
		if (atoi(buffer) != File_Version)
		{
			//版本异常
			file.close();
			if (Message(L"文件版本异常，是否重新写入？") == IDYES)
			{
				Save_List(Path);
			}
			return;
		}
	}
	else
	{
		//文件异常
		file.close();
		if (Message(L"文件异常，是否重新写入？") == IDYES)
		{
			Save_List(Path);
		}
		return;
	}

	position = file.tellg();
	while (true)
	{
		file.getline(buffer, sizeof(buffer), '=');

		if (string_Compere(buffer, "whiteList")) white_List_Number++;
		else if (string_Compere(buffer, "blackList")) black_List_Number++;
		else if (string_Compere(buffer, "end")) break;
		else if (file.eof())
		{
			file.clear();
			break;
		}

		file.ignore(0xFFFF, ';');
	}

	white_List = new char* [white_List_Number];
	black_List = new char* [black_List_Number];
	for (unsigned i = 0; i < white_List_Number; i++) white_List[i] = null_List_char; //可以不用，但是留着吧，增强鲁棒性
	for (unsigned i = 0; i < black_List_Number; i++) black_List[i] = null_List_char;
	unsigned now_white = 0;
	unsigned now_black = 0;

	file.seekg(position);
	while (true)
	{
		file.getline(buffer, sizeof(buffer), '=');

		if (string_Compere(buffer, "whiteList"))
		{
			file.getline(buffer, sizeof(buffer), ';');

			size_t size = strlen(buffer) + 1;
			white_List[now_white] = new char[size]; //忽略此处的C6386和C6385，缓冲区大小是经过统计的，不会溢出
			strcpy_s(white_List[now_white], size, buffer);
			now_white++;
		}

		else if (string_Compere(buffer, "blackList"))
		{
			file.getline(buffer, sizeof(buffer), ';');

			size_t size = strlen(buffer) + 1;
			black_List[now_black] = new char[size]; //忽略此处的C6386和C6385，缓冲区大小是经过统计的，不会溢出
			strcpy_s(black_List[now_black], size, buffer);
			now_black++;
		}

		else if (string_Compere(buffer, "showConsole"))
		{
			file.getline(buffer, sizeof(buffer), ';');

			if (atoi(buffer) != 0) Show_Console = true; //非零数字改为true，其他数据不更改
			if (string_Compere(buffer,"0")) Show_Console = false; //"0"改为false
			if (string_Compere(buffer, "true")) Show_Console = true;
			if (string_Compere(buffer, "false")) Show_Console = false;
			std::clog << "显示控制台:" << (Show_Console ? "是" : "否") << endl;
		}

		else if (string_Compere(buffer, "end"))
		{
			break;
		}

		else //未知数据
		{
			std::clog << "未知数据:" << buffer << endl;

			//文件结束
			if (file.eof())
				break;

			//错误数值
			Message(L"存在未知数值，请检查配置文件");
			file.ignore(0xFFFF, ';');
		}
	}

	file.close();

	std::clog << "白名单数量:" << white_List_Number << endl;
	for (unsigned i = 0; i < white_List_Number; i++) std::clog << i + 1 << ':' << white_List[i] << endl;
	std::clog << "黑名单数量:" << black_List_Number << endl;
	for (unsigned i = 0; i < black_List_Number; i++) std::clog << i + 1 << ':' << black_List[i] << endl;

	return;
}


void search_All_Window()
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
}

void update_All_Window()
{
	//退回最前
	static Window_Infomation* temp;
	temp = now_Window;
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
		temp->Send_Message(VK_SPACE, 0x00390001);
	}
}

void close_All_Window()
{
	while (now_Window->Get_Last_Window_ptr() != nullptr)
		now_Window = now_Window->Get_Last_Window_ptr();
	for (Window_Infomation* temp = now_Window; temp != nullptr; temp = temp->Get_Next_Window_ptr())
	{
		temp->Send_Message(VK_ESCAPE, 0x00010001);
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
LPCTSTR szWndName = TEXT("守护进程");
HMENU hMenu = nullptr;
HINSTANCE hInstance = 0;
constexpr char no_Window_Text[] = "未找到窗口";
constexpr char have_Window_Text[] = "已找到窗口";

constexpr unsigned long long ID_AUTO = 40001;
constexpr unsigned long long ID_HPAUSE = 40002;
constexpr unsigned long long ID_PAUSED = 40003;
constexpr unsigned long long ID_SEARCH = 40004;
constexpr unsigned long long ID_CHANGE = 40005;
constexpr unsigned long long ID_CLOSE = 40006;
constexpr unsigned long long ID_EXIT = 40007;

void tray()
{
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;

	hInstance = GetModuleHandle(NULL);
	int iCmdShow = SW_SHOW;

	HWND handle = FindWindow(szAppName, szWndName);
	if (handle != NULL)
	{
		//MessageBox(NULL, TEXT("Application is already running"), szAppName, MB_ICONERROR);
		MessageBox(NULL, TEXT("已有一个守护进程"), szAppName, MB_ICONERROR);
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
	NOTIFYICONDATA nid = { NULL };
	UINT WM_TASKBARCREATED;

	// 不要修改TaskbarCreated，这是系统任务栏自定义的消息
	WM_TASKBARCREATED = RegisterWindowMessage(TEXT("TaskbarCreated"));
	switch (message)
	{
	case WM_CREATE:
	{
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
		AppendMenuA(hMenu, MF_STRING, ID_CLOSE, "关闭壁纸");
		AppendMenuA(hMenu, MF_STRING, ID_EXIT, "退出");

		Shell_NotifyIcon(NIM_ADD, &nid);
		break;
	}
	case WM_USER:
	{
		switch (lParam)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
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
				search_All_Window();
				break;
			}
			case ID_CHANGE:
			{
				if (hPause) paused = !paused;
				pause_All_Window();
				break;
			}
			case ID_CLOSE:
			{
				close_All_Window();
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
		case WM_MBUTTONDOWN:
		{
			hPause = !hPause;
			pause_All_Window();
			break;
		}
		}
		break;
	}
	case WM_DESTROY:
	{
		Shell_NotifyIcon(NIM_DELETE, &nid);
		PostQuitMessage(0);
		break;
	}
	case ME_SEARCH:
	{
		search_All_Window();
		break;
	}
	case ME_CHANGE:
	{
		//1.0.4.1
		hPause = !hPause;
		pause_All_Window();
		break;
	}
	default:
	{
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
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

bool string_Compere(const char STR1[], const char STR2[])
{
	unsigned i = 0;
	unsigned j = 0;

	while (STR1[i] == ' ' || STR1[i] == '\n') i++;
	while (STR2[j] == ' ' || STR2[j] == '\n') j++;

	while (STR1[i] != '\0' && STR2[j] != '\0')
	{
		if (STR1[i] != STR2[j] && (STR1[i] - STR2[j] != 32) && (STR2[j] - STR1[i] != 32)) return false; //不相等 且 不是大小写
		i++;
		j++;
		while (STR1[i] == ' ' || STR1[i] == '\n') i++;//排除空格
		while (STR2[j] == ' ' || STR2[j] == '\n') j++;
	}

	if (STR1[i] == '\0')
	{
		while (STR2[j] != '\0')
		{
			if (STR2[j] == ' ' || STR2[j] == '\n') j++;
			else return false;//后边仍有非空字符
		}
	}

	if (STR2[j] == '\0')
	{
		while (STR1[i] != '\0')
		{
			if (STR1[i] == ' ' || STR1[i] == '\n') i++;
			else return false;//后边仍有非空字符
		}
	}

	return true;
}

bool text_In(const char string[], char** string_List, unsigned list_Count)
{
	for (unsigned i = 0; i < list_Count; i++)
	{
		if (0 == strcmp(string, string_List[i]))
			return true;
	}
	return false;
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

		static HWND active_HWND;
		static RECT active_Rect;
		static unsigned int mul = 0;
		static unsigned char continues = 0;
		static bool target_State = 0;
		static char buffer[100] = "";

		active_HWND = GetForegroundWindow();
		GetWindowTextA(active_HWND, buffer, sizeof(buffer));
		
		if (text_In(buffer, black_List, black_List_Number))
		{
			//黑名单
			mul = target_Screen_Pixels;
		}
		else if (text_In(buffer, white_List, white_List_Number) ||
			active_HWND == Window_Infomation::Get_W12_Window_HWND(1) ||
			active_HWND == Window_Infomation::Get_W12_Window_HWND(2))
		{
			//白名单
			mul = 0;
		}
		else
		{
			//剩下的
			GetWindowRect(active_HWND, &active_Rect);
			mul = (active_Rect.right - active_Rect.left) * (active_Rect.bottom - active_Rect.top);
		}

		target_State = mul >= target_Screen_Pixels;

		if (target_State != paused) continues++; //要变
		else continues = 0; //相同置零

		if (continues > continue_Time[target_State])
			pause_All_Window(); //达到预计时间
		//else
		//	std::cout << "now " << (paused ? "paused" : "play") << endl;
	}
}
