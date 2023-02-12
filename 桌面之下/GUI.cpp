#include "Variable.h"

/*
* SFML:https://www.sfml-dev.org/tutorials
* ���ļ�����GUI
*/

sf::RenderWindow Window;//����
//sf::Vector2u Window_Size(0, 0);//�Լ��Ĵ�С1.0.2.3ɾ��
sf::Font Text_Fort;//����
char Font_Name[Font_Name_Lenght] = "msyh.ttc";//��������
sf::Image Progream_Icon;//ͼ��
//HWND Self_HWND;//�Լ��ľ��
HWND Console_HWND;//����̨�ľ��
HWND Keep_HWND;//�ػ����̵ľ�� 1.0.4.0
char FFplay_Path[FFplay_Path_Lenght] = ".\\ffplay.exe";//ffplay·��
char FFmpeg_Path[FFplay_Path_Lenght] = ".\\ffmpeg.exe";//ffmpeg·��
char Keep_Path[Keep_Path_Lenght] = ".\\�ػ�����.exe";//�ػ�����·��
char Setting_Floor_Path[Setting_File_Lenght] = ".\\����֮��\\";//�����ļ��е�·��
char Setting_File_Name[Setting_File_Lenght] = "setting.txt";//�����ļ�������

//extern char Video_Recorder[Recorder_Lenght];//��Ƶ¼����Դ
extern char Audio_Recorder[Recorder_Lenght];//��Ƶ¼����Դ

char Quick_Video_Path[4][Video_Path_Lenght] = { "" };//���ٲ��ŵ���Ƶ·��
char Quick_Video_Name[4][20] = { "" };//���ٲ���ͼƬ����
char Quick_Video_Decoder[4][Video_Decoder_Lenght] = { "" };//���ٲ��Ž�����
sf::Texture Quick_Video_Texture[4];//���ٲ���ͼƬ
ENUM::Input_Position Input_Position = ENUM::Input_Position::INP_NULL;//��ʾ���������λ��
unsigned Input_Char_Position = 0;//������ַ�λ��

int SPF = (int)(1000.0f / 30.0f);//��Ⱦ֡�ʵĵ���
bool Show_Console = false;//��ʾ����̨
//bool Keep_Undered = false;//��������֮�� 1.0.2.5ɾ��
bool HEX_Mode = false;//ʮ���������
//bool Self_Most_Top = false;//�Ƿ��ö�
bool Changing_Position = false;//����λ����(������ʾ����λ��)
bool Update_Zero_HWND = false;//ǿ����Ⱦ0
bool Update_Draw = true;//������Ⱦ
//bool Update_Position = true;//����λ��

int main(const int argc, const char* argv[]);
void Initialize();//��ʼ��
void EnEtialize();//�����ͷ�
//void Print();//��Ⱦ
//void Sleep();//��Ϣ
//void Event();//�¼�����
void Event_Key(sf::Event::KeyEvent Key, ENUM::Input_Position Input_Position);//�������


