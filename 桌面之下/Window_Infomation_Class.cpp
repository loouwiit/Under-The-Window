#include "Window_Infomation_Class.h"

HWND Window_Infomation::PM_Window_HWND = NULL;
HWND Window_Infomation::W1_Window_HWND = NULL;
HWND Window_Infomation::W2_Window_HWND = NULL;
bool Window_Infomation::W1_W2_Error = false;
HWND Window_Infomation::Self_Window_HWND = NULL;
bool Window_Infomation::Self_Window_Top_Most = false;
bool Window_Infomation::Self_Window_Exist = false;
int Window_Infomation::DeskTop_Size[2] = { 0,0 };
int Window_Infomation::All_Window_Number = 0;


/// <summary>
/// 构造函数
/// </summary>
Window_Infomation::Window_Infomation()
{
	if (!IsWindow(Window_Infomation::PM_Window_HWND))
	{
		//获取PM窗口的句柄
		Window_Infomation::PM_Window_HWND = Get_And_Hide_PM_Window_HWND();
	}
	if (!IsWindow(Self_Window_HWND)) Window_Infomation::Find_Self_Window_HWND();
	if (this->DeskTop_Size[0] == 0 || this->DeskTop_Size[1] == 0)
	{
		this->DeskTop_Size[0] = GetSystemMetrics(SM_CXSCREEN);
		this->DeskTop_Size[1] = GetSystemMetrics(SM_CYSCREEN);
	}

	this->Last_Window_ptr = nullptr;
	this->Next_Window_ptr = nullptr;
	Window_Infomation::All_Window_Number++;

	//this->Window_Is_Pause = false;
	Update_Is_ffplay_Window();
}

/// <summary>
/// 构造函数
/// </summary>
/// <param name="Window_HWND">窗口句柄</param>
Window_Infomation::Window_Infomation(HWND Window_HWND)
{
	if (!IsWindow(Window_Infomation::PM_Window_HWND))
	{
		//获取PM窗口的句柄
		Window_Infomation::PM_Window_HWND = Get_And_Hide_PM_Window_HWND();
	}
	if (!IsWindow(Self_Window_HWND)) Window_Infomation::Find_Self_Window_HWND();
	if (this->DeskTop_Size[0] == 0 || this->DeskTop_Size[1] == 0)
	{
		this->DeskTop_Size[0] = GetSystemMetrics(SM_CXSCREEN);
		this->DeskTop_Size[1] = GetSystemMetrics(SM_CYSCREEN);
	}

	this->Last_Window_ptr = nullptr;
	this->Next_Window_ptr = nullptr;
	Window_Infomation::All_Window_Number++;
	this->Window_HWND = Window_HWND;

	this->Update_Window_HWND();
}

/// <summary>
/// 析构函数
/// </summary>
Window_Infomation::~Window_Infomation()
{
	Window_Infomation::All_Window_Number--;
}

/// <summary>
/// 获取顶层窗口属性
/// </summary>
/// <returns>是否为顶层窗口</returns>
 bool Window_Infomation::Is_Top_Most()
{
	return this->Window_Top_Most;
}

