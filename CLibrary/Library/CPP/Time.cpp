#include "..\Library.h"
namespace Library
{
	
	LibTime GetNowTime()
	{
		LibTime time;
		time.year = GetYear();
		time.mon = GetMon();
		time.day = GetDay();
		time.hour = GetHour();
		time.min = GetMin();
		time.sec = GetSec();
		time.millisec = GetMilliSec();
		time.microsec = Getmicrosec();
		time.nanosec = Getnanosec();
		time.timestamp = chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
		return time;
	}
	//��ȡ����
	int GetMilliSec()
	{
		return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count() % 1000;
	}
	//��ȡ΢��
	int Getmicrosec()
	{
		return chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch()).count() % 1000;
	}
	//��ȡ����
	int Getnanosec()
	{
		return chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count() % 1000;
	}
	//��ȡ��
	int GetSec()
	{
		time_t time_tm = chrono::system_clock::to_time_t(chrono::system_clock::now());
		auto time_now = localtime(&time_tm);
		return time_now->tm_sec;
	}
	//��ȡ����
	int GetMin()
	{
		time_t time_tm = chrono::system_clock::to_time_t(chrono::system_clock::now());
		auto time_now = localtime(&time_tm);
		return time_now->tm_min;
	}
	//��ȡСʱ
	int GetHour()
	{
		time_t time_tm = chrono::system_clock::to_time_t(chrono::system_clock::now());
		auto time_now = localtime(&time_tm);
		return time_now->tm_hour;
	}
	//��ȡ��
	int GetDay()
	{
		time_t time_tm = chrono::system_clock::to_time_t(chrono::system_clock::now());
		auto time_now = localtime(&time_tm);
		return time_now->tm_mday;
	}
	//��ȡ��
	int GetMon()
	{
		time_t time_tm = chrono::system_clock::to_time_t(chrono::system_clock::now());
		auto time_now = localtime(&time_tm);
		return time_now->tm_mon + 1;
	}
	//��ȡ��
	int GetYear()
	{
		time_t time_tm = chrono::system_clock::to_time_t(chrono::system_clock::now());
		auto time_now = localtime(&time_tm);
		return time_now->tm_year + 1900;
	}
	//��ȡʱ���
	uint64_t GetTimeStamp()
	{
		return chrono::duration_cast<chrono::nanoseconds>(chrono::system_clock::now().time_since_epoch()).count();
	}
	void LibTimer::Timer_Start()
	{
		Temp_Time = GetNowTime();
	}
	void LibTimer::Timer_End()
	{
		Time_Temp = GetNowTime();
		uint64_t timestamp = Time_Temp.timestamp - Temp_Time.timestamp;
		Time_Temp.timestamp = timestamp;
		time_t time_tm = (int)(timestamp / 1000000000);
		//cout << (int)(timestamp / 1000000000) << endl;
		auto time_now = localtime(&time_tm);
		Time_Temp.year = time_now->tm_year - 70;
		Time_Temp.mon = time_now->tm_mon;
		Time_Temp.day = time_now->tm_mday - 1;
		Time_Temp.hour = time_now->tm_hour - 8;
		Time_Temp.min = time_now->tm_min;
		Time_Temp.sec = time_now->tm_sec;
		Time_Temp.nanosec = timestamp % 1000;
		Time_Temp.microsec = (int)(timestamp / 1000000 % 1000);
		Time_Temp.millisec = (int)(timestamp / 1000 % 1000);

	}
	const char* LibTimer::Timer_string()
	{
		string Temp__;
		Temp__ += to_string(Time_Temp.year);
		Temp__ += "-";
		Temp__ += to_string(Time_Temp.mon);
		Temp__ += "-";
		Temp__ += to_string(Time_Temp.day);
		Temp__ += " ";
		Temp__ += to_string(Time_Temp.hour);
		Temp__ += ":";
		Temp__ += to_string(Time_Temp.min);
		Temp__ += ":";
		Temp__ += to_string(Time_Temp.sec);
		Temp__ += " ";
		Temp__ += to_string(Time_Temp.millisec);
		Temp__ += ".";
		Temp__ += to_string(Time_Temp.microsec);
		Temp__ += to_string(Time_Temp.nanosec);
		Temp__ += "ms";
		return Temp__.c_str();
	}
	LibTime LibTimer::GetTimer()
	{
		return Time_Temp;
	}
}