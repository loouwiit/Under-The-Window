#include "Variable.h"

/*
* SFML:https://www.sfml-dev.org/tutorials
* 此文件用于GUI
*/

sf::RenderWindow Window;//窗口
//sf::Vector2u Window_Size(0, 0);//自己的大小1.0.2.3删除
sf::Font Text_Fort;//字体
char Font_Name[Font_Name_Lenght] = "msyh.ttc";//字体名称
sf::Image Progream_Icon;//图标
//HWND Self_HWND;//自己的句柄
HWND Console_HWND;//控制台的句柄
HWND Keep_HWND;//守护进程的句柄 1.0.4.0
char FFplay_Path[FFplay_Path_Lenght] = ".\\ffplay.exe";//ffplay路径
char FFmpeg_Path[FFplay_Path_Lenght] = ".\\ffmpeg.exe";//ffmpeg路径
char Keep_Path[Keep_Path_Lenght] = ".\\守护进程.exe";//守护进程路径
char Setting_Floor_Path[Setting_File_Lenght] = ".\\桌面之下\\";//设置文件夹的路径
char Setting_File_Name[Setting_File_Lenght] = "setting.txt";//设置文件的名称

//extern char Video_Recorder[Recorder_Lenght];//视频录制来源
extern char Audio_Recorder[Recorder_Lenght];//音频录制来源

char Quick_Video_Path[4][Video_Path_Lenght] = { "" };//快速播放的视频路径
char Quick_Video_Name[4][20] = { "" };//快速播放图片名称
char Quick_Video_Decoder[4][Video_Decoder_Lenght] = { "" };//快速播放解码器
sf::Texture Quick_Video_Texture[4];//快速播放图片
ENUM::Input_Position Input_Position = ENUM::Input_Position::INP_NULL;//表示按键输入的位置
unsigned Input_Char_Position = 0;//输入的字符位置

int SPF = (int)(1000.0f / 30.0f);//渲染帧率的倒数
bool Show_Console = false;//显示控制台
//bool Keep_Undered = false;//保持桌面之下 1.0.2.5删除
bool HEX_Mode = false;//十六进制输出
//bool Self_Most_Top = false;//是否置顶
bool Changing_Position = false;//更改位置中(控制显示更改位置)
bool Update_Zero_HWND = false;//强制渲染0
bool Update_Draw = true;//更新渲染
//bool Update_Position = true;//更新位置

int main(const int argc, const char* argv[]);
void Initialize();//初始化
void EnEtialize();//结束释放
//void Print();//渲染
//void Sleep();//休息
//void Event();//事件处理
void Event_Key(sf::Event::KeyEvent Key, ENUM::Input_Position Input_Position);//按键检测


int main(const int argc, const char* argv[])
{
	std::cout << Progream_Version << endl;
#ifdef _DEBUG
	std::cout << "Debug版本" << endl;
#endif // _DEBUG
#ifndef _WIN64
	std::cout << "32位版本" << endl;
#endif // _WIN64

	if (!CMD_Call(argc, argv))
	{
		Initialize();
		while (Window.isOpen())
		{
			Event(true);
			Print();
			Sleep();
		}
		EnEtialize();
	}
	return 0;
}

void Initialize()
{
	char Setting_File_Path[Setting_File_Lenght] = "";
	sprintf_s(Setting_File_Path, Setting_File_Lenght, "%s%s", Setting_Floor_Path, Setting_File_Name);

	Reset_Setting();
	Get_File_Setting(Setting_File_Path);//读取设置
	Get_Child_Window(Now_Window);//获取并链接PM子窗口
	Window.create(sf::VideoMode(Self_Original_Size[0], Self_Original_Size[1]), L"桌面之下");//创建窗口

	//设置图标
	{
		char Icon_Path[Setting_File_Lenght] = "";
		sprintf_s(Icon_Path, Setting_File_Lenght, "%sicon.png", Setting_Floor_Path);
		if (Progream_Icon.loadFromFile(Icon_Path))
			Window.setIcon(Progream_Icon.getSize().x, Progream_Icon.getSize().y, Progream_Icon.getPixelsPtr());
	}

	Console_HWND = FindWindow(L"ConsoleWindowClass", NULL);//获取句柄
	if (Console_HWND != NULL && !Show_Console) ShowWindow(Console_HWND, SW_HIDE);//隐藏窗口
	Window_Infomation::Set_Self_Window_Exist(true);
	Window_Infomation::Find_Self_Window_HWND();
	Keep_HWND = FindWindow(L"守护进程", L"守护进程");//1.0.4.1

	SPF = 33;//设置帧率

	wchar_t System_Floor[50] = L"";
	char Fonts_Path[50] = "";
	GetSystemDirectory(System_Floor,50);//C:\windows\system32
	System_Floor[1] = L'\0';//截止C
	sprintf_s(Fonts_Path, "%ws:/Windows/Fonts/%s", System_Floor, Font_Name);//设置路径
	if (!Text_Fort.loadFromFile(Fonts_Path))
	{
		//失败后自动加载msyh.ttf;
		if (strcmp(Fonts_Path, "msth.ttc"))
		{
			Fonts_Path[7] = 'f';//ttc->ttf
			Text_Fort.loadFromFile(Fonts_Path);
		}
	}

	//Self_HWND = FindWindow(L"SFML_Window", L"桌面之下");//获取自己的句柄 1.0.2.5交给对象实现

	DeskTop_Size[0] = GetSystemMetrics(SM_CXSCREEN); DeskTop_Size[1] = GetSystemMetrics(SM_CYSCREEN);
	Update_Draw = true; //Update_Position = true;
}

