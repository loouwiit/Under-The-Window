#pragma once

#include <Windows.h>
#include <iostream>

class Window_Infomation
{
public:
	Window_Infomation();//���캯��
	Window_Infomation(HWND Window_HWND);
	Window_Infomation(Window_Infomation&) = delete;//�������캯��
	Window_Infomation(Window_Infomation&&) = delete;//�ƶ����캯��
	~Window_Infomation();//��������

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

	std::istream& operator<<(std::istream& Stream);//�ڶ�����д���ز���д�ڶ�������
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

	static int All_Window_Number;//�ܵ�����
	static int DeskTop_Size[2];//�����С
	static HWND PM_Window_HWND;//PM���ھ��
	static HWND W1_Window_HWND;//W1���ھ��
	static HWND W2_Window_HWND;//W2���ھ��
	static bool W1_W2_Error;//W1��W2�Ƿ����
	static HWND Self_Window_HWND;//�Լ��Ĵ��ھ��
	static bool Self_Window_Top_Most;//�Լ����ö�״̬
	static bool Self_Window_Exist;//�Լ������Ƿ����

	bool Window_Undered = false;//���ڵ�����֮�����
	bool Window_Top_Most = false;//�����Ƿ��Ƕ��㴰��
	bool Window_Is_ffplay = false;//�����Ƿ���ffplay�ı�ֽ
	//bool Window_Is_Pause = false;//ffplay�Ƿ���ͣ
	HWND Window_HWND = NULL;//���ھ��
	HWND Window_Parent_HWND = NULL;//���ڸ����ھ��
	RECT Window_Now_RECT = RECT{};//�������ڵĴ�С
	RECT Window_Oreginal_RECT = RECT{};//����ԭʼ��С

	Window_Infomation* Next_Window_ptr = NULL;//��һ���ڵ�
	Window_Infomation* Last_Window_ptr = NULL;//��һ���ڵ�
};