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
/// ���캯��
/// </summary>
Window_Infomation::Window_Infomation()
{
	if (!IsWindow(Window_Infomation::PM_Window_HWND))
	{
		//��ȡPM���ڵľ��
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
/// ���캯��
/// </summary>
/// <param name="Window_HWND">���ھ��</param>
Window_Infomation::Window_Infomation(HWND Window_HWND)
{
	if (!IsWindow(Window_Infomation::PM_Window_HWND))
	{
		//��ȡPM���ڵľ��
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
/// ��������
/// </summary>
Window_Infomation::~Window_Infomation()
{
	Window_Infomation::All_Window_Number--;
}

/// <summary>
/// ��ȡ���㴰������
/// </summary>
/// <returns>�Ƿ�Ϊ���㴰��</returns>
 bool Window_Infomation::Is_Top_Most()
{
	return this->Window_Top_Most;
}

/// <summary>
/// ���ö��㴰������
/// </summary>
/// <param name="Flag">�Ƿ�����Ϊ���㴰��</param>
 void Window_Infomation::Set_Top_Most(bool Flag)
{
	if (Flag)
		SetWindowPos(this->Window_HWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	else
		SetWindowPos(this->Window_HWND, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	
	if (this->Window_HWND == this->Self_Window_HWND)
		Window_Infomation::Set_Self_Top_Most(Flag);//�Լ���ר�õĺ���
	else
		SetWindowPos(this->Self_Window_HWND, this->Self_Window_Top_Most ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	this->Window_Top_Most = Flag;
}

 /// <summary>
 /// ��ȡ�Ƿ���ffplay�Ķ�̬��ֽ
 /// </summary>
 /// <returns>�Ƿ���ffplay�Ķ�̬��ֽ</returns>
 bool Window_Infomation::Is_ffplay_Window()
 {
	 return this->Window_Is_ffplay;
 }

 /// <summary>
 /// �����Ƿ���ffplay�Ķ�̬��ֽ
 /// </summary>
 /// <param name="Flag"�Ƿ���ffplay�Ķ�̬��ֽ></param>
 void Window_Infomation::Set_ffplay_Window(bool Flag)
 {
	 this->Window_Is_ffplay = Flag;
 }

 /// <summary>
 /// �Զ��ж��Ƿ���ffplay�Ķ�̬��ֽ
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
 ///// ��ȡ�Ƿ��Ѿ���ͣ
 ///// ����ffplay���ڽ�����false
 ///// </summary>
 ///// <returns>�Ƿ���ͣ</returns>
 //bool Window_Infomation::Is_Pause()
 //{
	// return Window_Is_ffplay && Window_Is_Pause;
 //}

 ///// <summary>
 ///// �����Ƿ��Ѿ���ͣ
 ///// </summary>
 ///// <param name="Flag">�Ƿ���ͣ</param>
 //void Window_Infomation::Set_Pause(bool Flag)
 //{
	// Window_Is_Pause = Flag;
 //}

 /// <summary>
 /// ��ͣffplay
 /// ���Ϳո����Ϣ
 /// </summary>
 void Window_Infomation::Pause_ffplay()
 {
	 if (!Window_Is_ffplay) return;

	 PostMessageA(Window_HWND, WM_KEYDOWN, VK_SPACE, 0x00390001);
	 PostMessageA(Window_HWND, WM_CHAR, VK_SPACE, 0x00390001);
	 PostMessageA(Window_HWND, WM_KEYUP, VK_SPACE, 0xC0390001);
 }

/// <summary>
/// ���´�����Ϣ
/// </summary>
void Window_Infomation::Update_Infomation()
{
	//������Update_Window����
	if (!IsWindow(this->Window_HWND)) return;//NULL�����ֱ�ӷ���
	GetWindowRect(this->Window_HWND, &this->Window_Now_RECT);

	this->Window_Now_RECT.right -= this->Window_Now_RECT.left;
	this->Window_Now_RECT.bottom -= this->Window_Now_RECT.top;

	//�Ը����ڽ���ƫ��
	if (this->Window_Parent_HWND != NULL)
	{
		RECT Parent_RECT{};
		GetWindowRect(this->Window_Parent_HWND, &Parent_RECT);

		this->Window_Now_RECT.top -= Parent_RECT.top; this->Window_Now_RECT.bottom -= Parent_RECT.top;
		this->Window_Now_RECT.left -= Parent_RECT.left; this->Window_Now_RECT.right -= Parent_RECT.right;
	}

	//�����ö����
	this->Window_Top_Most = GetWindowLong(this->Window_HWND, GWL_EXSTYLE) & WS_EX_TOPMOST;
	if (this->Window_HWND == this->Self_Window_HWND) this->Self_Window_Top_Most = this->Window_Top_Most;

	//��������֮�����
	this->Window_Undered = GetAncestor(this->Window_HWND, GA_PARENT) == Window_Infomation::Get_PM_Window_HWND() || GetAncestor(this->Window_HWND, GA_PARENT) == Window_Infomation::W1_Window_HWND;//PM�����»���W1������
}

RECT& Window_Infomation::Get_Infomation()
{
	return this->Window_Now_RECT;
}

/// <summary>
/// ���ô��ھ��
/// ͬʱ�Զ����ø�����
/// </summary>
/// <param name="Window_HWND">���ھ��</param>
void Window_Infomation::Set_Window_HWND(HWND Window_HWND)
{
	if (this->Window_Undered)
		this->Set_Window_Up();

	this->Window_HWND = Window_HWND;
	this->Update_Window_HWND();
}

/// <summary>
/// ��ȡ���ھ��
/// </summary>
/// <returns>���ھ��</returns>
HWND Window_Infomation::Get_Window_HWND()
{
	return this->Window_HWND;
}

/// <summary>
/// ���¾��
/// ���������ڡ�λ����Ϣ�ĸ���
/// </summary>
void Window_Infomation::Update_Window_HWND()
{
	if (GetAncestor(this->Window_HWND, GA_ROOT) != this->Window_HWND)
	{
		//�������Լ�
		if (GetAncestor(this->Window_HWND, GA_PARENT) == this->Get_PM_Window_HWND())//����PM����
			this->Window_Parent_HWND = NULL;//û��
		else if (GetAncestor(this->Window_HWND, GA_PARENT) == Window_Infomation::W1_Window_HWND)//����W1����
			Window_Infomation::W1_W2_Error = true;//W1��W2����
		else
			this->Window_Parent_HWND = GetAncestor(this->Window_HWND, GA_PARENT);//�е�
	}
	else
	{
		//�����Լ�->�޸�����
		this->Window_Parent_HWND = NULL;
	}
	this->Update_Infomation();
}

/// <summary>
/// ���ľ��
/// </summary>
/// <param name="Flag">�������+-*/</param>
/// <param name="Number">����</param>
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
/// ��ȡ�����ھ��
/// </summary>
/// <returns>�����ھ��</returns>
HWND Window_Infomation::Get_Parent_Window_HWND()
{
	return this->Window_Parent_HWND;
}

/// <summary>
/// ��ȡ�Ƿ�������֮��
/// </summary>
/// <returns>�Ƿ�������֮��</returns>
bool Window_Infomation::Is_Undered()
{
	return this->Window_Undered;
}

/// <summary>
/// �ƶ�����
/// </summary>
/// <param name="X_Position">Xλ��</param>
/// <param name="Y_Position">Yλ��</param>
/// <param name="Width">���</param>
/// <param name="Hight">����</param>
/// <param name="Delta_Mode">�Ƿ�������˶�</param>
void Window_Infomation::Move(int X_Position, int Y_Position, int Width, int Hight,bool Delta_Mode)
{
	if (!IsWindow(this->Window_HWND)) return;
	if (Delta_Mode)
	{
		SetWindowPos(this->Window_HWND, NULL, this->Window_Now_RECT.left + X_Position, Window_Now_RECT.top + Y_Position, Window_Now_RECT.right + Width, Window_Now_RECT.bottom + Hight, NULL);
	}
	else
	{
		//��������
		SetWindowPos(this->Window_HWND, NULL, X_Position, Y_Position, Width, Hight, NULL);
	}
//	SetWindowPos(this->Self_Window_HWND, this->Self_Window_Top_Most ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	this->Update_Infomation();//����Ӱ�츸����
}

/// <summary>
/// �Զ��ж��ƶ����ڵ�����֮�»�������֮��
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
/// �ƶ����ڵ�����֮��
/// </summary>
void Window_Infomation::Set_Window_Under()
{
	if (this->Window_HWND != NULL && Window_HWND != this->Self_Window_HWND)
	{
		GetWindowRect(Window_HWND, &this->Window_Oreginal_RECT);//������Ϣ
		this->Window_Oreginal_RECT.right -= this->Window_Oreginal_RECT.left;
		this->Window_Oreginal_RECT.bottom -= this->Window_Oreginal_RECT.top;//ת��info

		if (this->Window_Parent_HWND != NULL)
		{
			//�и�����
			RECT Parent_Rect;
			GetWindowRect(this->Window_Parent_HWND, &Parent_Rect);

			this->Window_Oreginal_RECT.left -= Parent_Rect.left;
			this->Window_Oreginal_RECT.right -= Parent_Rect.left;
			this->Window_Oreginal_RECT.top -= Parent_Rect.top;
			this->Window_Oreginal_RECT.bottom -= Parent_Rect.top;
		}
		SetParent(this->Window_HWND, Window_Infomation::Get_PM_Window_HWND());//����Ƶ��������ΪPM���Ӵ��� ʹ�ú������Զ��޸�PM���ڻָ�
		SetWindowPos(Window_HWND, NULL, 0, 0, this->DeskTop_Size[0], this->DeskTop_Size[1], NULL);//ȫ��
		this->Window_Undered = true;
	}
	return;
}

/// <summary>
/// �ƶ����ڵ�����֮��
/// </summary>
void Window_Infomation::Set_Window_Up()
{
	SetParent(this->Window_HWND, Window_Parent_HWND);//NULL������
	if (this->Window_Oreginal_RECT.right == 0 || this->Window_Oreginal_RECT.bottom == 0)
	{
		//�쳣��� û�����ݣ���Ҫ�ֶ�������С
		this->Window_Oreginal_RECT.top = (int)(DeskTop_Size[0] * 0.25);
		this->Window_Oreginal_RECT.left = (int)(DeskTop_Size[1] * 0.25);
		this->Window_Oreginal_RECT.right = (int)(DeskTop_Size[0] * 0.5);
		this->Window_Oreginal_RECT.bottom = (int)(DeskTop_Size[1] * 0.5);
	}
	SetWindowPos(this->Window_HWND, NULL, this->Window_Oreginal_RECT.left, this->Window_Oreginal_RECT.top, Window_Oreginal_RECT.right, this->Window_Oreginal_RECT.bottom, NULL);//��ԭλ��
	this->Window_Undered = false;

	return;
}

/// <summary>
/// �޸�Win+Tab���W1��W2�쳣
/// </summary>
void Window_Infomation::Repair_W1_W2()
{
	if (this->Is_Undered() && this->Get_Window_HWND() != Self_Window_HWND)//IsWindow����SetParent()
		SetParent(this->Window_HWND, Window_Infomation::Get_PM_Window_HWND());//PM���Ӵ���
}

/// <summary>
/// ������һ��ָ��
/// </summary>
/// <param name="Next_Window_Infomation_ptr">��һ��ָ��</param>
void Window_Infomation::Set_Next_Window_ptr(Window_Infomation* Next_Window_Infomation_ptr)
{
	this->Next_Window_ptr = Next_Window_Infomation_ptr;
}

/// <summary>
/// ������һ��ָ��
/// </summary>
/// <param name="Last_Window_Infomation_ptr">��һ��ָ��</param>
void Window_Infomation::Set_Last_Window_ptr(Window_Infomation* Last_Window_Infomation_ptr)
{
	this->Last_Window_ptr = Last_Window_Infomation_ptr;
}

/// <summary>
/// ��ȡ��һ��ָ��
/// </summary>
/// <returns>��һ��ָ��</returns>
Window_Infomation* Window_Infomation::Get_Next_Window_ptr()
{
	return this->Next_Window_ptr;
}

/// <summary>
/// ��ȡ��һ��ָ��
/// </summary>
/// <returns>��һ��ָ��</returns>
Window_Infomation* Window_Infomation::Get_Last_Window_ptr()
{
	return this->Last_Window_ptr;
}

/// <summary>
/// ��ȡ���󡪡��������������Ƿ�������֮�¡��Ƿ��Ƕ��㴰�ڡ�XY������
/// </summary>
/// <param name="Stream">�����������</param>
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
/// ������󡪡��������������Ƿ�������֮�¡��Ƿ��Ƕ��㴰�ڡ�XY������
/// </summary>
/// <param name="Stream">�����������</param>
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
/// ��ȡ�����Ƿ����ö�
/// </summary>
/// <returns>�Ƿ��ö�</returns>
bool Window_Infomation::Is_Self_Top_Most()
{
	return Window_Infomation::Self_Window_Top_Most;
}

/// <summary>
/// ���������ö�
/// </summary>
/// <param name="Flag">�Ƿ��ö�</param>
void Window_Infomation::Set_Self_Top_Most(bool Flag)
{
	if (Flag)
		SetWindowPos(Window_Infomation::Self_Window_HWND, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	else
		SetWindowPos(Window_Infomation::Self_Window_HWND, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	Window_Infomation::Self_Window_Top_Most = Flag;
}

/// <summary>
/// ��������ǰ̨
/// </summary>
void Window_Infomation::Set_Self_Up()
{
	SetWindowPos(Window_Infomation::Self_Window_HWND, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

/// <summary>
/// ��ȡ��������
/// </summary>
/// <returns>����</returns>
int Window_Infomation::Get_Window_Number()
{
	return Window_Infomation::All_Window_Number;
}

/// <summary>
/// ��ȡ����ľ��
/// </summary>
/// <returns>���ھ��</returns>
HWND Window_Infomation::Get_Self_Window_HWND()
{
	return Window_Infomation::Self_Window_HWND;
}

void Window_Infomation::Find_Self_Window_HWND()
{
	if (Self_Window_Exist)
	{
		//��ȡ�Լ��ľ��
		Self_Window_HWND = FindWindow(L"SFML_Window", L"����֮��");//��ȡ�Լ��ľ��

		if (!IsWindow(Self_Window_HWND))
			Self_Window_HWND = FindWindow(L"SFML_Window", NULL);//���Ʋ��ԣ��ٴ�Ѱ��
	}
}

/// <summary>
/// ��ȡPM���ھ��
/// </summary>
/// <returns>PM���ھ��</returns>
HWND Window_Infomation::Get_PM_Window_HWND()
{
	if (!IsWindow(PM_Window_HWND))
	{
		//��ȡPM���ڵľ��
		Window_Infomation::PM_Window_HWND = Get_And_Hide_PM_Window_HWND();
	}
	return Window_Infomation::PM_Window_HWND;
}

HWND Window_Infomation::Get_W12_Window_HWND(int Number)
{
	return Number == 2 ? W2_Window_HWND : W1_Window_HWND;
}
/// <summary>
/// �����Լ����ڴ������
/// </summary>
/// <param name="Flag">�Լ������Ƿ����</param>
void Window_Infomation::Set_Self_Window_Exist(bool Flag)
{
	Self_Window_Exist = Flag;
}

/// <summary>
/// ��ȡ�Լ����ڴ������
/// </summary>
/// <returns>�Լ������Ƿ����</returns>
bool Window_Infomation::Get_Self_Window_Exist()
{
	return Self_Window_Exist;
}

/// <summary>
/// ��ȡW1��W2����״��
/// </summary>
/// <returns>W1��W2�Ƿ����</returns>
bool Window_Infomation::Get_W1_W2_Error()
{
	return Window_Infomation::W1_W2_Error;
}

/// <summary>
/// ����W1��W2����
/// </summary>
void Window_Infomation::Reset_W1_W2_Error()
{
	Window_Infomation::W1_W2_Error = false;
}

/// <summary>
/// ��ȡPM���ڵľ����������PM_W2����
/// </summary>
/// <returns>PM���ڵľ��</returns>
HWND Window_Infomation::Get_And_Hide_PM_Window_HWND()
{
	HWND PM_Window_HWND = FindWindow(L"Progman", 0);//�ҵ�PM����
	SendMessageTimeout(PM_Window_HWND, 0x52C, 0, 0, 0, 100, 0);//������������Ϣ
	Window_Infomation::Find_And_Hide_PM_W2_Window();//�ҵ��ڶ���WorkerW���ڲ�������
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
/// ����PM_W2���ڵĻص�����
/// </summary>
BOOL CALLBACK Window_Infomation::Hide_PM_W2_Window_Callback(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);//��hwnd�²�����Ϊ������ͼ�ꡱ�Ĵ���
	if (hDefView != 0)//�ҵ�����ͼ�꣬�ң�hwnd��һ����W1
	{
		// ��������һ�����ڣ�����ΪWorkerW��������
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);//�����棨0����Ѱ�� W1��hwnd��֮�� ��Ϊ��W�����ڣ�W2��
		ShowWindow(hWorkerw, SW_HIDE);//����W2
		Window_Infomation::W1_Window_HWND = hwnd;//����W1����
		Window_Infomation::W2_Window_HWND = hWorkerw;//����W2����
		return FALSE;
	}
	return TRUE;
}