int main(const int argc, const char* argv[])
{
	std::cout << Progream_Version << endl;
#ifdef _DEBUG
	std::cout << "Debug�汾" << endl;
#endif // _DEBUG
#ifndef _WIN64
	std::cout << "32λ�汾" << endl;
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
	Get_File_Setting(Setting_File_Path);//��ȡ����
	Get_Child_Window(Now_Window);//��ȡ������PM�Ӵ���
	Window.create(sf::VideoMode(Self_Original_Size[0], Self_Original_Size[1]), L"����֮��");//��������

	//����ͼ��
	{
		char Icon_Path[Setting_File_Lenght] = "";
		sprintf_s(Icon_Path, Setting_File_Lenght, "%sicon.png", Setting_Floor_Path);
		if (Progream_Icon.loadFromFile(Icon_Path))
			Window.setIcon(Progream_Icon.getSize().x, Progream_Icon.getSize().y, Progream_Icon.getPixelsPtr());
	}

	Console_HWND = FindWindow(L"ConsoleWindowClass", NULL);//��ȡ���
	if (Console_HWND != NULL && !Show_Console) ShowWindow(Console_HWND, SW_HIDE);//���ش���
	Window_Infomation::Set_Self_Window_Exist(true);
	Window_Infomation::Find_Self_Window_HWND();
	Keep_HWND = FindWindow(L"�ػ�����", L"�ػ�����");//1.0.4.1

	SPF = 33;//����֡��

	wchar_t System_Floor[50] = L"";
	char Fonts_Path[50] = "";
	GetSystemDirectory(System_Floor,50);//C:\windows\system32
	System_Floor[1] = L'\0';//��ֹC
	sprintf_s(Fonts_Path, "%ws:/Windows/Fonts/%s", System_Floor, Font_Name);//����·��
	if (!Text_Fort.loadFromFile(Fonts_Path))
	{
		//ʧ�ܺ��Զ�����msyh.ttf;
		if (strcmp(Fonts_Path, "msth.ttc"))
		{
			Fonts_Path[7] = 'f';//ttc->ttf
			Text_Fort.loadFromFile(Fonts_Path);
		}
	}

	//Self_HWND = FindWindow(L"SFML_Window", L"����֮��");//��ȡ�Լ��ľ�� 1.0.2.5��������ʵ��

	DeskTop_Size[0] = GetSystemMetrics(SM_CXSCREEN); DeskTop_Size[1] = GetSystemMetrics(SM_CYSCREEN);
	Update_Draw = true; //Update_Position = true;
}

void EnEtialize()
{
	if (File_Exist)//�����ĵ�ʱ��д��
	{
		char Setting_File_Path[Setting_File_Lenght] = "";
		sprintf_s(Setting_File_Path, Setting_File_Lenght, "%s%s", Setting_Floor_Path, Setting_File_Name);
		Set_File_Setting(Setting_File_Path);
	}
	//�Ƶ���ǰ
	while (Now_Window->Get_Last_Window_ptr() != nullptr)
	{
		Now_Window = Now_Window->Get_Last_Window_ptr();
	}
	//ɾ�����
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
		system("pause");//�ȴ�
	else if (Console_HWND != NULL)
		ShowWindow(Console_HWND, SW_SHOW);//��ʾ����
}

