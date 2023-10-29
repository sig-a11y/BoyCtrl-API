// BoyCtrlTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include "../BoyCtrl/BoyCtrl.h"

using namespace std;

void __stdcall speakCompleteCallback()
{
	MessageBeep(0xffffffff);
}

int main()
{
#ifndef BOYCTRLTEST_X64
	auto dllHandle = LoadLibrary(L"BoyCtrl.dll");
#else
	auto dllHandle = LoadLibrary(L"BoyCtrl-x64.dll");
#endif
	if (!dllHandle)
	{
#ifndef BOYCTRLTEST_X64
		cerr << "Failed to load BoyCtrl.dll error = " << GetLastError() << endl;
#else
		cerr << "Failed to load BoyCtrl-x64.dll error = " << GetLastError() << endl;
#endif
		return 1;
	}
	auto initFunc = (decltype(BoyCtrlInitialize)*)GetProcAddress(dllHandle, "BoyCtrlInitialize");
	if (!initFunc)
	{
		cerr << "Failed to get BoyCtrlInitialize" << endl;
		FreeLibrary(dllHandle);
		return 1;
	}
	auto uninitFunc = (decltype(BoyCtrlUninitialize)*)GetProcAddress(dllHandle, "BoyCtrlUninitialize");
	if (!uninitFunc)
	{
		cerr << "Failed to get BoyCtrlUninitialize" << endl;
		FreeLibrary(dllHandle);
		return 1;
	}
	auto err = initFunc(L"D:\\boyCtrl.log");
	if (err != e_bcerr_success)
	{
		cerr << "init failed error = " << err << endl;
		FreeLibrary(dllHandle);
		return 1;
	}
	cout << "ready" << endl;

	auto speakFunc = (decltype(BoyCtrlSpeak)*)GetProcAddress(dllHandle, "BoyCtrlSpeak");
	if (!speakFunc)
	{
		cerr << "Failed to get BoyCtrlSpeak" << endl;
		uninitFunc();
		FreeLibrary(dllHandle);
		return 1;
	}
	auto speakU8Func = (decltype(BoyCtrlSpeakU8)*)GetProcAddress(dllHandle, "BoyCtrlSpeakU8");
	if (!speakU8Func)
	{
		cerr << "Failed to get BoyCtrlSpeakU8" << endl;
		uninitFunc();
		FreeLibrary(dllHandle);
		return 1;
	}
	auto stopFunc = (decltype(BoyCtrlStopSpeaking)*)GetProcAddress(dllHandle, "BoyCtrlStopSpeaking");
	if (!stopFunc)
	{
		cerr << "Failed to get BoyCtrlStopSpeaking" << endl;
		uninitFunc();
		FreeLibrary(dllHandle);
		return 1;
	}
	for (int i = 1; i <= 4; ++i)
	{
		cout << i << " Press <Enter> to speak" << endl;
		getchar();
		if (i < 3)
		{
			wostringstream oss;
			oss << i << L"使用独立语音朗读，打断模式，有完成通知";
			err = speakFunc(oss.str().c_str(), true, false, speakCompleteCallback);
		}
		else
			err = speakU8Func(u8"朗读字符串使用UTF-8", true, false, speakCompleteCallback);
		if (err != e_bcerr_success)
		{
			cerr << "error = " << err << endl;
		}
	}
	uninitFunc();
	FreeLibrary(dllHandle);
	cout << "Press <Enter> to exit" << endl;
	getchar();
}
