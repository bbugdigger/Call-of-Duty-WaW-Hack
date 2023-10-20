#include "includes.h"
#include <iostream>

// data
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;
D3DCOLOR colorRed = D3DCOLOR_ARGB(255, 255, 0, 0);

Hack* hack;
Vec2 vScreen;

// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice)
		pDevice = o_pDevice;

	// drawing stuff
	Vec3* cords = (Vec3*)(dwLocalPlayer + 0x20);
	int health = *(int*)(dwLocalPlayer + 0x164);

	for (int i = 0; i < 24; i++) {
		Zombie* currZombie = (Zombie*)(dwEntityList + i * 0x88);
		Vec3 zombieCords = currZombie->GetOrigin();

		// check for nullptr here because of the way Zombie entity is set up (padding chars!!!) 
		if (zombieCords.x == 0 && zombieCords.y == 0 && zombieCords.z == 0)
			break;

		//if (!currZombie->isAlive())
		if (currZombie->getHealth() <= 0)
			continue;

		// snapline
		//if (WorldToScreen(zombieCords, vScreen, viewMatrix, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN))) {
		if (WorldToScreen(zombieCords, vScreen, viewMatrix, windowWidth, windowHeight)) {
			DrawLine(vScreen.x, vScreen.y, windowWidth / 2, windowHeight, 2, colorRed);
		}
	}
	
	// crosshair
	DrawFilledRect(windowWidth / 2 - 2, windowHeight / 2 - 2, 4, 4, D3DCOLOR_ARGB(255, 255, 255, 255));

	// call og function
	oEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {
	// hook
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);

		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
	}

	// hack loop
	while (!GetAsyncKeyState(VK_END)) {
		hack->Update();
	}

	// unhook
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

	// uninject
	FreeLibraryAndExitThread(hModule, 0);
}








DWORD WINAPI TestHackThread(HMODULE hModule) {
	//Create Debugging Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	std::cout << "[*] Debugging Console Started\n";

	hack = new Hack();

	Vec3* cords = (Vec3*)(dwLocalPlayer + 0x20);
	int health = *(int*)(dwLocalPlayer + 0x164);

	// hack loop
	while (!GetAsyncKeyState(VK_END)) {

		system("cls");

		std::cout << "LocalPlayer cords: "  << cords->x << ", " << cords->y << ", " << cords->z << std::endl;
		std::cout << "LocalPlayer health: " << health << std::endl;

		std::cout << "=================\n";

		for (int i = 0; i < 24; i++) {
			Zombie* currZombie = (Zombie*)(dwEntityList + i * 0x88);
			Vec3 zombieCords = currZombie->GetOrigin();

			// check for nullptr here because of the way Zombie entity is set up (padding chars!!!) 
			if (zombieCords.x == 0 && zombieCords.y == 0 && zombieCords.z == 0)
				break;
			
			int zombieHealth = currZombie->getHealth();
			//Vec3* zombieCords = (Vec3*)(hack->dwEntityList + i * 0x88 + 0x14);
			//int zombieHealth = *(int*)(*(uintptr_t*)(hack->dwEntityList + i * 0x88) + 0x1C8);

			std::cout << "Zombie" << i << " cords: " << zombieCords.x << ", " << zombieCords.y << ", " << zombieCords.z << std::endl;
			std::cout << "Zombie" << i << " health: " << zombieHealth << std::endl;
			std::cout << "=================\n";
		}

		Sleep(100);
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	if (reason == DLL_PROCESS_ATTACH)
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
	return TRUE;
}