void EnEtialize()
{
	if (File_Exist)//存在文档时才写入
	{
		char Setting_File_Path[Setting_File_Lenght] = "";
		sprintf_s(Setting_File_Path, Setting_File_Lenght, "%s%s", Setting_Floor_Path, Setting_File_Name);
		Set_File_Setting(Setting_File_Path);
	}
	//移到最前
	while (Now_Window->Get_Last_Window_ptr() != nullptr)
	{
		Now_Window = Now_Window->Get_Last_Window_ptr();
	}
	//删到最后
	while (Now_Window->Get_Next_Window_ptr() != nullptr)
	{
		Now_Window = Now_Window->Get_Next_Window_ptr();
		delete Now_Window->Get_Last_Window_ptr();
	}
	delete Now_Window;
	Now_Window = nullptr;

	/*
	if (ffplay_Thread != nullptr)
	{
		ffplay_Pause_Thread_Enabled = false;
		ffplay_Thread->join();
		delete ffplay_Thread;
		ffplay_Thread = nullptr;
	}
	*/

	if (Show_Console)
		system("pause");//等待
	else if (Console_HWND != NULL)
		ShowWindow(Console_HWND, SW_SHOW);//显示窗口
}

void Print()
{
	static sf::RectangleShape Back_Rectangle[2];
	static sf::RectangleShape Button_Rectangle[12];
	static sf::Sprite Quick_Video_Sprite[4];
	static bool Update_Position = true;//第一次初始化要用
	static sf::Text Text[12];
	/*
	0:句柄
	1:名称
	2:X
	3:Y
	4:dX
	5:dY
	6:输入句柄
	7:设置快速视频
	8:选择窗口
	9:设置焦点
	10:设置/恢复
	11:ffplay路径
	12:分割线
	13:窗口信息填充
	*/
	//if (Update_Position)
	//{
	//	Update_Position = false;
	//	Update_Draw = true;
	//	sf::Vector2f Size(Window.getSize().x * 0.25f , Window.getSize().y * 0.1f);//略比1/3和1/6小
	//	for (int i = 0; i < 12; i++)
	//	{
	//		Back_Rectangle[i].setSize(Size);
	//		Back_Rectangle[i].setPosition(Size.x * (i % 2 * 1.1f + 0.1f),
	//			Size.y * (floor(i / 2.0f) * 1.05f + 0.05f));
	//	}
	//	Back_Rectangle[12].setSize(sf::Vector2f((float)Window.getSize().y,5.0f));
	//	Back_Rectangle[12].setPosition(Window.getSize().x * 2.0f / 3.0f, 0);
	//	Back_Rectangle[12].setRotation(90);
	//	Back_Rectangle[13].setSize(Size * 2.1);
	//	Back_Rectangle[13].setPosition(Size.x * 0.05f, Size.y * 0.05f);
	//	Back_Rectangle[13].setSize(sf::Vector2f(0,0));
	//}
	if (Update_Position)
	{
		Update_Position = false;
		Update_Draw = true;
		sf::Vector2f Size(Window.getSize().x * 0.3f , Window.getSize().y * 0.11f);
		for (int i = 0; i < 12; i++)
		{
			Button_Rectangle[i].setSize(Size);
			if (i < 6)
			{
				Button_Rectangle[i].setPosition(Size.x * (i % 2 * 1.05f + 0.07f),
					Size.y * (floor(i / 2.0f) * 1.4f + 0.3f));
			}
			else
			{
				Button_Rectangle[i].setPosition(Size.x * (i % 2 * 1.05f + 0.07f),
					Size.y * (floor(i / 2.0f) * 1.4f + 0.5f));
			}
		}
		Back_Rectangle[0].setSize(sf::Vector2f((float)Window.getSize().y,5.0f));
		Back_Rectangle[0].setPosition(Window.getSize().x * 2.0f / 3.0f, 0);
		Back_Rectangle[0].setRotation(90);
		Back_Rectangle[1].setSize(sf::Vector2f(Window.getSize().x*0.63f , Window.getSize().y *0.48f));
		Back_Rectangle[1].setPosition(Size.x * 0.05f, Size.y * 0.1f);
		Back_Rectangle[1].setFillColor(sf::Color(0x66CCCC99));

		for (int i = 0; i < 12; i++)
		{
			Text[i].setFont(Text_Fort);
			Text[i].setFillColor(sf::Color(0x000000FF));
			Text[i].setScale(0.9f, 0.9f);
			if (i < 6)
			{
				Text[i].setPosition(Size.x * (i % 2 * 1.05f + 0.07f),
					Size.y * (floor(i / 2.0f) * 1.4f + 0.3f));
			}
			else
			{
				Text[i].setPosition(Size.x * (i % 2 * 1.05f + 0.07f),
					Size.y * (floor(i / 2.0f) * 1.4f + 0.5f));
			}
		}
		Text[0].setString(L"窗口句柄");
		Text[1].setString(L"窗口名");
		Text[2].setString(L"窗口坐标");
		Text[3].setString(L"窗口长高");
		Text[4].setString(L"上一个窗口");
		Text[5].setString(L"下一个窗口");
		Text[6].setString(L"输入窗口句柄");
		Text[7].setString(L"设置快捷视频");
		Text[8].setString(L"拖动选择窗口");
		Text[9].setString(L"设置窗口置顶");
		Text[10].setString(L"移到桌面之下");
		Text[11].setString(L"非壁纸窗口");

		for (int i = 0; i < 4; i++)
		{
			if (Quick_Video_Path[i][0] != '\0')
			{
				Spict_Name(Quick_Video_Path[i], Quick_Video_Name[i], 20);//分割名字

				char Video_Texture_Path[Setting_File_Lenght] = { "" };
				sprintf_s(Video_Texture_Path, Setting_File_Lenght, "%s%s.png", Setting_Floor_Path, Quick_Video_Name[i]);
//				std::cout << "加载" << Video_Texture_Path << endl;//信息
				if (!Quick_Video_Texture[i].loadFromFile(Video_Texture_Path))
				{
					Creat_Vidoe_Texture(i);

					//再次判断
					//if (!Quick_Video_Texture[i].loadFromFile(Video_Texture_Path))
					//{
					//	std::cout << "创建失败" << endl;
					//}
				}
			}
			else
			{
				Quick_Video_Texture[i].create((int)((float)Window.getSize().x * 0.3f), (int)((float)Window.getSize().y * 0.2f));
			}
			Quick_Video_Sprite[i].setPosition((float)Window.getSize().x * 0.7f, (float)Window.getSize().y * 0.23f * (i + 0.16f));
			Quick_Video_Sprite[i].setTexture(Quick_Video_Texture[i]);
			Quick_Video_Sprite[i].setScale(((float)Window.getSize().x * 0.26f) / (float)Quick_Video_Texture[i].getSize().x, ((float)Window.getSize().y * 0.2f) / (float)Quick_Video_Texture[i].getSize().y);
		}
	}

	if (Update_Draw)
	{
		Update_Draw = false;
		if (Now_Window->Get_Window_HWND() != NULL || (Update_Zero_HWND ? Update_Zero_HWND = false, true : false))
		{
			//更新字
			constexpr unsigned Buffer_Size = Video_Path_Lenght + 10;
			wchar_t Buffer[Buffer_Size] = L"";

			swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"0x%llX" : L"%lld", (long long)Now_Window->Get_Window_HWND());
			//#:十六进制数显示0x ll长 X十六进制数 但是#的大小写跟随X
			Text[0].setString(Buffer);//句柄

			//名字
			if (ENUM::Input_Position::INP_Video_1 <= Input_Position && Input_Position <= ENUM::Input_Position::INP_Video_4)
			{
				//先判断输入
				wchar_t Video_Path_W[Video_Path_Lenght] = L"";
				CharToTchar(Quick_Video_Path[(short)Input_Position - (short)ENUM::Input_Position::INP_Video_1], Video_Path_W, CP_ACP);

				swprintf_s(Buffer, Buffer_Size, L"视频%d:%ws", (short)Input_Position - (short)ENUM::Input_Position::INP_Video_1 + 1, Video_Path_W);
			}
			else if (Now_Window->Get_Parent_Window_HWND() == NULL)
			{
				GetWindowText(Now_Window->Get_Window_HWND(), Buffer, Buffer_Size);//显示标题
				if (Buffer[0] == L'\0')
				{
					//没有返回标题
					if (IsWindow(Now_Window->Get_Window_HWND()))
						wcsncpy_s(Buffer, Buffer_Size, L"(空标题)", 6);//窗口不存在一共6个字(\0)
					else
						wcsncpy_s(Buffer, Buffer_Size, L"(窗口不存在)", 8);//窗口不存在一共8个字(\0)
				}
			}
			else
			{
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"父窗口:0x%llX" : L"父窗口:%lld", (long long)Now_Window->Get_Parent_Window_HWND());//显示父窗口 #:十六进制数显示0x ll长 X十六进制数 但是#的大小写跟随X
			}
			Text[1].setString(Buffer);

			//坐标
			switch (Input_Position)
			{
			case ENUM::Input_Position::INP_X_Position:
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"X:0x%lX" : L"X:%ld", Now_Window->Get_Infomation().left);
				break;
			case ENUM::Input_Position::INP_Y_Position:
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"Y:0x%lX" : L"Y:%ld", Now_Window->Get_Infomation().top);
				break;
			default:
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"坐标:0x%lX,0x%lX" : L"坐标:%ld,%ld", Now_Window->Get_Infomation().left, Now_Window->Get_Infomation().top);
				break;
			}
			Text[2].setString(Buffer);

			//宽高
			switch (Input_Position)
			{
			case ENUM::Input_Position::INP_Wide:
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"宽:0x%lX" : L"宽:%ld", Now_Window->Get_Infomation().right);
				break;
			case ENUM::Input_Position::INP_Hight:
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"高:0x%lX" : L"高:%ld", Now_Window->Get_Infomation().bottom);
				break;
			default:
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"宽高:0x%lX,0x%lX" : L"宽高:%ld,%ld", Now_Window->Get_Infomation().right, Now_Window->Get_Infomation().bottom);
				break;
			}
			Text[3].setString(Buffer);
		}
		if (Now_Window->Is_ffplay_Window())
		{
			//1.0.4.0
			if (Keep_HWND == 0)
			{
				//没有
				Text[9].setString(L"启动守护进程");
			}
			else
			{
				Text[9].setString(L"已存在守护进程");
			}
		}
		else
		{
			if (Now_Window->Is_Top_Most())
				Text[9].setString(L"取消窗口置顶");
			else
				Text[9].setString(L"设置窗口置顶");
		}

		if (Changing_Position)
			Text[10].setString(L"更改位置");
		else if (Now_Window->Is_ffplay_Window())
		{
			Text[10].setString(L"关闭壁纸"); //1.0.3.19
		}
		else
		{
			if (Now_Window->Is_Undered())
				Text[10].setString(L"恢复窗口");
			else
				Text[10].setString(L"移到桌面之下");
		}

		/*
		if (Keep_Undered)
			Text[11].setString(L"退出时保留");
		else
			Text[11].setString(L"退出时恢复");
		*/

		if (!Now_Window->Is_ffplay_Window()) //1.0.3.16加入
			Text[11].setString(L"非壁纸窗口");
		else
			Text[11].setString(L"暂停壁纸");


		Window.clear(sf::Color(0x99FFFFFF));
		for (int i = 0; i < 2; i++)
			Window.draw(Back_Rectangle[i]);
		for (int i = 0; i < 12; i++)
			Window.draw(Button_Rectangle[i]);

		//先图片
		for (int i = 0; i < 4; i++)
			Window.draw(Quick_Video_Sprite[i]);
		//后字
		for (int i = 0; i < 12; i++)
			Window.draw(Text[i]);

		Window.display();//显示
	}
}

