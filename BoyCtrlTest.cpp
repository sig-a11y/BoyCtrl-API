// BoyCtrlTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
#include "BoyCtrl.h"

using namespace std;
#define BOYCTRLTEST_X64


void __stdcall speakCompleteCallback(int reason)
{
	cerr << "reason = " << reason << endl;
}

int main()
{
#ifndef BOYCTRLTEST_X64
	auto dllHandle = LoadLibrary(L"BoyCtrl.dll");
#else
	auto dllHandle = LoadLibrary(L"x64/BoyCtrl-x64.dll");
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
	auto err = initFunc(L"boyCtrl.log");
	if (err != e_bcerr_success)
	{
		cerr << "init failed error = " << err << endl;
		FreeLibrary(dllHandle);
		return 1;
	}
	cout << "ready" << endl;

	/* 加载读取输出函数 */
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

	bool withSlave=false, append=false, allowBreak=true;
	for (int i = 1; i <= 4; ++i)
	{
		cout << i << " Press <Enter> to speak" << endl;
		getchar();
		if (i < 3)
		{
			wostringstream oss;
			oss << i << L" Boy control Speak 中文";
			err = speakFunc(oss.str().c_str(), withSlave, append, allowBreak, speakCompleteCallback);
		}
		else
			err = speakU8Func(u8"Boy control Speak UTF-8 中文", withSlave, append, allowBreak, speakCompleteCallback);
		if (err != e_bcerr_success)
		{
			cerr << "error = " << err << endl;
		}
	}

	uninitFunc();
	FreeLibrary(dllHandle);
	cout << "Press <Enter> to exit" << endl;
	getchar();
	return 0;
}

// cl /source-charset:utf-8 /DUNICODE /D_UNICODE BoyCtrlTest.cpp
