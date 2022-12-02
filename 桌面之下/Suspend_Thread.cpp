/*

#include "Variable.h"

constexpr int Sleep_Time = 5000;
int Order_Window_Size = 0;
unsigned Order_Percent = 80;

std::thread* ffplay_Thread = nullptr;//ffplay�߳�
bool ffplay_Pause_Thread_Enabled = false;//ffplay�߳��Ƿ�����
std::vector<HWND> ffplay_Windows{};//��ffplay_thread�߳�ʹ��

bool IsForegroundFullscreen();//�ж��Ƿ񳬹���ֵ

void ffplay_Pause_Thread(std::vector<HWND>& ffplay_Windows)
{
	std::cout << "��̬��ֽ�����߳�������" << endl;
	std::vector<HWND>::iterator Now_ffplay_Window = ffplay_Windows.begin();
	bool Pause_ffplay = false;
	ffplay_Pause_Thread_Enabled = true;

	if (Order_Window_Size == 0) Order_Window_Size = DeskTop_Size[0] * DeskTop_Size[1] * Order_Percent / 100;

	while (ffplay_Pause_Thread_Enabled)
	{
		//�жϴ����Ƿ����
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
		//�յĻ��Ͳ����ж���
		if (ffplay_Windows.empty())
		{
			ffplay_Pause_Thread_Enabled = false;
			continue;
		}

		//�鿴�Ƿ���Ҫ�����߳�
		if (IsForegroundFullscreen())
		{
			if (!Pause_ffplay)
			{
				Pause_ffplay = true;
				std::cout << "����̬��ֽ" << endl;
				for (Now_ffplay_Window = ffplay_Windows.begin(); Now_ffplay_Window != ffplay_Windows.end(); Now_ffplay_Window++)
					SuspendThread(*Now_ffplay_Window);//����ÿһ��
			}
		}
		else
		{
			if (Pause_ffplay)
			{
				Pause_ffplay = false;
				std::cout << "�ָ���̬��ֽ" << endl;
				for (Now_ffplay_Window = ffplay_Windows.begin(); Now_ffplay_Window != ffplay_Windows.end(); Now_ffplay_Window++)
					ResumeThread(*Now_ffplay_Window);//�ָ�ÿһ��
			}
		}
		Sleep(Sleep_Time);
	}
	std::cout << "��̬��ֽ�����߳��˳�" << endl;
}

bool IsForegroundFullscreen()
{
	HWND User_Now_Window_HWND;
	RECT User_Now_Window_RECT;

	User_Now_Window_HWND = GetForegroundWindow();//��ȡ��ǰ�������û�������ǰ̨���ھ��

	if ((User_Now_Window_HWND != GetDesktopWindow()) && (User_Now_Window_HWND != GetShellWindow()))//���ǰ̨���ڲ������洰�ڣ�Ҳ���ǿ���̨����
	{
		GetWindowRect(User_Now_Window_HWND, &User_Now_Window_RECT);//��ȡǰ̨���ڵ�����
		if ((User_Now_Window_RECT.right - User_Now_Window_RECT.left) * (User_Now_Window_RECT.bottom - User_Now_Window_RECT.top)
			> Order_Window_Size)
		{
			//ռ�����������ֵ
			return true;
		}
	}//���ǰ̨���������洰�ڣ������ǿ���̨���ڣ���ֱ�ӷ��ز���ȫ��

	return false;
}

*/