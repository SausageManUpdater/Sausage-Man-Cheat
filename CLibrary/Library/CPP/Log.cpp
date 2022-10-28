#include "..\Library.h"
namespace Library
{
	void LOG_OUT_(Level M, const char* Char, string code, int line)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		cout << "[";
		switch (M)
		{
		case Level::L_INFO:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
			cout << "Info";
			break;
		case Level::L_DEBUG:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
			cout << "Debug";
			break;
		case Level::L_WARNING:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
			cout << "Warning";
			break;
		case Level::L_ERROR:
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			cout << "Error";
			break;
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		cout << "] [";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);
		cout << GetNameFromFile(code.c_str()) << ":" << line;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		cout << "] ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		cout << GetHour() << ":" << GetMin() << ":" << GetSec() << "." << GetMilliSec();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		cout << ">> " << Char;
		cout << endl;
	}

	HWND LOG_START(const char* title, bool Close)
	{
		HANDLE g_hOutput = 0;
		HWND hwnd = NULL;
		HMENU hmenu = NULL;
		AllocConsole();
		g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		//设置控制台窗口的属性
		SetConsoleTitle(title);
		SetConsoleTextAttribute((HANDLE)g_hOutput, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		while (NULL == hwnd) hwnd = ::FindWindow(NULL, (LPCTSTR)title);
		//屏蔽掉控制台窗口的关闭按钮，以防窗口被删除
		hmenu = ::GetSystemMenu(hwnd, FALSE);
		if (!Close)
		{
			DeleteMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		freopen_s((FILE**)stderr, "CONOUT$", "w", stderr);
		return hwnd;
	}
	void LOG_END()
	{
		fclose(stdout);
		fclose(stderr);
		FreeConsole();
	}
}