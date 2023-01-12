#pragma once
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" ) // 设置入口地址
#include <iostream>
//#include <thread>
//#include <vector>
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Operator.h"
#include "Window_Infomation_Class.h"

/*
* 在这里共用变量和函数
*/

/*
* 更新日志
* 1.0.0.0 初代版本
* 1.0.1.0 重写为GUI 但是有部分功能未实现
* 1.0.1.1 修复选择窗口时不会恢复的bug
* 1.0.1.2 可以选择字体了 文件版本升级为2_储存其他设置信息 拖动直接选择父窗口 选择结束后才会恢复窗口
* 1.0.1.3 分离路径和名称，图片正常加载了
* 1.0.1.4 自动创建缩略图 文件版本改为3_储存ffmpeg路径
* 1.0.1.5 删除设置ffplay 改为退出时保留 文件版本改为4_储存保留窗口信息
* 1.0.1.6 鼠标点击会取消句柄输入了 输入句柄0时也会正常显示了 输入句柄时会自动恢复
* 1.0.1.7 更改保留窗口为显示当前的状态
* 1.0.1.8 切换输入模式 十六进制(x) 十进制(s)
* 1.0.1.9 如果有父窗口会显示父窗口名，恢复时恢复到父窗口中 点击句柄可以转换十六进制了 修复保留窗口后打开时状态为Under的bug 水平拖动X、Y可以移动被选中窗口了 选择窗口时也能处理事件了
* 1.0.1.10 修复鼠标点击切换十六进制时会误触发XY移动的bug（没写break）单击父窗口句柄可以切换到父窗口了 修复错误获取父窗口为桌面窗口的bug 统一显示和输入时的十六进制模式 输入句柄时会清除输入缓存了 调整桌面之下的大小的比例以原始比例为准了
* 1.0.1.11 整合Input_HWND到Key_Event中 添加X、Y、长、宽的输入模式 为了整合将Under_Window_Rect改为Under_Window_Info 删除s更改十进制，改为X切换模式 移动窗口时会置顶窗口了
* 1.0.2.0 (1.0.1.12) 添加修改快速视频功能（按键或右键右侧窗口），但图片不会自动加载 修改设置焦点为置顶窗口 修复窗口在桌面之下时不会把窗口恢复，不能输入句柄的bug 修改回车与空格的功能为退出输入模式 修复窗口在桌面之下时拖动选择窗口自动恢复位置的错误
* 1.0.2.1 添加Video_1_Decoeder(V1D)选项 可以选择解码器
* 1.0.2.2 合并XY和宽高的显示与输入 2022/4/23
* 1.0.2.3 修复调整窗口大小时调整比例的循环调整bug
* 1.0.2.4 修复启动时判断窗口是否在桌面之下的异常（桌面也是父窗口）。修改为在ResetSetting时获取PM窗口，在PM下为在桌面之下，否则不是
* 1.0.3.0 (1.0.2.5)强制保持桌面之下 多个窗口模式，引入Class，将运算交给Class做 修复不选择更新文件后的异常写入bug
* 1.0.3.1 修复不能置顶自己的bug，修复设置置顶后不刷新显示的bug 刷新信息时会自动判断是否是置顶窗口
* 1.0.3.2 尝试(SuspendThread使用ResumeThread控制快速视频的暂停 将ffplay调用更改为单独的线程中调用)但是失败 读取时会判断是否存在，删除窗口的依据改为空窗口而不是0了
* 1.0.3.3 命令行调用支持(-play -version -help -start) 移动Play_Quick_Video到Play_Video 结束后会显示命令行窗口了
* 1.0.3.4 命令行加入(-set -get)，可获取（修改）(ffplay_path ffmpeg_path show_console font Nvideo_path Nvideo_decoder) 加入(-under -up)，可以设置窗口到桌面之下 加入-settingfile，指定设置路径的位置
* 1.0.3.5 修复调用ffplay时的临时内存不够的bug 修复-get_show_console的输出错误 加入对VNP、VND、SC、FN、FPP、FMP简写的支持 自动枚举PM窗口下的子窗口，并自动加入链表中 不再保存是否置顶和是否在桌面之下，改为自动判断 文件版本升为7 窗口不存在和窗口标题为空时会有显示了 修复选择父窗口后不会新建一个窗口的bug 修复第一个窗口永远不会在读取时删除的bug
* 1.0.3.6 禁用命令行调用后Get_File_Setting()的警告信息 添加-up_all来恢复所有窗口 桌面之下命令行调用时也会搜索PM窗口的子窗口 获取全部视频信息时会分行显示第一个信息 2022/5/15
* 1.0.3.7 加入-play_数字_的语法 play的时候会显示参数了 修复-set SC的错误判断bug 支持直接拖拽的语法(桌面之下.exe [-play] Video_Path)
* 1.0.3.8 修复名字过长后无名字的bug 修复分割字符时可能的数组越界
* 1.0.3.9 更改分离名称时内存不足的提示为申请空间不足 替换std::endl为'\n' enum改为enum class:char(bool)减少enum的内存占用 修复不能设置自己的置顶状态的bug(为调用针对自己的特殊函数) 在移动位置前会先更新一次信息，移动后会设置焦点到桌面之下
* 1.0.3.10 重新修复移动窗口时自身置顶的bug，移动窗口时被移窗口会临时取消置顶 修复路径斜杠重复的bug
* 1.0.3.11 PM窗口句柄失效后会自动重搜 修复路径中含有空格时ffplay调用失败的bug（参数中没加引号） 记录W1、W2窗口句柄，如果窗口处于W1下则再次隐藏W2并重新设置桌面之下（防止Win+Tab） 聚焦、切换窗口自动更新窗口信息 ffplay改为低优先级播放
* 1.0.3.12 修复不能正常隐藏ffplay窗口的bug
* 1.0.3.13 加入-repair参数，恢复所有在PM窗口下的bug窗口
* 1.0.3.14 修复调用ffmpeg时的C6335句柄泄露bug
* 1.0.3.15 在最后一项向后将搜索PM的子窗口
* 1.0.3.16 加入暂停ffplay的功能 2023/1/6
* 1.0.3.17 修复识别ffplay需要点一下非壁纸窗口的bug(update_Infomation中自动调用Update_Is_ffplay_Window)
* 1.0.3.18 尝试修复有时ffplay窗口在任务栏显示的bug 配合守护线程完善自我窗口存在的标记 配合守护线程加入获取桌面大小的函数 启动将在控制台输出版本消息
* 
* Next 更改窗口名称 守护线程 指定视频分辨率(-x -y) -repair 加入录制屏幕功能(ffmpeg) 重置设置文件格式 任务栏显示ffplay的bug
*/

