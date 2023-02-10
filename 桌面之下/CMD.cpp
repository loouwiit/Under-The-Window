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
	if (argc == 1 || argc == 0) return false;//��֪��Ϊɶargc����Ϊ0��Ȼ��play�Ͷ���0x00000001����

	std::cout << "����֮�������е���" << endl;

	if (strcmp(argv[1], "-version") == 0)
	{
		std::cout << "����֮�� �汾:" << Progream_Version << endl;
	}
	else if (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-about") == 0 || strcmp(argv[1], "-?") == 0)
	{
		//Self -Help [command]
		CMD_Help(argc > 2 ? argv[2] : "");
	}
	else if (strcmp(argv[1], "-start") == 0)
	{
		std::cout << "��������֮��" << endl;
		//Show_Console = true;
		return false;
	}
	else if (strcmp(argv[1], "-settingfilepath") == 0)
	{
		if (argc > 2)
		{
			Spict_Path(argv[2], Setting_Floor_Path, Setting_File_Lenght);
			Spict_Name(argv[2], Setting_File_Name, Setting_File_Lenght);
			std::cout << "ָ������·��Ϊ" << Setting_Floor_Path << Setting_File_Name << endl
				<< "��������֮��" << endl;
			return false;
		}
	}
	else if (strcmp(argv[1], "-play") == 0 || argv[1][0] != '-')//ֱ������Ƶ·��
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
			//ֻ����Ƶ·��
			char Setting_File_Path[Setting_File_Lenght] = "";
			sprintf_s(Setting_File_Path, Setting_File_Lenght, "%s%s", Setting_Floor_Path, Setting_File_Name);
			CMD_Play(argv[2], Setting_File_Path);
			break;
		}
		case 4:
		{
			//��������·��
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
		std::cout << "δָ֪�����-help��ȡ����" << endl;
	}
	return true;
}