void Sleep()
{
	static long Last_Time = 0, Now_Time = 0;

	Now_Time = clock();
	Sleep(max(SPF + Last_Time - Now_Time, 1L));
	Last_Time = Now_Time;
}

void Event(bool Flag)
{
	sf::Event Event;//事件
	while (Window.pollEvent(Event))//处理每一个事件
	{
		switch (Event.type)
		{
		case sf::Event::Closed://关闭窗口
		{
			Window.close();
			break;
		}
		case sf::Event::Resized://调整大小
		{
			constexpr float Self_Original_Size_K = (float)Self_Original_Size[0] / (float)Self_Original_Size[1];
			static bool Operation_Size = true;

			if (Operation_Size)
			{
				Operation_Size = false;

				sf::Vector2u Size = {};
				Size.x = (unsigned)((Event.size.width + Event.size.height * Self_Original_Size_K) / 2);
				Size.y = (unsigned)((Event.size.height + Event.size.width / Self_Original_Size_K) / 2);
//				std::cout << Event.size.width << "x" << Event.size.height << "|" << Size.x << "x" << Size.y << endl;//debug用
				Window.setSize(Size);
			}
			else
			{
				Operation_Size = true;
			}
//			Update_Position = true;
			Update_Draw = true;
			break;
		}
		case sf::Event::LostFocus://失焦
		{
			break;
		}
		case sf::Event::GainedFocus://聚焦
		{
			Update_Draw = true;
			Now_Window->Update_Window_HWND();
			Repair_W1_W2_Error();//处理W1、W2错误
			break;
		}
		case sf::Event::KeyPressed://按键
		{
			if (!Flag)break;
			Event_Key(Event.key, Input_Position);
			break;
		}
		case sf::Event::TextEntered://输入
		{
			if (ENUM::Input_Position::INP_Video_1 <= Input_Position && Input_Position <= ENUM::Input_Position::INP_Video_4)
			{
				bool Changed = false;
				wchar_t Input_Video_Path[Video_Path_Lenght] = L"\0";
				CharToTchar(Quick_Video_Path[(short)Input_Position - (short)ENUM::Input_Position::INP_Video_1], Input_Video_Path, CP_ACP);

				//0x8和0x7F
				if (Event.text.unicode == 0x8 || Event.text.unicode == 0x7F)
				{
					//删除
					if (Input_Char_Position > 0)
					{
						Input_Char_Position--;
						Input_Video_Path[Input_Char_Position] = L'\0';
						Changed = true;
					}
				}
				//\r \n
				else if (Event.text.unicode==0xD)
				{
					//回车
					Input_Position = ENUM::Input_Position::INP_NULL;
				}
				else if ((0x0 <= Event.text.unicode && Event.text.unicode <= 0x1F) || (0x7F < Event.text.unicode && Event.text.unicode < 0xA0))
				{
					//NULL
				}
				else
				{
					//输入
					if (Input_Char_Position < Video_Path_Lenght - 1)
					{
						Input_Video_Path[Input_Char_Position] = Event.text.unicode;
						Input_Char_Position++;
						Input_Video_Path[Input_Char_Position] = L'\0';
						Changed = true;
					}
				}
				if (Changed)
				{
					TcharToChar(Input_Video_Path, Quick_Video_Path[(short)Input_Position - (short)ENUM::Input_Position::INP_Video_1], CP_ACP);
					Update_Zero_HWND = true;
					Update_Draw = true;
				}
			}//有输入位置

			break;
		}
		case sf::Event::MouseButtonPressed://鼠标按键
		{
			if (!Flag)break;
			if (Event.mouseButton.x < Window.getSize().x * 0.66f)
			{
				//左侧
				switch ((int)(Event.mouseButton.y / (Window.getSize().y / 6)))
				{
				case 0:
				{
					Update_Draw = true; Update_Zero_HWND = true;
					if (Event.mouseButton.x < Window.getSize().x * 0.33)
					{
						HEX_Mode = !HEX_Mode;
						//Input_Position = ENUM::Input_Position::INP_HWND;//以后再说
					}
					else
					{
						if (Now_Window->Get_Parent_Window_HWND() == NULL)
						{
							//没有父窗口，是名称
							HEX_Mode = !HEX_Mode;
						}
						else
						{
							//选择父窗口 
							Input_Position = ENUM::Input_Position::INP_NULL;//输入模式归位

							//在窗口后面加一个父窗口
							Window_Infomation* Temp_Window = new Window_Infomation(Now_Window->Get_Parent_Window_HWND());
							if (Now_Window->Get_Next_Window_ptr())
							{
								Now_Window->Get_Next_Window_ptr()->Set_Last_Window_ptr(Temp_Window);
								Temp_Window->Set_Next_Window_ptr(Now_Window->Get_Next_Window_ptr());
							}
							Temp_Window->Set_Last_Window_ptr(Now_Window);
							Now_Window->Set_Next_Window_ptr(Temp_Window);

							Now_Window = Temp_Window;
							Temp_Window = nullptr;

							//if (Now_Window->Is_Undered()) Now_Window->Set_Window_Up();
							//Now_Window->Set_Window_HWND(Now_Window->Get_Parent_Window_HWND());
							/*
							* 判断是否是子窗口，1.0.2.5后全权交给对象处理
							if (GetAncestor(Now_Window->Get_Window_HWND(), GA_ROOT) != Now_Window->Get_Window_HWND())//不是自己
							{
								////非桌面窗口
								RECT Parent_Rect;
								//Now_Window->Get_Parent_Window_HWND() = GetAncestor(Now_Window->Get_Window_HWND(), GA_PARENT);自动的，不用手动操作
								GetWindowRect(Now_Window->Get_Window_HWND(), &Now_Window->Get_Infomation());
								Now_Window->Get_Infomation().right -= Now_Window->Get_Infomation().left; Now_Window->Get_Infomation().bottom -= Now_Window->Get_Infomation().top;
								GetWindowRect(Now_Window->Get_Parent_Window_HWND(), &Parent_Rect);
								Parent_Rect.right = Parent_Rect.left; Parent_Rect.bottom = Parent_Rect.top;//此时Rect仅为减数
								Now_Window->Get_Infomation() = Now_Window->Get_Infomation() - Parent_Rect;//已重载减法
							}
							else
							{
								//桌面窗口
								Now_Window->Get_Parent_Window_HWND() = NULL;
							}
							*/
						}
					}
					break;
				}
				case 1:
				{
					//Changing_Position = true;
					Update_Draw = true; Update_Zero_HWND = true;//可能存在点击的情况
					if (Event.mouseButton.x < Window.getSize().x * 0.33)
					{
						//窗口坐标
						if (Input_Position != ENUM::Input_Position::INP_X_Position)
							Input_Position = ENUM::Input_Position::INP_X_Position;
						else
							Input_Position = ENUM::Input_Position::INP_Y_Position;

						Move_Window(Now_Window, ENUM::Move_Type::XAndY);
					}
					else
					{
						//窗口宽高
						if (Input_Position != ENUM::Input_Position::INP_Wide)
							Input_Position = ENUM::Input_Position::INP_Wide;
						else
							Input_Position = ENUM::Input_Position::INP_Hight;
						Move_Window(Now_Window, ENUM::Move_Type::WideAndHight);
					}
					break;
				}
				case 2:
				{
					//换页
					Input_Position = ENUM::Input_Position::INP_NULL;
					if (Event.mouseButton.x < Window.getSize().x * 0.33)
					{
						//上一个
						if (Now_Window->Get_Last_Window_ptr() != nullptr)
						{
							Now_Window = Now_Window->Get_Last_Window_ptr();
							Now_Window->Update_Window_HWND();
							Repair_W1_W2_Error();

							//判断是否需要删除下一个
							if (!IsWindow(Now_Window->Get_Next_Window_ptr()->Get_Window_HWND()) && Now_Window->Get_Next_Window_ptr()->Get_Next_Window_ptr() != nullptr)
							{
								//My	N	NN!=null
								//K		D	K

								//需要删除下一个
								Window_Infomation* Next_Window = Now_Window->Get_Next_Window_ptr();
								Now_Window->Set_Next_Window_ptr(Next_Window->Get_Next_Window_ptr());
								Next_Window->Get_Next_Window_ptr()->Set_Last_Window_ptr(Now_Window);
								delete Next_Window;
							}

							Update_Draw = true;
						}//有上一个
					}//左侧
					else
					{
						//下一个
						if (Now_Window->Get_Next_Window_ptr() != nullptr)
						{
							//有下一个
							Now_Window = Now_Window->Get_Next_Window_ptr();
							Now_Window->Update_Window_HWND();
							Repair_W1_W2_Error();

							//判断是否需要删除上一个
							if (!IsWindow(Now_Window->Get_Last_Window_ptr()->Get_Window_HWND()))
							{
								//LL	L	My
								//K		D	K

								//需要删除上一个
								Window_Infomation* Last_Window = Now_Window->Get_Last_Window_ptr();
								Now_Window->Set_Last_Window_ptr(Last_Window->Get_Last_Window_ptr());
								if (Last_Window->Get_Last_Window_ptr() != nullptr)
								{
									//有之前的窗口
									Last_Window->Get_Last_Window_ptr()->Set_Next_Window_ptr(Now_Window);
								}
								delete Last_Window;
							}
							Update_Draw = true;
							Update_Zero_HWND = true;
						}
						else
						{
							//没有下一个
							if (Now_Window->Get_Window_HWND() != NULL)
							{
								//且当前不是0-->创建下一个
								Now_Window->Set_Next_Window_ptr(new Window_Infomation);
								Now_Window->Get_Next_Window_ptr()->Set_Last_Window_ptr(Now_Window);
								Now_Window = Now_Window->Get_Next_Window_ptr();
								Update_Draw = true;
								Update_Zero_HWND = true;
							}
							else
							{
								//且当前是0-->搜索PM窗口
								Get_Child_Window(Now_Window);//获取并链接PM子窗口
								Update_Draw = true; //1.0.3.19
							}
						}
					}//右侧
					break;
				}
				case 3:
				{
					//输入句柄和设置视频
					if (Event.mouseButton.x < Window.getSize().x * 0.33)
					{
						Input_Position = ENUM::Input_Position::INP_HWND;
						if (Now_Window->Is_Undered()) Now_Window->Set_Window_Up();
					}
					else
					{
						Set_Quick_Video();//设置快速播放
					}
					break;
				}
				case 4:
				{
					//选择窗口和窗口置顶、启动守护进程
					Input_Position = ENUM::Input_Position::INP_NULL;//输入模式归位
					if (Event.mouseButton.x < Window.getSize().x * 0.33)
					{
						Choise_Window();
					}
					else
					{
						//Set_Focus();//这是好之前的版本了
						//Set_Most_Window(Now_Window->Get_Window_HWND()); 1.0.2.5删除
						if (Now_Window->Is_ffplay_Window())
						{
							//1.0.4.0 启动守护进程
							Keep_Massage(0);
							Update_Draw = true;
						}
						else
						{
							Now_Window->Set_Top_Most(!Now_Window->Is_Top_Most());
							Update_Draw = true;
						}
					}
					break;
				}
				case 5:
				{
					//设置桌面之下、移动窗口、关闭壁纸和保持桌面之下
					Input_Position = ENUM::Input_Position::INP_NULL;//输入模式归位
					if (Event.mouseButton.x < Window.getSize().x * 0.33)
					{
						if (Changing_Position)
						{
							//移动窗口
							Changing_Position = false;
							Input_Position = ENUM::Input_Position::INP_NULL;

							SetWindowPos(Now_Window->Get_Window_HWND(), Now_Window->Is_Top_Most() ? HWND_TOPMOST : HWND_NOTOPMOST , Now_Window->Get_Infomation().left, Now_Window->Get_Infomation().top, Now_Window->Get_Infomation().right, Now_Window->Get_Infomation().bottom, NULL);

							//if (!Window_Infomation::Is_Self_Top_Most())
							//	SetWindowPos(Window_Infomation::Get_Self_Window_HWND(), HWND_NOTOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);//取消置顶
							//1.0.3.9表示我不理解这玩意干啥的
							Update_Draw = true;
						}
						else if (Now_Window->Is_ffplay_Window())
						{
							//1.0.3.19
							Now_Window->Send_Message(VK_ESCAPE,0x00010001);
							//Now_Window->Update_Window_HWND(); //时间太短，更新没有用
							Update_Draw = true; //窗口不存在，需要重新渲染
						}
						else
						{
							//桌面之下
							//Under_or_Up();1.0.2.5
							Now_Window->Set_Window_Under_Or_Up();
							Update_Draw = true;
						}
					}
					else
					{
						//Keep_Under();//保持之下 1.0.2.5删除
						//Record();//1.0.3.14试图添加
						if (Now_Window->Is_ffplay_Window())
						{
							if (IsWindow(Keep_HWND))
								Keep_Massage(ME_CHANGE); //1.0.4.1转移给守护进程处理
							else
								Now_Window->Send_Message(VK_SPACE, 0x00390001); //1.0.3.16加入
						}
						else
							Now_Window->Update_Is_ffplay_Window();
					}
					break;
				}
				default:
					break;
				}//看Y
			}//左侧
			else
			{
				//右侧
				if (Event.mouseButton.button == sf::Mouse::Left)
				{
					//左键
					Input_Position = ENUM::Input_Position::INP_NULL;//不再输入
					Play_Video(Quick_Video_Path[(int)(Event.mouseButton.y / (Window.getSize().y / 4))], Quick_Video_Decoder[(int)(Event.mouseButton.y / (Window.getSize().y / 4))]);
				}
				else
				{
					//其他按键（右键、滚轮）
					switch ((int)(Event.mouseButton.y / (Window.getSize().y / 4)))
					{
					case 0:
						Input_Position = ENUM::Input_Position::INP_Video_1;
						Input_Char_Position = (unsigned)strlen(Quick_Video_Path[0]);
						Update_Zero_HWND = true;
						Update_Draw = true;
						break;
					case 1:
						Input_Position = ENUM::Input_Position::INP_Video_2;
						Input_Char_Position = (unsigned)strlen(Quick_Video_Path[1]);
						Update_Zero_HWND = true;
						Update_Draw = true;
						break;
					case 2:
						Input_Position = ENUM::Input_Position::INP_Video_3;
						Input_Char_Position = (unsigned)strlen(Quick_Video_Path[2]);
						Update_Zero_HWND = true;
						Update_Draw = true;
						break;
					case 3:
						Input_Position = ENUM::Input_Position::INP_Video_4;
						Input_Char_Position = (unsigned)strlen(Quick_Video_Path[3]);
						Update_Zero_HWND = true;
						Update_Draw = true;
						break;
					default:
						break;
					}
				}
			}
			break;
		}
		case sf::Event::MouseMoved://鼠标移动
		{
			//Event.mouseMove.x, Event.mouseMove.y;
			break;
		}
		default:
			break;
		}
	}//处理每一个事件
}

