#include <fstream>
#include "Variable.h"

//#define SPACE " "

/*
* 此文件用于处理窗口之间的逻辑
* 包括GUI所调用的函数
*/

Window_Infomation* Now_Window = new Window_Infomation{};//当前窗口
//HWND PM_Window_HWND = NULL;
//HWND Now_Window->Get_Window_HWND() = (HWND)-1;
//HWND Parent_HWND;//父窗口句柄
//RECT Under_Window_Info = { 0,0,0,0 };
unsigned DeskTop_Size[2] = { 0,0 };
bool File_Exist = false;
//bool Undered = false;//是否在桌面之下
//bool Top_Most = false;//是否窗口置顶

struct Window_And_HWND
{
	Window_Infomation* Window_Infomation_ptr;
	HWND Window_HWND;
};

void Get_Other_Setting(std::ifstream& File, const bool Disabled_Warning);//获取其他设置
BOOL CALLBACK Enum_Child_Widow(HWND Window_HWND, LPARAM lparam);//枚举的回调函数

void Reset_Setting()
{
	File_Exist = false;
//	Undered = false; 1.0.2.5后交给对象处理
//	strcpy_s(FFplay_Path , "");
	strcpy_s(Font_Name, Font_Name_Lenght, "msyh.ttc");
	for (int i = 0; i < 4; i++)
		strcpy_s(Quick_Video_Path[i], (size_t)Video_Path_Lenght, "");

//	Get_PM_Window_HWND(PM_Window_HWND); 1.0.2.5后交给对象自动处理
}

void Set_File_Setting(const char Setting_File_Path[])
{
	std::ofstream File;
	File.open(Setting_File_Path);
	if (!File.is_open())
	{
		//失败
		system("md 桌面之下");
		File.open(Setting_File_Path);
		if (!File.is_open())
		{
			//仍然失败
			Message(L"写入文件失败");
			return;
		}
		else
		{
			wchar_t* Message_W_String = new wchar_t[(int)strlen(Setting_File_Path) + 10];//strlen不计算'\0'

			wchar_t* Setting_File_Path_W = new wchar_t[(int)strlen(Setting_File_Path) + 1];//还有'\0'
			CharToTchar(Setting_File_Path, Setting_File_Path_W, CP_ACP);

			swprintf_s(Message_W_String, strlen(Setting_File_Path) + (size_t)10, L"已创建文件\n路径:%ws", Setting_File_Path_W);
			Message(Message_W_String);

			delete[] Message_W_String; Message_W_String = nullptr;
			delete[] Setting_File_Path_W; Setting_File_Path_W = nullptr;
		}
	}
	//开始写入
	File << File_Version << endl;//版本
	File << FFplay_Path << endl;//FFplay路径
	File << FFmpeg_Path << endl;//FFmpeg路径

	for (int i = 0; i < 4; i++)
		File << Quick_Video_Path[i] << endl;//写入快速播放

	//保留桌面之下 1.0.2.5后永久保持
	//if (Keep_Undered)
	//{
	//	File << (long long)Now_Window->Get_Window_HWND() << SPACE << Under_Window_Info.left << SPACE << Under_Window_Info.top << SPACE << Under_Window_Info.right << SPACE << Under_Window_Info.bottom;
	//	File << SPACE << (long long)Parent_HWND << endl;//父窗口
	//}
	//else
	//{
	//	File << NULL <<endl;
	//}

	//写入窗口信息
	File << Window_Infomation::Get_Window_Number() << endl;//数量
	Window_Infomation* Out_Window = Now_Window;
	//回到最前
	while (Out_Window->Get_Last_Window_ptr() != nullptr)
	{
		Out_Window = Out_Window->Get_Last_Window_ptr();
	}
	//写到最后
	while (Out_Window->Get_Next_Window_ptr() != nullptr)
	{
		*Out_Window >> File;
		File << endl;
		Out_Window = Out_Window->Get_Next_Window_ptr();
	}
	//最后一个
	{
		*Out_Window >> File;
		File << endl;
	}

	if (Show_Console) File << "Show_Console" << endl;//显示控制台
	if (strcmp(Font_Name, "msyh.ttc") != 0) File << "Font_Name" << endl << Font_Name << endl;//写入字体名称
	for (int Number = 0; Number < 4; Number++)
		if (Quick_Video_Decoder[Number][0] != '\0') File << "Video_" << Number + 1 << "_Decoder" << endl << Quick_Video_Decoder[Number] << endl;//写入解码器
}