/// <summary>
/// 设置顶层窗口属性
/// </summary>
/// <param name="Flag">是否设置为顶层窗口</param>
 void Window_Infomation::Set_Top_Most(bool Flag)
{
	if (Flag)
		SetWindowPos(this->Window_HWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	else
		SetWindowPos(this->Window_HWND, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	
	if (this->Window_HWND == this->Self_Window_HWND)
		Window_Infomation::Set_Self_Top_Most(Flag);//自己有专用的函数
	else
		SetWindowPos(this->Self_Window_HWND, this->Self_Window_Top_Most ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	this->Window_Top_Most = Flag;
}

 /// <summary>
 /// 获取是否是ffplay的动态壁纸
 /// </summary>
 /// <returns>是否是ffplay的动态壁纸</returns>
 bool Window_Infomation::Is_ffplay_Window()
 {
	 return this->Window_Is_ffplay;
 }

 /// <summary>
 /// 设置是否是ffplay的动态壁纸
 /// </summary>
 /// <param name="Flag"是否是ffplay的动态壁纸></param>
 void Window_Infomation::Set_ffplay_Window(bool Flag)
 {
	 this->Window_Is_ffplay = Flag;
 }

 /// <summary>
 /// 自动判断是否是ffplay的动态壁纸
 /// </summary>
 void Window_Infomation::Update_Is_ffplay_Window()
 {
	 if (IsWindow(this->Window_HWND))
	 {
		 wchar_t Window_Class_Name[8] = L"";//SDL_app
		 wchar_t Window_Name[10] = L"";//Wallpaper
		 GetClassName(this->Window_HWND, Window_Class_Name, 8);
		 GetWindowText(this->Window_HWND, Window_Name, 10);
		 if (wcscmp(Window_Class_Name, L"SDL_app") == 0 && wcscmp(Window_Name, L"Wallpaper") == 0)
			 this->Window_Is_ffplay = true;
		 else
			 this->Window_Is_ffplay = false;
	 }
	 else
	 {
		 this->Window_Is_ffplay = false;
	 }
 }

 ///// <summary>
 ///// 获取是否已经暂停
 ///// 若非ffplay窗口将返回false
 ///// </summary>
 ///// <returns>是否暂停</returns>
 //bool Window_Infomation::Is_Pause()
 //{
	// return Window_Is_ffplay && Window_Is_Pause;
 //}

 ///// <summary>
 ///// 设置是否已经暂停
 ///// </summary>
 ///// <param name="Flag">是否暂停</param>
 //void Window_Infomation::Set_Pause(bool Flag)
 //{
	// Window_Is_Pause = Flag;
 //}

 /// <summary>
 /// 暂停ffplay
 /// 发送空格键消息
 /// </summary>
 void Window_Infomation::Pause_ffplay()
 {
	 if (!Window_Is_ffplay) return;

	 PostMessageA(Window_HWND, WM_KEYDOWN, VK_SPACE, 0x00390001);
	 PostMessageA(Window_HWND, WM_CHAR, VK_SPACE, 0x00390001);
	 PostMessageA(Window_HWND, WM_KEYUP, VK_SPACE, 0xC0390001);
 }

/// <summary>
/// 更新窗口信息
/// </summary>
void Window_Infomation::Update_Infomation()
{
	//可能由Update_Window调用
	if (!IsWindow(this->Window_HWND)) return;//NULL的情况直接返回
	GetWindowRect(this->Window_HWND, &this->Window_Now_RECT);

	this->Window_Now_RECT.right -= this->Window_Now_RECT.left;
	this->Window_Now_RECT.bottom -= this->Window_Now_RECT.top;

	//对父窗口进行偏移
	if (this->Window_Parent_HWND != NULL)
	{
		RECT Parent_RECT{};
		GetWindowRect(this->Window_Parent_HWND, &Parent_RECT);

		this->Window_Now_RECT.top -= Parent_RECT.top; this->Window_Now_RECT.bottom -= Parent_RECT.top;
		this->Window_Now_RECT.left -= Parent_RECT.left; this->Window_Now_RECT.right -= Parent_RECT.right;
	}

	//更新置顶情况
	this->Window_Top_Most = GetWindowLong(this->Window_HWND, GWL_EXSTYLE) & WS_EX_TOPMOST;
	if (this->Window_HWND == this->Self_Window_HWND) this->Self_Window_Top_Most = this->Window_Top_Most;

	//更新桌面之下情况
	this->Window_Undered = GetAncestor(this->Window_HWND, GA_PARENT) == Window_Infomation::Get_PM_Window_HWND() || GetAncestor(this->Window_HWND, GA_PARENT) == Window_Infomation::W1_Window_HWND;//PM窗口下或者W1窗口下
}

RECT& Window_Infomation::Get_Infomation()
{
	return this->Window_Now_RECT;
}

/// <summary>
/// 设置窗口句柄
/// 同时自动设置父窗口
/// </summary>
/// <param name="Window_HWND">窗口句柄</param>
void Window_Infomation::Set_Window_HWND(HWND Window_HWND)
{
	if (this->Window_Undered)
		this->Set_Window_Up();

	this->Window_HWND = Window_HWND;
	this->Update_Window_HWND();
}

/// <summary>
/// 获取窗口句柄
/// </summary>
/// <returns>窗口句柄</returns>
HWND Window_Infomation::Get_Window_HWND()
{
	return this->Window_HWND;
}

/// <summary>
/// 更新句柄
/// 包括父窗口、位置信息的更新
/// </summary>
void Window_Infomation::Update_Window_HWND()
{
	if (GetAncestor(this->Window_HWND, GA_ROOT) != this->Window_HWND)
	{
		//根不是自己
		if (GetAncestor(this->Window_HWND, GA_PARENT) == this->Get_PM_Window_HWND())//属于PM窗口
			this->Window_Parent_HWND = NULL;//没爹
		else if (GetAncestor(this->Window_HWND, GA_PARENT) == Window_Infomation::W1_Window_HWND)//属于W1窗口
			Window_Infomation::W1_W2_Error = true;//W1、W2错误
		else
			this->Window_Parent_HWND = GetAncestor(this->Window_HWND, GA_PARENT);//有爹
	}
	else
	{
		//根是自己->无父窗口
		this->Window_Parent_HWND = NULL;
	}
	this->Update_Infomation();
}

/// <summary>
/// 更改句柄
/// </summary>
/// <param name="Flag">运算符：+-*/</param>
/// <param name="Number">数字</param>
void Window_Infomation::Change_Window_HWND(char Flag, int Number)
{
	if (this->Window_Undered)
		Set_Window_Up();

	switch (Flag)
	{
	case '+':
	{
		long long Buffer = (long long)this->Window_HWND;
		Buffer += Number;
		Window_HWND = (HWND)Buffer;
		Update_Window_HWND();
		break;
	}
	case '-':
	{
		long long Buffer = (long long)this->Window_HWND;
		Buffer -= Number;
		Window_HWND = (HWND)Buffer;
		Update_Window_HWND();
		break;
	}
	case '*':
	{
		long long Buffer = (long long)this->Window_HWND;
		Buffer *= Number;
		Window_HWND = (HWND)Buffer;
		Update_Window_HWND();
		break;
	}
	case '/':
	{
		long long Buffer = (long long)this->Window_HWND;
		Buffer /= Number;
		Window_HWND = (HWND)Buffer;
		Update_Window_HWND();
		break;
	}
	
	default:
		break;
	}
}

/// <summary>
/// 获取父窗口句柄
/// </summary>
/// <returns>父窗口句柄</returns>
HWND Window_Infomation::Get_Parent_Window_HWND()
{
	return this->Window_Parent_HWND;
}

/// <summary>
/// 获取是否在桌面之下
/// </summary>
/// <returns>是否在桌面之下</returns>
bool Window_Infomation::Is_Undered()
{
	return this->Window_Undered;
}

/// <summary>
/// 移动窗口
/// </summary>
/// <param name="X_Position">X位置</param>
/// <param name="Y_Position">Y位置</param>
/// <param name="Width">宽度</param>
/// <param name="Hight">长度</param>
/// <param name="Delta_Mode">是否是相对运动</param>
void Window_Infomation::Move(int X_Position, int Y_Position, int Width, int Hight,bool Delta_Mode)
{
	if (!IsWindow(this->Window_HWND)) return;
	if (Delta_Mode)
	{
		SetWindowPos(this->Window_HWND, NULL, this->Window_Now_RECT.left + X_Position, Window_Now_RECT.top + Y_Position, Window_Now_RECT.right + Width, Window_Now_RECT.bottom + Hight, NULL);
	}
	else
	{
		//绝对坐标
		SetWindowPos(this->Window_HWND, NULL, X_Position, Y_Position, Width, Hight, NULL);
	}
//	SetWindowPos(this->Self_Window_HWND, this->Self_Window_Top_Most ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	this->Update_Infomation();//不会影响父窗口
}

/// <summary>
/// 自动判断移动窗口到桌面之下还是桌面之上
/// </summary>
void Window_Infomation::Set_Window_Under_Or_Up()
{
	if (this->Window_Undered)
		Set_Window_Up();
	else
		Set_Window_Under();
	this->Update_Infomation();
}

/// <summary>
/// 移动窗口到桌面之下
/// </summary>
void Window_Infomation::Set_Window_Under()
{
	if (this->Window_HWND != NULL && Window_HWND != this->Self_Window_HWND)
	{
		GetWindowRect(Window_HWND, &this->Window_Oreginal_RECT);//保存信息
		this->Window_Oreginal_RECT.right -= this->Window_Oreginal_RECT.left;
		this->Window_Oreginal_RECT.bottom -= this->Window_Oreginal_RECT.top;//转换info

		if (this->Window_Parent_HWND != NULL)
		{
			//有父窗口
			RECT Parent_Rect;
			GetWindowRect(this->Window_Parent_HWND, &Parent_Rect);

			this->Window_Oreginal_RECT.left -= Parent_Rect.left;
			this->Window_Oreginal_RECT.right -= Parent_Rect.left;
			this->Window_Oreginal_RECT.top -= Parent_Rect.top;
			this->Window_Oreginal_RECT.bottom -= Parent_Rect.top;
		}
		SetParent(this->Window_HWND, Window_Infomation::Get_PM_Window_HWND());//将视频窗口设置为PM的子窗口 使用函数将自动修复PM窗口恢复
		SetWindowPos(Window_HWND, NULL, 0, 0, this->DeskTop_Size[0], this->DeskTop_Size[1], NULL);//全屏
		this->Window_Undered = true;
	}
	return;
}

/// <summary>
/// 移动窗口到桌面之上
/// </summary>
void Window_Infomation::Set_Window_Up()
{
	SetParent(this->Window_HWND, Window_Parent_HWND);//NULL是桌面
	if (this->Window_Oreginal_RECT.right == 0 || this->Window_Oreginal_RECT.bottom == 0)
	{
		//异常情况 没有数据，需要手动调整大小
		this->Window_Oreginal_RECT.top = (int)(DeskTop_Size[0] * 0.25);
		this->Window_Oreginal_RECT.left = (int)(DeskTop_Size[1] * 0.25);
		this->Window_Oreginal_RECT.right = (int)(DeskTop_Size[0] * 0.5);
		this->Window_Oreginal_RECT.bottom = (int)(DeskTop_Size[1] * 0.5);
	}
	SetWindowPos(this->Window_HWND, NULL, this->Window_Oreginal_RECT.left, this->Window_Oreginal_RECT.top, Window_Oreginal_RECT.right, this->Window_Oreginal_RECT.bottom, NULL);//还原位置
	this->Window_Undered = false;

	return;
}

/// <summary>
/// 修复Win+Tab后的W1、W2异常
/// </summary>
void Window_Infomation::Repair_W1_W2()
{
	if (this->Is_Undered() && this->Get_Window_HWND() != Self_Window_HWND)//IsWindow交给SetParent()
		SetParent(this->Window_HWND, Window_Infomation::Get_PM_Window_HWND());//PM的子窗口
}

/// <summary>
/// 设置下一个指针
/// </summary>
/// <param name="Next_Window_Infomation_ptr">下一个指针</param>
void Window_Infomation::Set_Next_Window_ptr(Window_Infomation* Next_Window_Infomation_ptr)
{
	this->Next_Window_ptr = Next_Window_Infomation_ptr;
}

/// <summary>
/// 设置上一个指针
/// </summary>
/// <param name="Last_Window_Infomation_ptr">上一个指针</param>
void Window_Infomation::Set_Last_Window_ptr(Window_Infomation* Last_Window_Infomation_ptr)
{
	this->Last_Window_ptr = Last_Window_Infomation_ptr;
}

/// <summary>
/// 获取下一个指针
/// </summary>
/// <returns>下一个指针</returns>
Window_Infomation* Window_Infomation::Get_Next_Window_ptr()
{
	return this->Next_Window_ptr;
}

/// <summary>
/// 获取上一个指针
/// </summary>
/// <returns>上一个指针</returns>
Window_Infomation* Window_Infomation::Get_Last_Window_ptr()
{
	return this->Last_Window_ptr;
}

/// <summary>
/// 获取对象——句柄、父句柄、是否在桌面之下、是否是顶层窗口、XY、长宽
/// </summary>
/// <param name="Stream">输入的流对象</param>
/// <returns></returns>
std::istream& Window_Infomation::operator<<(std::istream& Stream)
{
	long long Input = 0;
	Stream >> Input;
	this->Window_HWND = (HWND)Input;
	Stream >> Input;
	this->Window_Parent_HWND = (HWND)Input;
	Stream >> this->Window_Oreginal_RECT.top >> this->Window_Oreginal_RECT.left
		>> this->Window_Oreginal_RECT.right >> this->Window_Oreginal_RECT.bottom;

	this->Update_Window_HWND();
	return Stream;
}

/// <summary>
/// 输出对象——句柄、父句柄、是否在桌面之下、是否是顶层窗口、XY、长宽
/// </summary>
/// <param name="Stream">输出的流对象</param>
/// <returns></returns>
std::ostream& Window_Infomation::operator>>(std::ostream& Stream)
{
	Stream << (long long)(this->Window_HWND) << ' '
		<< (long long)(this->Window_Parent_HWND) << ' '
		<< this->Window_Oreginal_RECT.top << ' ' << this->Window_Oreginal_RECT.left << ' '
		<< this->Window_Oreginal_RECT.right << ' ' << this->Window_Oreginal_RECT.bottom << ' ';
	return Stream;
}

/// <summary>
/// 获取自身是否是置顶
/// </summary>
/// <returns>是否置顶</returns>
bool Window_Infomation::Is_Self_Top_Most()
{
	return Window_Infomation::Self_Window_Top_Most;
}

/// <summary>
/// 设置自身置顶
/// </summary>
/// <param name="Flag">是否置顶</param>
void Window_Infomation::Set_Self_Top_Most(bool Flag)
{
	if (Flag)
		SetWindowPos(Window_Infomation::Self_Window_HWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	else
		SetWindowPos(Window_Infomation::Self_Window_HWND, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	Window_Infomation::Self_Window_Top_Most = Flag;
}

/// <summary>
/// 设置自身前台
/// </summary>
void Window_Infomation::Set_Self_Up()
{
	SetWindowPos(Window_Infomation::Self_Window_HWND, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

/// <summary>
/// 获取窗口总数
/// </summary>
/// <returns>总数</returns>
int Window_Infomation::Get_Window_Number()
{
	return Window_Infomation::All_Window_Number;
}

/// <summary>
/// 获取自身的句柄
/// </summary>
/// <returns>窗口句柄</returns>
HWND Window_Infomation::Get_Self_Window_HWND()
{
	return Window_Infomation::Self_Window_HWND;
}

void Window_Infomation::Find_Self_Window_HWND()
{
	if (Self_Window_Exist)
	{
		//获取自己的句柄
		Self_Window_HWND = FindWindow(L"SFML_Window", L"桌面之下");//获取自己的句柄

		if (!IsWindow(Self_Window_HWND))
			Self_Window_HWND = FindWindow(L"SFML_Window", NULL);//名称不对，再次寻找
	}
}

/// <summary>
/// 获取PM窗口句柄
/// </summary>
/// <returns>PM窗口句柄</returns>
HWND Window_Infomation::Get_PM_Window_HWND()
{
	if (!IsWindow(PM_Window_HWND))
	{
		//获取PM窗口的句柄
		Window_Infomation::PM_Window_HWND = Get_And_Hide_PM_Window_HWND();
	}
	return Window_Infomation::PM_Window_HWND;
}

HWND Window_Infomation::Get_W12_Window_HWND(int Number)
{
	return Number == 2 ? W2_Window_HWND : W1_Window_HWND;
}
/// <summary>
/// 设置自己窗口存在情况
/// </summary>
/// <param name="Flag">自己窗口是否存在</param>
void Window_Infomation::Set_Self_Window_Exist(bool Flag)
{
	Self_Window_Exist = Flag;
}

/// <summary>
/// 获取自己窗口存在情况
/// </summary>
/// <returns>自己窗口是否存在</returns>
bool Window_Infomation::Get_Self_Window_Exist()
{
	return Self_Window_Exist;
}

/// <summary>
/// 获取W1、W2错误状况
/// </summary>
/// <returns>W1、W2是否错误</returns>
bool Window_Infomation::Get_W1_W2_Error()
{
	return Window_Infomation::W1_W2_Error;
}

/// <summary>
/// 重置W1、W2错误
/// </summary>
void Window_Infomation::Reset_W1_W2_Error()
{
	Window_Infomation::W1_W2_Error = false;
}

/// <summary>
/// 获取PM窗口的句柄，并隐藏PM_W2窗口
/// </summary>
/// <returns>PM窗口的句柄</returns>
HWND Window_Infomation::Get_And_Hide_PM_Window_HWND()
{
	HWND PM_Window_HWND = FindWindow(L"Progman", 0);//找到PM窗口
	SendMessageTimeout(PM_Window_HWND, 0x52C, 0, 0, 0, 100, 0);//给它发特殊消息
	Window_Infomation::Find_And_Hide_PM_W2_Window();//找到第二个WorkerW窗口并隐藏它
	return PM_Window_HWND;
}

void Window_Infomation::Find_And_Hide_PM_W2_Window()
{
	EnumWindows(Window_Infomation::Hide_PM_W2_Window_Callback, (LPARAM)nullptr);
}

void Window_Infomation::Hide_PM_W2_Window()
{
	if (!IsWindow(W2_Window_HWND))
		Find_And_Hide_PM_W2_Window();
	else	
		ShowWindow(W2_Window_HWND, SW_HIDE);
}

/// <summary>
/// 隐藏PM_W2窗口的回调函数
/// </summary>
BOOL CALLBACK Window_Infomation::Hide_PM_W2_Window_Callback(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);//在hwnd下查找名为“桌面图标”的窗口
	if (hDefView != 0)//找到桌面图标，我（hwnd）一定是W1
	{
		// 找它的下一个窗口，类名为WorkerW，隐藏它
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);//在桌面（0）下寻找 W1（hwnd）之后 名为“W”窗口（W2）
		ShowWindow(hWorkerw, SW_HIDE);//隐藏W2
		Window_Infomation::W1_Window_HWND = hwnd;//保存W1窗口
		Window_Infomation::W2_Window_HWND = hWorkerw;//保存W2窗口
		return FALSE;
	}
	return TRUE;
}
