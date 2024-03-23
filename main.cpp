#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc/proc.h"


int main() {
	DWORD procId = GetProcId(L"ac_client.exe");

	std::cout << "ProcId = " << procId << std::endl;

	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

	std::cout << "Module Base = 0x" << std::hex << moduleBase << std::endl;

	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x17E0A8;

	std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << std::uppercase << dynamicPtrBaseAddr << std::endl;


	while (true) {
		std::vector<unsigned int> ammoOffsets = { 0x364, 0x14, 0x0 };
		uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

		std::cout << "ammoAddr = " << "0x" << std::hex << std::uppercase << ammoAddr << std::endl;

		int ammoValue = 0;
		ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

		int newAmmo = 1337;
		if (ammoValue != newAmmo) {
			WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);
		}

		ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
	}
	return 0;
}