void CMD_Help(const char Command[])
{
	if (Command[0] == '\0')
	{
		std::cout << "-version" << TAB << "��ȡ�汾" << endl
			<< "-help" << TAB << "��ȡ����" << endl
			<< "-about" << TAB << "ͬ-help" << endl
			<< "-?" << TAB << "ͬ-help" << endl
			<< "-start" << TAB << "��������֮��" << endl
			<< "-repair" << TAB << "�޸������쳣" << endl
			<< "-settingfilepath" << TAB << "ָ������·������������֮��" << endl
			<< "-play" << TAB << "������Ƶ" << endl
			<< "-set" << TAB << "���ò���" << endl
			<< "-get" << TAB << "��ȡ����" << endl
			<< "-under" << TAB << "�������Ƶ�����֮��" << endl
			<< "-up" << TAB << "�����ڻָ�" << endl
			<< "-pause" << TAB << "����ֽ��ͣ" << endl;
	}
	else
	{
		if (strcmp(Command, "-version") == 0) std::cout << "-version:��ȡ�汾" << endl
			<< "�﷨:-version" << endl;
		else if (strcmp(Command, "-help") == 0) std::cout << "-help:��ȡ����" << endl
			<< "�﷨:-help [����]" << endl;
		else if (strcmp(Command, "-about") == 0) std::cout << "-about:ͬhelp����ȡ����" << endl
			<< "�﷨:-about [����]" << endl;
		else if (strcmp(Command, "-?") == 0) std::cout << "-?:ͬhelp����ȡ����" << endl
			<< "�﷨:-? [����]" << endl;
		else if (strcmp(Command, "-start") == 0) std::cout << "-start:��������֮��" << endl
			<< "�﷨:-start" << endl;
		else if (strcmp(Command, "-repair") == 0) std::cout << "-start:�޸������쳣" << endl
			<< "�﷨:-repair" << endl;
		else if (strcmp(Command, "-settingfilepath") == 0) std::cout << "-settingfilepath:ָ������·����λ�ú���������֮��" << endl
			<< "�﷨:-settingfilepath �����ļ�·��" << endl;
		else if (strcmp(Command, "-play") == 0) std::cout << "-play:������Ƶ���Ƶ�����֮��" << endl
			<< "�﷨:[-play] ��Ƶ·�� [�����ļ�·��]" << endl
			<< "�﷨:-play ��Ƶ��� [�����ļ�·��]" << endl;
		else if (strcmp(Command, "-get") == 0) std::cout << "-get:��ȡ����" << endl
			<< "�﷨:-get �������� [�����ļ�·��]" << endl
			<< "���ò���:" << endl
			<< "ffplay_path" << TAB << "��ȡffplay��·��" << endl
			<< "ffmpeg_path" << TAB << "��ȡffmpeg��·��" << endl
			<< "keep_path" << TAB << "��ȡ�ػ����̵�·��" << endl
			<< "show_console" << TAB << "��ȡ�Ƿ���ʾ����̨" << endl
			<< "font" << TAB << "��ȡ��������" << endl
			<< "Nvideo_path" << TAB << "��ȡ��ƵN��·��" << endl
			<< "Nvideo_decoder" << TAB << "��ȡ��ƵN�Ľ�����" << endl;
		else if (strcmp(Command, "-set") == 0) std::cout << "-set:���ò���" << endl
			<< "�﷨:-set �������� ������ֵ [�����ļ�·��]" << endl
			<< "���ò���:" << endl
			<< "ffplay_path" << TAB << "����ffplay��·��" << endl
			<< "ffmpeg_path" << TAB << "����ffmpeg��·��" << endl
			<< "keep_path" << TAB << "�����ػ����̵�·��" << endl
			<< "show_console" << TAB << "�����Ƿ���ʾ����̨" << endl
			<< "font" << TAB << "������������" << endl
			<< "Nvideo_path" << TAB << "������ƵN��·��" << endl
			<< "Nvideo_decoder" << TAB << "������ƵN�Ľ�����" << endl
			<< "clear" << TAB << "���ȫ��������" << endl;
		else if (strcmp(Command, "-under") == 0) std::cout << "-under:�������Ƶ�����֮��" << endl
			<< "�﷨:-under ���ھ�� [�����ļ�·��]" << endl;
		else if (strcmp(Command, "-up") == 0) std::cout << "-up:�����ڻָ�" << endl
			<< "�﷨:-up ���ھ�� [�����ļ�·��]" << endl
			<< "�﷨:-up all [�����ļ�·��]" << endl;
		else if (strcmp(Command, "-pause") == 0)std::cout << "-pause:����ֽ��ͣ" << endl
			<< "�﷨:-pause [�����ļ�·��]" << endl;
	}
}

