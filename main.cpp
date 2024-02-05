#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc/proc.h"


int main() {
	// Get Process ID of the target process
	DWORD procId = GetProcId(L"ac_client.exe");

	// GetModuleBaseAddress
	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

	// Get Handle
	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	// Resolve Base Address of the pointer chain
	uintptr_t dynamicPtrBaseAddr = moduleBase + 0x17E0A8;

	std::cout << "DynamicPtrBaseAddr = " << "0x" << std::hex << std::uppercase << dynamicPtrBaseAddr << std::endl;

	// Resolve ammo pointer chain
	std::vector<unsigned int> ammoOffsets = {0x364, 0x14, 0x0};
	uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);

	std::cout << "ammoAddr = " << "0x" << std::hex << std::uppercase << ammoAddr << std::endl;

	// Read Ammo Value
	int ammoValue = 0;
	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

	std::cout << "Current Ammo = " << std::dec << ammoValue << std::endl;

	// Write to it
	int newAmmo = 1337;
	WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

	// Read out again
	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

	std::cout << "New ammo = " << std::dec << ammoValue << std::endl;

	getchar();
	return 0;
}