#define	Message(STRING) MessageBox(NULL, STRING, L"桌面之下", MB_YESNO)

constexpr unsigned File_Version = 7;
constexpr char Progream_Version[] = "1.0.3.18";
//constexpr unsigned Progream_Version = 2;
constexpr char endl = '\n';

namespace ENUM
{
	enum class Move_Type :bool
	{
		XAndY,
		WideAndHight,
	};
	enum class Input_Position :char
	{
		INP_NULL = NULL,
		INP_HWND,
		INP_X_Position,
		INP_Y_Position,
		INP_Wide,
		INP_Hight,
		INP_Video_1,
		INP_Video_2,
		INP_Video_3,
		INP_Video_4,
	};
};

constexpr unsigned Setting_File_Lenght = 100;
constexpr unsigned Video_Path_Lenght = 100;
constexpr unsigned FFplay_Path_Lenght = 100;
constexpr unsigned Recorder_Lenght = 100;
constexpr unsigned Video_Decoder_Lenght = 20;
constexpr unsigned Font_Name_Lenght = 50;

extern Window_Infomation* Now_Window;
//extern bool Undered;//是否在桌面之下
//extern bool Top_Most;//是否窗口置顶
extern bool File_Exist;//是否存在文件
extern bool Show_Console;//显示控制台
extern char Setting_Floor_Path[Setting_File_Lenght];//设置文件夹的路径
extern char Setting_File_Name[Setting_File_Lenght];//设置文件的名称
extern HWND Console_HWND;//控制台的句柄
//extern bool Keep_Undered;//保持桌面之下 1.0.2.5删除
extern bool HEX_Mode;//十六进制输出
//extern bool Self_Most_Top;//是否置顶
extern bool Changing_Position;//更改位置中
extern bool Update_Draw;//更新渲染
extern bool Update_Zero_HWND;//强制渲染0
//extern bool Update_Position;//更新位置
//extern HWND PM_Window_HWND;//PM窗口句柄
//extern HWND Under_Window_HWND;//窗口句柄
//extern HWND Parent_HWND;//父窗口句柄
//extern HWND Self_HWND;//自己的句柄
//extern RECT Under_Window_Info;//窗口的原信息
constexpr int Self_Original_Size[2] = { 640,340 };//原始窗口大小
extern unsigned DeskTop_Size[2];//桌面的大小
extern char Font_Name[Font_Name_Lenght];//字体名称
extern char FFplay_Path[FFplay_Path_Lenght];//ffplay路径
extern char FFmpeg_Path[FFplay_Path_Lenght];//ffmpeg路径
//extern char Video_Recorder[Recorder_Lenght];//视频录制来源
extern char Audio_Recorder[Recorder_Lenght];//音频录制来源
extern char Quick_Video_Path[4][Video_Path_Lenght];//快速播放路径
extern char Quick_Video_Name[4][20];//快速播放图片名称
extern char Quick_Video_Decoder[4][Video_Decoder_Lenght];//快速播放解码器
extern sf::Texture Quick_Video_Texture[4];//快速播放图片
extern ENUM::Input_Position Input_Position;//表示按键输入的位置
extern unsigned Input_Char_Position;//输入的字符位置

