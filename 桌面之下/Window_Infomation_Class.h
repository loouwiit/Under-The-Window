#pragma once

#include <Windows.h>
#include <iostream>

class Window_Infomation
{
public:
	Window_Infomation();//构造函数
	Window_Infomation(HWND Window_HWND);
	Window_Infomation(Window_Infomation&) = delete;//拷贝构造函数
	Window_Infomation(Window_Infomation&&) = delete;//移动构造函数
	~Window_Infomation();//析构函数

	bool Is_Top_Most();
	void Set_Top_Most(bool Flag);

	bool Is_ffplay_Window();
	void Set_ffplay_Window(bool Flag);
	void Update_Is_ffplay_Window();
	//bool Is_Pause();
	//void Set_Pause(bool Flag);
	void Pause_ffplay();

	void Update_Infomation();
	RECT& Get_Infomation();

	void Set_Window_HWND(HWND Window_HWND);
	HWND Get_Window_HWND();
	void Update_Window_HWND();
	void Change_Window_HWND(char Flag, int Number);
	HWND Get_Parent_Window_HWND();

	bool Is_Undered();
	void Move(int X_Position, int Y_Position, int Width, int Hight, bool Delta_Mode = false);

	void Set_Window_Under_Or_Up();
	void Set_Window_Under();
	void Set_Window_Up();
	void Repair_W1_W2();

	void Set_Next_Window_ptr(Window_Infomation* Next_Window_Infomation_ptr);
	void Set_Last_Window_ptr(Window_Infomation* Last_Window_Infomation_ptr);
	Window_Infomation* Get_Next_Window_ptr();
	Window_Infomation* Get_Last_Window_ptr();

	std::istream& operator<<(std::istream& Stream);//在对象里写重载不用写第二个参数
	std::ostream& operator>>(std::ostream& Stream);

	static bool Is_Self_Top_Most();
	static void Set_Self_Top_Most(bool Flag);
	static void Set_Self_Up();

	static int Get_Window_Number();
	static HWND Get_PM_Window_HWND();
	static HWND Get_W12_Window_HWND(int Number);
	static HWND Get_Self_Window_HWND();
	static void Find_Self_Window_HWND();
	static void Set_Self_Window_Exist(bool Flag);
	static bool Get_Self_Window_Exist();

	static void Get_Desktop_Size(int& x, int& y);
	static bool Get_W1_W2_Error();
	static void Reset_W1_W2_Error();
	static HWND Get_And_Hide_PM_Window_HWND();
	static void Find_And_Hide_PM_W2_Window();
	static void Hide_PM_W2_Window();

private:
	static BOOL CALLBACK Hide_PM_W2_Window_Callback(_In_ HWND hwnd, _In_ LPARAM Lparam);

	static int All_Window_Number;//总的数量
	static int DeskTop_Size[2];//桌面大小
	static HWND PM_Window_HWND;//PM窗口句柄
	static HWND W1_Window_HWND;//W1窗口句柄
	static HWND W2_Window_HWND;//W2窗口句柄
	static bool W1_W2_Error;//W1、W2是否错误
	static HWND Self_Window_HWND;//自己的窗口句柄
	static bool Self_Window_Top_Most;//自己的置顶状态
	static bool Self_Window_Exist;//自己窗口是否存在

	bool Window_Undered = false;//窗口的桌面之下情况
	bool Window_Top_Most = false;//窗口是否是顶层窗口
	bool Window_Is_ffplay = false;//窗口是否是ffplay的壁纸
	//bool Window_Is_Pause = false;//ffplay是否暂停
	HWND Window_HWND = NULL;//窗口句柄
	HWND Window_Parent_HWND = NULL;//窗口父窗口句柄
	RECT Window_Now_RECT = RECT{};//窗口现在的大小
	RECT Window_Oreginal_RECT = RECT{};//窗口原始大小

	Window_Infomation* Next_Window_ptr = NULL;//下一个节点
	Window_Infomation* Last_Window_ptr = NULL;//上一个节点
};