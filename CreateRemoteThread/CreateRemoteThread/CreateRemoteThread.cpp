// CreateRemoteThread.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	char* inject_dll = "C:\\drivers\\inject.dll";	//the dll to be injected
	cout << inject_dll << endl;
	//process ID of process to be injected in
	int proc = 2308;

	//get a handle to that process
	HANDLE process = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, proc);
	cout << "proc handle: " << process;
	if (process == NULL){
		DWORD lasterror = GetLastError();
		cout << "Error: not able to find process bcoz of " <<lasterror<< endl;
	}
	else{
		//get address of loadlibrary
		LPVOID addr = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");
		//	cout << "\n proc addr: " << addr;
		if (addr == NULL){
			cout << "Error: ProcAddr error" << endl;
		}
		else{
			//allocate memory in the process
			LPVOID alloc = (LPVOID)VirtualAllocEx(process, NULL, strlen(inject_dll), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			if (alloc == NULL){
				cout << "error: the memory could not be allocated" << endl;
			}
			else{
				//write name of our dll in that process's memory space
				int n = WriteProcessMemory(process, alloc, inject_dll, strlen(inject_dll), NULL);
				if (n == 0){
					cout << "Error: not able to write" << endl;
				}
				else{
					//hook dll
					HANDLE threadID = CreateRemoteThread(process, NULL, 0, (LPTHREAD_START_ROUTINE)addr, alloc, NULL, NULL);

					if (threadID == NULL){
						cout << "Error: thread not created" << endl;
					}
					else
					{
						cout << "thread created!!!!!\n";
					}
				}
			}
		}

	}
	CloseHandle(process);
	getchar();

	return 0;
}

