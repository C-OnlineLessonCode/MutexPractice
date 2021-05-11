#include <iostream>
#include <windows.h>
#include<sstream>

HANDLE g_mutex = INVALID_HANDLE_VALUE;

void Wharf(const std::string& shipName) {
	for (int i = 0; i < 10; ++i) {
		Sleep(200);
		std::cout << shipName << " wharfed amount: " << i << std::endl;
	}
}

DWORD WINAPI ThreadMain(LPVOID threadParameter) {
	WaitForSingleObject(g_mutex, INFINITE);
	std::stringstream ss;
	ss << "name" << reinterpret_cast<int>(threadParameter);
	Wharf(ss.str());
	ReleaseMutex(g_mutex);
	
	return 0;
}

int main()
{
	g_mutex = CreateMutexA(0, FALSE, 0);

	HANDLE threads[10] = {};

	for (int i = 0; i < std::size(threads); ++i) {
		DWORD threadId = 0; // TID
		threads[i] = CreateThread(
			0,
			0,
			ThreadMain,
			reinterpret_cast<void*>(i + 1),
			0,
			&threadId
		);		
	}

	WaitForMultipleObjects(std::size(threads), threads, TRUE, INFINITE);
	for (int i = 0; i < std::size(threads); ++i) {
		CloseHandle(threads[i]);
	}
	CloseHandle(g_mutex);
}