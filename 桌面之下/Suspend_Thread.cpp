/*

#include "Variable.h"

constexpr int Sleep_Time = 5000;
int Order_Window_Size = 0;
unsigned Order_Percent = 80;

std::thread* ffplay_Thread = nullptr;//ffplay线程
bool ffplay_Pause_Thread_Enabled = false;//ffplay线程是否启动
std::vector<HWND> ffplay_Windows{};//给ffplay_thread线程使用

bool IsForegroundFullscreen();//判断是否超过阈值

void ffplay_Pause_Thread(std::vector<HWND>& ffplay_Windows)
{
	std::cout << "动态壁纸挂起线程已启动" << endl;
	std::vector<HWND>::iterator Now_ffplay_Window = ffplay_Windows.begin();
	bool Pause_ffplay = false;
	ffplay_Pause_Thread_Enabled = true;

	if (Order_Window_Size == 0) Order_Window_Size = DeskTop_Size[0] * DeskTop_Size[1] * Order_Percent / 100;

	while (ffplay_Pause_Thread_Enabled)
	{
		//判断窗口是否存在
		Now_ffplay_Window = ffplay_Windows.begin();
		while (Now_ffplay_Window != ffplay_Windows.end())
		{
			if (!IsWindow(*Now_ffplay_Window))
			{
				ffplay_Windows.erase(Now_ffplay_Window);
				Now_ffplay_Window = ffplay_Windows.begin();
				continue;
			}
			Now_ffplay_Window++;
		}
		//空的话就不用判断了
		if (ffplay_Windows.empty())
		{
			ffplay_Pause_Thread_Enabled = false;
			continue;
		}

		//查看是否需要挂起线程
		if (IsForegroundFullscreen())
		{
			if (!Pause_ffplay)
			{
				Pause_ffplay = true;
				std::cout << "挂起动态壁纸" << endl;
				for (Now_ffplay_Window = ffplay_Windows.begin(); Now_ffplay_Window != ffplay_Windows.end(); Now_ffplay_Window++)
					SuspendThread(*Now_ffplay_Window);//挂起每一项
			}
		}
		else
		{
			if (Pause_ffplay)
			{
				Pause_ffplay = false;
				std::cout << "恢复动态壁纸" << endl;
				for (Now_ffplay_Window = ffplay_Windows.begin(); Now_ffplay_Window != ffplay_Windows.end(); Now_ffplay_Window++)
					ResumeThread(*Now_ffplay_Window);//恢复每一项
			}
		}
		Sleep(Sleep_Time);
	}
	std::cout << "动态壁纸挂起线程退出" << endl;
}

bool IsForegroundFullscreen()
{
	HWND User_Now_Window_HWND;
	RECT User_Now_Window_RECT;

	User_Now_Window_HWND = GetForegroundWindow();//获取当前正在与用户交互的前台窗口句柄

	if ((User_Now_Window_HWND != GetDesktopWindow()) && (User_Now_Window_HWND != GetShellWindow()))//如果前台窗口不是桌面窗口，也不是控制台窗口
	{
		GetWindowRect(User_Now_Window_HWND, &User_Now_Window_RECT);//获取前台窗口的坐标
		if ((User_Now_Window_RECT.right - User_Now_Window_RECT.left) * (User_Now_Window_RECT.bottom - User_Now_Window_RECT.top)
			> Order_Window_Size)
		{
			//占据面积超过阈值
			return true;
		}
	}//如果前台窗口是桌面窗口，或者是控制台窗口，就直接返回不是全屏

	return false;
}

*/