void Get_File_Setting(const char Setting_File_Path[], const bool Disabled_Warning)
{
	std::ifstream File;
	int Input_File_Version;
	char Input[100] = "";
	File.open(Setting_File_Path);
	if (!File.is_open())
	{
		//失败
		File_Exist = false;
		return;
	}

	//存在setting.txt
	File_Exist = true;

	//版本
	File >> Input;
	Input_File_Version = atoi(Input);
	if (Input_File_Version != File_Version)
	{
		//文件版本错误
		if (!Disabled_Warning)
		{
			if (Message(L"文件版本错误\n是否重写写入?") == IDYES)
			{
				Set_File_Setting(Setting_File_Path);
			}
			else
			{
				File_Exist = false;
			}
		}
		else
		{
			//Disabled
			std::cout << "文件版本错误" << endl;
			File_Exist = false;
		}
		return;
	}//文件版本错误
	File.ignore();

	//ffplay
	File.getline(FFplay_Path, 100);
	File.getline(FFmpeg_Path, 100);

	//快速播放
	for (int i = 0; i < 4; i++)
		File.getline(Quick_Video_Path[i], Video_Path_Lenght);

	////保留桌面之下
	//File >> Input;
	//Now_Window->Get_Window_HWND() = (HWND)atoll(Input);
	//if (Now_Window->Get_Window_HWND() != NULL)//存在保留
	//{
	//	File >> Under_Window_Info.left >> Under_Window_Info.top >> Under_Window_Info.right >> Under_Window_Info.bottom;
	//	File >> Input;
	//	Parent_HWND = (HWND)atoll(Input);//父窗口
	//	Keep_Undered = true;
	//	std::cout << PM_Window_HWND;
	//	Undered = GetAncestor(Now_Window->Get_Window_HWND(), GA_ROOT) == PM_Window_HWND ? true : false;//在PM窗口之下：桌面之下
	//}

	//获取窗口信息
	File >> Input;
	int Number = (int)atoll(Input);
	Window_Infomation* Tmp_Window = nullptr;

	while (Now_Window->Get_Window_HWND() == NULL && Number > 0)
	{
		*Now_Window << File;
		//Now_Window->Is_ffplay_Window();
		if (!IsWindow(Now_Window->Get_Window_HWND()))
		{
			Number--;
			Now_Window->Set_Window_HWND(NULL);
		}
	}
	for (int i = 1; i < Number; i++)
	{
		if (Tmp_Window == nullptr) Tmp_Window = new Window_Infomation;
		*Tmp_Window << File;
		if (IsWindow(Tmp_Window->Get_Window_HWND()))
		{
			Tmp_Window->Set_Last_Window_ptr(Now_Window);
			Now_Window->Set_Next_Window_ptr(Tmp_Window);
			Now_Window = Tmp_Window;
			Tmp_Window = nullptr;
			//Now_Window->Is_ffplay_Window();
		}
	}
	if (Tmp_Window != nullptr)
	{
		//删除最后一个多余的窗口
		delete Tmp_Window;
		Tmp_Window = nullptr;
	}

	//倒回第一个
	while (Now_Window->Get_Last_Window_ptr() != NULL)
	{
		Now_Window = Now_Window->Get_Last_Window_ptr();
	}

	File.ignore();//忽略换行

	//其他设置
	Get_Other_Setting(File, Disabled_Warning);
}

/* 1.0.2.5 删除
void Set_Window_Under(HWND Window_HWND)
{
	if (Window_HWND != NULL && Window_HWND != Self_HWND)
	{
		GetWindowRect(Window_HWND, &Under_Window_Info);//保存信息
		Under_Window_Info.right -= Under_Window_Info.left; Under_Window_Info.bottom -= Under_Window_Info.top;
		if (Parent_HWND != NULL)
		{
			//有父窗口
			RECT Parent_Rect;
			GetWindowRect(Parent_HWND, &Parent_Rect);
			Under_Window_Info = Under_Window_Info - &Parent_Rect;//已重载偏移减法
		}
		SetParent(Window_HWND, PM_Window_HWND);// 将视频窗口设置为PM的子窗口
		SetWindowPos(Window_HWND, NULL, 0, 0, DeskTop_Size[0], DeskTop_Size[1], NULL);//全屏
		Undered = true;
	}
	return;
}
*/

/* 1.0.2.5删除
void Set_Window_Up(HWND Window_HWND, HWND Window_Parent_HWND, RECT Window_Info)
{
	SetParent(Window_HWND, Window_Parent_HWND);//NULL是桌面
	SetWindowPos(Window_HWND, NULL, Window_Info.left, Window_Info.top, Window_Info.right , Window_Info.bottom, NULL);//还原位置
	Undered = false;

	return;
}
*/

//1.0.2.5 删除
//BOOL CALLBACK Hide_PM_W2(_In_ HWND hwnd, _In_ LPARAM Lparam)
//{
//	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
//	if (hDefView != 0) {
//		// 找它的下一个窗口，类名为WorkerW，隐藏它
//		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
//		ShowWindow(hWorkerw, SW_HIDE);
//
//		return FALSE;
//	}
//	return TRUE;
//}

