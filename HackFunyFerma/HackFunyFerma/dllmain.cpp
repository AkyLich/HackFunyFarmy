#include <Windows.h>

DWORD baseAddres = NULL;

void Write(void *addr, void *bytes, size_t byteSize) {
	DWORD prot;
	VirtualProtect(addr, byteSize, PAGE_EXECUTE_READWRITE, &prot);
	memcpy(addr, bytes, byteSize);
	VirtualProtect(addr, byteSize, prot, &prot);
}

void NOP(void* addr, size_t bytes) {
	DWORD prot;
	VirtualProtect(addr, bytes, PAGE_EXECUTE_READWRITE, &prot);
	memset(addr, 0x90, bytes);
	VirtualProtect(addr, bytes, prot, &prot);
}

bool infinityWater = false;

BOOL WINAPI mainThread(HMODULE hModule) {
	while (!GetAsyncKeyState(VK_END)) {

		if (GetAsyncKeyState(VK_NUMPAD1) & 1) {//добавить 100 щекеле 

			__int32 address = (__int32)baseAddres + 0x2F9020;
			*(__int32*)(*(__int32*)address + 0x60) += 100;
		}

		if (GetAsyncKeyState(VK_NUMPAD2) & 1) { // вода блять
			infinityWater ^= true;
			infinityWater ? NOP((void*)(baseAddres + 0x7D9B7), 8U) :
			Write((void*)(baseAddres + 0x7D9B7), (BYTE*)"\xF3\x0F\x11\x86\xC0\x00\x00\x00", 8U);
		}
	
	}

	FreeLibraryAndExitThread(hModule, NULL);
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		baseAddres = (DWORD)GetModuleHandle("FarmFrenzy_HeaveHo.exe");
		CloseHandle(CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)mainThread, NULL, NULL, NULL));
        break;
    }
    return TRUE;
}