void CMD_Play(const char Video_Path[], const char Setting_Path[])
{
	Reset_Setting();
	Get_File_Setting(Setting_Path, true);//��ȡ����

	if (!IsWindow(Keep_HWND)) Keep_HWND = FindWindow(L"�ػ�����", L"�ػ�����"); //��ȡ�ػ����̾��

	int Quick_Video_Number = -1;
	if (Video_Path[1] == '\0')
	{
		//���ֵ����ж�
		Quick_Video_Number = atoi(Video_Path) - 1;
		//����ж�ʧ����ô����-1
		if (Quick_Video_Number > 3) Quick_Video_Number = -1;//�����ܴ���3
	}
	
	DeskTop_Size[0] = GetSystemMetrics(SM_CXSCREEN); DeskTop_Size[1] = GetSystemMetrics(SM_CYSCREEN);//��ȡ���ڴ�С
	
	std::cout << (Quick_Video_Number == -1 ? Video_Path : Quick_Video_Path[Quick_Video_Number]) << endl;
	std::cout << FFplay_Path << endl;

	Play_Video((Quick_Video_Number == -1 ? Video_Path : Quick_Video_Path[Quick_Video_Number]), "");

	if (File_Exist) Set_File_Setting(Setting_Path);//д������
	else std::cout << "�����������ļ����˲���δ����" << endl;
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

	if (strcmp(Param_Name, "ffplay_path") == 0 || strcmp(Param_Name, "FPP") == 0) std::cout << "ffplay·��:" << FFplay_Path << endl;
	else if (strcmp(Param_Name, "ffmpeg_path") == 0 || strcmp(Param_Name, "FMP") == 0) std::cout << "ffmpeg·��:" << FFmpeg_Path << endl;
	else if (strcmp(Param_Name, "keep_path") == 0||strcmp(Param_Name, "KP") == 0) std::cout << "�ػ�����·��:" << Keep_Path << endl;
	else if (strcmp(Param_Name, "show_console") == 0 || strcmp(Param_Name, "SC") == 0) std::cout << "��ʾ����̨:" << (Show_Console ? "true" : "false") << endl;
	else if (strcmp(Param_Name, "font") == 0 || strcmp(Param_Name, "FN") == 0) std::cout << "��������:" << Font_Name << endl;
	else if (strcmp(Param_Name, "0video_path") == 0 || strcmp(Param_Name, "V0P") == 0) std::cout << "ȫ����Ƶ��·��:" << endl << Quick_Video_Path[0] << endl << Quick_Video_Path[1] << endl << Quick_Video_Path[2] << endl << Quick_Video_Path[3] << endl;
	else if (strcmp(Param_Name, "1video_path") == 0 || strcmp(Param_Name, "V1P") == 0) std::cout << "��Ƶ1��·��:" << Quick_Video_Path[0] << endl;
	else if (strcmp(Param_Name, "2video_path") == 0 || strcmp(Param_Name, "V2P") == 0) std::cout << "��Ƶ2��·��:" << Quick_Video_Path[1] << endl;
	else if (strcmp(Param_Name, "3video_path") == 0 || strcmp(Param_Name, "V3P") == 0) std::cout << "��Ƶ3��·��:" << Quick_Video_Path[2] << endl;
	else if (strcmp(Param_Name, "4video_path") == 0 || strcmp(Param_Name, "V4P") == 0) std::cout << "��Ƶ4��·��:" << Quick_Video_Path[3] << endl;
	else if (strcmp(Param_Name, "0video_decoder") == 0 || strcmp(Param_Name, "V0D") == 0) std::cout << "ȫ����Ƶ�Ľ�����:" << endl << Quick_Video_Decoder[0] << endl << Quick_Video_Decoder[1] << endl << Quick_Video_Decoder[2] << endl << Quick_Video_Decoder[3] << endl;
	else if (strcmp(Param_Name, "1video_decoder") == 0 || strcmp(Param_Name, "V1D") == 0) std::cout << "��Ƶ1�Ľ�����:" << Quick_Video_Decoder[0] << endl;
	else if (strcmp(Param_Name, "2video_decoder") == 0 || strcmp(Param_Name, "V2D") == 0) std::cout << "��Ƶ2�Ľ�����:" << Quick_Video_Decoder[1] << endl;
	else if (strcmp(Param_Name, "3video_decoder") == 0 || strcmp(Param_Name, "V3D") == 0) std::cout << "��Ƶ3�Ľ�����:" << Quick_Video_Decoder[2] << endl;
	else if (strcmp(Param_Name, "4video_decoder") == 0 || strcmp(Param_Name, "V4D") == 0) std::cout << "��Ƶ4�Ľ�����:" << Quick_Video_Decoder[3] << endl;
	else std::cout << "δ֪����" << Param_Name << endl;
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
	else std::cout << "δ֪����" << Param_Name << endl;

	if (File_Exist) Set_File_Setting(Setting_Path);
	else std::cout << "�����������ļ����˲���δ����" << endl;
}