void Print()
{
	static sf::RectangleShape Back_Rectangle[2];
	static sf::RectangleShape Button_Rectangle[12];
	static sf::Sprite Quick_Video_Sprite[4];
	static bool Update_Position = true;//��һ�γ�ʼ��Ҫ��
	static sf::Text Text[12];
	/*
	0:���
	1:����
	2:X
	3:Y
	4:dX
	5:dY
	6:������
	7:���ÿ�����Ƶ
	8:ѡ�񴰿�
	9:���ý���
	10:����/�ָ�
	11:ffplay·��
	12:�ָ���
	13:������Ϣ���
	*/
	//if (Update_Position)
	//{
	//	Update_Position = false;
	//	Update_Draw = true;
	//	sf::Vector2f Size(Window.getSize().x * 0.25f , Window.getSize().y * 0.1f);//�Ա�1/3��1/6С
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
		Text[0].setString(L"���ھ��");
		Text[1].setString(L"������");
		Text[2].setString(L"��������");
		Text[3].setString(L"���ڳ���");
		Text[4].setString(L"��һ������");
		Text[5].setString(L"��һ������");
		Text[6].setString(L"���봰�ھ��");
		Text[7].setString(L"���ÿ����Ƶ");
		Text[8].setString(L"�϶�ѡ�񴰿�");
		Text[9].setString(L"���ô����ö�");
		Text[10].setString(L"�Ƶ�����֮��");
		Text[11].setString(L"�Ǳ�ֽ����");

		for (int i = 0; i < 4; i++)
		{
			if (Quick_Video_Path[i][0] != '\0')
			{
				Spict_Name(Quick_Video_Path[i], Quick_Video_Name[i], 20);//�ָ�����

				char Video_Texture_Path[Setting_File_Lenght] = { "" };
				sprintf_s(Video_Texture_Path, Setting_File_Lenght, "%s%s.png", Setting_Floor_Path, Quick_Video_Name[i]);
//				std::cout << "����" << Video_Texture_Path << endl;//��Ϣ
				if (!Quick_Video_Texture[i].loadFromFile(Video_Texture_Path))
				{
					Creat_Vidoe_Texture(i);

					//�ٴ��ж�
					//if (!Quick_Video_Texture[i].loadFromFile(Video_Texture_Path))
					//{
					//	std::cout << "����ʧ��" << endl;
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
			//������
			constexpr unsigned Buffer_Size = Video_Path_Lenght + 10;
			wchar_t Buffer[Buffer_Size] = L"";

			swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"0x%llX" : L"%lld", (long long)Now_Window->Get_Window_HWND());
			//#:ʮ����������ʾ0x ll�� Xʮ�������� ����#�Ĵ�Сд����X
			Text[0].setString(Buffer);//���

			//����
			if (ENUM::Input_Position::INP_Video_1 <= Input_Position && Input_Position <= ENUM::Input_Position::INP_Video_4)
			{
				//���ж�����
				wchar_t Video_Path_W[Video_Path_Lenght] = L"";
				CharToTchar(Quick_Video_Path[(short)Input_Position - (short)ENUM::Input_Position::INP_Video_1], Video_Path_W, CP_ACP);

				swprintf_s(Buffer, Buffer_Size, L"��Ƶ%d:%ws", (short)Input_Position - (short)ENUM::Input_Position::INP_Video_1 + 1, Video_Path_W);
			}
			else if (Now_Window->Get_Parent_Window_HWND() == NULL)
			{
				GetWindowText(Now_Window->Get_Window_HWND(), Buffer, Buffer_Size);//��ʾ����
				if (Buffer[0] == L'\0')
				{
					//û�з��ر���
					if (IsWindow(Now_Window->Get_Window_HWND()))
						wcsncpy_s(Buffer, Buffer_Size, L"(�ձ���)", 6);//���ڲ�����һ��6����(\0)
					else
						wcsncpy_s(Buffer, Buffer_Size, L"(���ڲ�����)", 8);//���ڲ�����һ��8����(\0)
				}
			}
			else
			{
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"������:0x%llX" : L"������:%lld", (long long)Now_Window->Get_Parent_Window_HWND());//��ʾ������ #:ʮ����������ʾ0x ll�� Xʮ�������� ����#�Ĵ�Сд����X
			}
			Text[1].setString(Buffer);

			//����
			switch (Input_Position)
			{
			case ENUM::Input_Position::INP_X_Position:
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"X:0x%lX" : L"X:%ld", Now_Window->Get_Infomation().left);
				break;
			case ENUM::Input_Position::INP_Y_Position:
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"Y:0x%lX" : L"Y:%ld", Now_Window->Get_Infomation().top);
				break;
			default:
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"����:0x%lX,0x%lX" : L"����:%ld,%ld", Now_Window->Get_Infomation().left, Now_Window->Get_Infomation().top);
				break;
			}
			Text[2].setString(Buffer);

			//���
			switch (Input_Position)
			{
			case ENUM::Input_Position::INP_Wide:
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"��:0x%lX" : L"��:%ld", Now_Window->Get_Infomation().right);
				break;
			case ENUM::Input_Position::INP_Hight:
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"��:0x%lX" : L"��:%ld", Now_Window->Get_Infomation().bottom);
				break;
			default:
				swprintf_s(Buffer, Buffer_Size, HEX_Mode ? L"���:0x%lX,0x%lX" : L"���:%ld,%ld", Now_Window->Get_Infomation().right, Now_Window->Get_Infomation().bottom);
				break;
			}
			Text[3].setString(Buffer);
		}
		if (Now_Window->Is_ffplay_Window())
		{
			//1.0.4.0
			if (Keep_HWND == 0)
			{
				//û��
				Text[9].setString(L"�����ػ�����");
			}
			else
			{
				Text[9].setString(L"�Ѵ����ػ�����");
			}
		}
		else
		{
			if (Now_Window->Is_Top_Most())
				Text[9].setString(L"ȡ�������ö�");
			else
				Text[9].setString(L"���ô����ö�");
		}

		if (Changing_Position)
			Text[10].setString(L"����λ��");
		else if (Now_Window->Is_ffplay_Window())
		{
			Text[10].setString(L"�رձ�ֽ"); //1.0.3.19
		}
		else
		{
			if (Now_Window->Is_Undered())
				Text[10].setString(L"�ָ�����");
			else
				Text[10].setString(L"�Ƶ�����֮��");
		}

		/*
		if (Keep_Undered)
			Text[11].setString(L"�˳�ʱ����");
		else
			Text[11].setString(L"�˳�ʱ�ָ�");
		*/

		if (!Now_Window->Is_ffplay_Window()) //1.0.3.16����
			Text[11].setString(L"�Ǳ�ֽ����");
		else
			Text[11].setString(L"��ͣ��ֽ");


		Window.clear(sf::Color(0x99FFFFFF));
		for (int i = 0; i < 2; i++)
			Window.draw(Back_Rectangle[i]);
		for (int i = 0; i < 12; i++)
			Window.draw(Button_Rectangle[i]);

		//��ͼƬ
		for (int i = 0; i < 4; i++)
			Window.draw(Quick_Video_Sprite[i]);
		//����
		for (int i = 0; i < 12; i++)
			Window.draw(Text[i]);

		Window.display();//��ʾ
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
	sf::Event Event;//�¼�
	while (Window.pollEvent(Event))//����ÿһ���¼�
	{
		switch (Event.type)
		{
		case sf::Event::Closed://�رմ���
		{
			Window.close();
			break;
		}
		case sf::Event::Resized://������С
		{
			constexpr float Self_Original_Size_K = (float)Self_Original_Size[0] / (float)Self_Original_Size[1];
			static bool Operation_Size = true;

			if (Operation_Size)
			{
				Operation_Size = false;

				sf::Vector2u Size = {};
				Size.x = (unsigned)((Event.size.width + Event.size.height * Self_Original_Size_K) / 2);
				Size.y = (unsigned)((Event.size.height + Event.size.width / Self_Original_Size_K) / 2);
//				std::cout << Event.size.width << "x" << Event.size.height << "|" << Size.x << "x" << Size.y << endl;//debug��
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
		case sf::Event::LostFocus://ʧ��
		{
			break;
		}
		case sf::Event::GainedFocus://�۽�
		{
			Update_Draw = true;
			Now_Window->Update_Window_HWND();
			Repair_W1_W2_Error();//����W1��W2����
			break;
		}
		case sf::Event::KeyPressed://����
		{
			if (!Flag)break;
			Event_Key(Event.key, Input_Position);
			break;
		}
		case sf::Event::TextEntered://����
		{
			if (ENUM::Input_Position::INP_Video_1 <= Input_Position && Input_Position <= ENUM::Input_Position::INP_Video_4)
			{
				bool Changed = false;
				wchar_t Input_Video_Path[Video_Path_Lenght] = L"\0";
				CharToTchar(Quick_Video_Path[(short)Input_Position - (short)ENUM::Input_Position::INP_Video_1], Input_Video_Path, CP_ACP);

				//0x8��0x7F
				if (Event.text.unicode == 0x8 || Event.text.unicode == 0x7F)
				{
					//ɾ��
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
					//�س�
					Input_Position = ENUM::Input_Position::INP_NULL;
				}
				else if ((0x0 <= Event.text.unicode && Event.text.unicode <= 0x1F) || (0x7F < Event.text.unicode && Event.text.unicode < 0xA0))
				{
					//NULL
				}
				else
				{
					//����
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
			}//������λ��

			break;
		}
		case sf::Event::MouseButtonPressed://��갴��
		{
			if (!Flag)break;
			if (Event.mouseButton.x < Window.getSize().x * 0.66f)
			{
				//���
				switch ((int)(Event.mouseButton.y / (Window.getSize().y / 6)))
				{
				case 0:
				{
					Update_Draw = true; Update_Zero_HWND = true;
					if (Event.mouseButton.x < Window.getSize().x * 0.33)
					{
						HEX_Mode = !HEX_Mode;
						//Input_Position = ENUM::Input_Position::INP_HWND;//�Ժ���˵
					}
					else
					{
						if (Now_Window->Get_Parent_Window_HWND() == NULL)
						{
							//û�и����ڣ�������
							HEX_Mode = !HEX_Mode;
						}
						else
						{
							//ѡ�񸸴��� 
							Input_Position = ENUM::Input_Position::INP_NULL;//����ģʽ��λ

							//�ڴ��ں����һ��������
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
							* �ж��Ƿ����Ӵ��ڣ�1.0.2.5��ȫȨ����������
							if (GetAncestor(Now_Window->Get_Window_HWND(), GA_ROOT) != Now_Window->Get_Window_HWND())//�����Լ�
							{
								////�����洰��
								RECT Parent_Rect;
								//Now_Window->Get_Parent_Window_HWND() = GetAncestor(Now_Window->Get_Window_HWND(), GA_PARENT);�Զ��ģ������ֶ�����
								GetWindowRect(Now_Window->Get_Window_HWND(), &Now_Window->Get_Infomation());
								Now_Window->Get_Infomation().right -= Now_Window->Get_Infomation().left; Now_Window->Get_Infomation().bottom -= Now_Window->Get_Infomation().top;
								GetWindowRect(Now_Window->Get_Parent_Window_HWND(), &Parent_Rect);
								Parent_Rect.right = Parent_Rect.left; Parent_Rect.bottom = Parent_Rect.top;//��ʱRect��Ϊ����
								Now_Window->Get_Infomation() = Now_Window->Get_Infomation() - Parent_Rect;//�����ؼ���
							}
							else
							{
								//���洰��
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
					Update_Draw = true; Update_Zero_HWND = true;//���ܴ��ڵ�������
					if (Event.mouseButton.x < Window.getSize().x * 0.33)
					{
						//��������
						if (Input_Position != ENUM::Input_Position::INP_X_Position)
							Input_Position = ENUM::Input_Position::INP_X_Position;
						else
							Input_Position = ENUM::Input_Position::INP_Y_Position;

						Move_Window(Now_Window, ENUM::Move_Type::XAndY);
					}
					else
					{
						//���ڿ��
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
					//��ҳ
					Input_Position = ENUM::Input_Position::INP_NULL;
					if (Event.mouseButton.x < Window.getSize().x * 0.33)
					{
						//��һ��
						if (Now_Window->Get_Last_Window_ptr() != nullptr)
						{
							Now_Window = Now_Window->Get_Last_Window_ptr();
							Now_Window->Update_Window_HWND();
							Repair_W1_W2_Error();

							//�ж��Ƿ���Ҫɾ����һ��
							if (!IsWindow(Now_Window->Get_Next_Window_ptr()->Get_Window_HWND()) && Now_Window->Get_Next_Window_ptr()->Get_Next_Window_ptr() != nullptr)
							{
								//My	N	NN!=null
								//K		D	K

								//��Ҫɾ����һ��
								Window_Infomation* Next_Window = Now_Window->Get_Next_Window_ptr();
								Now_Window->Set_Next_Window_ptr(Next_Window->Get_Next_Window_ptr());
								Next_Window->Get_Next_Window_ptr()->Set_Last_Window_ptr(Now_Window);
								delete Next_Window;
							}

							Update_Draw = true;
						}//����һ��
					}//���
					else
					{
						//��һ��
						if (Now_Window->Get_Next_Window_ptr() != nullptr)
						{
							//����һ��
							Now_Window = Now_Window->Get_Next_Window_ptr();
							Now_Window->Update_Window_HWND();
							Repair_W1_W2_Error();

							//�ж��Ƿ���Ҫɾ����һ��
							if (!IsWindow(Now_Window->Get_Last_Window_ptr()->Get_Window_HWND()))
							{
								//LL	L	My
								//K		D	K

								//��Ҫɾ����һ��
								Window_Infomation* Last_Window = Now_Window->Get_Last_Window_ptr();
								Now_Window->Set_Last_Window_ptr(Last_Window->Get_Last_Window_ptr());
								if (Last_Window->Get_Last_Window_ptr() != nullptr)
								{
									//��֮ǰ�Ĵ���
									Last_Window->Get_Last_Window_ptr()->Set_Next_Window_ptr(Now_Window);
								}
								delete Last_Window;
							}
							Update_Draw = true;
							Update_Zero_HWND = true;
						}
						else
						{
							//û����һ��
							if (Now_Window->Get_Window_HWND() != NULL)
							{
								//�ҵ�ǰ����0-->������һ��
								Now_Window->Set_Next_Window_ptr(new Window_Infomation);
								Now_Window->Get_Next_Window_ptr()->Set_Last_Window_ptr(Now_Window);
								Now_Window = Now_Window->Get_Next_Window_ptr();
								Update_Draw = true;
								Update_Zero_HWND = true;
							}
							else
							{
								//�ҵ�ǰ��0-->����PM����
								Get_Child_Window(Now_Window);//��ȡ������PM�Ӵ���
								Update_Draw = true; //1.0.3.19
							}
						}
					}//�Ҳ�
					break;
				}
				case 3:
				{
					//��������������Ƶ
					if (Event.mouseButton.x < Window.getSize().x * 0.33)
					{
						Input_Position = ENUM::Input_Position::INP_HWND;
						if (Now_Window->Is_Undered()) Now_Window->Set_Window_Up();
					}
					else
					{
						Set_Quick_Video();//���ÿ��ٲ���
					}
					break;
				}
				case 4:
				{
					//ѡ�񴰿ںʹ����ö��������ػ�����
					Input_Position = ENUM::Input_Position::INP_NULL;//����ģʽ��λ
					if (Event.mouseButton.x < Window.getSize().x * 0.33)
					{
						Choise_Window();
					}
					else
					{
						//Set_Focus();//���Ǻ�֮ǰ�İ汾��
						//Set_Most_Window(Now_Window->Get_Window_HWND()); 1.0.2.5ɾ��
						if (Now_Window->Is_ffplay_Window())
						{
							//1.0.4.0 �����ػ�����
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
					//��������֮�¡��ƶ����ڡ��رձ�ֽ�ͱ�������֮��
					Input_Position = ENUM::Input_Position::INP_NULL;//����ģʽ��λ
					if (Event.mouseButton.x < Window.getSize().x * 0.33)
					{
						if (Changing_Position)
						{
							//�ƶ�����
							Changing_Position = false;
							Input_Position = ENUM::Input_Position::INP_NULL;

							SetWindowPos(Now_Window->Get_Window_HWND(), Now_Window->Is_Top_Most() ? HWND_TOPMOST : HWND_NOTOPMOST , Now_Window->Get_Infomation().left, Now_Window->Get_Infomation().top, Now_Window->Get_Infomation().right, Now_Window->Get_Infomation().bottom, NULL);

							//if (!Window_Infomation::Is_Self_Top_Most())
							//	SetWindowPos(Window_Infomation::Get_Self_Window_HWND(), HWND_NOTOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE);//ȡ���ö�
							//1.0.3.9��ʾ�Ҳ�����������ɶ��
							Update_Draw = true;
						}
						else if (Now_Window->Is_ffplay_Window())
						{
							//1.0.3.19
							Now_Window->Send_Message(VK_ESCAPE,0x00010001);
							//Now_Window->Update_Window_HWND(); //ʱ��̫�̣�����û����
							Update_Draw = true; //���ڲ����ڣ���Ҫ������Ⱦ
						}
						else
						{
							//����֮��
							//Under_or_Up();1.0.2.5
							Now_Window->Set_Window_Under_Or_Up();
							Update_Draw = true;
						}
					}
					else
					{
						//Keep_Under();//����֮�� 1.0.2.5ɾ��
						//Record();//1.0.3.14��ͼ���
						if (Now_Window->Is_ffplay_Window())
						{
							if (IsWindow(Keep_HWND))
								Keep_Massage(ME_CHANGE); //1.0.4.1ת�Ƹ��ػ����̴���
							else
								Now_Window->Send_Message(VK_SPACE, 0x00390001); //1.0.3.16����
						}
						else
							Now_Window->Update_Is_ffplay_Window();
					}
					break;
				}
				default:
					break;
				}//��Y
			}//���
			else
			{
				//�Ҳ�
				if (Event.mouseButton.button == sf::Mouse::Left)
				{
					//���
					Input_Position = ENUM::Input_Position::INP_NULL;//��������
					Play_Video(Quick_Video_Path[(int)(Event.mouseButton.y / (Window.getSize().y / 4))], Quick_Video_Decoder[(int)(Event.mouseButton.y / (Window.getSize().y / 4))]);
				}
				else
				{
					//�����������Ҽ������֣�
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
		case sf::Event::MouseMoved://����ƶ�
		{
			//Event.mouseMove.x, Event.mouseMove.y;
			break;
		}
		default:
			break;
		}
	}//����ÿһ���¼�
}

void Event_Key(sf::Event::KeyEvent Key, ENUM::Input_Position Input_Position)
{
	bool Could_Change_HWND = false;
	bool HWND_Changed = false;
	LONG* Change_Long = NULL;

	//Change_Long<<4���������ֵ�Ŀ���Ա����ѷ�ţ�

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
			//*Change_Long += 0;//���˸���į
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

	//����������
	//if (HWND_Changed)
	//{
	//	if (IsWindow(Now_Window->Get_Window_HWND())
	//	{
	//		//�ɹ��þ������
	//		Now_Window->Get_Infomation().right -= Now_Window->Get_Infomation().left; Now_Window->Get_Infomation().bottom -= Now_Window->Get_Infomation().top;
	//		if (GetAncestor(Now_Window->Get_Window_HWND(), GA_ROOT) != Now_Window->Get_Window_HWND())//�����Լ�
	//		{
	//			//һ���и�����
	//			Now_Window->Get_Parent_Window_HWND() = GetAncestor(Now_Window->Get_Window_HWND(), GA_PARENT);//��ȡ������
	//			if (PM_Window_HWND == NULL) Get_PM_Window_HWND(PM_Window_HWND);//��ȡPM���ھ��
	//			//�ж��Ƿ���PM����
	//			if (Now_Window->Get_Parent_Window_HWND() != PM_Window_HWND)
	//			{
	//				RECT Parent_Rect;
	//				GetWindowRect(Now_Window->Get_Parent_Window_HWND(), &Parent_Rect);
	//				Now_Window->Get_Infomation() -= &Parent_Rect;//������ƫ�Ƽ���
	//			}
	//			else
	//			{
	//				//PM���Ӵ��ڣ����п����ǲ������˳����µ�
	//				Now_Window->Get_Parent_Window_HWND() = NULL;
	//			}
	//		}
	//		else
	//		{
	//			Now_Window->Get_Parent_Window_HWND() = NULL;//������������
	//		}
	//	}
	//	else
	//	{
	//		//����ʧ��
	//		Now_Window->Get_Infomation() = { 0,0,0,0 };//��ʽ��0
	//	}
	Update_Draw = true;
	Update_Zero_HWND = true;
}
