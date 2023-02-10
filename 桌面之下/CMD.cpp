#include "Variable.h"

constexpr char TAB = '	';

void CMD_Help(const char Command[]);
void CMD_Play(const char Video_Path[], const char Setting_Path[]);
void CMD_Repair();
void CMD_Get(const char Setting_Path[], const char Param_Name[]);
void CMD_Set(const char Setting_Path[], const char Param_Name[], const char Param_Value[]);
void CMD_Under(const char Window_HWND_Char[], const char Setting_Path[]);
void CMD_Up(const char Window_HWND_Char[], const char Setting_Path[]);
void CMD_Pause(const char Setting_Path[]);

bool CMD_Call(const int argc, const char* argv[])
{
	if (argc == 1 || argc == 0) return false;//不知道为啥argc可能为0，然后play就读了0x00000001崩了

	std::cout << "桌面之下命令行调用" << endl;

	if (strcmp(argv[1], "-version") == 0)
	{
		std::cout << "桌面之下 版本:" << Progream_Version << endl;
	}
	else if (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-about") == 0 || strcmp(argv[1], "-?") == 0)
	{
		//Self -Help [command]
		CMD_Help(argc > 2 ? argv[2] : "");
	}
	else if (strcmp(argv[1], "-start") == 0)
	{
		std::cout << "启动桌面之下" << endl;
		//Show_Console = true;
		return false;
	}
	else if (strcmp(argv[1], "-settingfilepath") == 0)
	{
		if (argc > 2)
		{
			Spict_Path(argv[2], Setting_Floor_Path, Setting_File_Lenght);
			Spict_Name(argv[2], Setting_File_Name, Setting_File_Lenght);
			std::cout << "指定设置路径为" << Setting_Floor_Path << Setting_File_Name << endl
				<< "启动桌面之下" << endl;
			return false;
		}
	}
	else if (strcmp(argv[1], "-play") == 0 || argv[1][0] != '-')//直接是视频路径
	{
		//self -play video_path setting_path
		switch (argc)
		{
		case 2:
		{
			//self Video_path
			char Setting_File_Path[Setting_File_Lenght] = "";
			sprintf_s(Setting_File_Path, Setting_File_Lenght, "%s%s", Setting_Floor_Path, Setting_File_Name);
			CMD_Play(argv[1], Setting_File_Path);
		}
		case 3:
		{
			//只有视频路径
			char Setting_File_Path[Setting_File_Lenght] = "";
			sprintf_s(Setting_File_Path, Setting_File_Lenght, "%s%s", Setting_Floor_Path, Setting_File_Name);
			CMD_Play(argv[2], Setting_File_Path);
			break;
		}
		case 4:
		{
			//存在设置路径
			CMD_Play(argv[2], argv[3]);
			break;
		}
		default:
			break;
		}
	}
	else if (strcmp(argv[1], "-repair") == 0)
	{
		//Self -repair
		CMD_Repair();
	}
	else if (strcmp(argv[1], "-get") == 0)
	{
		//self -get param_name setting_path
		switch (argc)
		{
		case 3:
		{
			char Setting_File_Path[Setting_File_Lenght] = "";
			sprintf_s(Setting_File_Path, Setting_File_Lenght, "%s%s", Setting_Floor_Path, Setting_File_Name);
			CMD_Get(Setting_File_Path, argv[2]);
			break;
		}
		case 4:
		{
			CMD_Get(argv[3], argv[2]);
			break;
		}
		default:
			break;
		}
	}
	else if (strcmp(argv[1], "-set") == 0)
	{
		//self -set param_name param_value setting_path
		switch (argc)
		{
		case 4:
		{
			char Setting_File_Path[Setting_File_Lenght] = "";
			sprintf_s(Setting_File_Path, Setting_File_Lenght, "%s%s", Setting_Floor_Path, Setting_File_Name);
			CMD_Set(Setting_File_Path, argv[2], argv[3]);
			break;
		}
		case 5:
		{
			CMD_Set(argv[4], argv[2], argv[3]);
			break;
		}
		default:
			break;
		}
	}
	else if (strcmp(argv[1], "-under") == 0)
	{
		//self -under HWND setting_path
		switch (argc)
		{
		case 3:
		{
			char Setting_File_Path[Setting_File_Lenght] = "";
			sprintf_s(Setting_File_Path, Setting_File_Lenght, "%s%s", Setting_Floor_Path, Setting_File_Name);
			CMD_Under(argv[2], Setting_File_Path);
			break;
		}
		case 4:
		{
			CMD_Under(argv[2], argv[3]);
			break;
		}
		default:
			break;
		}
	}
	else if (strcmp(argv[1], "-up") == 0)
	{
		//self -up HWND setting_path
		switch (argc)
		{
		case 3:
		{
			char Setting_File_Path[Setting_File_Lenght] = "";
			sprintf_s(Setting_File_Path, Setting_File_Lenght, "%s%s", Setting_Floor_Path, Setting_File_Name);
			CMD_Up(argv[2], Setting_File_Path);
			break;
		}
		case 4:
		{
			CMD_Up(argv[2], argv[3]);
			break;
		}
		default:
			break;
		}
	}
	else if (strcmp(argv[1], "-pause") == 0)
	{
		//self -pause setting_path
		switch (argc)
		{
		case 2:
		{
			char Setting_File_Path[Setting_File_Lenght] = "";
			sprintf_s(Setting_File_Path, Setting_File_Lenght, "%s%s", Setting_Floor_Path, Setting_File_Name);
			CMD_Pause(Setting_File_Path);
			break;
		}
		case 3:
		{
			CMD_Pause(argv[2]);
		}
		}
	}
	else
	{
		std::cout << "未知指令，输入-help获取帮助" << endl;
	}
	return true;
}

void CMD_Help(const char Command[])
{
	if (Command[0] == '\0')
	{
		std::cout << "-version" << TAB << "获取版本" << endl
			<< "-help" << TAB << "获取帮助" << endl
			<< "-about" << TAB << "同-help" << endl
			<< "-?" << TAB << "同-help" << endl
			<< "-start" << TAB << "启动桌面之下" << endl
			<< "-repair" << TAB << "修复窗口异常" << endl
			<< "-settingfilepath" << TAB << "指定设置路径后启动桌面之下" << endl
			<< "-play" << TAB << "播放视频" << endl
			<< "-set" << TAB << "设置参数" << endl
			<< "-get" << TAB << "获取参数" << endl
			<< "-under" << TAB << "将窗口移到桌面之下" << endl
			<< "-up" << TAB << "将窗口恢复" << endl
			<< "-pause" << TAB << "将壁纸暂停" << endl;
	}
	else
	{
		if (strcmp(Command, "-version") == 0) std::cout << "-version:获取版本" << endl
			<< "语法:-version" << endl;
		else if (strcmp(Command, "-help") == 0) std::cout << "-help:获取帮助" << endl
			<< "语法:-help [命令]" << endl;
		else if (strcmp(Command, "-about") == 0) std::cout << "-about:同help，获取帮助" << endl
			<< "语法:-about [命令]" << endl;
		else if (strcmp(Command, "-?") == 0) std::cout << "-?:同help，获取帮助" << endl
			<< "语法:-? [命令]" << endl;
		else if (strcmp(Command, "-start") == 0) std::cout << "-start:启动桌面之下" << endl
			<< "语法:-start" << endl;
		else if (strcmp(Command, "-repair") == 0) std::cout << "-start:修复窗口异常" << endl
			<< "语法:-repair" << endl;
		else if (strcmp(Command, "-settingfilepath") == 0) std::cout << "-settingfilepath:指定设置路径的位置后启动桌面之下" << endl
			<< "语法:-settingfilepath 设置文件路径" << endl;
		else if (strcmp(Command, "-play") == 0) std::cout << "-play:播放视频并移到桌面之下" << endl
			<< "语法:[-play] 视频路径 [设置文件路径]" << endl
			<< "语法:-play 视频编号 [设置文件路径]" << endl;
		else if (strcmp(Command, "-get") == 0) std::cout << "-get:获取参数" << endl
			<< "语法:-get 参数名称 [设置文件路径]" << endl
			<< "可用参数:" << endl
			<< "ffplay_path" << TAB << "获取ffplay的路径" << endl
			<< "ffmpeg_path" << TAB << "获取ffmpeg的路径" << endl
			<< "keep_path" << TAB << "获取守护进程的路径" << endl
			<< "show_console" << TAB << "获取是否显示控制台" << endl
			<< "font" << TAB << "获取字体名称" << endl
			<< "Nvideo_path" << TAB << "获取视频N的路径" << endl
			<< "Nvideo_decoder" << TAB << "获取视频N的解码器" << endl;
		else if (strcmp(Command, "-set") == 0) std::cout << "-set:设置参数" << endl
			<< "语法:-set 参数名称 参数的值 [设置文件路径]" << endl
			<< "可用参数:" << endl
			<< "ffplay_path" << TAB << "设置ffplay的路径" << endl
			<< "ffmpeg_path" << TAB << "设置ffmpeg的路径" << endl
			<< "keep_path" << TAB << "设置守护进程的路径" << endl
			<< "show_console" << TAB << "设置是否显示控制台" << endl
			<< "font" << TAB << "设置字体名称" << endl
			<< "Nvideo_path" << TAB << "设置视频N的路径" << endl
			<< "Nvideo_decoder" << TAB << "设置视频N的解码器" << endl
			<< "clear" << TAB << "清除全部的设置" << endl;
		else if (strcmp(Command, "-under") == 0) std::cout << "-under:将窗口移到桌面之下" << endl
			<< "语法:-under 窗口句柄 [设置文件路径]" << endl;
		else if (strcmp(Command, "-up") == 0) std::cout << "-up:将窗口恢复" << endl
			<< "语法:-up 窗口句柄 [设置文件路径]" << endl
			<< "语法:-up all [设置文件路径]" << endl;
		else if (strcmp(Command, "-pause") == 0)std::cout << "-pause:将壁纸暂停" << endl
			<< "语法:-pause [设置文件路径]" << endl;
	}
}

void CMD_Play(const char Video_Path[], const char Setting_Path[])
{
	Reset_Setting();
	Get_File_Setting(Setting_Path, true);//读取设置

	if (!IsWindow(Keep_HWND)) Keep_HWND = FindWindow(L"守护进程", L"守护进程"); //获取守护进程句柄

	int Quick_Video_Number = -1;
	if (Video_Path[1] == '\0')
	{
		//数字调用判断
		Quick_Video_Number = atoi(Video_Path) - 1;
		//如果判断失败那么还是-1
		if (Quick_Video_Number > 3) Quick_Video_Number = -1;//不可能大于3
	}
	
	DeskTop_Size[0] = GetSystemMetrics(SM_CXSCREEN); DeskTop_Size[1] = GetSystemMetrics(SM_CYSCREEN);//获取窗口大小
	
	std::cout << (Quick_Video_Number == -1 ? Video_Path : Quick_Video_Path[Quick_Video_Number]) << endl;
	std::cout << FFplay_Path << endl;

	Play_Video((Quick_Video_Number == -1 ? Video_Path : Quick_Video_Path[Quick_Video_Number]), "");

	if (File_Exist) Set_File_Setting(Setting_Path);//写入设置
	else std::cout << "不存在设置文件，此操作未保存" << endl;
}

void CMD_Repair()
{
	Window_Infomation::Get_And_Hide_PM_Window_HWND();
	for (Get_Child_Window(Now_Window,Window_Infomation::Get_W12_Window_HWND(1)); Now_Window != nullptr; Now_Window = Now_Window->Get_Next_Window_ptr())
	{
		Now_Window->Repair_W1_W2();
	}
}

void CMD_Get(const char Setting_Path[], const char Param_Name[])
{
	Reset_Setting();
	Get_File_Setting(Setting_Path, true);

	if (strcmp(Param_Name, "ffplay_path") == 0 || strcmp(Param_Name, "FPP") == 0) std::cout << "ffplay路径:" << FFplay_Path << endl;
	else if (strcmp(Param_Name, "ffmpeg_path") == 0 || strcmp(Param_Name, "FMP") == 0) std::cout << "ffmpeg路径:" << FFmpeg_Path << endl;
	else if (strcmp(Param_Name, "keep_path") == 0||strcmp(Param_Name, "KP") == 0) std::cout << "守护进程路径:" << Keep_Path << endl;
	else if (strcmp(Param_Name, "show_console") == 0 || strcmp(Param_Name, "SC") == 0) std::cout << "显示控制台:" << (Show_Console ? "true" : "false") << endl;
	else if (strcmp(Param_Name, "font") == 0 || strcmp(Param_Name, "FN") == 0) std::cout << "字体名称:" << Font_Name << endl;
	else if (strcmp(Param_Name, "0video_path") == 0 || strcmp(Param_Name, "V0P") == 0) std::cout << "全部视频的路径:" << endl << Quick_Video_Path[0] << endl << Quick_Video_Path[1] << endl << Quick_Video_Path[2] << endl << Quick_Video_Path[3] << endl;
	else if (strcmp(Param_Name, "1video_path") == 0 || strcmp(Param_Name, "V1P") == 0) std::cout << "视频1的路径:" << Quick_Video_Path[0] << endl;
	else if (strcmp(Param_Name, "2video_path") == 0 || strcmp(Param_Name, "V2P") == 0) std::cout << "视频2的路径:" << Quick_Video_Path[1] << endl;
	else if (strcmp(Param_Name, "3video_path") == 0 || strcmp(Param_Name, "V3P") == 0) std::cout << "视频3的路径:" << Quick_Video_Path[2] << endl;
	else if (strcmp(Param_Name, "4video_path") == 0 || strcmp(Param_Name, "V4P") == 0) std::cout << "视频4的路径:" << Quick_Video_Path[3] << endl;
	else if (strcmp(Param_Name, "0video_decoder") == 0 || strcmp(Param_Name, "V0D") == 0) std::cout << "全部视频的解码器:" << endl << Quick_Video_Decoder[0] << endl << Quick_Video_Decoder[1] << endl << Quick_Video_Decoder[2] << endl << Quick_Video_Decoder[3] << endl;
	else if (strcmp(Param_Name, "1video_decoder") == 0 || strcmp(Param_Name, "V1D") == 0) std::cout << "视频1的解码器:" << Quick_Video_Decoder[0] << endl;
	else if (strcmp(Param_Name, "2video_decoder") == 0 || strcmp(Param_Name, "V2D") == 0) std::cout << "视频2的解码器:" << Quick_Video_Decoder[1] << endl;
	else if (strcmp(Param_Name, "3video_decoder") == 0 || strcmp(Param_Name, "V3D") == 0) std::cout << "视频3的解码器:" << Quick_Video_Decoder[2] << endl;
	else if (strcmp(Param_Name, "4video_decoder") == 0 || strcmp(Param_Name, "V4D") == 0) std::cout << "视频4的解码器:" << Quick_Video_Decoder[3] << endl;
	else std::cout << "未知参数" << Param_Name << endl;
}

void CMD_Set(const char Setting_Path[], const char Param_Name[], const char Param_Value[])
{
	Reset_Setting();
	Get_File_Setting(Setting_Path, true);

	if (strcmp(Param_Name, "ffplay_path") == 0 || strcmp(Param_Name, "FPP") == 0) strcpy_s(FFplay_Path, FFplay_Path_Lenght, Param_Value);
	else if (strcmp(Param_Name, "ffmpeg_path") == 0 || strcmp(Param_Name, "FMP") == 0) strcpy_s(FFmpeg_Path, FFplay_Path_Lenght, Param_Value);
	else if (strcmp(Param_Name, "keep_path") == 0 || strcmp(Param_Name, "KP") == 0) strcpy_s(Keep_Path, Keep_Path_Lenght, Param_Value);
	else if (strcmp(Param_Name, "show_console") == 0 || strcmp(Param_Name, "SC") == 0) Show_Console = Param_Value[0] == 'T' || Param_Value[0] == 't' || atoi(Param_Value) != 0;
	else if (strcmp(Param_Name, "font") == 0 || strcmp(Param_Name, "FN") == 0) strcpy_s(Font_Name, Font_Name_Lenght, Param_Value);
	else if (strcmp(Param_Name, "1video_path") == 0 || strcmp(Param_Name, "V1P") == 0) strcpy_s(Quick_Video_Path[0], Video_Path_Lenght, Param_Value);
	else if (strcmp(Param_Name, "2video_path") == 0 || strcmp(Param_Name, "V2P") == 0) strcpy_s(Quick_Video_Path[1], Video_Path_Lenght, Param_Value);
	else if (strcmp(Param_Name, "3video_path") == 0 || strcmp(Param_Name, "V3P") == 0) strcpy_s(Quick_Video_Path[2], Video_Path_Lenght, Param_Value);
	else if (strcmp(Param_Name, "4video_path") == 0 || strcmp(Param_Name, "V4P") == 0) strcpy_s(Quick_Video_Path[3], Video_Path_Lenght, Param_Value);
	else if (strcmp(Param_Name, "1video_decoder") == 0 || strcmp(Param_Name, "V1D") == 0) strcpy_s(Quick_Video_Decoder[0], Video_Decoder_Lenght, Param_Value);
	else if (strcmp(Param_Name, "2video_decoder") == 0 || strcmp(Param_Name, "V2D") == 0) strcpy_s(Quick_Video_Decoder[1], Video_Decoder_Lenght, Param_Value);
	else if (strcmp(Param_Name, "3video_decoder") == 0 || strcmp(Param_Name, "V3D") == 0) strcpy_s(Quick_Video_Decoder[2], Video_Decoder_Lenght, Param_Value);
	else if (strcmp(Param_Name, "4video_decoder") == 0 || strcmp(Param_Name, "V4D") == 0) strcpy_s(Quick_Video_Decoder[3], Video_Decoder_Lenght, Param_Value);
	else if (strcmp(Param_Name, "clear") == 0) Reset_Setting();
	else std::cout << "未知参数" << Param_Name << endl;

	if (File_Exist) Set_File_Setting(Setting_Path);
	else std::cout << "不存在设置文件，此操作未保存" << endl;
}

void CMD_Under(const char Window_HWND_Char[], const char Setting_Path[])
{
	HWND Window_HWND = (HWND)atoll(Window_HWND_Char);

	Reset_Setting();
	Get_File_Setting(Setting_Path, true);

	//回到第一个
	while (Now_Window->Get_Last_Window_ptr()!=nullptr)
	{
		Now_Window = Now_Window->Get_Last_Window_ptr();
	}

	//进行遍历
	while (Now_Window->Get_Next_Window_ptr() != nullptr)
	{
		if (Now_Window->Get_Window_HWND() == Window_HWND)
			break;

		Now_Window = Now_Window->Get_Next_Window_ptr();
	}
	
	if (Now_Window->Get_Window_HWND() == NULL)
	{
		//直接用
		Now_Window->Set_Window_HWND(Window_HWND);
	}
	else if (Now_Window->Get_Window_HWND() != Window_HWND)
	{
		//创建新的
		Window_Infomation* Temp_Window = new Window_Infomation{};
		Temp_Window->Set_Last_Window_ptr(Now_Window);
		Now_Window->Set_Next_Window_ptr(Temp_Window);
		Now_Window = Temp_Window;
		Temp_Window = nullptr;

		Now_Window->Set_Window_HWND(Window_HWND);
	}

	Now_Window->Set_Window_Under();

	if (File_Exist) Set_File_Setting(Setting_Path);
	else std::cout << "不存在设置文件，此操作未保存" << endl;
}

void CMD_Up(const char Window_HWND_Char[], const char Setting_Path[])
{
	Reset_Setting();
	Get_File_Setting(Setting_Path, true);
	Get_Child_Window(Now_Window);//所有窗口一定存在于链表之中，1.0.3.5后的PM枚举

	//回到第一个
	while (Now_Window->Get_Last_Window_ptr() != nullptr)
	{
		Now_Window = Now_Window->Get_Last_Window_ptr();
	}

	if (strcmp(Window_HWND_Char, "all") == 0)
	{
		//进行遍历
		while (Now_Window->Get_Next_Window_ptr() != nullptr)
		{
			if (Now_Window->Is_Undered() == true)
			{
				//如果在桌面之下就移动
				Now_Window->Set_Window_Up();
			}

			Now_Window = Now_Window->Get_Next_Window_ptr();
		}
	}
	else
	{
		HWND Window_HWND = (HWND)atoll(Window_HWND_Char);
		
		//进行遍历
		while (Now_Window->Get_Next_Window_ptr() != nullptr)
		{
			if (Now_Window->Get_Window_HWND() == Window_HWND)
				break;

			Now_Window = Now_Window->Get_Next_Window_ptr();
		}

		//一定存在与窗口之中，1.0.3.5后有桌面之下枚举
		//if (Now_Window->Get_Window_HWND() == NULL)
		//{
		//	//直接用
		//	Now_Window->Set_Window_HWND(Window_HWND);
		//}
		//else if (Now_Window->Get_Window_HWND() != Window_HWND)
		//{
		//	//创建新的
		//	Window_Infomation* Temp_Window = new Window_Infomation{};
		//	Temp_Window->Set_Last_Window_ptr(Now_Window);
		//	Now_Window->Set_Next_Window_ptr(Temp_Window);
		//	Now_Window = Temp_Window;
		//	Temp_Window = nullptr;

		//	Now_Window->Set_Window_HWND(Window_HWND);
		//}

		if (DeskTop_Size[0] == 0 || DeskTop_Size[1] == 0)
			DeskTop_Size[0] = GetSystemMetrics(SM_CXSCREEN); DeskTop_Size[1] = GetSystemMetrics(SM_CYSCREEN);//获取窗口大小
		Now_Window->Set_Window_Up();
	}
	

	if (File_Exist) Set_File_Setting(Setting_Path);
	else std::cout << "不存在设置文件，此操作未保存" << endl;
}

void CMD_Pause(const char Setting_Path[])
{
	Reset_Setting();
	Get_File_Setting(Setting_Path, true);
	Get_Child_Window(Now_Window);//所有窗口一定存在于链表之中，1.0.3.5后的PM枚举

	if (!IsWindow(Keep_HWND)) Keep_HWND = FindWindow(L"守护进程", L"守护进程");
	if (Keep_HWND == NULL)
	{
		//自己处理
		while (Now_Window->Get_Last_Window_ptr() != nullptr)
			Now_Window = Now_Window->Get_Last_Window_ptr();

		while (Now_Window != nullptr)
		{
			if (Now_Window->Is_ffplay_Window()) Now_Window->Send_Message(VK_SPACE, 0x00390001);
			Now_Window = Now_Window->Get_Next_Window_ptr();
		}
	}
	else
	{
		Keep_Massage(ME_CHANGE);
	}
}