void CMD_Under(const char Window_HWND_Char[], const char Setting_Path[])
{
	HWND Window_HWND = (HWND)atoll(Window_HWND_Char);

	Reset_Setting();
	Get_File_Setting(Setting_Path, true);

	//�ص���һ��
	while (Now_Window->Get_Last_Window_ptr()!=nullptr)
	{
		Now_Window = Now_Window->Get_Last_Window_ptr();
	}

	//���б���
	while (Now_Window->Get_Next_Window_ptr() != nullptr)
	{
		if (Now_Window->Get_Window_HWND() == Window_HWND)
			break;

		Now_Window = Now_Window->Get_Next_Window_ptr();
	}
	
	if (Now_Window->Get_Window_HWND() == NULL)
	{
		//ֱ����
		Now_Window->Set_Window_HWND(Window_HWND);
	}
	else if (Now_Window->Get_Window_HWND() != Window_HWND)
	{
		//�����µ�
		Window_Infomation* Temp_Window = new Window_Infomation{};
		Temp_Window->Set_Last_Window_ptr(Now_Window);
		Now_Window->Set_Next_Window_ptr(Temp_Window);
		Now_Window = Temp_Window;
		Temp_Window = nullptr;

		Now_Window->Set_Window_HWND(Window_HWND);
	}

	Now_Window->Set_Window_Under();

	if (File_Exist) Set_File_Setting(Setting_Path);
	else std::cout << "�����������ļ����˲���δ����" << endl;
}

void CMD_Up(const char Window_HWND_Char[], const char Setting_Path[])
{
	Reset_Setting();
	Get_File_Setting(Setting_Path, true);
	Get_Child_Window(Now_Window);//���д���һ������������֮�У�1.0.3.5���PMö��

	//�ص���һ��
	while (Now_Window->Get_Last_Window_ptr() != nullptr)
	{
		Now_Window = Now_Window->Get_Last_Window_ptr();
	}

	if (strcmp(Window_HWND_Char, "all") == 0)
	{
		//���б���
		while (Now_Window->Get_Next_Window_ptr() != nullptr)
		{
			if (Now_Window->Is_Undered() == true)
			{
				//���������֮�¾��ƶ�
				Now_Window->Set_Window_Up();
			}

			Now_Window = Now_Window->Get_Next_Window_ptr();
		}
	}
	else
	{
		HWND Window_HWND = (HWND)atoll(Window_HWND_Char);
		
		//���б���
		while (Now_Window->Get_Next_Window_ptr() != nullptr)
		{
			if (Now_Window->Get_Window_HWND() == Window_HWND)
				break;

			Now_Window = Now_Window->Get_Next_Window_ptr();
		}

		//һ�������봰��֮�У�1.0.3.5��������֮��ö��
		//if (Now_Window->Get_Window_HWND() == NULL)
		//{
		//	//ֱ����
		//	Now_Window->Set_Window_HWND(Window_HWND);
		//}
		//else if (Now_Window->Get_Window_HWND() != Window_HWND)
		//{
		//	//�����µ�
		//	Window_Infomation* Temp_Window = new Window_Infomation{};
		//	Temp_Window->Set_Last_Window_ptr(Now_Window);
		//	Now_Window->Set_Next_Window_ptr(Temp_Window);
		//	Now_Window = Temp_Window;
		//	Temp_Window = nullptr;

		//	Now_Window->Set_Window_HWND(Window_HWND);
		//}

		if (DeskTop_Size[0] == 0 || DeskTop_Size[1] == 0)
			DeskTop_Size[0] = GetSystemMetrics(SM_CXSCREEN); DeskTop_Size[1] = GetSystemMetrics(SM_CYSCREEN);//��ȡ���ڴ�С
		Now_Window->Set_Window_Up();
	}
	

	if (File_Exist) Set_File_Setting(Setting_Path);
	else std::cout << "�����������ļ����˲���δ����" << endl;
}

void CMD_Pause(const char Setting_Path[])
{
	Reset_Setting();
	Get_File_Setting(Setting_Path, true);
	Get_Child_Window(Now_Window);//���д���һ������������֮�У�1.0.3.5���PMö��

	if (!IsWindow(Keep_HWND)) Keep_HWND = FindWindow(L"�ػ�����", L"�ػ�����");
	if (Keep_HWND == NULL)
	{
		//�Լ�����
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