//extern std::thread* ffplay_Thread;//ffplay线程
//extern bool ffplay_Pause_Thread_Enabled;//ffplay线程是否启动
//extern std::vector<HWND> ffplay_Windows;//给ffplay_thread线程使用

bool CMD_Call(const int argc, const char* argv[]);//命令行调用
void Print();//渲染 GUI
void Sleep();//休息
void Event(bool Flag = true);//事件 GUI

void Reset_Setting();//重置设置
void Get_File_Setting(const char Setting_File_Path[], const bool Disabled_Warning = false);//获取设置
void Set_File_Setting(const char Setting_File_Path[]);//保存设置
void Get_Child_Window(Window_Infomation* Window_Infomation_ptr, HWND Parent_Window_WHND = Window_Infomation::Get_PM_Window_HWND());//枚举并连接PM的子窗口
void Repair_W1_W2_Error(Window_Infomation* Window_Infomation_ptr = nullptr);//修复W1、W2错误
//void Set_Window_Under(HWND Window_HWND);//将Window_HWND隐藏 1.0.2.5删除
//void Set_Window_Up(HWND Window_HWND, HWND Window_Parent_HWND = Parent_HWND, RECT Window_Info = Under_Window_Info);//将Window_HWND还原 1.0.2.5删除
//BOOL CALLBACK Hide_PM_W2(_In_ HWND hwnd, _In_ LPARAM Lparam);//找W2窗口并隐藏 1.0.2.5删除

//void ffplay_Pause_Thread(std::vector<HWND>& ffplay_Windows);//ffplay线程
//void Record();//录制
void Play_Video(const char Video_Path_Param[], const char Video_Decoder_Param[]);//播放视频
void Creat_Vidoe_Texture(int Number);//创建图片
//void Get_PM_Window_HWND(HWND& Return);//获取PM窗口句柄

void Move_Window(Window_Infomation* Node, ENUM::Move_Type Flag);//移动窗口

//void Input_HWND();//输入句柄 现已整合到Key_Event中
void Set_Quick_Video();//设置快速播放
void Choise_Window();//选择窗口
//void Set_Focus();//设置焦点 1.0.1.12删除
//void Set_Most_Window(HWND Window_HWND);//设置置顶 1.0.2.5删除
//void Under_or_Up(Window_Infomation Node);//设置或恢复 1.0.2.5删除
//void Keep_Under();//保持在桌面之下 1.0.2.5删除

void CharToTchar(const char* _char, TCHAR* tchar, UINT CodePage = CP_ACP);//https://blog.csdn.net/imxiangzi/article/details/39483041 转换char和tchar
void TcharToChar(const TCHAR* tchar, char* _char, UINT CodePage = CP_ACP);//https://blog.csdn.net/imxiangzi/article/details/39483041 转换char和tchar
void Spict_Name(const char Source[], char Name[], const int Name_Lenght);//分解路径
void Spict_Path(const char Source[], char Path[], const int Path_Lenght);//分解名称