void Event_Key(sf::Event::KeyEvent Key, ENUM::Input_Position Input_Position)
{
	bool Could_Change_HWND = false;
	bool HWND_Changed = false;
	LONG* Change_Long = NULL;

	//Change_Long<<4：这个不赋值的开发员就是逊呐！

	switch (Input_Position)
	{
	case ENUM::Input_Position::INP_HWND:
		Could_Change_HWND = true;
		break;
	case ENUM::Input_Position::INP_X_Position:
		Change_Long = &Now_Window->Get_Infomation().left;
		break;
	case ENUM::Input_Position::INP_Y_Position:
		Change_Long = &Now_Window->Get_Infomation().top;
		break;
	case ENUM::Input_Position::INP_Wide:
		Change_Long = &Now_Window->Get_Infomation().right;
		break;
	case ENUM::Input_Position::INP_Hight:
		Change_Long = &Now_Window->Get_Infomation().bottom;
		break;
	case ENUM::Input_Position::INP_NULL:
	default:
		break;
	}

	switch (Key.code)
	{
	case sf::Keyboard::Escape:
	{
		Window.close();
		break;
	}
	case sf::Keyboard::Enter:
	case sf::Keyboard::Space:
	{
		if (Input_Position != ENUM::Input_Position::INP_NULL)
		{
			Input_Position = ENUM::Input_Position::INP_NULL;
			Update_Draw = true; Update_Zero_HWND = true;
		}
		break;
	}
	//	case sf::Keyboard::Enter:
	//	{
	//		Under_or_Up();
	//		Update_Draw = true;
	//		break;
	//	}
	//	case sf::Keyboard::Space:
	//	{
	//		Set_Focus();
	//		break;
	//	}
	case sf::Keyboard::BackSpace:
	{
		if (Could_Change_HWND)
		{
			Now_Window->Change_Window_HWND('/', HEX_Mode ? 16 : 10);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode ? *Change_Long >>= 4 : *Change_Long /= 10;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::Num0:
	case sf::Keyboard::Numpad0:
	{
		if (Could_Change_HWND)
		{
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			//Now_Window->Change_Window_HWND('+',0;
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			//*Change_Long += 0;//加了个寂寞
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::Num1:
	case sf::Keyboard::Numpad1:
	{
		if (Could_Change_HWND)
		{
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 1);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 1;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::Num2:
	case sf::Keyboard::Numpad2:
	{
		if (Could_Change_HWND)
		{
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 2);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 2;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::Num3:
	case sf::Keyboard::Numpad3:
	{
		if (Could_Change_HWND)
		{
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 3);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 3;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::Num4:
	case sf::Keyboard::Numpad4:
	{
		if (Could_Change_HWND)
		{
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 4);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 4;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::Num5:
	case sf::Keyboard::Numpad5:
	{
		if (Could_Change_HWND)
		{
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 5);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 5;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::Num6:
	case sf::Keyboard::Numpad6:
	{
		if (Could_Change_HWND)
		{
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 6);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 6;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::Num7:
	case sf::Keyboard::Numpad7:
	{
		if (Could_Change_HWND)
		{
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 7);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 7;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::Num8:
	case sf::Keyboard::Numpad8:
	{
		if (Could_Change_HWND)
		{
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 8);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 8;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::Num9:
	case sf::Keyboard::Numpad9:
	{
		if (Could_Change_HWND)
		{
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 9);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 9;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::A:
	{
		if (Could_Change_HWND)
		{
			HEX_Mode = true;
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 0xA);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode = true;
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 0xA;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::B:
	{
		if (Could_Change_HWND)
		{
			HEX_Mode = true;
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 0xB);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode = true;
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 0xB;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::C:
	{
		if (Could_Change_HWND)
		{
			HEX_Mode = true;
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 0xC);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode = true;
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 0xC;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::D:
	{
		if (Could_Change_HWND)
		{
			HEX_Mode = true;
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 0xD);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode = true;
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 0xD;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::E:
	{
		if (Could_Change_HWND)
		{
			HEX_Mode = true;
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 0xE);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode = true;
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 0xE;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::F:
	{
		if (Could_Change_HWND)
		{
			HEX_Mode = true;
			Now_Window->Change_Window_HWND('*', HEX_Mode ? 16 : 10);
			Now_Window->Change_Window_HWND('+', 0xF);
			HWND_Changed = true;
		}
		if (Change_Long != NULL)
		{
			HEX_Mode = true;
			HEX_Mode ? *Change_Long <<= 4 : *Change_Long *= 10;
			*Change_Long += 0xF;
			Changing_Position = true;
			Update_Draw = true;
		}
		break;
	}
	case sf::Keyboard::X:
	{
		HEX_Mode = !HEX_Mode;
		Update_Draw = true;
		break;
	}
	default:
		break;
	}

	//交给对象处理
	//if (HWND_Changed)
	//{
	//	if (IsWindow(Now_Window->Get_Window_HWND())
	//	{
	//		//成功该句柄存在
	//		Now_Window->Get_Infomation().right -= Now_Window->Get_Infomation().left; Now_Window->Get_Infomation().bottom -= Now_Window->Get_Infomation().top;
	//		if (GetAncestor(Now_Window->Get_Window_HWND(), GA_ROOT) != Now_Window->Get_Window_HWND())//不是自己
	//		{
	//			//一定有父窗口
	//			Now_Window->Get_Parent_Window_HWND() = GetAncestor(Now_Window->Get_Window_HWND(), GA_PARENT);//获取父窗口
	//			if (PM_Window_HWND == NULL) Get_PM_Window_HWND(PM_Window_HWND);//获取PM窗口句柄
	//			//判断是否是PM窗口
	//			if (Now_Window->Get_Parent_Window_HWND() != PM_Window_HWND)
	//			{
	//				RECT Parent_Rect;
	//				GetWindowRect(Now_Window->Get_Parent_Window_HWND(), &Parent_Rect);
	//				Now_Window->Get_Infomation() -= &Parent_Rect;//已重载偏移减法
	//			}
	//			else
	//			{
	//				//PM的子窗口，极有可能是不正常退出导致的
	//				Now_Window->Get_Parent_Window_HWND() = NULL;
	//			}
	//		}
	//		else
	//		{
	//			Now_Window->Get_Parent_Window_HWND() = NULL;//父窗口是桌面
	//		}
	//	}
	//	else
	//	{
	//		//函数失败
	//		Now_Window->Get_Infomation() = { 0,0,0,0 };//格式归0
	//	}
	Update_Draw = true;
	Update_Zero_HWND = true;
}