void Creat_Vidoe_Texture(int Number)
{
	//wchar_t Text[150];
	//wchar_t Name[50];
	//CharToTchar(Quick_Video_Name[Number], Name, CP_ACP);
	//swprintf_s(Text, L"缺少缩略图\n缺少./桌面之下/%ws.png", Name);
	//Message(Text);
	if (FFmpeg_Path[0] != '\0')
	{
		//char Text[300] = "";
		//sprintf_s(Text, "start %s -ss 00:00:10 -i %s -f image2 -y ./桌面之下/%s.png", FFmpeg_Path, Quick_Video_Path[Number], Quick_Video_Name[Number]);
		//system(Text);

		//STARTUPINFO si{ 0 };
		STARTUPINFOA si{ 0 };
		PROCESS_INFORMATION pi{ 0 };

		//wchar_t FFmpeg_Path_W[Video_Path_Lenght] = L"";
		//wchar_t Quick_Video_Path_W[Video_Path_Lenght] = L"";
		//wchar_t Quick_Video_Name_W[Video_Path_Lenght] = L"";

		//wchar_t Param[Video_Path_Lenght * 2 + 100] = L"";
		char Param[Video_Path_Lenght * 2 + 100] = "";

		//CharToTchar(FFmpeg_Path, FFmpeg_Path_W, CP_ACP);//字符转换
		//CharToTchar(Quick_Video_Path[Number], Quick_Video_Path_W, CP_ACP);
		//CharToTchar(Quick_Video_Name[Number], Quick_Video_Name_W, CP_ACP);

		//设置参数
		//swprintf_s(Param, L" -ss 00:00:00.1 -i \"%ws\" -loglevel quiet -f image2 -y \"./桌面之下/%ws.png\"",  Quick_Video_Path_W, Quick_Video_Name_W);
		sprintf_s(Param, " -ss 00:00:00.1 -i \"%s\" -loglevel quiet -f image2 -y \"./桌面之下/%s.png\"",  Quick_Video_Path[Number], Quick_Video_Name[Number]);

		CreateProcessA((LPCSTR)FFmpeg_Path, (LPSTR)Param, 0, 0, 0, 0, 0, 0, &si, &pi); //C6335 句柄泄露
		CloseHandle(pi.hProcess);//关闭句柄 https://learn.microsoft.com/zh-cn/cpp/code-quality/c6335?view=msvc-170
		CloseHandle(pi.hThread);

		if (Show_Console) std::cout << "创建" << Quick_Video_Path[Number] << "的预览图：" << Quick_Video_Name[Number] << ".png" << endl
			<< "参数：" << Param << endl;
	}
	else
	{
		if (Show_Console) std::cout << "未设置ffmpege 创建" << Quick_Video_Path[Number] << "失败" << endl;
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
	for (Window_Infomation_ptr = Now_Window; Window_Infomation_ptr != nullptr;
		Window_Infomation_ptr = Window_Infomation_ptr->Get_Last_Window_ptr())
	{
		//自己及左边
		Window_Infomation_ptr->Repair_W1_W2();
	}
	for (Window_Infomation_ptr = Now_Window->Get_Next_Window_ptr(); Window_Infomation_ptr != nullptr;
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

	if (GetAncestor(Window_HWND, GA_PARENT) == ((Window_And_HWND*)lparam)->Window_HWND)
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

		//连接
		Now_Window_ptr->Set_Next_Window_ptr(Temp_Window_ptr);
		Temp_Window_ptr->Set_Last_Window_ptr(Now_Window_ptr);
		Temp_Window_ptr = nullptr;
	}

	//枚举下一个
	return TRUE;
}

void Record()
{
	/*
	$date = get-date -Format 'yyyy-MM-dd'
	$time = get-date -Format 'yyyy-MM-dd hh-mm-ss'
	$time += '.mp4'
	$path = '.\' + $date + '\' + $time

	mkdir $date
	ffmpeg -f gdigrab -framerate 30 -i desktop -f dshow -i audio="立体声混音 (Realtek(R) Audio)" -vcodec h264_amf -b:v 2m  $path
	*/

	/*
	STARTUPINFOA si{ 0 };
	PROCESS_INFORMATION pi{ 0 };

	char Param[FFplay_Path_Lenght + Recorder_Lenght + 100] = "";

	//设置参数
	sprintf_s(Param, " -f gdigrab -framerate 30 -i desktop -f dshow -i audio=\"%s\" -vcodec h264_amf -b:v 2m  $path", Audio_Recorder,Encoder);
	CreateProcessA((LPCSTR)FFmpeg_Path, (LPSTR)Param, 0, 0, 0, 0, 0, 0, &si, &pi); //C6335 句柄泄露
	*/
}

void Play_Video(const char Video_Path_Param[], const char Video_Decoder_Param[])
{
	constexpr int Param_Lengh = Video_Decoder_Lenght + 8 + Video_Path_Lenght + 150;
	char Param[Param_Lengh] = "";
	//char Video_Path_W[Param_Lengh] = "";
	//char FFplay_Path_W[Param_Lengh] = "";
	char Video_Decoder[Video_Decoder_Lenght + 8] = "";
	//STARTUPINFO si{ 0 };
	STARTUPINFOA si{ 0 };
	PROCESS_INFORMATION pi{ 0 };
	HWND ffplay = NULL;
	HWND ffplay_Console = NULL;

	//创建新的窗口储存位置
	while (Now_Window->Get_Next_Window_ptr() != nullptr)
	{
		Now_Window = Now_Window->Get_Next_Window_ptr();
	}
	if (Now_Window->Get_Window_HWND() != NULL)
	{
		//创建下一个
		Now_Window->Set_Next_Window_ptr(new Window_Infomation);
		Now_Window->Get_Next_Window_ptr()->Set_Last_Window_ptr(Now_Window);
		Now_Window = Now_Window->Get_Next_Window_ptr();
	}

	//CharToTchar(Video_Path_Param, Video_Path_W, CP_ACP);
	//CharToTchar(FFplay_Path, FFplay_Path_W, CP_ACP);
	if (Video_Decoder_Param[0] != '\0')
	{
		//char Buffer[Video_Decoder_Lenght] = "";
		//CharToTchar(Video_Decoder_Param, Buffer, CP_ACP);
		sprintf_s(Video_Decoder, (size_t)Video_Decoder_Lenght + 8, "-vcodec %s", Video_Decoder_Param);
	}

	sprintf_s(Param, Param_Lengh, " \"%s\" %s -noborder -x %i -y %i -loop 0 %s -an -framedrop -window_title Wallpaper -exitonmousedown", Video_Path_Param, Video_Decoder, DeskTop_Size[0], DeskTop_Size[1], Show_Console ? "" : "-loglevel quiet");
	//-exitonkeydown 1.0.3.16删除

	if (Show_Console) std::cout << "播放视频，调用信息:" << Param << endl;//给出参数

	if (CreateProcessA(FFplay_Path, Param, 0, 0, 0, CREATE_NEW_CONSOLE | IDLE_PRIORITY_CLASS, 0, 0, &si, &pi))//新控制台，低优先级 //C6335 句柄泄露
	{
		CloseHandle(pi.hProcess);//关闭句柄 https://learn.microsoft.com/zh-cn/cpp/code-quality/c6335?view=msvc-170
		CloseHandle(pi.hThread);
		long Max_Time = clock() + 10000;//10秒
		do
		{
			Now_Window->Set_Window_HWND(FindWindow(L"SDL_app", L"Wallpaper"));//找到视频窗口
			Sleep(250);
		} while (Now_Window->Get_Window_HWND() == NULL && (long)clock() < Max_Time);//NULL且未超时的情况下去寻找
		if (Now_Window->Get_Window_HWND() != NULL)
		{

			Now_Window->Set_Window_Under();
			Now_Window->Set_ffplay_Window(true); //1.0.3.17

			//1.0.3.18 尝试隐藏任务栏图标
			//{
			//	DWORD dwExStyle = GetWindowLong(Now_Window->Get_Window_HWND(), GWL_STYLE);
			//	//dwExStyle &= ~(WS_VISIBLE);
			//	dwExStyle |= WS_EX_TOOLWINDOW;
			//	dwExStyle &= ~(WS_EX_APPWINDOW);
			//	SetWindowLong(Now_Window->Get_Window_HWND(), GWL_STYLE, dwExStyle);
			//	//ShowWindow(Now_Window->Get_Window_HWND(), SW_SHOW);
			//	//ShowWindow(Now_Window->Get_Window_HWND(), SW_HIDE);
			//	//UpdateWindow(Now_Window->Get_Window_HWND());
			//	printf_s("%d", GetLastError()); //1400
			//}

			if (!Show_Console)
			{
				//获取并隐藏ffplay
				//char ffplay_Full_Path[FFplay_Path_Lenght] = "";
				//wchar_t ffplay_Path_W[FFplay_Path_Lenght] = L"";
				//CharToTchar(FFplay_Path, ffplay_Path_W, CP_ACP);
				//GetFullPathNameA(FFplay_Path, FFplay_Path_Lenght, ffplay_Full_Path, NULL);
				//不需要全地址 1.0.3.12
				ffplay_Console = FindWindowA("ConsoleWindowClass", FFplay_Path);
				if (ffplay_Console != NULL)	ShowWindow(ffplay_Console, SW_HIDE);
			}

			/*
			//加入挂起线程
			ffplay_Windows.push_back(Now_Window->Get_Window_HWND());
			if (!ffplay_Pause_Thread_Enabled)
			{
				//启动线程
				if (ffplay_Thread != nullptr) delete ffplay_Thread;
				ffplay_Thread = new std::thread(ffplay_Pause_Thread, std::ref(ffplay_Windows));
			}
			*/
			Update_Draw = true;
		}
		else
		{
			Message(L"播放超时");
		}
	}
	else
	{
		Message(L"播放失败");
	}
}

void Move_Window(Window_Infomation* Node, ENUM::Move_Type Flag)
{
	//	Changing_Position = false;//无需显示更改位置
	if (Node->Get_Window_HWND() == NULL) return;
	int Start_X = 0; int Start_Y = 0;
	//RECT Now_Window_Info{};1.0.2.5
	POINT Point = { 0,0 };
	bool Is_Self_Top_Most = Window_Infomation::Is_Self_Top_Most();//自己是否置顶
	bool Is_Window_Top_Most = Node->Is_Top_Most();//被移窗口是否置顶

	GetCursorPos(&Point); Start_X = Point.x; Start_Y = Point.y;//确定起始XY

	//交给对象处理 1.0.2.5
	//GetWindowRect(Node.Get_Window_HWND(), &Now_Window_Info);
	//Now_Window_Info.right = Now_Window_Info.right - Now_Window_Info.left; Now_Window_Info.bottom = Now_Window_Info.bottom - Now_Window_Info.top;//转换长宽
	//if (GetAncestor(Node.Get_Window_HWND(), GA_PARENT) != NULL)
	//{
	//	//有父窗口
	//	RECT Parent_Rect;
	//	GetWindowRect(GetAncestor(Node.Get_Window_HWND(), GA_PARENT), &Parent_Rect);
	//	Now_Window_Info.top -= Parent_Rect.top;
	//	Now_Window_Info.left -= Parent_Rect.left;
	//}
	if (Is_Window_Top_Most) Node->Set_Top_Most(false);//取消被移窗口置顶
	if (!Is_Self_Top_Most) Window_Infomation::Set_Self_Top_Most(true);//置顶自己

	Node->Update_Infomation();//更新状态，防止未及时更新的数据出错
	Update_Draw = true;//渲染更新后的状态
	Print();//因为阻塞所以需要手动调用

	while (GetAsyncKeyState(VK_LBUTTON) && 0x8000)
	{
		Event(false);
		GetCursorPos(&Point);//获取鼠标当前位置
		if (Point.x != Start_X || Point.y != Start_Y)
		{
			Input_Position = ENUM::Input_Position::INP_NULL;//没有输入位置了
			switch (Flag)
			{
			case ENUM::Move_Type::XAndY:
				Node->Move(Point.x - Start_X, Point.y - Start_Y, 0, 0, true);//会自动更新信息
				break;
			case ENUM::Move_Type::WideAndHight:
				Node->Move(0, 0, Point.x - Start_X, Point.y - Start_Y, true);//会自动更新信息
				break;
			default:
				break;
			}
			Start_X = Point.x;
			Start_Y = Point.y;
			Update_Draw = true;//需要重新渲染
			Print();
		}
		Sleep();
	}

	if (Is_Window_Top_Most) Node->Set_Top_Most(true);//恢复被移窗口置顶
	Window_Infomation::Set_Self_Top_Most(Is_Self_Top_Most);//将焦点设置为自己，同时恢复置顶

	Node->Update_Infomation();//更新信息
	Update_Draw = true;//修复移动时的置顶信息错误
}

/*
void Input_Value(Union_Value::Union_Value Value, Union_Value::Union_Value_Flag Flag)
{
	long long Input = 0;
	bool Finish = false;

	if (Undered) Set_Window_Up(Now_Window->Get_Window_HWND());

	while (!Finish)
	{
		if ((GetAsyncKeyState(VK_NUMPAD0) || GetAsyncKeyState('0'))) { HEX_Mode ? Input <<= 4 : Input *= 10; Input += 0; }
		if ((GetAsyncKeyState(VK_NUMPAD1) || GetAsyncKeyState('1'))) { HEX_Mode ? Input <<= 4 : Input *= 10; Input += 1; }
		if ((GetAsyncKeyState(VK_NUMPAD2) || GetAsyncKeyState('2'))) { HEX_Mode ? Input <<= 4 : Input *= 10; Input += 2; }
		if ((GetAsyncKeyState(VK_NUMPAD3) || GetAsyncKeyState('3'))) { HEX_Mode ? Input <<= 4 : Input *= 10; Input += 3; }
		if ((GetAsyncKeyState(VK_NUMPAD4) || GetAsyncKeyState('4'))) { HEX_Mode ? Input <<= 4 : Input *= 10; Input += 4; }
		if ((GetAsyncKeyState(VK_NUMPAD5) || GetAsyncKeyState('5'))) { HEX_Mode ? Input <<= 4 : Input *= 10; Input += 5; }
		if ((GetAsyncKeyState(VK_NUMPAD6) || GetAsyncKeyState('6'))) { HEX_Mode ? Input <<= 4 : Input *= 10; Input += 6; }
		if ((GetAsyncKeyState(VK_NUMPAD7) || GetAsyncKeyState('7'))) { HEX_Mode ? Input <<= 4 : Input *= 10; Input += 7; }
		if ((GetAsyncKeyState(VK_NUMPAD8) || GetAsyncKeyState('8'))) { HEX_Mode ? Input <<= 4 : Input *= 10; Input += 8; }
		if ((GetAsyncKeyState(VK_NUMPAD9) || GetAsyncKeyState('9'))) { HEX_Mode ? Input <<= 4 : Input *= 10; Input += 9; }
		if (GetAsyncKeyState('S')) { HEX_Mode = false; Update_Draw = true; }
		if (GetAsyncKeyState('X')) { HEX_Mode = true; Update_Draw = true; }
		if (GetAsyncKeyState('A')) { HEX_Mode = true; Input <<= 4; Input += 0xA; }
		if (GetAsyncKeyState('B')) { HEX_Mode = true; Input <<= 4; Input += 0xB; }
		if (GetAsyncKeyState('C')) { HEX_Mode = true; Input <<= 4; Input += 0xC; }
		if (GetAsyncKeyState('D')) { HEX_Mode = true; Input <<= 4; Input += 0xD; }
		if (GetAsyncKeyState('E')) { HEX_Mode = true; Input <<= 4; Input += 0xE; }
		if (GetAsyncKeyState('F')) { HEX_Mode = true; Input <<= 4; Input += 0xF; }
		if (GetAsyncKeyState(VK_BACK) || GetAsyncKeyState(VK_DELETE)) { HEX_Mode ? Input >>= 4 : Input /= 10; }
		if (GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_RETURN)) { Finish = true; break; }

		if ((long long)Now_Window->Get_Window_HWND() != Input)
		{
			Now_Window->Get_Window_HWND() = (HWND)Input;
			if (GetWindowRect(Now_Window->Get_Window_HWND(), &Under_Window_Rect))//读取
			{
				//成功该句柄存在
				if (GetAncestor(Now_Window->Get_Window_HWND(), GA_ROOT) != Now_Window->Get_Window_HWND())//不是自己
				{
					//非桌面窗口一定有父窗口
					Parent_HWND = GetAncestor(Now_Window->Get_Window_HWND(), GA_PARENT);//获取父窗口
					if (PM_Window_HWND == NULL) Get_PM_Window_HWND(PM_Window_HWND);//获取PM窗口句柄
					//判断是否是PM窗口
					if (Parent_HWND != PM_Window_HWND)
					{
						RECT Parent_Rect;
						GetWindowRect(Parent_HWND, &Parent_Rect);
						Parent_Rect.right = Parent_Rect.left; Parent_Rect.bottom = Parent_Rect.top;//此时Rect仅为减数
						Under_Window_Rect = Under_Window_Rect - Parent_Rect;//已重载减法
					}
					else
					{
						//PM的子窗口，极有可能是不正常退出导致的
						Parent_HWND = NULL;
					}
				}
				else
				{
					Parent_HWND = NULL;//父窗口是桌面
				}
			}
			else
			{
				//函数失败
				Under_Window_Rect = { 0,0,0,0 };//格式归0
				Parent_HWND = NULL;
			}
			Update_Draw = true;
		}
		Print(true);
		Sleep(100);
	}
	return;
}
*/

void Set_Quick_Video()
{
//	if (FFplay_Path[0] == '\0')
//	{
//		Message(L"请设置ffplay路径");
//	}
//	Message(L"尚未开发\n请到./桌面之下/setting.txt中设置");//1.0.1.12

	Update_Draw = true; Update_Zero_HWND = true;
	switch (Input_Position)
	{
	case ENUM::Input_Position::INP_Video_1:
		Input_Position = ENUM::Input_Position::INP_Video_2;
		break;
	case ENUM::Input_Position::INP_Video_2:
		Input_Position = ENUM::Input_Position::INP_Video_3;
		break;
	case ENUM::Input_Position::INP_Video_3:
		Input_Position = ENUM::Input_Position::INP_Video_4;
		break;
	default:
		Input_Position = ENUM::Input_Position::INP_Video_1;
		break;
	}
	Input_Char_Position = (unsigned)strlen(Quick_Video_Path[2]);
}

void Choise_Window()
{
	//交给对象处理 1.0.2.5
	//HWND Old_Window_HWND = Now_Window->Get_Window_HWND();
	//HWND Old_Under_Window_Parent_HWND = Now_Window->Get_Parent_Window_HWND();
	//RECT Old_Under_Window_Info = Under_Window_Info;

	HWND Return_Parent_HWND = NULL;
	POINT Point = { 0,0 };
	//Parent_HWND = NULL;//这时的窗口一定没有父窗口
	while (GetAsyncKeyState(VK_LBUTTON) && 0x8000)
	{
		Event(false);

		GetCursorPos(&Point);//获取鼠标当前位置
		if (WindowFromPoint(Point) != Now_Window->Get_Window_HWND())
		{
			Now_Window->Set_Window_HWND(WindowFromPoint(Point));//获取鼠标所在窗口的句柄
			//无需判断窗口句柄存在，此窗口一定存在
			Return_Parent_HWND = GetAncestor(Now_Window->Get_Window_HWND(), GA_ROOT);//寻找父窗口
			if (Return_Parent_HWND != NULL) Now_Window->Set_Window_HWND(Return_Parent_HWND);//替换

//			SetWindowPos(Now_Window->Get_Window_HWND(), NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DRAWFRAME);//边框
//			std::cout << "Most" << endl;

			//GetWindowRect(Now_Window->Get_Window_HWND(), &Under_Window_Info);//获取信息 交给对象自动处理1.0.2.5
			//Under_Window_Info.right -= Under_Window_Info.left; Under_Window_Info.bottom -= Under_Window_Info.top;
			Update_Draw = true;
			Update_Zero_HWND = true;//有可能是NULL
			Print();
		}
		Sleep(100);
	}

	//交给对象自动处理 1.0.2.5
	//if (Undered)
	//{
	//	Set_Window_Up(Old_Now_Window->Get_Window_HWND(), Old_Under_Window_Parent_HWND, Old_Under_Window_Info);
	//}


	//一定没有父窗口
	/*
	if (Parent_HWND != NULL)
	{
		//有父窗口
		RECT Parent_Rect;
		GetWindowRect(Parent_HWND, &Parent_Rect);
		Parent_Rect.right = Parent_Rect.left; Parent_Rect.bottom = Parent_Rect.top;//此时Rect仅为减数
		Under_Window_Rect = Under_Window_Rect - Parent_Rect;//已重载减法
	}
	*/
}

/* 1.0.1.12删除
void Set_Focus()
{
	if (Undered)
	{
//		ShowWindow(Self_HWND, SW_MINIMIZE);//最小化自己
		
//		SetActiveWindow(Now_Window->Get_Window_HWND());
//		SetFocus(Now_Window->Get_Window_HWND());//设置焦点

		SetWindowPos(Self_HWND, NULL, DeskTop_Size[0] - 160, DeskTop_Size[1] - 100, 160, 100, NULL);//移到右下角
		Under_or_Up();
		Under_or_Up();
	}
}
*/

//void Set_Most_Window(Window_Infomation Node)//1.0.1.12加入 1.0.2.5删除
//{
//	Node.Set_Top_Most(!Node.Is_Top_Most());
//	Update_Draw = true; Update_Zero_HWND = true;//你不能相信用户
//}

//1.0.2.5 删除
//void Under_or_Up(Window_Infomation Node)
//{
//	Node.Set_Window_Under_Or_Up();
//	return;
//}

//1.0.2.5后强制启动
//void Keep_Under()
//{
//	Keep_Undered = !Keep_Undered;//取否
//	Update_Draw = true;//渲染
//}

void CharToTchar(const char* _char, TCHAR* tchar, UINT CodePage)
{
	int iLength;

	iLength = MultiByteToWideChar(CodePage, 0, _char, (int)strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CodePage, 0, _char, (int)strlen(_char) + 1, tchar, iLength);
}

void TcharToChar(const TCHAR* tchar, char* _char, UINT CodePage)
{
	int iLength;
	//获取字节长度   
	iLength = WideCharToMultiByte(CodePage, 0, tchar, -1, NULL, 0, NULL, NULL);
	//将tchar值赋给_char    
	WideCharToMultiByte(CodePage, 0, tchar, -1, _char, iLength, NULL, NULL);
}

void Get_Other_Setting(std::ifstream& File, const bool Disabled_Warning)
{
	char Input[100] = "";

	while (File.peek() != EOF)
	{
		File.getline(Input, 100);

		//debug显示控制台
		if (strcmp(Input, "Show_Console") == 0 || strcmp(Input, "SC") == 0)
		{
			Show_Console = true;
			if (!Disabled_Warning) std::cout << "显示控制台" << endl;
		}
		else if (strcmp(Input, "Font_Name") == 0 || strcmp(Input, "FN") == 0)
		{
			File.getline(Font_Name, Font_Name_Lenght);
			if (!Disabled_Warning) std::cout << "使用字体：" << Font_Name << endl;
		}
		else if (strcmp(Input, "V1D") == 0 || strcmp(Input, "Video_1_Decoder") == 0)
		{
			File.getline(Quick_Video_Decoder[0], Video_Decoder_Lenght);
			if (!Disabled_Warning) std::cout << "视频1使用解码器" << Quick_Video_Decoder[0] << endl;
		}
		else if (strcmp(Input, "V2D") == 0 || strcmp(Input, "Video_2_Decoder") == 0)
		{
			File.getline(Quick_Video_Decoder[1], Video_Decoder_Lenght);
			if (!Disabled_Warning) std::cout << "视频2使用解码器" << Quick_Video_Decoder[1] << endl;
		}
		else if (strcmp(Input, "V3D") == 0 || strcmp(Input, "Video_3_Decoder") == 0)
		{
			File.getline(Quick_Video_Decoder[2], Video_Decoder_Lenght);
			if (!Disabled_Warning) std::cout << "视频3使用解码器" << Quick_Video_Decoder[2] << endl;
		}
		else if (strcmp(Input, "V4D") == 0 || strcmp(Input, "Video_4_Decoder") == 0)
		{
			File.getline(Quick_Video_Decoder[3], Video_Decoder_Lenght);
			if (!Disabled_Warning) std::cout << "视频4使用解码器" << Quick_Video_Decoder[3] << endl;
		}
	}
}

//1.0.2.5 删除
//void Get_PM_Window_HWND(HWND& Return)
//{
//	Return = FindWindow(L"Progman", 0);// 找到PM窗口
//	SendMessageTimeout(Return, 0x52C, 0, 0, 0, 100, 0);// 给它发特殊消息
//	EnumWindows(Hide_PM_W2, 0);// 找到第二个WorkerW窗口并隐藏它
//}

void Spict_Name(const char Source[], char Name[], const  int Name_Lenght)
{
	int Copy_Position = -1;
	int Copy_Lenght = 0;
	int Source_Lenght = 0;

	Source_Lenght = (int)strlen(Source);
	for (int i = Source_Lenght; i >= 0; i--)
	{
		if (Source[i] == '\\' || Source[i] == '/')
		{
			Copy_Position = i;//寻找最后一个斜杠
			i = -1;//跳出循环
		}
	}
	Copy_Position++;//我们要下一个位置
	Copy_Lenght = Source_Lenght - Copy_Position;

	//长度不足
	if (Name_Lenght - 1 < Copy_Lenght)//留给\0
	{
		for (Copy_Lenght = 0; Copy_Lenght < Name_Lenght - 1; Copy_Lenght++)
		{
			if (!(0 < Source[Copy_Position + Copy_Lenght] && Source[Copy_Position + Copy_Lenght] < 128))
			{
				if (Copy_Lenght < Name_Lenght - 2) Copy_Lenght++;
				else break;
			}
		}

		if (Show_Console) std::cout << "分离" << Source << "的名称时检测到申请空间不足" << endl
			<< "仅截取前" << Copy_Lenght << "个字符" << endl
			<< "如出现缩略图错误请减少路径长度或联系开发员" << endl;
	}

	//拷贝
	for (int i = 0; i < Copy_Lenght; i++)
	{
		Name[i] = Source[Copy_Position + i];
	}
	Name[Copy_Lenght] = '\0';
	return;
}

void Spict_Path(const char Source[], char Path[], const int Path_Lenght)
{
	int Copy_Lenght = 0;
	int Source_Lenght = 0;

	Source_Lenght = (int)strlen(Source);
	for (int i = Source_Lenght; i >= 0; i--)
	{
		if (Source[i] == '\\' || Source[i] == '/')
		{
			Copy_Lenght = i;//寻找最后一个斜杠
			i = -1;//跳出循环
		}
	}

	Copy_Lenght++;//取后一个

	//长度不足
	if (Path_Lenght - 1 < Copy_Lenght)//留给\0
	{
		if (Show_Console) std::cout << "分离" << Source << "的路径时检测到申请空间不足"<<endl
			<<"如出现缩略图错误请减少路径长度或联系开发员" << endl;
		return;
	}

	//拷贝
	for (int i = 0; i < Copy_Lenght; i++)
	{
		Path[i] = Source[i];
	}
	Path[Copy_Lenght] = '\0';

	return;
}
