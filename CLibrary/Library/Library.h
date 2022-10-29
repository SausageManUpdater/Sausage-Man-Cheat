#pragma once
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <WinUser.h>
#include <TlHelp32.h>
#include <chrono>
#include <filesystem>
#include <d3d11.h>
#include <d3d10.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <d3d9.h>
#include <dwmapi.h>
#include <string>
#include <vector>
#include <cstdio>
#include <map>
#include <imgui.h>
#include <unordered_set>
#include <imgui_stdlib.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_dx10.h>
#include <imgui_impl_dx9.h>
#include <imgui_impl_win32.h>
#include <detours.h>
#include <json.hpp>
#include <json_fwd.hpp>
#include <ordered_map.hpp>
#include <SimpleIni.h>
#include <magic_enum.hpp>
#include <magic_enum_format.hpp>
#include <magic_enum_fuse.hpp>
#include <stb_image_resize.h>
#include <core.h>
#include <os.h>
#include <chrono.h>
#include <ranges.h>
#include <args.h>
#include <color.h>
#include <compile.h>
#include <ostream.h>
#include <printf.h>
#include <std.h>
#include <thread>
#include <xchar.h>
#include <implot.h>
#include <implot_internal.h>
#include <imnodes.h>
#include <imnodes_internal.h>

#pragma comment (lib, "D3dcompiler.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3d10.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib,"User32.lib")
#pragma comment (lib,"Gdi32.lib")
#pragma comment(lib, "shlwapi.lib")


#pragma warning (disable:4996)

#define _CRT_SECURE_NO_WARNINGS

#if _WIN64
#include <imgui_impl_dx12.h>
#pragma comment (lib,"detours-x64.lib")
#include <dxgi1_4.h>
#include <d3d12.h>
#pragma comment (lib, "d3d12.lib")
#else
#pragma comment (lib,"detours-x86.lib")
#endif

using namespace std;
using Json = nlohmann::json;

namespace Library
{
	unsigned char* getallfont();
	struct LibTime
	{
		int nanosec;
		int microsec;
		int millisec;
		int sec;
		int min;
		int hour;
		int day;
		int mon;
		int year;
		uint64_t timestamp;
	};

	//获取时间
	LibTime GetNowTime();
	//获取毫秒
	int GetMilliSec();
	//获取微秒
	int Getmicrosec();
	//获取纳秒
	int Getnanosec();
	//获取秒
	int GetSec();
	//获取分钟
	int GetMin();
	//获取小时
	int GetHour();
	//获取天
	int GetDay();
	//获取月
	int GetMon();
	//获取年
	int GetYear();
	//获取时间戳
	uint64_t GetTimeStamp();
	//计时器(纳秒级)
	class LibTimer
	{
	public:
		//计时开始
		void Timer_Start();
		//计时结束
		void Timer_End();
		//计时转文本
		const char* Timer_string();
		//获取时间
		LibTime GetTimer();
	private:
		LibTime Temp_Time;
		LibTime Time_Temp;
	};

	//进程名取ID(进程名)
	DWORD GetProcessIdByName(const char* name);
	//进程ID取句柄(进程ID)
	int GetHandleById(DWORD id);
	//枚举进程信息
	//枚举进程模块
	//取进程信息
	//取进程模块
	//注入DLL
	bool LoadLibraryDLL(HANDLE hProc, const std::string& dllpath);
	bool ManualMapDLL(HANDLE hProc, const std::string& filepath);
	bool ManualMapDLL(HANDLE hProc, BYTE* pSrcData, SIZE_T FileSize, bool ClearHeader = false, bool ClearNonNeededSections = false, bool AdjustProtections = true, bool SEHExceptionSupport = false, DWORD fdwReason = DLL_PROCESS_ATTACH);
	bool UpPrivilegeValue();
	//创建线程(函数指针)
	LPDWORD CreateThreads(void(*p));
	//结束线程(线程ID)
	bool EndThreads(LPDWORD ThreadId);

	//从路径获取文件名(路径)
	const char* GetNameFromFile(const char* path);
	//GBK转UTF8(文本)
	string tf8(const string& str);

	//输出类型
	enum Level :int
	{
		L_INFO = 1,//信息
		L_DEBUG,//调试
		L_WARNING,//警告
		L_ERROR//错误
	};
	//控制台输出(模式 Level 开头 , 内容文本 , 是否换行)
#define LOG_OUT(M,Char) LOG_OUT_(M,Char,__FILE__,__LINE__)
//控制台输出(模式Level, 内容文本, 是否换行, 文件, 行号)
	void LOG_OUT_(Level M, const char* Char, string code, int line);
	//打开控制台(标题 , 是否可以关闭)
	HWND LOG_START(const char* title, bool Close);
	//关闭控制台()
	void LOG_END();

	enum DXVersion
	{
		D3D9 = 9,
		D3D10,
		D3D11,
		D3D12
	};

	//安装D3DHook(D3D版本,函数指针)
	void InitD3DHook(DXVersion version,void (*p)());
	//设置穿透(逻辑)
	void SetInputLock(void* this_, bool value);
	//是否穿透
	bool IsInputLocked();
	//绘制URL标签
	void TextURL(const char* name_, const char* URL_, bool SameLineBefore_, bool SameLineAfter_);
	//添加下划线
	void AddUnderLine(ImColor col_);


	//使用前请先初始化
	class LibMemory
	{
	public:
		//x32读x64请使用Ntdll或Driver
		enum Type :int
		{
			Ntdll,//内核
			Common,//普通
			Driver//驱动
		};
		//初始化(读写类型)
		void initialize(Type readtype);

		//写内存
		bool Read(HANDLE hProcess, ULONG64 address, LPVOID IPBUFFER, SIZE_T size);
		//读内存
		bool Write(HANDLE hProcess, ULONG64 address, LPVOID IPBUFFER, SIZE_T size);

		//读字节型(进程句柄,地址)
		BYTE Read_byte(DWORD pid, ULONG64 address);
		//写字节型(进程句柄,地址,值)
		bool Write_byte(DWORD pid, ULONG64 address, BYTE value);

		//读整数型(进程句柄,地址)
		DWORD Read_int(DWORD pid, ULONG64 address);
		//写整数型(进程句柄,地址,值)
		bool Write_int(DWORD pid, ULONG64 address, DWORD value);

		//读长整数型(进程句柄,地址)
		DWORD64 Read_long_int(DWORD pid, ULONG64 address);
		//写长整数型(进程句柄,地址,值)
		bool Write_long_int(DWORD pid, ULONG64 address, DWORD64 value);

		//读浮点型(进程句柄,地址)
		float Read_float(DWORD pid, ULONG64 address);
		//写浮点型(进程句柄,地址,值)
		bool Write_float(DWORD pid, ULONG64 address, float value);

		//读双浮点型(进程句柄,地址)
		double Read_double(DWORD pid, ULONG64 address);
		//写双浮点型(进程句柄,地址,值)
		bool Write_double(DWORD pid, ULONG64 address, double value);

		//搜索特征码(进程句柄,特征码)
		vector<DWORD64> Search_Signatures(DWORD pid, ULONG64 START, ULONG64 END, const char* Signatures);

	private:
		bool initializeNT();
		bool initializeDR();
		void ReadProcessMemorys();
		int Mode;
	};
}
