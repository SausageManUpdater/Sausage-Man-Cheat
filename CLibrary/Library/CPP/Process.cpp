#include "..\Library.h"
namespace Library
{
	DWORD GetProcessIdByName(const char* name)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot) {
			return NULL;
		}
		PROCESSENTRY32 pe = { sizeof(pe) };
		for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {
			if (strcmp(pe.szExeFile, name) == 0) {
				CloseHandle(hSnapshot);
				return pe.th32ProcessID;
			}
			//printf("%-6d %s\n", pe.th32ProcessID, pe.szExeFile);
		}
		CloseHandle(hSnapshot);
		return 0;
	}

	int  GetHandleById(DWORD id)
	{
		return (int)OpenProcess(PROCESS_ALL_ACCESS, false, id);
	}

	LPDWORD CreateThreads(void(*p))
	{
		LPDWORD ID = nullptr;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)p, 0, 0, ID);
		return ID;
	}

	bool UpPrivilegeValue()
	{
		//OpenProcessToken()函数用来打开与进程相关联的访问令牌
		HANDLE hToken = nullptr;
		if (FALSE == OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
		{
			return false;
		}
		//LookupPrivilegeValue()函数查看系统权限的特权值
		LUID luid;
		if (FALSE == LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid))
		{
			CloseHandle(hToken);
			return false;
		}
		//调整权限设置
		TOKEN_PRIVILEGES Tok;
		Tok.PrivilegeCount = 1;
		Tok.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		Tok.Privileges[0].Luid = luid;
		if (FALSE == AdjustTokenPrivileges(hToken, FALSE, &Tok, sizeof(Tok), nullptr, nullptr))
		{
			CloseHandle(hToken);
			return false;
		}

		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		{
			CloseHandle(hToken);
			return false;
		}

		CloseHandle(hToken);
		return true;
	}

}